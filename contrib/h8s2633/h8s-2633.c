/*
  Copyright (c) 1999 by      William A. Gatliff
  All rights reserved.     bgat@open-widgets.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.

  The author welcomes feedback regarding this file.

  Mostly rewritten from sh2-704x.c by Petr Ledvina, 
  ledvinap@kae.zcu.cz, 2001/10
  
  BTW: Can't imagine houw could sh2 stub work ... 
    It would be probably good idea to compare it with this file
    and fix bugs
  
*/

/* $Id: h8s-2633.c,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */

#include "common.h"
#include "h8s-2633-rmap.h"
#include "gdb.h"
#include "h8s-2633-sci.h"

/* define this to fetch MAC into regfile */
#define USE_MAC 1
/* define this to suport mode2 exceptions (untested, probably broken) */
#define USE_EXR 0

/* trapa #3, the code is <5><3><trapno><0> */
#define STEP_OPCODE 0x5730

/* TODO 8bit ccr and exr */
typedef struct {
  unsigned long  r[8];
  unsigned long  pc;
  unsigned long  ccr;
  unsigned long  exr;
#if USE_MAC
  unsigned long  mach;
  unsigned long  macl;
#endif
} gdb_register_file_T;

gdb_register_file_T gdb_register_file
__attribute__((section(".regfile")));

/*
  Stuffs a byte out the serial port.
 */
void gdb_putc ( char c )
{
  sci3_putc(c);
}


/*
  Blocks until a byte is received at the serial port.
 */
char gdb_getc ( void )
{
  return sci3_getc();
}


/*
  Retrieves a register value from gdb_register_file.
  Returns the size of the register, in bytes,
  or zero if an invalid id is specified (which *will*
  happen--- gdb.c uses this functionality to tell
  how many registers we actually have).
*/
short gdb_peek_register_file ( short id, long *val )
{
  /* all our registers are longs */
  short retval = sizeof( long );


  switch( id ) {

  case 0:  case 1:  case 2:  case 3:
  case 4:  case 5:  case 6:  case 7:
    *val = gdb_register_file.r[id];
    break;

  case 8:
    *val = gdb_register_file.ccr;
    break;
  case 9:
    *val = gdb_register_file.pc;
    break;
  case 10:
    *val = gdb_register_file.exr;
    break;
#if USE_MAC
  case 11:
    *val = gdb_register_file.mach;
    break;
  case 12:
    *val = gdb_register_file.macl;
    break;
#endif
  default:
    retval = 0;
  }
  return retval;
}

/*
  Stuffs a register value into gdb_register_file.
  Returns the size of the register, in bytes,
  or zero if an invalid id is specified.
 */
short gdb_poke_register_file ( short id, long val )
{
  /* all our registers are longs */
  short retval = sizeof( long );


  switch( id ) {

  case 0:  case 1:  case 2:  case 3:
  case 4:  case 5:  case 6:  case 7:
    gdb_register_file.r[id] = val;
    break;

  case 8:   /* ccr */
    gdb_register_file.ccr = val;
    break;
  case 9:   /* pc */
    gdb_register_file.pc = val;
    break;
  case 10:  /* cycles */
    gdb_register_file.exr = val;
    break;
#if USE_MAC
  case 11:
    gdb_register_file.mach = val;
    break;
  case 12:
    gdb_register_file.macl = val;
    break;
#endif
  default:
    retval = 0;
  }
  return retval;
}

/* 
   table to clasify instruction according to first byte 
   Only first half, second if always 4 
*/

#define D_FIRST 11
#define D_LD   11
#define D_BCC2 12 
#define D_BCC4 13
#define D_JMPR 14
#define D_JMPA 15
#define D_JMPI 16
#define D_BSR2 17
#define D_BSR4 18
#define D_JSRR 19
#define D_JSRA 20
#define D_JSRI 21
#define D_MV   22
#define D_RTS  23
#define D_RTE  24

unsigned char opcode_0_to_length[128] = 
    { 
/*       0      1      2      3      4      5      6      7  
         8      9      a      b      c      d      e      f */
/* 0x */
         2,  D_LD,     2,     2,     2,     2,     2,     2,   
         2,     2,     2,     2,     2,     2,     2,     2, 
/* 1x */
         2,     2,     2,     2,     2,     2,     2,     2,   
         2,     2,     2,     2,     2,     2,     2,     2, 
/* 2x */
         2,     2,     2,     2,     2,     2,     2,     2,   
         2,     2,     2,     2,     2,     2,     2,     2, 
/* 3x */
         2,     2,     2,     2,     2,     2,     2,     2,   
         2,     2,     2,     2,     2,     2,     2,     2, 
/* 4x */
    D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,   
    D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2,D_BCC2, 
/* 5x */
         2,     2,     2,     2, D_RTS,D_BSR2, D_RTE,     2,   
    D_BCC4,D_JMPR,D_JMPA,D_JMPI,D_BSR4,D_JSRR,D_JSRA,D_JSRI, 
/* 6x */
         2,     2,     2,     2,     2,     2,     2,     2,   
         2,     2,  D_MV,  D_MV,     2,     2,     4,     4, 
/* 7x */
         2,     2,     2,     2,     2,     2,     2,     2,  
         8,     4,     6,     4,     4,     4,     4,     4 
    };  

/* test global ccr state, return if given condition is true */
int test_bcc(int op) {
  register char ccr=gdb_register_file.ccr;
#define C 0x01
#define V 0x02
#define Z 0x04
#define N 0x08
  switch(op) {
  case 0x0: return 1;
  case 0x1: return 0;
  case 0x2: return !(ccr&(C|Z));
  case 0x3: return ccr&(C|Z);
  case 0x4: return !(ccr&C);
  case 0x5: return ccr&C;
  case 0x6: return !(ccr&Z);
  case 0x7: return ccr&Z;
  case 0x8: return !(ccr&V);
  case 0x9: return ccr&V;
  case 0xa: return !(ccr&N);
  case 0xb: return ccr&N;
  case 0xc: return (ccr&N)? (ccr&V):!(ccr&V);
  case 0xd: return (ccr&N)?!(ccr&V):(ccr&V);
  case 0xe: return !(ccr&Z) && (ccr&N)? (ccr&V):!(ccr&V);
  case 0xf: return  (ccr&Z) || (ccr&N)?!(ccr&V): (ccr&V);
  }
#undef C
#undef V
#undef N
#undef Z
  return 0;
}

/*
  Analyzes the next instruction, to see where the program
  will go to when it runs.  Returns the destination address.
  
  Maybe some code opcodes may be used to automagicaly
  generate this ... 
  (There are some magic opcodes in gdb's opcodes, so don't forget
    to filter them )

  Instruction length is decoded using hardwired tests+table
  The table contains operation necesary for given prefix
  Length of non-jump instruction must be computed to gen next instr
  
  This code is partialy tested, the table was generated form /opcodes
  Probably some more intensive testing is neccesary
*/

long gdb_get_stepi_dest ( void )
{
  unsigned char op = ((unsigned char*)gdb_register_file.pc)[0];
  unsigned char op_t;
  long addr = gdb_register_file.pc;
  
  /* test if opcode > 0x80 -> 2b */ 
  if(op>=0x80) 
    return addr+2;
  
  /* fetch length from opcode table, dispatch results */
  op_t=opcode_0_to_length[op];
  switch(op_t) {
  case D_LD:   // prefix 01, ld and similar
    op=((unsigned char*)gdb_register_file.pc)[1];
    switch(op>>4) {
    case 0x0:
    case 0x4:
      op=((unsigned char*)gdb_register_file.pc)[2];
      switch(op) {
      case 0x6b: return addr+(((unsigned char*)gdb_register_file.pc)[3]&2)?8:6;
      case 0x6f: return addr+6;
      case 0x78: return addr+10;
      default: return addr+4;  
      }
    case 0x8:
    case 0xa:
      return addr+2;
    default:
      /* some invalid opcodes here, but ignore it ... */
      return addr+4;
    }
  case D_BCC2:
    if(test_bcc(op&0x0f))
      return addr+2+((unsigned char*)gdb_register_file.pc)[1];
    else return addr+2;
  case D_BCC4:
    if(test_bcc(((unsigned char*)gdb_register_file.pc)[1]>>4))
      return addr+4+((unsigned short*)gdb_register_file.pc)[1];
    else return addr+4;
  case D_JMPR:
  case D_JSRR:
    return gdb_register_file.r[((unsigned char*)gdb_register_file.pc)[1]>>4];
  case D_JMPA:
  case D_JSRA:
    return ((unsigned long*)gdb_register_file.pc)[0]&0x00ffffff;
  case D_JMPI:
  case D_JSRI:
    return 
      ((unsigned long*)(0))
      [((unsigned char*)gdb_register_file.pc)[1]]&0x00ffffff;
  case D_BSR2:
    return addr+2+((unsigned char*)gdb_register_file.pc)[1];
  case D_BSR4:
    return addr+4+((unsigned short*)gdb_register_file.pc)[1];
  case D_MV:
    switch(((unsigned char*)gdb_register_file.pc)[1]>>4) {
    case 0x1:
    case 0x2: 
    case 0xa: return addr+6;
    case 0x3: return addr+8;
    default:  return addr+4;
    }
  case D_RTS:
    return ((unsigned long*)gdb_register_file.r[7])[0]&0x00ffffff;
  case D_RTE:
#if USE_EXR
    return ((unsigned long*)(gdb_register_file.r[7]+2))[0]&0x00ffffff;
#else
    return ((unsigned long*)(gdb_register_file.r[7]+0))[0]&0x00ffffff;
#endif
  default:
    return addr+op_t;
  }
}

/* Write back data caches, and invalidates instruction caches */
/* NOTE: only used on SH4 for now */
void gdb_flush_cache(void *start UNUSED, void *end UNUSED)
{
}


/*
  Uses a TRAP to generate an exception
  after we execute the next instruction.
*/

short gdb_stepped_opcode=STEP_OPCODE;
unsigned long  gdb_step_pc;

void gdb_step ( char *hargs )
{
  long addr = 0;


  /* parse address, if any */
  while( *hargs != '#' )
    addr = ( addr << 4 ) + hex_to_long( *hargs++ );

  /* if we're stepping from an address, adjust pc (untested!) */
  /* TODO: test gdb_step when PC is supplied */
  if( addr ) gdb_register_file.pc = addr;

  /* determine where the target instruction will send us to */
  addr = gdb_get_stepi_dest();

  /* replace it */
  gdb_stepped_opcode = *(short*)addr;
  gdb_step_pc = addr;
  *(short*)addr = STEP_OPCODE;
  
  /* we're all done now */
  gdb_return_from_exception();

  return;
}

/* 
   pc could be bad when stepping trapa #3 ( gdb breakpoint )
   or when other exception occurs in stepi 
   so remember address of replaced instruction separately
*/
void gdb_undo_step ( void )
{
  /* quite bad idea to use 0 here, 0 is NOP */
  /* STEP_OPCODE should be ok               */
  if( gdb_stepped_opcode != STEP_OPCODE) {
    *(short*)gdb_step_pc = gdb_stepped_opcode;
    gdb_stepped_opcode = 0;
  }
  return;
}

/*
  Continue program execution at addr,
  or at the current pc if addr == 0.
*/
void gdb_continue ( char *hargs )
{
  long addr = 0;


  /* parse address, if any */
  while( *hargs != '#' )
    addr = ( addr << 4 ) + hex_to_long( *hargs++ );

  if( addr )
    gdb_register_file.pc = addr;

  gdb_return_from_exception();

  return;
}


/*
  Kills the current application.
  Simulates a reset by jumping to
  the address taken from the reset
  vector at address 0.
 */
void gdb_kill ( void )
{
  /* return control to monitor */
  asm( "jmp @@0");
}

/* all inline assembly till end f file ... 
   probably shoud be moved into .s file 
*/

/*
  Generic code to save processor context.
  Assumes the stack looks like this:

0   32  sigval               <-sp
4   32  er1
8   32  er0
[12  16  exr+pad (mode 2 (USE_EXR))]
14  32  pc+ccr  (mode 0,2)
18
*/

asm("save_registers_handle_exception:");

asm(" /* find end of gdb_register_file */");
asm(" mov.l #_gdb_register_file_end, er0");

#if USE_MAC
asm(" /* copy mac to register file */");
asm(" stmac macl,er1 ");
asm(" mov.l er1, @-er0");
asm(" stmac mach,er1 ");
asm(" mov.l er1, @-er0");
#endif

#if USE_EXR
asm(" /* copy exr to register file */");
asm(" mov.w   @(12, sp), r1");
asm(" extu.l  er1");
asm(" mov.l   er1, @-er0");

asm(" /* copy ccr */  /* TODO better ccr handling */");
asm(" mov.b   @(14, sp), r1l");
asm(" extu.w  r1");
asm(" extu.l  er1");
asm(" mov.l   er1, @-er0");
asm(" /* copy pc */"); 
asm(" mov.l   @(14, sp), er1");
asm(" and.l   #0x00ffffff, er1");
asm(" mov.l   er1, @-er0");
#else
asm(" mov.l   #0xffffffff, er1");
asm(" mov.l   er1, @-er0");   /* skip exr */
asm(" mov.b   @(12, sp), r1l");
asm(" extu.w  r1");
asm(" extu.l  er1");
asm(" mov.l   er1, @-er0");
asm(" /* copy pc */");
asm(" mov.l   @(12, sp), er1");
asm(" and.l   #0x00ffffff, er1");
asm(" mov.l   er1, @-er0");
#endif

asm(" /* sigval, r1, r0, pc, ccr, exr are already on the stack, */");
asm(" /* so esp isn't the same as it was immediately before */");
asm(" /* we took the current exception.  We have to adjust */");
asm(" /* esp in the register file so that gdb gets the right */");
asm(" /* stack pointer value */");
asm(" mov.l   sp, er1");
#if USE_EXR
asm(" add.l   #18, er1");
#else
asm(" add.l   #16, er1");
#endif
asm(" mov.l   er1, @-er0");

asm(" /* save r6-r2 */");
asm(" mov.l   er6, @-er0");
asm(" mov.l   er5, @-er0");
asm(" mov.l   er4, @-er0");
asm(" mov.l   er3, @-er0");
asm(" mov.l   er2, @-er0");

asm(" /* copy r1 to register file */");
asm(" mov.l   @(4, sp), er1");
asm(" mov.l   er1, @-er0");

asm(" /* copy r0 to register file */");
asm(" mov.l   @(8, sp), er1");
asm(" mov.l   er1, @-er0");

asm(" /* call gdb_handle_exception */");
asm(" mov.l   @sp, er0               /* sigval, pass in register */");
asm(" jmp     _gdb_handle_exception  /* this function is not expected to return */");

/*
  Unhandled exception isr.

  Not really much we can do here, so
  we just send a SIGUSR to gdb_handle_exception().

  Would be nice to tell user which exception trigered this
  It could be done using jsr instead of jmp in shadow vector table
  But stach frame will be different, so I'll do it later :) 
*/
extern void gdb_unhandled_isr ( void );

asm(" .global _gdb_unhandled_isr ");
asm(" _gdb_unhandled_isr: ");

asm(" /* push er0, er1 on the stack */ ");
asm(" push.l  er0 ");
asm(" push.l  er1 ");

asm(" /* put SIGUSR on stack */ ");
asm(" mov.l   #30, er0 ");
asm(" push.l  er0 ");

asm(" /* save registers, call gdb_handle_exception */ ");
asm(" jmp save_registers_handle_exception ");


/*
  TRAPA #3  (breakpoint) isr.
  Sends a SIGTRAP to gdb_handle_exception().

  Because we always subtract 2 from the pc
  stacked during exception processing, this
  function won't permit compiled-in breakpoints.
  If you compile a TRAPA #3 into the code, we'll
  loop on it indefinitely.  Use TRAPA #2 instead.

  GDB's trapa #3 breakpoints are just fine
*/
extern void gdb_trapa3_isr ( void );

asm(" .global _gdb_trapa3_isr ");
asm(" _gdb_trapa3_isr: ");

asm(" /* push er0, er1 on the stack */ ");
asm(" push.l  er0 ");
asm(" push.l  er1 ");

asm(" /* put SIGTRAP on stack */ ");
asm(" mov.l   #5, er0 ");
asm(" push.l  er0 ");

asm(" /* fudge pc, so we re-execute the instruction replaced */ ");
asm(" /* by the trap; this breaks compiled-in breakpoints! */ ");
asm(" /* this should be fine for gdb inserted breakpoints */ ");
asm(" mov.l   @(12, sp), er0 ");
asm(" add     #-2, er0 ");
asm(" mov.l   er0, @(12, sp) ");

asm(" /* save registers, call gdb_handle_exception */ ");
asm(" jmp save_registers_handle_exception ");

/*
  TRAPA #3 (compiled-in breakpoint) isr.
  Sends a SIGTRAP to gdb_handle_exception().
*/
extern void gdb_trapa2_isr ( void );

asm(" .global _gdb_trapa2_isr ");
asm(" _gdb_trapa2_isr: ");

asm(" /* push er0, er1 on the stack */ ");
asm(" push.l  er0 ");
asm(" push.l  er1 ");

asm(" /* put SIGUSR on stack */ ");
asm(" mov.l   #5, er0 ");
asm(" push.l  er0 ");

asm(" /* save registers, call gdb_handle_exception */ ");
asm(" jmp save_registers_handle_exception ");


/*
  Restores registers to the values specified
  in gdb_register_file.
*/
asm("   .global _gdb_return_from_exception  ");
asm("   _gdb_return_from_exception:         ");

asm("   /* find gdb_register_file */        ");
asm("   /* skip er0 and er1 for now,          ");
asm("      since we're using them */        ");
asm("   mov.l #_gdb_register_file+8, er0       ");

asm("   /* restore a few more registers */  ");
asm("   mov.l @er0+, er2                      ");
asm("   mov.l @er0+, er3                      ");
asm("   mov.l @er0+, er4                      ");
asm("   mov.l @er0+, er5                      ");
asm("   mov.l @er0+, er6                      ");
asm(" /* Next mov will restore sp to position before exception */ ");
asm("   mov.l @er0+, er7                      ");

asm("   /* put pc+ccr onto stack */         ");
asm("   /* pc  */                           ");
asm("   mov.l @er0+, er1                    ");
asm("   push.l er1                          ");

asm("   mov.l @er0+, er1                    ");
asm(" /* big endian - upper byte */         ");
asm("   mov.b r1l, @sp                      ");
asm("   /* pc, use and */                   ");     

#if USE_EXR
asm("   /* put exr onto stack */            ");
asm("   mov.l @er0+, er1                    ");
asm("   push.w r1                           ");
#else
asm("   adds #4, er0                        ");
#endif
asm("   /* do not restore MAC (at least now) */");

asm("   /* restore er1, er0 */              ");
asm("   add #-40, er0                       ");
asm("   /* er0 now points to (stored)er1*/  ");
asm("   mov.l @er0, er1                     ");
asm("   add.l #-4, er0                      ");  
asm("   mov.l @er0, er0                     ");

asm("   /* we're done-- return */           ");
asm("   rte                                 ");
