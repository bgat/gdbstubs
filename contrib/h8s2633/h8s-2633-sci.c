#include "h8s-2633-rmap.h"
#include "h8s-2633-sci.h"

void sci3_init(unsigned char baud, unsigned char config)
{
  SC_SCR3 = 0x00;                          // clear TE & RE in SCR
  SC_SMR3 = config;                        // set char length, parity, & # of stop bits
  SC_BRR3 = baud;                          // set baud rate

  SC_SCR3 = 0x30;                          // enable RE & TE                        
}

void sci3_putc(char ch)
{
  while((SC_SSR3&SCI_TDRE)==0); // wait for empty buffer ... 
  SC_TDR3=ch;
  SC_SSR3&=~SCI_TDRE;
}

int sci3_getc(void)
{
  unsigned char ch;
  do {
    if(SC_SSR3&(SCI_PER|SCI_FER|SCI_ORER)) {
      /* clear any detected errors */
      SC_SSR3 &= ~(SCI_PER|SCI_FER|SCI_ORER);
    }
    /* wait for a byte */
  } while(!(SC_SSR3&SCI_RDRF));

  /* got one-- return it */
  ch = SC_RDR3;
  SC_SSR3 &= ~SCI_RDRF;

  return ch;
}
