/*
  Copyright (c) 2001 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.

  The author welcomes feedback regarding this file.
*/

/* $Id: sh2-7045edk.c,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */

/* code for the Hitachi SH7045 EDK */

#include "gdb.h"


#define SH2_704X_SCI_SSR_TDRE 0x80
#define SH2_704X_SCI_SSR_RDRF 0x40
#define SH2_704X_SCI_SSR_ORER 0x20
#define SH2_704X_SCI_SSR_FER  0x10
#define SH2_704X_SCI_SSR_PER  0x08
#define SH2_704X_SCI_SSR_TEND 0x04
#define SH2_704X_SCI_SSR_MPB  0x02
#define SH2_704X_SCI_SSR_MPBT 0x01

#define SH2_704X_SCI_SMR0 ((volatile char *)0xffff81a0L) 
#define SH2_704X_SCI_BRR0 ((volatile char *)0xffff81a1L)
#define SH2_704X_SCI_SCR0 ((volatile char *)0xffff81a2L)
#define SH2_704X_SCI_TDR0 ((volatile char *)0xffff81a3L)
#define SH2_704X_SCI_SSR0 ((volatile char *)0xffff81a4L)
#define SH2_704X_SCI_RDR0 ((volatile char *)0xffff81a5L)

#define SH2_704X_SCI_SMR1 ((volatile char *)0xffff81b0L) 
#define SH2_704X_SCI_BRR1 ((volatile char *)0xffff81b1L)
#define SH2_704X_SCI_SCR1 ((volatile char *)0xffff81b2L)
#define SH2_704X_SCI_TDR1 ((volatile char *)0xffff81b3L)
#define SH2_704X_SCI_SSR1 ((volatile char *)0xffff81b4L)
#define SH2_704X_SCI_RDR1 ((volatile char *)0xffff81b5L)

#define SH2_704X_UBC_UBARH  ((volatile short *)0xffff8600L)
#define SH2_704X_UBC_UBARL  ((volatile short *)0xffff8602L)
#define SH2_704X_UBC_UBAMRH ((volatile short *)0xffff8604L)
#define SH2_704X_UBC_UBAMRL ((volatile short *)0xffff8606L)
#define SH2_704X_UBC_UBBR   ((volatile short *)0xffff8608L)


#define TDRE SH2_704X_SCI_SSR_TDRE
#define RDRF SH2_704X_SCI_SSR_RDRF
#define PER  SH2_704X_SCI_SSR_PER
#define FER  SH2_704X_SCI_SSR_FER
#define ORER SH2_704X_SCI_SSR_ORER


















#include "sh2-704x.h"


/* these symbols are created by the linker
   to help us find the text, bss and data
   sections, respectively; they aren't
   actual allocations */
extern char _data_start;
extern char _data_end;
extern const char _text_end;
extern char _bss_start;
extern char _bss_end;
extern char _stack_end;


/* our stack */
long stack[400] __attribute__((section(".gdbstack")));


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
  register const char *src;
  register char *dst;


  while( 1 ) {
    
    /* initialize hardware */

    *SH2_704X_BSC_WCR1 = 0xff3f;
    *SH2_704X_BSC_WCR2 = 0x000f;
    *SH2_704X_BSC_BCR1 = 0x2020;
    *SH2_704X_BSC_BCR2 = 0xf3dd;
    *SH2_704X_BSC_DCR = 0;
    *SH2_704X_BSC_RTCSR = 0;
    *SH2_704X_BSC_RTCNT = 0;
    *SH2_704X_BSC_RTCOR = 0;

    *SH2_704X_PFC_PACRH = 0x5000;
    *SH2_704X_PFC_PACRL1 = 0x1550;

    /* enable rx1, tx1, rx0 and tx0 lines */
    *SH2_704X_PFC_PACRL2 = 0x0145;
  
    *SH2_704X_PFC_PBCR2 = 0x2005;
    *SH2_704X_PFC_PDCRH1 = 0x5555;
    *SH2_704X_PFC_PDCRH2 = 0x5555;
    *SH2_704X_PFC_PDCRL = 0xffff;
    *SH2_704X_PFC_PCCR = 0xffff;
    *SH2_704X_PFC_IFCR = 0;
    *SH2_704X_PFC_PEIOR = 0x0020;
    *SH2_704X_IO_PEDR = 0x0020;

    /* set CMI0 and ADI0 irq priorities to 1 */
    *SH2_704X_INTC_IPRG = 0x1010;

    /* set DEI0 and DEI1 irq priorities to 1 */
    *SH2_704X_INTC_IPRC = 0x1100;

    /* set up sci0 for 115k asynch,
       re + te, no interrupts */
    *SH2_704X_SCI_SMR0 = 0;
    *SH2_704X_SCI_BRR0 = 7;
    *SH2_704X_SCI_SCR0 = 0x30;

    /* set up sci1 for 115k asynch,
       re + te, no interrupts */
    *SH2_704X_SCI_SMR1 = 0;
    *SH2_704X_SCI_BRR1 = 7; /* 7 == 115k, 23 == 38.4k */
    *SH2_704X_SCI_SCR1 = 0x30;

    /* turn on the 7045EDK's green led */
    *SH2_704X_PFC_PEIOR = 0x20;
    *SH2_704X_IO_PEDR = 0;

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

    /* zero out bss */
    for( dst = &_bss_start;
	 dst < &_bss_end;
	 dst++ ) {

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
  and vector base register, then calls cstart().
*/
extern void start ( void );
asm( ".align 2                                   " );
asm( ".global _start                             " );

asm( "_start:                                    " );

/* pad to stay out of another's delay slot */
asm( "   nop                                     " );

/* load known values into a few registers (for testing) */
asm( "   mov   #0, r0                            " );
asm( "   mov   #1, r1                            " );
asm( "   mov   #2, r2                            " );
asm( "   mov   #3, r3                            " );
asm( "   mov   #4, r4                            " );
asm( "   mov   #5, r5                            " );
asm( "   mov   #6, r6                            " );
asm( "   mov   #7, r7                            " );
asm( "   mov   #8, r8                            " );
asm( "   mov   #9, r9                            " );
asm( "   mov   #10, r10                          " );
asm( "   mov   #11, r11                          " );
asm( "   mov   #12, r12                          " );
asm( "   mov   #13, r13                          " );
asm( "   mov   #14, r14                          " );

/*  jump to C */
asm( "   bra   _cstart                           " );
asm( "   nop                                     " );



const void *vector_table[]
__attribute__((section(".vect"))) = {

  /* power-on reset pc, r15 */
  start,
  &_stack_end,

  /* manual reset pc, r15 */
  start,
  &_stack_end, /* stack gets set up in start() */
  
  /* general illegal instruction */
  gdb_unhandled_isr,
  
  /* (reserved by system) */
  gdb_unhandled_isr,
  
  /* slot illegal instruction */
  gdb_unhandled_isr,
  
  /* (reserved by system) */
  gdb_unhandled_isr,
  
  /* (reserved by system) */
  gdb_unhandled_isr,
  
  /* cpu address error */
  gdb_unhandled_isr,

  /* dmac/dtc address error */
  gdb_unhandled_isr,
  
  /* nmi interrupt */
  gdb_unhandled_isr,
  
  /* ubc interrupt */
  gdb_ubc_isr,
  
  /* (reserved by system: 13--31) */
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  
  /* trapa 32--63 */
  gdb_trapa32_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  /* irq0 */
  gdb_unhandled_isr,
  
  /* irq1 */
  gdb_unhandled_isr,
  
  /* irq2 */
  gdb_unhandled_isr,
  
  /* irq3 */
  gdb_unhandled_isr,
  
  /* irq4 */
  gdb_unhandled_isr,
  
  /* irq5 */
  gdb_unhandled_isr,

  /* irq6 */
  gdb_unhandled_isr,
  
  /* irq7 */
  gdb_unhandled_isr,
  
  /* on-chip perhiperal modules, 72--255 */
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,

  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
  gdb_unhandled_isr,
};
