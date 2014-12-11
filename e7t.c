#if 0
#include <stdio.h>
#endif
int main (void)
{
#if 0
  printf("Hello, world!\n");
#endif
  return 0;
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
  module_header_t *self;
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


extern char _etext,__data_start, __bss_start__, __bss_end__;

module_header_t module_header =
  {
    magic: MODULE_MAGIC,
    major: MAJOR_VERSION,
    minor: MINOR_VERSION,
    self: &module_header,
    checksum: 0,
    title: "gdbstub",
    help: "gdbstub         v0.01 Apr  4 2003",
    ro_base: (void*)0x8000,
    ro_limit: &_etext,
    rw_base: &__data_start,
    zi_base: &__bss_start__,
    zi_limit: &__bss_end__
  };
