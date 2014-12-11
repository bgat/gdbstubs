/*
  Copyright (c) 1999 by      William A. Gatliff
  All rights reserved.     bgat@open-widgets.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.

  The author welcomes feedback regarding this file.

  SH-4 support added by Benoit Miller (fulg@iname.com).

  
  Some changes by Petr Ledvina, ledvinap@kae.zcu.cz, 2001/10
  * Changes some paramaters from int to unsigned short
  * short sigval has problems when passing uchar* pointer to it, unsigned char now 
  * small changes in register sending routines (id not zeroed on error)
  * use GDB_STATUS_EXPEDITED
*/

/* $Id: gdb.c,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */

/* Remote communication protocol.

   A debug packet whose contents are <data>
   is encapsulated for transmission in the form:

        $ <data> # CSUM1 CSUM2

        <data> must be ASCII alphanumeric and cannot include characters
        '$' or '#'.  If <data> starts with two characters followed by
        ':', then the existing stubs interpret this as a sequence number.

        CSUM1 and CSUM2 are ascii hex representation of an 8-bit
        checksum of <data>, the most significant nibble is sent first.
        the hex digits 0-9,a-f are used.

   Receiver responds with:

        +       - if CSUM is correct and ready for next packet
        -       - if CSUM is incorrect

   <data> is as follows:
   All values are encoded in ascii hex digits.

        Request         Packet

        read registers  g
        reply           XX....X         Each byte of register data
                                        is described by two hex digits.
                                        Registers are in the internal order
                                        for GDB, and the bytes in a register
                                        are in the same order the machine uses.
                        or ENN          for an error.

        write regs      GXX..XX         Each byte of register data
                                        is described by two hex digits.
        reply           OK              for success
                        ENN             for an error

        write reg       Pn...=r...      Write register n... with value r...,
                                        which contains two hex digits for each
                                        byte in the register (target byte
                                        order).
        reply           OK              for success
                        ENN             for an error
        (not supported by all stubs).

        read mem        mAA..AA,LLLL    AA..AA is address, LLLL is length.
        reply           XX..XX          XX..XX is mem contents
                                        Can be fewer bytes than requested
                                        if able to read only part of the data.
                        or ENN          NN is errno

        write mem       MAA..AA,LLLL:XX..XX
                                        AA..AA is address,
                                        LLLL is number of bytes,
                                        XX..XX is data
        reply           OK              for success
                        ENN             for an error (this includes the case
                                        where only part of the data was
                                        written).

        write mem       XAA..AA,LLLL:XX..XX
         (binary)                       AA..AA is address,
                                        LLLL is number of bytes,
                                        XX..XX is binary data
        reply           OK              for success
                        ENN             for an error

        cont            cAA..AA         AA..AA is address to resume
                                        If AA..AA is omitted,
                                        resume at same address.

        step            sAA..AA         AA..AA is address to resume
                                        If AA..AA is omitted,
                                        resume at same address.

        last signal     ?               Reply the current reason for stopping.
                                        This is the same reply as is generated
                                        for step or cont : SAA where AA is the
                                        signal number.

        There is no immediate reply to step or cont.
        The reply comes when the machine stops.
        It is           SAA             AA is the "signal number"

        or...           TAAn...:r...;n:r...;n...:r...;
                                        AA = signal number
                                        n... = register number
                                        r... = register contents
        or...           WAA             The process exited, and AA is
                                        the exit status.  This is only
                                        applicable for certains sorts of
                                        targets.
        kill request    k

        toggle debug    d               toggle debug flag (see 386 & 68k stubs)
        reset           r               reset -- see sparc stub.
        reserved        <other>         On other requests, the stub should
                                        ignore the request and send an empty
                                        response ($#<checksum>).  This way
                                        we can extend the protocol and GDB
                                        can tell whether the stub it is
                                        talking to uses the old or the new.
        search          tAA:PP,MM       Search backwards starting at address
                                        AA for a match with pattern PP and
                                        mask MM.  PP and MM are 4 bytes.
                                        Not supported by all stubs.

        general query   qXXXX           Request info about XXXX.
        general set     QXXXX=yyyy      Set value of XXXX to yyyy.
        query sect offs qOffsets        Get section offsets.  Reply is
                                        Text=xxx;Data=yyy;Bss=zzz
        console output  Otext           Send text to stdout.  Only comes from
                                        remote target.

        Responses can be run-length encoded to save space.  A '*' means that
        the next character is an ASCII encoding giving a repeat count which
        stands for that many repititions of the character preceding the '*'.
        The encoding is n+29, yielding a printable character where n >=3
        (which is where rle starts to win).  Don't use an n > 126.

        So
        "0* " means the same as "0000".  */

/* 
   use some functions from newlibc (strlen and such)
   try to avoid giant functions like sprintf
*/
#include <string.h>

#include "common.h"
#include "gdb.h"

/* define this to accept rx_csum 0 (to simplify console debugging */
#define GDB_ALLOW_CSUM_0 1

#define GDB_RXBUFLEN 300
#define GDB_TXBUFLEN 300


const char nibble_to_hex_table[] = "0123456789abcdef";

/*
  Converts '[0-9,a-f,A-F]'
  to its long integer equivalent.
*/
long hex_to_long ( char h )
{
  if( h >= 'a' && h <= 'f' )
    return h - 'a' + 10;

  if( h >= '0' && h <= '9' )
    return h - '0';

  if( h >= 'A' && h <= 'F' )
    return h - 'A' + 10;

  return 0;
}


/*
  Converts the low nibble of i
  to its hex character equivalent.
*/
char lnibble_to_hex ( char i )
{
  return nibble_to_hex_table[i & 0xf];
}


/*
  Converts a memory region (addr) into a string of hex bytes.
  len is the length of the memory region, in bytes.

  Returns the number of bytes in hexbuf consumed.

  Note: This function preserves the endianness of the target 
        because that's what gdb expects.
*/
short mem_to_hexbuf ( const void *addr, char *hexbuf, unsigned short len )
{
  unsigned short i = 0;
  int ch;
  unsigned short orig_len = len;
  const char* pmem = (const char*)addr;
  char tmpbuf[ sizeof( long ) ];

  while( len > 0 ) {

    /* read in long units where we can
       (this is important if the address is
       a peripheral register or other memory
       location that only supports long accesses) */
    if( len >= sizeof( long )
        && ( (unsigned long)pmem % sizeof( long ) == 0 )) {

      *(long*)tmpbuf = *(long*)pmem;

      for (i = 0; i < sizeof( long ); i++)
      {
        ch = tmpbuf[i];
        *hexbuf++ = lnibble_to_hex( ch >> 4 );
        *hexbuf++ = lnibble_to_hex( ch );
      }

      pmem += sizeof( long );
      len -= sizeof( long );
    }

    /* read in short units where we can
       (same reasons as above) */
    if( len >= sizeof( short )
        && ( (unsigned long)pmem % sizeof( short ) == 0 )) {

      *(short*)tmpbuf = *(short*)pmem;

      for (i = 0; i < sizeof( short ); i++)
      {
        ch = tmpbuf[i];
        *hexbuf++ = lnibble_to_hex( ch >> 4 );
        *hexbuf++ = lnibble_to_hex( ch );
      }

      pmem += sizeof( short );
      len -= sizeof( short );
    }

    if( len == 1
        || ( len && ( (unsigned long)pmem % sizeof( short ) != 0 ))) {

      /* request is totally misaligned;
         read a byte, and hope for the best */
      ch = *pmem;
      *hexbuf++ = lnibble_to_hex( ch >> 4 );
      *hexbuf++ = lnibble_to_hex( ch );

      pmem += sizeof( char );
      len -= sizeof( char );
    }
  }

  return orig_len * 2;
}


/*
  Converts an arbitrary number of hex
  digits into a memory region.

  Returns a pointer to the character 
  after the last byte written.
*/
char *hexbuf_to_mem ( const char *buf, void *addr, unsigned short len )
{
  unsigned short i;
  unsigned char ch;
  char *pmem = (char*)addr;
  char tmpbuf[ sizeof( long ) ];

  void *cache_flush_start = addr;
  void *cache_flush_end = (void*)((char*)addr + len);


  /* keep writing bytes until we've written them all. Prefer larger writes
     over smaller writes since some peripherals with large registers need
     to be written in a single transaction. */

  while( len > 0 ) {

    /* can we write an aligned long? */
    if ((unsigned long)pmem % sizeof( long ) == 0 
       && len >= sizeof( long )) {

      for (i = 0; i < sizeof( long ); i++) 
      {
        ch = hex_to_long( *buf++ ) << 4;
        ch = ch + hex_to_long( *buf++ );
        tmpbuf[i] = ch;
      }

      *(long*)pmem = *(long*)tmpbuf;

      pmem += sizeof(long);
      len -= sizeof(long);
      continue;
    }

    /* we couldn't write an aligned long. Can we write an aligned short? */
    if ( ((unsigned long)pmem % sizeof( short )) == 0
         && len >= sizeof( short ) ) {

      for (i = 0; i < sizeof( short ); i++)
      {
        ch = hex_to_long( *buf++ ) << 4;
        ch = ch + hex_to_long( *buf++ );
        tmpbuf[i] = ch;
      }

      *(short*)pmem = *(short*)tmpbuf;

      pmem += sizeof(short);
      len -= sizeof(short);
      continue;
    }

    /* all else has failed. Write a single byte. */
    ch = hex_to_long( *buf++ ) << 4;
    ch = ch + hex_to_long( *buf++ );
    *pmem = ch;

    pmem += sizeof(char);
    len -= sizeof(char);
  }

  /* some processors (like the SH-4) need their caches flushed 
     at this point */
  gdb_flush_cache(cache_flush_start, cache_flush_end);

  return pmem;
}


/*
  Convert the escaped-binary array pointed to by buf into binary, to
  be placed in mem. Return a pointer to the character after the last
  byte written.
*/
char *ebin_to_mem ( const char *buf, void *addr, unsigned short len )
{
  unsigned short i;
  char *pmem = (char*)addr;
  char tmpbuf[ sizeof( long ) ];

  void *cache_flush_start = addr;
  void *cache_flush_end = (void*)((char*)addr + len);


  /* keep writing bytes until we've written them all. Prefer larger writes
     over smaller writes since some peripherals with large registers need
     to be written in a single transaction. */

  while( len > 0 ) {

    /* can we write an aligned long? */
    if ((unsigned long)pmem % sizeof( long ) == 0 
       && len >= sizeof( long )) {

      for (i = 0; i < sizeof( long ); i++) 
      {
        if( *buf == 0x7d )
          tmpbuf[i] = *(++buf) ^ 0x20;
        else
          tmpbuf[i] = *buf;

        buf++;
      }

      *(long*)pmem = *(long*)tmpbuf;

      pmem += sizeof(long);
      len -= sizeof(long);
      continue;
    }

    /* we couldn't write an aligned long. Can we write an aligned short? */
    if ( ((unsigned long)pmem % sizeof( short )) == 0
         && len >= sizeof( short ) ) {

      for (i = 0; i < sizeof( short ); i++)
      {
        if( *buf == 0x7d )
          tmpbuf[i] = *(++buf) ^ 0x20;
        else
          tmpbuf[i] = *buf;

        buf++;
      }

      *(short*)pmem = *(short*)tmpbuf;

      pmem += sizeof(short);
      len -= sizeof(short);
      continue;
    }

    /* all else has failed. Write a single byte. */
    if( *buf == 0x7d )
      *pmem = *(++buf) ^ 0x20;
    else
      *pmem = *buf;

    buf++;
    
    pmem += sizeof(char);
    len -= sizeof(char);
  }


  /* some processors (like the SH-4) need their caches flushed 
     at this point */
  gdb_flush_cache(cache_flush_start, cache_flush_end);

  return pmem;
}


/*
*/
unsigned char gdb_putstr ( short len, const char *buf )
{
  unsigned char sum = 0;

  while( len-- ) {
    sum += *buf;
    gdb_putc( *buf++ );
  }

  return sum;
}


/*
 */
void gdb_putmsg ( char c, const char *buf, short len )
{
  unsigned char sum;


  do {

    /* send the header */
    gdb_putc( '$' );

    /* send the message type, if specified */
    if( c )
      gdb_putc( c );

    /* send the data */
    sum = c + gdb_putstr( len, buf );

    /* send the footer */
    gdb_putc( '#' );
    gdb_putc( lnibble_to_hex( sum >> 4 ));
    gdb_putc( lnibble_to_hex( sum ));

  } while( '+' != gdb_getc() );

  return;
}


/*
*/
short gdb_getmsg ( char *rxbuf )
{
  char c;
  unsigned char sum;
  unsigned char rx_sum;
  char *buf;


 get_msg:

  /* wait around for start character, ignore all others */
  while( gdb_getc() != '$' );

  /* start counting bytes */
  buf = rxbuf;
  sum = 0;

  /* read until we see the '#' at the end of the packet */
  do {

    *buf++ = c = gdb_getc();

    if( c != '#' ) {
      sum += c;
    }

    /* since the buffer is ascii,
       may as well terminate it */
    *buf = 0;

  } while( c != '#' );

  /* receive checksum */
  rx_sum = hex_to_long( gdb_getc());
  rx_sum = ( rx_sum << 4 ) + hex_to_long( gdb_getc() );

  /* if computed checksum doesn't
     match received checksum, then reject */
  if( sum != rx_sum 
#ifdef GDB_ALLOW_CSUM_0
      && rx_sum!=0
#endif
      ) {

    gdb_putc( '-' );
    goto get_msg;
  }

  else {
    /* got the message ok */
    gdb_putc( '+' );
  }

  return 1;
}


/*
  "last signal" message

  "Sxx", where:
  xx is the signal number

*/
void gdb_last_signal ( unsigned char sigval )
{
  char tx_buf[2];


  gdb_putmsg( 'S', tx_buf,
              mem_to_hexbuf( &sigval, tx_buf, 1 ) );

  return;
}


/*
  "expedited response" message
  "Txx..........."

*/
void gdb_expedited ( unsigned char sigval )
{
  char tx_buf[sizeof( long ) * 2];
  long val;
  unsigned char id;
  short reglen;
  unsigned char sum;


  do {

    /* send header */
    gdb_putc( '$' );
    sum = gdb_putstr( 1, "T" );

    /* signal number */
    sum += gdb_putstr( mem_to_hexbuf( &sigval, tx_buf, 1 ), tx_buf );

    /* register values */
    id=0;
    while(( reglen = gdb_peek_register_file( id, &val )) != 0 ) {

      /* register id */
      sum += gdb_putstr( mem_to_hexbuf( &id, tx_buf, 1 ), tx_buf );
      sum += gdb_putstr( 1, ":" );

      /* register value
         (gdb 4.18 requires all 32 bits in values) 
	 = value length must match REGISTER_RAW_SIZE(regno) in gdb 
      */
      /* beware of big endian (ok for 32 bits) */
      sum += gdb_putstr( mem_to_hexbuf( &val, tx_buf, reglen ), tx_buf );
      sum += gdb_putstr( 1, ";" );

      /* try the next register */
      id++;
    }

    /* send the message footer */
    gdb_putc( '#' );
    gdb_putc( lnibble_to_hex( sum >> 4 ));
    gdb_putc( lnibble_to_hex( sum ));

  } while( '+' != gdb_getc() );

  return;
}


/*
 */
void gdb_read_memory ( const char *hargs )
{
  char tx_buf[GDB_TXBUFLEN];
  long addr = 0;
  long len = 0;
  unsigned char sum;

  /* parse address */
  while( *hargs != ',' )
    addr = ( addr << 4 ) + hex_to_long( *hargs++ );

  /* skip ',' */
  hargs++;

  /* parse length */
  while( *hargs != '#' )
    len = ( len << 4 ) + hex_to_long( *hargs++ );

  /* skip '#' */
  hargs++;


  do {
    /* send header */
    gdb_putc( '$' );

    /* send message content */
    sum = gdb_putstr( mem_to_hexbuf( (void*)addr, tx_buf, len ), 
		      tx_buf );

    /* send the message footer */
    gdb_putc( '#' );
    gdb_putc( lnibble_to_hex( sum >> 4 ));
    gdb_putc( lnibble_to_hex( sum ));

  } while('+' != gdb_getc());

  return;
}


/*
 */
void gdb_write_memory ( const char *hargs )
{
  long addr = 0;
  long len = 0;


  /* parse address */
  while( *hargs != ',' )
    addr = ( addr << 4 ) + hex_to_long( *hargs++ );

  /* skip ',' */
  hargs++;

  /* parse length */
  while( *hargs != ':' )
    len = ( len << 4 ) + hex_to_long( *hargs++ );

  /* skip ':' */
  hargs++;

  /* write all requested bytes */
  hexbuf_to_mem( hargs, (void*)addr, len );

  gdb_putmsg( 0, "OK", 3 );
  return;
}


/*
 */
void gdb_write_ebin_memory ( const char *hargs )
{
  long addr = 0;
  long len = 0;


  /* parse address */
  while( *hargs != ',' )
    addr = ( addr << 4 ) + hex_to_long( *hargs++ );

  /* skip ',' */
  hargs++;

  /* parse length */
  while( *hargs != ':' )
    len = ( len << 4 ) + hex_to_long( *hargs++ );

  /* skip ':' */
  hargs++;

  /* write all requested bytes */
  ebin_to_mem( hargs, (void*)addr, len );

  gdb_putmsg( 0, "OK", 3 );
  return;
}


/*
 */
void gdb_console_output( short len, const char *buf )
{
  char tx_buf[2];
  unsigned char sum;


  gdb_putc( '$' );
  sum = gdb_putstr( 1, "O" );
  if(len<0) len=strlen(buf);
  while( len-- ) {
    tx_buf[0] = lnibble_to_hex( *buf >> 4 );
    tx_buf[1] = lnibble_to_hex( *buf++ );

    sum += gdb_putstr( 2, tx_buf );
  }

  /* send the message footer */
  gdb_putc( '#' );
  gdb_putc( lnibble_to_hex( sum >> 4 ));
  gdb_putc( lnibble_to_hex( sum ));

  /* DON'T wait for response; we don't want to get hung
     up here and halt the application if gdb has gone away! */

  return;
}


/*
 */
void gdb_write_registers ( char *hargs )
{
  short id = 0;
  long val;
  short reglen;


  while( *hargs != '#' ) {

    /* how big is this register? */
    reglen = gdb_peek_register_file( id, &val );

    if( reglen ) {

      /* extract the register's value */
      hexbuf_to_mem( hargs, &val, reglen );
      hargs += sizeof( long ) * 2;

      /* stuff it into the register file */
      gdb_poke_register_file( id++, val );
    }

    else break;
  }

  gdb_putmsg( 0, "OK", 2 );

  return;
}


/*
 */
void gdb_read_registers ( char *hargs UNUSED)
{
  char tx_buf[sizeof( long ) * 2];
  long val;
  short id = 0;
  short reglen;
  unsigned char sum;


  do {

    gdb_putc( '$' );
    sum = 0;

    /* send register values */
    id=0;
    while(( reglen = gdb_peek_register_file( id++, &val ) ) != 0 )
      sum += gdb_putstr( mem_to_hexbuf( &val, tx_buf, reglen ), tx_buf );

    /* send the message footer */
    gdb_putc( '#' );
    gdb_putc( lnibble_to_hex( sum >> 4 ));
    gdb_putc( lnibble_to_hex( sum ));

  } while( '+' != gdb_getc() );

  return;
}


/*
 */
void gdb_write_register ( char *hargs )
{
  long id = 0;
  long val = 0;
  short reglen;


  while( *hargs != '=' )
    id = ( id << 4 ) + hex_to_long( *hargs++ );

  hargs++;

  reglen = gdb_peek_register_file( id, &val );
  hexbuf_to_mem( hargs, &val, reglen );

  gdb_poke_register_file( id, val );

  gdb_putmsg( 0, "OK", 2 );

  return;
}


/*
  The gdb command processor.
*/
void gdb_monitor ( short sigval )
{
  char rxbuf[GDB_RXBUFLEN];
  char *hargs;


  gdb_undo_step();

  while( 1 ) {

    gdb_getmsg( rxbuf );

    hargs = rxbuf;
    switch( *hargs++ ) {

    case '?' : /* send last signal */

      gdb_last_signal( sigval );
      break;


    case 'c' : /* continue (address optional) */

      gdb_continue( hargs );

      /* exit back to interrupted code */
      return;


    case 'g' :

      gdb_read_registers( hargs );
      break;


    case 'G' :

      gdb_write_registers( hargs );
      break;


    case 'H' :

      /* set thread---
         unimplemented, but gdb wants it */
      gdb_putmsg( 0, "OK", 2 );
      break;


    case 'k' : /* kill program */

      gdb_putmsg( 0, "OK", 2 );
      gdb_kill();
      break;


    case 'm' :

      gdb_read_memory( hargs );
      break;


    case 'M' : /* write to memory (source in hex format) */
 
     gdb_write_memory( hargs );
      break;


    case 'P':

      gdb_write_register( hargs );
      break;


    case 'q' : /* query */

      /* TODO: finish query command in gdb_handle_exception. */

      /* for now, only respond to "Offsets" query */
      gdb_putmsg( 0, "Text=0;Data=0;Bss=0", 19 );

      break;


    case 's' : /* step (address optional) */

      gdb_step( hargs );

      /* exit back to interrupted code */
      return;


    case 'X' : /* write to memory (source in escaped-binary format) */

      gdb_write_ebin_memory( hargs );
      break;


    default :

      /* received a command we don't recognize---
         send empty response per gdb spec */
      gdb_putmsg( 0, "", 0 );

    }
  }

  return;
}


/*
*/
void gdb_handle_exception( long sigval )
{
  /* tell the host why we're here */
#ifdef GDB_STATUS_EXPEDITED
  gdb_expedited( sigval );
#else
  gdb_last_signal( sigval );
#endif
  /* ask gdb what to do next */
  gdb_monitor( sigval );

  /* return to the interrupted code */
  gdb_return_from_exception();

  return;
}
