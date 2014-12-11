/*
  Copyright (c) 1999 by      William A. Gatliff
  All rights reserved.     bgat@open-widgets.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express
  or implied warranties, including, without limitation,
  the implied warranties of merchantability and fitness
  for a particular purpose.
  
  The author welcomes feedback regarding this file.
*/

/* $Id: sh2-704x-rmap.h,v 1.1.1.1 2002/03/19 02:53:13 bgat Exp $ */

#if !defined( SH2_704X_RMAP_H_INCLUDED )
#define SH2_704X_RMAP_H_INCLUDED


/* SCI */

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


/* UBC */

#define SH2_704X_UBC_UBARH  ((volatile short *)0xffff8600L)
#define SH2_704X_UBC_UBARL  ((volatile short *)0xffff8602L)
#define SH2_704X_UBC_UBAMRH ((volatile short *)0xffff8604L)
#define SH2_704X_UBC_UBAMRL ((volatile short *)0xffff8606L)
#define SH2_704X_UBC_UBBR   ((volatile short *)0xffff8608L)


/* BSC */

#define SH2_704X_BSC_WCR1   ((volatile short *)0xffff8624L)
#define SH2_704X_BSC_WCR2   ((volatile short *)0xffff8626L)
#define SH2_704X_BSC_BCR1   ((volatile short *)0xffff8620L)
#define SH2_704X_BSC_BCR2   ((volatile short *)0xffff8622L)
#define SH2_704X_BSC_DCR    ((volatile short *)0xffff862aL)
#define SH2_704X_BSC_RTCSR  ((volatile short *)0xffff862cL)
#define SH2_704X_BSC_RTCNT  ((volatile short *)0xffff862eL)
#define SH2_704X_BSC_RTCOR  ((volatile short *)0xffff8630L)


/* PFC */

#define SH2_704X_PFC_PAIORH ((volatile short *) 0xffff8384L)
#define SH2_704X_PFC_PAIORL ((volatile short *) 0xffff8386L)
#define SH2_704X_PFC_PACRH  ((volatile short *) 0xffff8388L)
#define SH2_704X_PFC_PACRL1 ((volatile short *) 0xffff838cL)
#define SH2_704X_PFC_PACRL2 ((volatile short *) 0xffff838eL)
#define SH2_704X_PFC_PBIOR  ((volatile short *) 0xffff8394L)
#define SH2_704X_PFC_PBCR1  ((volatile short *) 0xffff8398L)
#define SH2_704X_PFC_PBCR2  ((volatile short *) 0xffff839aL)
#define SH2_704X_PFC_PCIOR  ((volatile short *) 0xffff8396L)
#define SH2_704X_PFC_PCCR   ((volatile short *) 0xffff839cL)
#define SH2_704X_PFC_PDIORH ((volatile short *) 0xffff83a4L)
#define SH2_704X_PFC_PDIORL ((volatile short *) 0xffff83a6L)
#define SH2_704X_PFC_PDCRH1 ((volatile short *) 0xffff83a8L)
#define SH2_704X_PFC_PDCRH2 ((volatile short *) 0xffff83aaL)
#define SH2_704X_PFC_PDCRL  ((volatile short *) 0xffff83acL)
#define SH2_704X_PFC_PEIOR  ((volatile short *) 0xffff83b4L)
#define SH2_704X_PFC_PECR1  ((volatile short *) 0xffff83b8L)
#define SH2_704X_PFC_PECR2  ((volatile short *) 0xffff83baL)
#define SH2_704X_PFC_IFCR   ((volatile short *) 0xffff83c8L)


/* INTC */

#define SH2_704X_INTC_IPRG_ADI  0xf000
#define SH2_704X_INTC_IPRG_DTC  0x0f00
#define SH2_704X_INTC_IPRG_CMI0 0x00f0
#define SH2_704X_INTC_IPRG_CMI1 0x000f

#define SH2_704X_INTC_IPRA ((volatile short *) 0xffff8348L)
#define SH2_704X_INTC_IPRB ((volatile short *) 0xffff834aL)
#define SH2_704X_INTC_IPRC ((volatile short *) 0xffff834cL)
#define SH2_704X_INTC_IPRD ((volatile short *) 0xffff834eL)
#define SH2_704X_INTC_IPRE ((volatile short *) 0xffff8350L)
#define SH2_704X_INTC_IPRF ((volatile short *) 0xffff8352L)
#define SH2_704X_INTC_IPRG ((volatile short *) 0xffff8354L)
#define SH2_704X_INTC_IPRH ((volatile short *) 0xffff8356L)
#define SH2_704X_INTC_ICR  ((volatile short *) 0xffff8358L)
#define SH2_704X_INTC_ISR  ((volatile short *) 0xffff835aL)


/* IO */

#define SH2_704X_IO_PDDRH_PD27DR 0x0800

#define SH2_704X_IO_PADRH ((volatile short *) 0xffff8380L)
#define SH2_704X_IO_PADRL ((volatile short *) 0xffff8382L)
#define SH2_704X_IO_PBDR  ((volatile short *) 0xffff8390L)
#define SH2_704X_IO_PCDR  ((volatile short *) 0xffff8392L)
#define SH2_704X_IO_PDDRH ((volatile short *) 0xffff83a0L)
#define SH2_704X_IO_PDDRL ((volatile short *) 0xffff83a2L)
#define SH2_704X_IO_PEDR  ((volatile short *) 0xffff83b0L)
#define SH2_704X_IO_PFDR  ((volatile char  *) 0xffff83b3L)





#endif







