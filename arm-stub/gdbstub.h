/* How to stuff for GDB-Stub */


#ifndef NO_SERIAL_IO
#define SER_IO 1
#else
#define SER_IO 0
#endif
#define CRT_IO 2
#define GDB_IO 4
#ifndef DEFAULT_IO 
#define DEFAULT_IO GDB_IO + SER_IO + CRT_IO
#endif

int stub_setio(int channel);
int setup_gdbstub(void);
volatile void breakpoint(void);

#define BREAKPOINT_UND() asm("   .word 0xe7fddef0");
#define BREAKPOINT() BREAKPOINT_UND()
#define HANDLE_UNDEF
#define HANDLE_IRQ

int check_ether_break(void);
int cli(void);
void smc_enable_int(int flag);
