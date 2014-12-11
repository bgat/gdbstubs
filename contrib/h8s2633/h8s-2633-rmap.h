#ifndef H8S_2633_RMAP_H
#define H8S_2633_RMAP_H

#define R_UCHAR(a) (*(volatile unsigned char *)(a))

// Channel 0
#define SC_SMR0         R_UCHAR(0xFFFFFF78)   /* Serial mode       	*/
#define SC_BRR0         R_UCHAR(0xFFFFFF79)   /* Bit rate           	*/
#define SC_SCR0         R_UCHAR(0xFFFFFF7A)   /* Serial control       */
#define SC_TDR0         R_UCHAR(0xFFFFFF7B)   /* Transmit data        */
#define SC_SSR0         R_UCHAR(0xFFFFFF7C)   /* Serial status        */
#define SC_RDR0         R_UCHAR(0xFFFFFF7D)   /* Receive data         */
#define SC_SCMR0        R_UCHAR(0xFFFFFF7E)   /* Smart card mode      */

// Channel 1
#define SC_SMR1         R_UCHAR(0xFFFFFF80)   /* Serial mode          */
#define SC_BRR1         R_UCHAR(0xFFFFFF81)   /* Bit rate             */
#define SC_SCR1         R_UCHAR(0xFFFFFF82)   /* Serial control       */
#define SC_TDR1         R_UCHAR(0xFFFFFF83)   /* Transmit data        */
#define SC_SSR1         R_UCHAR(0xFFFFFF84)   /* Serial status        */
#define SC_RDR1         R_UCHAR(0xFFFFFF85)   /* Receive data         */
#define SC_SCMR1        R_UCHAR(0xFFFFFF86)   /* Smart card mode      */

// Channel 2
#define SC_SMR2         R_UCHAR(0xFFFFFF88)   /* Serial mode          */
#define SC_BRR2         R_UCHAR(0xFFFFFF89)   /* Bit rate             */
#define SC_SCR2         R_UCHAR(0xFFFFFF8A)   /* Serial control       */
#define SC_TDR2         R_UCHAR(0xFFFFFF8B)   /* Transmit data        */
#define SC_SSR2         R_UCHAR(0xFFFFFF8C)   /* Serial status        */
#define SC_RDR2         R_UCHAR(0xFFFFFF8D)   /* Receive data         */
#define SC_SCMR2        R_UCHAR(0xFFFFFF8E)   /* Smart card mode      */

// Channel 3
#define SC_SMR3         R_UCHAR(0xFFFFFDD0)   /* Serial mode          */
#define SC_BRR3         R_UCHAR(0xFFFFFDD1)   /* Bit rate             */
#define SC_SCR3         R_UCHAR(0xFFFFFDD2)   /* Serial control       */
#define SC_TDR3         R_UCHAR(0xFFFFFDD3)   /* Transmit data        */
#define SC_SSR3         R_UCHAR(0xFFFFFDD4)   /* Serial status        */
#define SC_RDR3         R_UCHAR(0xFFFFFDD5)   /* Receive data         */
#define SC_SCMR3        R_UCHAR(0xFFFFFDD6)   /* Smart card mode      */

// Channel 4
#define SC_SMR4         R_UCHAR(0xFFFFFDD8)   /* Serial mode          */
#define SC_BRR4         R_UCHAR(0xFFFFFDD9)   /* Bit rate             */
#define SC_SCR4         R_UCHAR(0xFFFFFDDA)   /* Serial control       */
#define SC_TDR4         R_UCHAR(0xFFFFFDDB)   /* Transmit data        */
#define SC_SSR4         R_UCHAR(0xFFFFFDDC)   /* Serial status        */
#define SC_RDR4         R_UCHAR(0xFFFFFDDD)   /* Receive data         */
#define SC_SCMR4        R_UCHAR(0xFFFFFDDE)   /* Smart card mode      */

#define SCI_PER  BIT(3)
#define SCI_FER  BIT(4) 
#define SCI_ORER BIT(5)

#define SCI_RDRF BIT(6)
#define SCI_TDRE BIT(7)

#endif
