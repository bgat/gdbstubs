/* 
  Copyright (c) 2001 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **

  $Id: csb335.c,v 1.3 2003/12/11 15:30:14 bgat Exp $
*/


#define CPU_HZ 77000000

#include "gdb.h"
#include "arm7tdmi.h"
#include "arm7tdmi-lh79520.h"

#define UARTFR UART0FR
#define UARTDR UART0DR

extern void arm7tdmi_und_handler (void);


void arm7tdmi_lh79520_startup (void)
{
  /* TODO: for now, assume uMON has us covered */
}


int gdb_putc (int c)
{
  while (!(UARTFR_TXFE & *UARTFR))
    ;
  *UARTDR = c;
  return c & 0xff;
}


int gdb_getc (void)
{
  int retval;
  while ((UARTFR_RXFE & *UARTFR))
    ;
  retval = *UARTDR;
  return retval;
}


void gdb_kill (void)
{
}


void gdb_detach (void)
{
}
  

/* uMON sets us up pretty nice, stick with that */
void _start (void)
{
  const int cpsr_system = 0xdf;

  /* go to System mode, in case we aren't there already */
  __asm__ volatile ("msr cpsr, %0" : : "r"(cpsr_system) : "cc");
  
  /* remap memory, in case it isn't already */
  *(long*)0xfffe2008 = 1;

  /* write an 'ldr pc, [pc, #0x18]' to the exception vector table */
  *(long*)0x4 = 0xe59ff018;
  
  /* aim the undefined exception to arm7tdmi_und_handler */
  *(long*)0x24 = (long)arm7tdmi_und_handler;

  gdb_startup();
	
  /* load some registers, force a "breakpoint" */
  __asm__ volatile (" mov r0, #0");
  __asm__ volatile (" mov r1, #1");
  __asm__ volatile (" mov r2, #2");
  __asm__ volatile (" mov r3, #3");
  __asm__ volatile (" mov r4, #4");
  __asm__ volatile (" mov r5, #5");
  __asm__ volatile (" mov r6, #6");
  __asm__ volatile (" mov r7, #7");
  __asm__ volatile (".word 0xe7ffdefe");

  /* some test code */
  __asm__ volatile ("top:	mov r0, #0");
  __asm__ volatile ("	mov r1, #1");
  __asm__ volatile ("	mov r2, #2");
  __asm__ volatile ("	mov r3, #3");
  __asm__ volatile ("	mov r4, #4");
  __asm__ volatile ("	mov r5, #5");
  __asm__ volatile ("	mov r6, #6");
  __asm__ volatile ("	mov r7, #7");
  __asm__ volatile ("	b 1f");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("1:	nop");
  __asm__ volatile ("	mov lr, pc");
  __asm__ volatile ("	add lr, lr, #12");
  __asm__ volatile ("	mov pc, lr");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	mov lr, pc");
  __asm__ volatile ("	add lr, lr, #20");
  __asm__ volatile ("	stmfd sp!, {r1,lr}");
  __asm__ volatile ("	mov r1, #0");
  __asm__ volatile ("	ldmfd sp!, {r1,pc}");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	nop");
  __asm__ volatile ("	b top");
}
