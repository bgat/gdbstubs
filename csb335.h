/* 
  Copyright (c) 2001 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **
*/

#if !defined(ARM7TDMI_KS32c50100_H__INCLUDED)
#define ARM7TDMI_KS32C50100_H__INCLUDED

extern void arm7tdmi_ks32c50100_startup(void);

#define SYSCFG 0x3ff0000


#define IOPMOD ((volatile unsigned long*)(SYSCFG + 0x5000))
#define IOPMOD_MASK 0x1ffff

#define IOPCON ((volatile unsigned long*)(SYSCFG + 0x5004))
#define IOPCON_MASK 0x1ffff

#define IOPDATA ((volatile unsigned long*)(SYSCFG + 0x5008))
#define IOPDATA_MASK 0x1ffff


#define INTMOD      ((volatile unsigned long*)(SYSCFG + 0x4000))
#define INTPND      ((volatile unsigned long*)(SYSCFG + 0x4004))
#define INTMSK      ((volatile unsigned long*)(SYSCFG + 0x4008))
#define INTPRI0     ((volatile unsigned long*)(SYSCFG + 0x400c))
#define INTPRI1     ((volatile unsigned long*)(SYSCFG + 0x4010))
#define INTPRI2     ((volatile unsigned long*)(SYSCFG + 0x4014))
#define INTPRI3     ((volatile unsigned long*)(SYSCFG + 0x4018))
#define INTPRI4     ((volatile unsigned long*)(SYSCFG + 0x401c))
#define INTPRI5     ((volatile unsigned long*)(SYSCFG + 0x4020))
#define INTOFFSET   ((volatile unsigned long*)(SYSCFG + 0x4024))
#define INTOSET_FIQ ((volatile unsigned long*)(SYSCFG + 0x4030))
#define INTOSET_IRQ ((volatile unsigned long*)(SYSCFG + 0x4034))
#define INTPNDPRI   ((volatile unsigned long*)(SYSCFG + 0x4028))
#define INTPNDTST   ((volatile unsigned long*)(SYSCFG + 0x402c))


#define TMOD        ((volatile unsigned long*)(SYSCFG + 0x6000))
#define TMOD_TE0    1
#define TMOD_TMD0   2
#define TMOD_TCLR0  4
#define TMOD_TE1    8
#define TMOD_TMD1   0x10
#define TMOD_TCLR1  0x20

#define TDATA0      ((volatile unsigned long*)(SYSCFG + 0x6004))
#define TDATA1      ((volatile unsigned long*)(SYSCFG + 0x6008))
#define TCNT0       ((volatile unsigned long*)(SYSCFG + 0x600c))
#define TCNT1       ((volatile unsigned long*)(SYSCFG + 0x6010))


#define ULCON0  ((volatile unsigned long*      )(SYSCFG + 0xD000))
#define ULCON1  ((volatile unsigned long*      )(SYSCFG + 0xE000))
#define UCON0   ((volatile unsigned long*      )(SYSCFG + 0xD004))
#define UCON1   ((volatile unsigned long*      )(SYSCFG + 0xE004))
#define USTAT0  ((volatile const unsigned long*)(SYSCFG + 0xD008))
#define USTAT1  ((volatile const unsigned long*)(SYSCFG + 0xE008))
#define UTXBUF0 ((volatile unsigned long*      )(SYSCFG + 0xD00C))
#define UTXBUF1 ((volatile unsigned long*      )(SYSCFG + 0xE00C))
#define URXBUF0 ((volatile const unsigned long*)(SYSCFG + 0xD010))
#define URXBUF1 ((volatile const unsigned long*)(SYSCFG + 0xE010))
#define UBRDIV0 ((volatile unsigned long*      )(SYSCFG + 0xD014))
#define UBRDIV1 ((volatile unsigned long*      )(SYSCFG + 0xE014))
#define BRDCNT0 ((volatile unsigned long*      )(SYSCFG + 0xD018))
#define BRDCNT1 ((volatile unsigned long*      )(SYSCFG + 0xE018))
#define BRDCLK0 ((volatile unsigned long*      )(SYSCFG + 0xD01C))
#define BRDCLK1 ((volatile unsigned long*      )(SYSCFG + 0xE01C))

#define ULCON_WL   0x03
#define ULCON_STB  0x04
#define ULCON_PMD  0x38
#define ULCON_SC   0x40
#define ULCON_IR   0x80

#define UCON_RXM  0x03
#define UCON_RXSI 0x04
#define UCON_TXM  0x18
#define UCON_DSR  0x20
#define UCON_SBK  0x40
#define UCON_LPB  0x80

#define USTAT_OV  0x01
#define USTAT_PE  0x02
#define USTAT_FE  0x04
#define USTAT_BKD 0x08
#define USTAT_DTR 0x10
#define USTAT_RDR 0x20
#define USTAT_TBE 0x40
#define USTAT_TC  0x80

#endif

