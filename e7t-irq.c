#include "arm7tdmi-ks32c50100.h"

#if defined(CRT0)
__asm__(
	"   .global _mainCRTStartup\n"
	"   .global _start\n"
	"   .text\n"
	"_start:"
	"_mainCRTStartup:\n"
	"    mov r0, #0xd3\n"
	"    msr cpsr, r0\n"
	"    ldr r3, stack_end\n"
	"    mov sp, r3\n"
	"    mov r1, #0\n"
	"    ldr r0, bss_start\n"
	"    ldr r2, bss_end\n"
	"    sub r2, r2, a1\n"
	"    bl memset\n"
	"    bl main\n"
	"bss_start: .word __bss_start__\n"
	"bss_end:   .word __bss_end__\n"
	"stack_end: .word 0x80000\n"
	);
#endif

extern void IRQ_stub(void);
__asm__(
	"   .global IRQ_vector\n"
	"   .global IRQ_pending\n"
	"   .text\n"
	"   .global IRQ_stub\n"
	"IRQ_stub:\n"
	"    stmfd sp!, {r0 - r12, lr}\n"
	"    ldr r0, vector\n"
	"    ldr r1, [r0]\n"
	"    mov lr, pc\n"
	"    mov pc, r1\n"
	"    bl IRQ_pending\n"
	"    tst r0, #1\n"
	"    ldmfd sp!, {r0 - r12, lr}\n"
#if 0
	"    subeqs pc, lr, #4\n"
#else
	"    subs pc, lr, #4\n"
#endif
	"    ldr pc, Angel_IRQ_handler\n"
	"vector:  .word IRQ_vector\n"
	"Angel_IRQ_handler:  .word 0x7C870\n"
	);

void (*IRQ_vector)(void);

int IRQ_pending (void)
{
  /* return nonzero if any interrupts are pending */
  return (*INTPND & 0x1fffffUL) ? 1 : 0;
}

void IRQ_handler (void)
{
#define SEG_MASK 0x1fc00

  static const int SEG[] = {0x5f << 10, 6 << 10, 0x3b << 10, 0x2f << 10, 0x66 << 10,
			    0x6d << 10, 0x7d << 10, 7 << 10, 0x7f << 10, 0x6f << 10};
  static signed int seg;

  if (*INTPND & (1 << 10))
    {
      /* force io ports to output, turn everything off */
      *IOPMOD |= SEG_MASK;
      *IOPDATA &= ~SEG_MASK;

      /* output a digit */
      if (10 <= seg || 0 > seg) seg = 0;
      *IOPDATA |= SEG[seg++];
      
      /* clear the pending interrupt flag (acknowledge) */
      *INTPND = (1 << 10);
    }
}

#define COUNTS_PER_SECOND 50000000

int main (void)
{
  /* mask all interrupts, but not globally */
  *INTMSK = ~(1 << 21) & 0x3fffff;

  /* make sure the cpsr doesn't disable IRQ interrupts */
  __asm__ volatile (" msr cpsr, %0"
		    : /* no outputs */
		    : "r"(0x53)
		    : "cc");
  
  /* install our IRQ stub, handler */
  *(unsigned long*)0x38 = (unsigned long)IRQ_stub;
  IRQ_vector = IRQ_handler;

  /* set timer0 */
  *TDATA0 = COUNTS_PER_SECOND;
  *TCNT0 = 0;

  /* unmask, clear any pending TIMER0 interrupts */
  *INTPND = (1 << 10);
  *INTMSK &= ~(1 << 10);

  /* start timer0 */
  *TMOD |= TMOD_TE0;

  while(1)
    ;
}



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

module_header_t module_header =
  {
    magic: MODULE_MAGIC,
    major: MAJOR_VERSION,
    minor: MINOR_VERSION,
    self: &module_header,
    start: _start,
    title: "gdbstub",
    help:  "gdbstub         v0.01 Apr  4 2003",
    ro_base: &__text_start__,
    ro_limit: &__text_end__,
  };
