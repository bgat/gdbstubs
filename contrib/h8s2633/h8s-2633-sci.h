#ifndef H8S_2633_SCI_H
#define H8S_2633_SCI_H

#ifndef BIT
#define BIT(n)    (1 << n)
#endif

#define CPU_HZ        (18432000)        // processor speed in hertz
#define SCI_BAUD(n)   (unsigned char)((CPU_HZ / (32.0 * (n))) - .5)
#define SCI_B9600     SCI_BAUD(9600)
#define SCI_B115200   SCI_BAUD(115200)

#define SMR_8BIT  (0)
#define SMR_7BIT  BIT(6)
#define SMR_NONE  (0)
#define SMR_EVEN  BIT(5)
#define SMR_ODD   BIT(5) + BIT(4)
#define SMR_1STOP (0)
#define SMR_2STOP BIT(3)

/* Definitions for typical SCI settings */
#define SCI_8N1   (unsigned char)(SMR_8BIT + SMR_NONE + SMR_1STOP)
#define SCI_7N1   (unsigned char)(SMR_7BIT + SMR_NONE + SMR_1STOP)
#define SCI_8N2   (unsigned char)(SMR_8BIT + SMR_NONE + SMR_2STOP)
#define SCI_7N2   (unsigned char)(SMR_7BIT + SMR_NONE + SMR_2STOP)
#define SCI_8E1   (unsigned char)(SMR_8BIT + SMR_EVEN + SMR_1STOP)
#define SCI_7E1   (unsigned char)(SMR_7BIT + SMR_EVEN + SMR_1STOP)
#define SCI_8E2   (unsigned char)(SMR_8BIT + SMR_EVEN + SMR_2STOP)
#define SCI_7E2   (unsigned char)(SMR_7BIT + SMR_EVEN + SMR_2STOP)
#define SCI_8O1   (unsigned char)(SMR_8BIT + SMR_ODD  + SMR_1STOP)
#define SCI_7O1   (unsigned char)(SMR_7BIT + SMR_ODD  + SMR_1STOP)
#define SCI_8O2   (unsigned char)(SMR_8BIT + SMR_ODD  + SMR_2STOP)
#define SCI_7O2   (unsigned char)(SMR_7BIT + SMR_ODD  + SMR_2STOP)

void sci3_init(unsigned char baud, unsigned char config);
void sci3_putc(char ch);
int sci3_getc(void);


#endif
