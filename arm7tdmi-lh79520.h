/* 
  Copyright (c) 2003 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **
*/

#if !defined(ARM7TDMI_LH79520_H__INCLUDED)
#define ARM7TDMI_LH79520_H__INCLUDED

extern void arm7tdmi_lh79520_startup(void);

#define UART0BASE 0xfffc0000
#define UART1BASE 0xfffc1000
#define UART2BASE 0xfffc2000


/* TODO: yes, these should be enumerations! */

#define UART0DR    ((volatile unsigned long*)(UART0BASE + 0))
#define UARTDR_OE 0x800
#define UARTDR_BE 0x400
#define UARTDR_PE 0x200
#define UARTDR_FE 0x100


#define UART0RSR   ((volatile unsigned long*)(UART0BASE + 4))
#define UARTRSR_OE 8
#define UARTRSR_BE 4
#define UARTRSR_PE 2
#define UARTRSR_FE 1


#define UART0ECR   ((volatile unsigned long*)(UART0BASE + 4))
#define UARTECR_OE 8
#define UARTECR_BE 4
#define UARTECR_PE 2
#define UARTECR_FE 1


#define UART0FR    ((const volatile unsigned long*)(UART0BASE + 0x18))
#define UARTFR_TXFE 0x80
#define UARTFR_RXFF 0x40
#define UARTFR_TXFF 0x20
#define UARTFR_RXFE 0x10
#define UARTFR_BUSY 0x08


#define UART0ILPR  ((volatile unsigned long*)(UART0BASE + 0x20))
#define UART0IBRD  ((volatile unsigned long*)(UART0BASE + 0x24))
#define UART0FBRD  ((volatile unsigned long*)(UART0BASE + 0x28))


#define UART0LCR_H ((volatile unsigned long*)(UART0BASE + 0x2c))
#define UARTLCR_H_SPS 0x80
#define UARTLCR_H_WLEN 0x60
#define UARTLCR_H_FEN 0x10
#define UARTLCR_H_STP2 0x08
#define UARTLCR_H_EPS 0x04
#define UARTLCR_H_PEN 0x02
#define UARTLCR_H_BRK 0x01


#define UART0CR    ((volatile unsigned long*)(UART0BASE + 0x30))
#define UARTCR_RXE 0x200
#define UARTCR_TXE 0x100
#define UARTCR_LBE 0x80
#define UARTCR_SIRLP 4
#define UARTCR_SIREN 2
#define UARTCR_UARTEN 1


#define UART0IFLS  ((volatile unsigned long*)(UART0BASE + 0x34))
#define UARTFLS_RXIFLSEL 0x38
#define UARTFLS_TXIFLSEL 0x7


#define UART0IMSC  ((volatile unsigned long*)(UART0BASE + 0x38))
#define UARTIMSC_OEIM 0x400
#define UARTIMSC_BEIM 0x200
#define UARTIMSC_PEIM 0x100
#define UARTIMSC_FEIM 0x80
#define UARTIMSC_RTIM 0x40
#define UARTIMSC_TXIM 0x20
#define UARTIMSC_RXIM 0x10


#define UART0RIS   ((volatile unsigned long*)(UART0BASE + 0x3c))
#define UARTRIS_OEIM 0x400
#define UARTRIS_BEIM 0x200
#define UARTRIS_PEIM 0x100
#define UARTRIS_FEIM 0x80
#define UARTRIS_RTIM 0x40
#define UARTRIS_TXIM 0x20
#define UARTRIS_RXIM 0x10


#define UART0MIS   ((volatile unsigned long*)(UART0BASE + 0x40))
#define UARTMIS_OEIM 0x400
#define UARTMIS_BEIM 0x200
#define UARTMIS_PEIM 0x100
#define UARTMIS_FEIM 0x80
#define UARTMIS_RTIM 0x40
#define UARTMIS_TXIM 0x20
#define UARTMIS_RXIM 0x10


#define UART0ICR   ((volatile unsigned long*)(UART0BASE + 0x44))
#define UARTICR_OEIC 0x400
#define UARTICR_BEIC 0x200
#define UARTICR_PEIC 0x100
#define UARTICR_FEIC 0x80
#define UARTICR_RTIC 0x40
#define UARTICR_TXIC 0x20
#define UARTICR_RXIC 0x10

#define UART0TCR   ((volatile unsigned long*)(UART0BASE + 0x80))

#define UART1DR    ((volatile unsigned long*)(UART1BASE + 0))
#define UART1RSR   ((volatile unsigned long*)(UART1BASE + 4))
#define UART1ECR   ((volatile unsigned long*)(UART1BASE + 4))
#define UART1FR    ((volatile unsigned long*)(UART1BASE + 0x18))
#define UART1ILPR  ((volatile unsigned long*)(UART1BASE + 0x20))
#define UART1IBRD  ((volatile unsigned long*)(UART1BASE + 0x24))
#define UART1FBRD   ((volatile unsigned long*)(UART1BASE + 0x28))
#define UART1LCR_H ((volatile unsigned long*)(UART1BASE + 0x2c))
#define UART1CR    ((volatile unsigned long*)(UART1BASE + 0x30))
#define UART1IFLS  ((volatile unsigned long*)(UART1BASE + 0x34))
#define UART1IMSC  ((volatile unsigned long*)(UART1BASE + 0x38))
#define UART1RIS   ((volatile unsigned long*)(UART1BASE + 0x3c))
#define UART1MIS   ((volatile unsigned long*)(UART1BASE + 0x40))
#define UART1ICR   ((volatile unsigned long*)(UART1BASE + 0x44))

#define UART2DR    ((volatile unsigned long*)(UART2BASE + 0))
#define UART2RSR   ((volatile unsigned long*)(UART2BASE + 4))
#define UART2ECR   ((volatile unsigned long*)(UART2BASE + 4))
#define UART2FR    ((volatile unsigned long*)(UART2BASE + 0x18))
#define UART2ILPR  ((volatile unsigned long*)(UART2BASE + 0x20))
#define UART2IBRD  ((volatile unsigned long*)(UART2BASE + 0x24))
#define URTA2FBRD   ((volatile unsigned long*)(UART2BASE + 0x28))
#define UART2LCR_H ((volatile unsigned long*)(UART2BASE + 0x2c))
#define UART2CR    ((volatile unsigned long*)(UART2BASE + 0x30))
#define UART2IFLS  ((volatile unsigned long*)(UART2BASE + 0x34))
#define UART2IMSC  ((volatile unsigned long*)(UART2BASE + 0x38))
#define UART2RIS   ((volatile unsigned long*)(UART2BASE + 0x3c))
#define UART2MIS   ((volatile unsigned long*)(UART2BASE + 0x40))
#define UART2ICR   ((volatile unsigned long*)(UART2BASE + 0x44))

#endif

