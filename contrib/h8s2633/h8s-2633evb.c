/*
  Copyright (c) 1999 by      William A. Gatliff
  All rights reserved.     bgat@open-widgets.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.

  The author welcomes feedback regarding this file.
  
  Some modifications by by Petr Ledvina, 
  ledvinap@kae.zcu.cz, 2001/10
  
  This stub is designed to be loaded into evb using 
  HDI. 
  I use SCI4 for gdb because sci2(connected through 
  flash programming board) is shared with memory line
  It's quite easy to wire it from expansion port to FPB
*/

/* $Id: h8s-2633evb.c,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */

#include "gdb.h"
#include "h8s-2633.h"
#include "h8s-2633-sci.h"

/* these symbols are created by the linker
   to help us find the text, bss and data
   sections, respectively; they aren't
   actual allocations */
/*
extern char _data_start;
extern char _data_end;
extern const char _text_end;
*/
extern char _s_bss;
extern char _se_bss;
extern char _se_stack;


/* our stack */
long stack[400] __attribute__((section(".stack")));


/* simple startup code that just forces us
   to give control to the debugging stub */
int main ( void )
{
  while( 1 )
    gdb_monitor( 5 );

  return 0;
}


/*
  Initializes peripheral hardware,
  sets up the C runtime environment,
  then calls main().
 */
void cstart ( void )
{
  /* register const char *src; */
  register char *dst;


  while( 1 ) {
    
    /* initialize hardware */
    sci3_init(SCI_B115200, SCI_8N1);
    
#if 0     // evb monitor will do it now
    /* disable this if code doesn't have any
       initialized global data, because we
       currently can't tell automatically if
       it's there or not */

    /* initialize data */
    for( dst = &_data_start, src = &_text_end;
	 dst < &_data_end;
	 dst++, src++ ) {

      *dst = *src;
    }
#endif
    /* zero out bss */
    for( dst = &_s_bss; dst < &_se_bss; dst++ ) {
      *dst = 0;
    }

    /* call main */
    main();
  }
}


/*
  Gotta have this, because gcc wants it.
  (related to gcc's c++ support)
*/
void __main()
{
}



/*
  THE startup function.

  Initializes the stack pointer, status register
  then calls cstart().
*/
extern void start ( void );
asm( ".align 2                                   " );
asm( ".global _start                             " );

asm( "_start:                                    " );
/* initialize stack */
asm( "   mov.l #__se_stack, sp                   " );

/* load known values into a few registers (for testing) */
asm( "   mov   #0, r0                            " );
asm( "   mov   #1, r1                            " );
asm( "   mov   #2, r2                            " );
asm( "   mov   #3, r3                            " );
asm( "   mov   #4, r4                            " );
asm( "   mov   #5, r5                            " );
asm( "   mov   #6, r6                            " );

/*  jump to C */
asm( "   bra   _cstart                           " );

