/*
  Copyright (c) 2001 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.

  The author welcomes feedback regarding this file.

  $Id: sh2-edk.c,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $
*/

/*
   Code for various Hitachi EDKs:

   ARCH=SH2 TARGET=SH2_7045EDK IO=SCI0  - Hitachi's 7045 EDK on SCI0

*/

/* TODO: zero .gdbregfile section */

#include "gdb.h"
#include "sh2.h"


#define PFC_PACRL2 ((volatile short *) 0xffff838eL)
#define PFC_PEIOR  ((volatile short *) 0xffff83b4L)
#define IO_PEDR  ((volatile short *) 0xffff83b0L)

#define SCI_SMR0 ((volatile char *)0xffff81a0L) 
#define SCI_BRR0 ((volatile char *)0xffff81a1L)
#define SCI_SCR0 ((volatile char *)0xffff81a2L)
#define SCI_TDR0 ((volatile char *)0xffff81a3L)
#define SCI_SSR0 ((volatile char *)0xffff81a4L)
#define SCI_RDR0 ((volatile char *)0xffff81a5L)

#define SCI_SMR1 ((volatile char *)0xffff81b0L) 
#define SCI_BRR1 ((volatile char *)0xffff81b1L)
#define SCI_SCR1 ((volatile char *)0xffff81b2L)
#define SCI_TDR1 ((volatile char *)0xffff81b3L)
#define SCI_SSR1 ((volatile char *)0xffff81b4L)
#define SCI_RDR1 ((volatile char *)0xffff81b5L)

#define TDRE 0x80
#define RDRF 0x40
#define PER  0x08
#define FER  0x10
#define ORER 0x20


#if defined(SH2_7045EDK)
#define SSR SCI_SSR1
#define TDR SCI_TDR1
#define RDR SCI_RDR1
#define SMR SCI_SMR1
#define BRR SCI_BRR1
#define SCR SCI_SCR1
#define PACRL2 0x0140 /* use 0x45 for SCI0 */
#define BRR_LOAD 7 /* 115k? */
#endif


void gdb_putc (char c)
{
  while (!(*SSR & TDRE));
  *TDR = c;
  *SSR &= ~TDRE;
  return;
}

char gdb_getc (void)
{
  char c;

  *SSR &= ~( PER | FER | ORER );
  while (!(*SSR & RDRF));
  c = *RDR;
  *SSR &= ~RDRF;
  return c;
}

void gdb_startup (void)
{
  /* enable txd/rxd lines */
  *PFC_PACRL2 = PACRL2;
  
  /* re + te, no interrupts */
  *SMR = 0;
  *BRR = BRR_LOAD;
  *SCR = 0x30;

  /* turn on the 7045EDK's green led */
  *PFC_PEIOR = 0x20;
  *IO_PEDR = 0;
  
  return;
}

/*
   Writes back data caches, and invalidates instruction caches.  It
   goes here because not all applications use the cache.

   TODO: implement flush_cache().
*/
void gdb_flush_cache (void *start, int len) { return; }


#if defined(CRT0)

/* A basic startup function. */
extern void start(void);

__asm__("

.align 2
.global _start
.global start
start:
_start:
  nop
  mov   #0, r0
  mov   #1, r1
  mov   #2, r2
  mov   #3, r3
  mov   #4, r4
  mov   #5, r5
  mov   #6, r6
  mov   #7, r7
  mov   #8, r8
  mov   #9, r9
  mov   #10, r10
  mov   #11, r11
  mov   #12, r12
  mov   #13, r13
  mov   #14, r14

  mov.l gdbstartup_k, r0
  jsr @r0
  nop

  mov.l gdbmonitor_k, r0
  mov #5, r4
  jsr @r0
  nop

  bra _start
  nop

.align 2
gdbstartup_k: .long _gdb_startup
gdbmonitor_k: .long _gdb_enter_monitor

");



const void *vector_table[]
__attribute__((section(".vect"))) = {

  /* power-on reset pc, r15 */
  start,
  (void*)0xfffffffc,

  /* manual reset pc, r15 */
  start,
  (void*)0xfffffffc,
  
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
  /* gdb_ubc_isr, */ gdb_unhandled_isr,
  
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
  gdb_unhandled_isr
};

#endif
