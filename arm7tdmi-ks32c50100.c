/* 
  Copyright (c) 2001 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **

  $Id: arm7tdmi-ks32c50100.c,v 1.6 2003/05/27 15:46:13 bgat Exp $
*/


#define CPU_HZ 50000000

#include "gdb.h"
#include "arm7tdmi.h"
#include "arm7tdmi-ks32c50100.h"

#define UTXBUF UTXBUF1
#define URXBUF URXBUF1
#define USTAT  USTAT1
#define UCON   UCON1
#define UBRDIV UBRDIV1


int gdb_putc (int c)
{
  while (!(USTAT_TBE & *USTAT))
    ;
  *UTXBUF = c;
  return c & 0xff;
}

int gdb_getc (void)
{
  int retval;

  while (!(USTAT_RDR & *USTAT))
    ;
  retval = *URXBUF;
  return retval;
}

void arm7tdmi_ks32c50100_startup (void)
{
  int brgout;
  int bps;

  // force polled i/o on both uart channels */
  *INTMSK |= ((1 << 7) + (1 << 6));
  *INTMSK |= ((1 << 5) + (1 << 4));

  *UCON = (*UCON & ~UCON_TXM) | 8;
  *UCON = (*UCON & ~UCON_RXM) | 1;
  
  // TODO: some math here

#warning MCLK2 == 50 MHz, bps == 9600 assumed.

  bps = 9600;

  // TODO: compute appropriate CNT0 and CNT1

  // from the manual:
  // BRGOUT = (MCLK2 or UCLK) / (CNT0 + 1) / (16^CNT1) / 16
  // i.e. 9600 = (50000000)/(CNT0 + 1)/(16^(CNT1))/16
  // where CNT1 == [0|1], so (16^CNT1) == [0|16]
  // (no other CNT1 values are legal)

  brgout = 162 << 4;
  *UBRDIV = brgout;
  return;
}


void gdb_kill (void)
{
}

void gdb_detach (void)
{
}

void update_leds (void)
{
#define SEG_MASK 0x1fc00

  static const int SEG[] = {0x5f << 10, 6 << 10, 0x3b << 10, 0x2f << 10,
			    0x66 << 10, 0x6d << 10, 0x7d << 10, 7 << 10,
			    0x7f << 10, 0x6f << 10};
  static int seg;
  
  /* force io ports to output, turn everything off */
  *IOPMOD |= SEG_MASK;
  *IOPDATA &= ~SEG_MASK;
  
  /* output a digit */
  if (10 <= seg) seg = 0;
  *IOPDATA |= SEG[seg++];
}
  
#if defined(CRT0)
#define MODULE_MAGIC 0x4d484944

#define UNPLUGGED_FLAG 1
#define AUTOSTART_FLAG 2
#define MAJOR_VERSION 1
#define MINOR_VERSION 1

typedef struct module_header_t module_header_t;

struct module_header_t {
  unsigned magic;
  unsigned flags:16;
  unsigned major:8;
  unsigned minor:8;
  unsigned checksum;
  void *ro_base;
  void *ro_limit;
  void *rw_base;
  void *zi_base;
  void *zi_limit;
  const module_header_t *self;
  void *start;
  void *init;
  void *final;
  void *service;
  const char *title;
  const char *help;
  void *cmdtbl;
  void *swi_base;
  void *swi_code;
}  __attribute__((packed));


extern char __text_start__, __text_end__;
extern void _start(void);
const module_header_t module_header __attribute__((aligned(4))) =
  {
    magic: MODULE_MAGIC,
    major: MAJOR_VERSION,
    minor: MINOR_VERSION,
    self: &module_header,
    start: _start,
    init: _start,
    title: "gdbstub",
    help:  "gdbstub         v0.01 May 22 2003",
    ro_base: &__text_start__,
    ro_limit: &__text_end__,
  };

#endif
