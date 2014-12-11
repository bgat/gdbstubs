/****************************************************************************
 *  Header: remcom.c,v 1.34 91/03/09 12:29:49 glenne Exp $
 *
 *  Module name: remcom.c $
 *  Revision: 1.34 $
 *  Date: 91/03/09 12:29:49 $
 *  Contributor:     Lake Stevens Instrument Division$
 *
 *  Description:     low level support for gdb debugger. $
 *
 *  Considerations:  only works on target hardware $
 *
 *  Written by:      Glenn Engel $
 *  ModuleState:     Experimental $
 *
 *  NOTES:           See Below $
 *
 *  Modified for Strong Arm 1100 by Thilo Jeremias, Keith & Koep GmbH
 *  Modified for 386 by Jim Kingdon, Cygnus Support.
 *
 *  To enable debugger support, two things need to happen.  One, a
 *  call to set_debug_traps() is necessary in order to allow any breakpoints
 *  or error conditions to be properly intercepted and reported to gdb.
 *  Two, a breakpoint needs to be generated to begin communication.  This
 *  is most easily accomplished by a call to breakpoint().  Breakpoint()
 *  simulates a breakpoint by executing a trap #1.
 *
 *  The external function exceptionHandler() is
 *  used to attach a specific handler to a specific 386 vector number.
 *  It should use the same privilege level it runs at.  It should
 *  install it as an interrupt gate so that interrupts are masked
 *  while the handler runs.
 *
 *  Because gdb will sometimes write to the stack area to execute function
 *  calls, this program cannot rely on using the supervisor stack so it
 *  uses it's own stack area reserved in the int array remcomStack.
 *
 *************
 *
 *    The following gdb commands are supported:
 *
 * command          function                               Return value
 *
 *    g             return the value of the CPU registers  hex data or ENN
 *    G             set the value of the CPU registers     OK or ENN
 *
 *    mAA..AA,LLLL  Read LLLL bytes at address AA..AA      hex data or ENN
 *    MAA..AA,LLLL: Write LLLL bytes at address AA.AA      OK or ENN
 *
 *    c             Resume at current address              SNN   ( signal NN)
 *    cAA..AA       Continue at address AA..AA             SNN
 *
 *    s             Step one instruction                   SNN
 *    sAA..AA       Step one instruction from AA..AA       SNN
 *
 *    k             kill
 *
 *    ?             What was the last sigval ?             SNN   (signal NN)
 *
 * All commands and responses are sent with a packet which includes a
 * checksum.  A packet consists of
 *
 * $<packet info>#<checksum>.
 *
 * where
 * <packet info> :: <characters representing the command or response>
 * <checksum>    :: < two hex digits computed as modulo 256 sum of <packetinfo>>
 *
 * When a packet is received, it is first acknowledged with either '+' or '-'.
 * '+' indicates a successful transfer.  '-' indicates a failed transfer.
 *
 * Example:
 *
 * Host:                  Reply:
 * $m0,10#2a               +$00010203040506070809101112131415#42
 *
 ****************************************************************************/

#include <stdio.h>
//#include <string.h>
#include "misc.h"
#include "gdbstub.h"
#include "hard.h"

/************************************************************************
 *
 * external low-level support routines
 */
#include "proto.h"

#define printk printf
extern int printk(const char *fmt, ...);

struct gpio_regs      *gpio   =(struct gpio_regs     *)0x90040000;
struct int_ctrl_regs  *intctrl=(struct int_ctrl_regs *)0x90050000;

int     remote_debug;
/*  debug >  0 prints ill-formed commands in valid packets & checksum errors */
#define debug_error( a, b ) if( remote_debug ) printk( a,b);

#define SER_IF	        0
#define ETHERNET_IF     1
#define ETHER_INT       10
#define KEITH           1
/***********************************************************************/

/***********************************************************************/
int  getserpacket(char *buffer);
void putserpacket(char *buffer);
extern int  init_net(void);				  /* Setup Ethernet & BootP request */
extern void put_udp_pckt(char *buffer);   /* Use Ethernet for sending data */
extern int  get_udp_pckt(char *buffer);   /* Use Ethernet for receiving data */
extern int iput_ack_pkt(int i);   		  /* Use Ethernet for sending Ack */
extern int en_ether_irq(int);			  /* Enable Ethernet Interrupts */

/* Pointer to the Interface used */
int  (* get_pckt)(char *)  = get_udp_pckt;
void (* put_pckt)(char *)  = put_udp_pckt;
int  (* putDebugChar)(int) = iput_ack_pkt;
int  (* getDebugChar)(void)= getchar_com1;
int  (* en_dev_irq)(int)   = enable_rcvint;
/* Name the Interface */
static int  act_if         = ETHERNET_IF;
/***********************************************************************/

typedef void (*ExceptionHook)(int);   /* pointer to function with int parm */
//typedef void (*Function)(void);       /* pointer to a function */

// #define DEBUG_DEBUGGER
/* This is for debugging the Debuger Stub itself */
#ifdef DEBUG_DEBUGGER
/* Restore old Exception vectors while this stub has control */
#define NUMEXCP 8
unsigned vtab[NUMEXCP]={0};
unsigned vadtab[NUMEXCP]={0};
unsigned alttab[NUMEXCP]={0};

void
swap_vectors(){
   unsigned old,*tab,*atab,*vec;
   vec = (unsigned *)0;
   tab = vtab;
   atab= alttab;
   while( (unsigned int)vec < (NUMEXCP*sizeof(unsigned)) ) {
	if ( (old=*tab) != 0 ) {
		*tab = *vec;
		*vec = old;
		old  = *atab;
		*atab= *(vec+0x10);
		*(vec+0x10)=old;
	}
	tab++;
	vec++;
	atab++;
   }
}

#else   /* Not DEBUG_DEBUGGER */

#define swap_vectors() /* NIL */
#endif

extern unsigned exceptionHandler(int vec,Function fkt);  /* assign an exception handler */

extern unsigned xinstr;
asm(".text");
asm(".global _xinstr");
asm("_xinstr:	ldr pc,[pc,#0x40-8]");

/* Install a Handler for vector v */
unsigned  exceptionHandler(int v,Function fkt)
{
  unsigned int routine, *vector,vec,oldvec;
  unsigned int avec;

  routine = (unsigned)fkt;
  vector  =(unsigned *)(v*4);

  vec = (routine- (unsigned)vector - 0x8 ) >>2;
  if ( vec & 0xff000000) 
  {
  	/* Handler is more tha 24Bit«s away -- No 1 Instr. Jump */
	
	// printk("Installation Handler failed");
	avec = routine ;
	vec = xinstr; // asm("ldr pc,[pc,#0x40-8]");  
  } else {
	  vec = 0xea000000 | vec;   /* Make it a relative Jump */
  }

  oldvec = *vector;

#ifdef DEBUG_DEBUGGER
  /* Save old vector for later */
  /* Could use it if the break is not meant for us */
  if ( v < NUMEXCP ) {
	  vtab[v] = oldvec;
	  vadtab[v] = oldvec;
	  alttab[v] = *(vector+0x10);
	  if ( (oldvec & 0x0e000000) == 0x0a000000 ) {
	    /* bran */
	    /* branl */
		vadtab[v] = (oldvec & 0x00ffffff );
		vadtab[v] <<= 2;
		if ( vadtab[v]      & 0x00800000 ) 
			vadtab[v]  |= 0xff000000;
	  }else
	  if ( (oldvec&0xfffff000) == 0xe59ff000 ) {
		vadtab[v]     &= 0x00000fff;
		if ( vadtab[v] & 0x00000800 )
		    vadtab[v] |= 0xfffff000;
	  }else {
		/* ERROR unexpected type of instr at VECTOR */
		debug_error("ERROR: Unexpected instr %x. at vector ",vadtab[v]);
		exit(1);
	  }
	 vadtab[v] += (unsigned) vector + 8 ;  /* it's alway PC relativ */
 }
#endif  /* DEBUG_DEBUGGER */
 *(vector+0x10)= avec;
 *vector= vec;
 // *(unsigned *)((long)vector+0xc0000000) = vec;
 return oldvec;
}








/************************************************************************/
/* BUFMAX defines the maximum number of characters in inbound/outbound buffers*/
/* at least NUMREGBYTES*2 are needed for register packets */
#define BUFMAX 400

static char initialized=0;  /* boolean flag. != 0 means we've been initialized */


static const char hexchars[]="0123456789abcdef";

/* Number of bytes of registers.  */
#define FP_REGS  8*12 // if Float:8 reg's 80 bits 
#define NUMREGBYTES (16*4 + FP_REGS + 4 + 4)

/* r0... PC : 32Bit
   f0... f7 : 90Bit    \ Floatingpoint unit
   FPS		: 32Bit    / not available in SA1100
   PS		: 32Bit
*/
enum regnames { 
  r0=0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,SP,LR,PC,
  f0  ,f1,f2,f3,f4,f5,f6,f7,
 // FPS,
//PS
};
/* OFFset to Status Register */
#define PS (16 + 3*8 + 1)
#if 0
  a1, a2, a3, a4, /*  0  1  2  3 */ \
  v1, v2, v3, v4, /*  4  5  6  7 */ \
  v5, v6, sl, fp, /*  8  9 10 11 */ \
  ip, sp, lr, PC, /* 12 13 14 15 */ \
  f0, f1, f2, f3, /* 16 17 18 19 */ \
  f4, f5, f6, f7, /* 20 21 22 23 */ \
  fps,PS};        /* 24 25       */

#endif


/*
 * these should not be static cuz they can be used outside this module
 */
int _registers[NUMREGBYTES/4];

#define STACKSIZE 10000
int remcomStack[STACKSIZE/sizeof(int)];  /* Debugger Stub Stack */
static int* _stackPtr = &remcomStack[STACKSIZE/sizeof(int) - 1];

/*
 * In many cases, the system will want to continue exception processing
 * when a continue command is given.
 * oldExceptionHook is a function to invoke in this case.
 */

static ExceptionHook oldExceptionHook;

/***************************  ASSEMBLY CODE MACROS *************************/
/* 									   */

extern void _return_to_prog (void);

/* Restore the program's registers (including the stack pointer, which
   means we get the right stack and don't have to worry about popping our
   return address and any stack frames and so on) and return.  */
asm(".text");
asm(".globl _xregisters ");
asm("_xregisters: 	.word  __registers");
asm("_stack:		.word  __stackPtr");
asm(".text");
asm(".globl __return_to_prog");
asm("__return_to_prog:");
asm("  	ldr     sp, _xregisters"); 
asm("	ldr	r8,[sp,#+4*(16 + 3*8 + 1)]");	/* PS */
asm("	msr	spsr,r8");
asm("	and	r8,r8,#0x1f");
asm("   cmp	r8,#0x10");    	/* if USER state */
asm("	orreq r8,r8,#0xf");	/* then set System State */
asm("   orr	r8,r8,#0xc0");	/* No Interrupts */
asm("	mrs r1,cpsr");		/* Save current state */
asm("   msr	cpsr,r8");		/* Switch register bank */
asm("  	ldr r0, _xregisters"); 

asm("	mov r2,r0");
asm("  	add	r0,r0,#+(3*4)");	/* Skip r0,r1,r2 */

asm("	ldmia   r0!,{r3-r14}"); /* Load R3-R14 */
asm("	msr	cpsr,r1");		/* Switch back to intr processor state */
asm("   ldr	r1,[r0]"); 		/* R15 == PC */
asm("	str	r1,[r2,#+(3*4)]");
asm("	ldmia r2,{r0,r1,r2,pc}^"); /* RETURN and update PS */
asm("	nop	"); 			/* allow Instr pipe & banked registers time */
asm("	nop	"); 
asm("	nop	"); 


/* In either case enable GDB for every Vector */
//#define HANDLE_RESET 
#define HANDLE_UNDEF 	 // Breakpoint
#define HANDLE_SWI 
#define HANDLE_PREABRT
#define HANDLE_DATABRT 
#define HANDLE_RESERVED 
#define HANDLE_IRQ
#define HANDLE_FIQ 
#undef  ANGEL_IN_USE
#undef  HANDLE_IRQ

/* GDB uses a different BREAKINSTR 
   than this stub.
   If we encounter "our" BREAKPOINT silently increment PC
     so the host-gdb does not see the Instr. anymore
   otherwise let the host-gdb deal with it
*/
int int_array[32768];
extern int int_hits;
extern long _irqlr,_irqr4,_irqr5,_irqr6,_irqr11;

int   pr_ints(void)
{
	int i=0;
	printf("System lr @ irq 0x%08lx, r4=0x%08lx,  r5=0x%08lx, r6=0x%08lx r11=0x%08lx \n", 
	       _irqlr,_irqr4, _irqr5,_irqr6, _irqr11);
	while( i++ < int_hits )
		printf("System was interrupted %4d @ 0x%08lx\n", i, int_array[i]);
	return(int_hits);	
}

asm(".text");
asm(".globl _int_array");
asm(".globl _int_hits");
asm(".globl __larr");
asm("_int_hits: .word   0");
asm("__larr:    .word   _int_array");
asm("__gedr:    .word   0x90040018");
asm("__irqlr:   .word   0");  /* link register of irq */
asm("__irqr4:   .word   0");  /* link register of irq */
asm("__irqr5:   .word   0");  /* link register of irq */
asm("__irqr6:   .word   0");  /* link register of irq */
asm("__irqr11:  .word   0");  /* link register of irq */
asm("__saver7:	.word	0");  /* storage */
asm("vecno:  .word   0");
asm(".text");
asm(".globl brkintr");
asm("brkintr:");
	BREAKPOINT();  /* use it to compare brk instr */
asm("	store_regs:");
	/* Save registers */	
	asm ("  str	r7,vecno");
	asm ("  ldr     r7, _xregisters"); 
	asm ("  stmia   r7!,{r0-r6}");         /* Save r0-r6 */

	asm(" ldr	r2,[lr]");
	asm(" ldr	r3,brkintr");
	asm(" cmp	r2,r3");
	asm(" addeq	lr,lr,#4");			/* Adjust for Breakpoint */
									/* Save old PC */
	asm(" str	lr,[r7,#((15)-7)*4]"); /* PC:   (PC-R7)*4  */

	asm(" ldr	r6,__saver7");		/* R6 holds  R7 */
	asm(" mrs   r1,spsr "); 		/* Saved Processor Status */
	asm(" str	r1,[r7,#((16 + 3*8 + 1)-7)*4]"); /* PS:   (PS-R7)*4  */
	asm(" and 	r1,r1,#0x1f");  	/* saved processor mode */ 
	asm(" cmp	r1,#0x10");			/* if USER then -> */
	asm(" orreq	r1,r1,#0xf"); 		/*   ->SYSTEM  (same register set)*/		
	asm(" orr	r1,r1,#0xd0"); 		/* Disable Interrupts */		
	asm(" mrs	r0,cpsr"); 			/* save current state */	
	asm(" msr	cpsr,r1");			/* Change mode */		
	asm(" stmia	r7,{r6,r8-r14}");   /* Save r7-r14 of interrupted mode*/
	asm(" msr   cpsr,r0");      	/* Change mode back */

	asm("ldr	r0,vecno");			/* Vector number */
	asm("ldr	r1,_stack");   		/* Load debugger Stackpointer */
	asm("ldr	sp,[r1]");		
	asm("bl __handle_exception");	/* let«s go C */
	

#define SAVE_REGISTER(i) \
	asm (" 	str	r7,__saver7"); \
	asm ("  mov	r7,#" #i); \
	asm("b store_regs");


#ifdef HANDLE_RESET  // that does not work !!??
/* Same thing for exception 0.  */
extern void _catchException0(void);
asm(".text");
asm(".globl _catchException0");
asm("_catchException0:");
SAVE_REGISTER(0);
#endif

#ifdef HANDLE_UNDEF
/* Same thing for exception 1.  */
extern void _catchException1(void);
asm(".text");
asm(".globl __catchException1");
asm("__catchException1:");
asm("	sub	lr,lr,#4");  /* Adjust for Breakpoint (NO REAL CHECK!)*/
SAVE_REGISTER(1);
#endif

#ifdef HANDLE_SWI
/* Same thing for exception 2.  */
extern void _catchException2(void);
asm(".text");
asm(".globl __catchException2");
asm("__catchException2:");
SAVE_REGISTER(2);
#endif

#ifdef HANDLE_PREABRT
/* Same thing for exception 3.  */
extern void _catchException3(void);
asm(".text");
asm(".globl __catchException3");
asm("__catchException3:");
asm("	sub lr,lr,#4");
SAVE_REGISTER(3);
#endif

#ifdef HANDLE_DATABRT
/* Same thing for exception 4.  */
extern void _catchException4(void);
asm(".text");
asm(".globl __catchException4");
asm("__catchException4:");
asm("	sub lr,lr,#8");
SAVE_REGISTER(4);
#endif

#ifdef HANDLE_RESERVED
/* Same thing for exception 5.  */
extern void _catchException5(void);
asm(".text");
asm(".globl __catchException5");
asm("__catchException5:");
SAVE_REGISTER(5);
#endif

#ifdef HANDLE_IRQ
/* Same thing for exception 6.  */
extern void _catchException6(void);
asm(".text");
asm(".globl __catchException6");
asm("__catchException6:");
SAVE_REGISTER(6);
#else
/* Same thing for exception 6.  */
extern void _catchException6(void);
asm(".text");
asm(".globl __catchException6");
asm("__catchException6:");
asm("  str lr,__irqlr");
asm("  str r4,__irqr4");
asm("  str r5,__irqr5");
asm("  str r6,__irqr6");
asm("  ldr r6,__larr");
asm("  str lr,[r6], #4");
asm("  str r6, __larr");
asm("  ldr r6,_int_hits");
asm("  add r6,r6,#1");
asm("  str r6,_int_hits");
asm("  ldr r6, __gedr");
asm("  mov r5,#0x100");
asm("  str r5,[r6]");
asm("  ldr r5,__irqr5");
asm("  ldr r6,__irqr6");
asm("  str r11,__irqr11");
asm("  subs pc,lr,#4");
#if 0
SAVE_REGISTER(6);
#endif
#endif

#ifdef HANDLE_FIQ
/* Same thing for exception 7.  */
extern void _catchException7(void);
asm(".text");
asm(".globl __catchException7");
asm("__catchException7:");
asm("	sub lr,lr,#4");
SAVE_REGISTER(7);
#endif


void _returnFromException(void)
{
  _return_to_prog ();
}

void _not_handle_exception(int exc)
{
	_returnFromException();	
}
  

int hex(char ch)
{
  if ((ch >= 'a') && (ch <= 'f')) return (ch-'a'+10);
  if ((ch >= '0') && (ch <= '9')) return (ch-'0');
  if ((ch >= 'A') && (ch <= 'F')) return (ch-'A'+10);
  return (-1);
}

/***********************************************************************/
/***********************************************************************/
void putpacket(char *buffer)
{
	put_pckt(buffer);	
}

int getpacket( char *buffer)
{
	return(get_pckt(buffer));
}

int set_dbg_iface(int intf)
{
	int ret=act_if;	
	int oldmask=en_dev_irq(0);
	
#if 0
	if( intf == ETHERNET_IF )
	{
		if( init_net() == 0 )	return(-1); /* No Network */	
#else
	if( intf == ETHERNET_IF && init_net() != 0 )
	{
#endif
		get_pckt=get_udp_pckt;
		put_pckt=put_udp_pckt;	
		putDebugChar=iput_ack_pkt;
		en_dev_irq=en_ether_irq;		
		act_if=intf;		
	}else
	{
		ini_simple_coms();		
		get_pckt=getserpacket;
		put_pckt=putserpacket;
		putDebugChar=putchar_com1;
		en_dev_irq=enable_rcvint;		
		act_if=intf;		
	}
	en_dev_irq(oldmask);	
	return(ret);	
}

/* scan for the sequence $<data>#<checksum>     */
int getserpacket(char *buffer)
{
  unsigned char checksum;
  unsigned char xmitcsum;
  int  i;
  int  count;
  char ch;

  do {
    /* wait around for the start character, ignore all other characters */
    while ((ch = getDebugChar()) != '$');
    checksum = 0;
    xmitcsum = -1;

    count = 0;

    /* now, read until a # or end of buffer is found */
    while (count < BUFMAX) {
      ch = getDebugChar();
      if (ch == '#') break;
      checksum = checksum + ch;
      buffer[count] = ch;
      count = count + 1;
      }
    buffer[count] = 0;

    if (ch == '#') {
      xmitcsum = hex(getDebugChar()) << 4;
      xmitcsum += hex(getDebugChar());
      if ((remote_debug ) && (checksum != xmitcsum)) {
        printk("bad checksum.  My count = 0x%x, sent=0x%x. buf=%s\n",checksum,xmitcsum,buffer);
      }

      if (checksum != xmitcsum) putDebugChar('-');  /* failed checksum */
      else {
	 putDebugChar('+');  /* successful transfer */
	 /* if a sequence char is present, reply the sequence ID */
	 if (buffer[2] == ':') {
	    putDebugChar( buffer[0] );
	    putDebugChar( buffer[1] );
	    /* remove sequence chars from buffer */
	    count = strlen(buffer);
	    for (i=3; i <= count; i++) buffer[i-3] = buffer[i];
	 }
      }
    }
  } while (checksum != xmitcsum);
  return(0);  
}

/* send the packet in buffer.  */


void putserpacket(char *buffer)
{
  unsigned char checksum;
  int  count;
  char ch;

  /*  $<packet info>#<checksum>. */
  do {
  putDebugChar('$');
  checksum = 0;
  count    = 0;

  while ((ch=buffer[count])!=0) 
  {
    if (! putDebugChar(ch)) return;
    checksum += ch;
    count += 1;
  }

  putDebugChar('#');
  putDebugChar(hexchars[checksum >> 4]);
  putDebugChar(hexchars[checksum % 16]);

  } while (getDebugChar() != '+');

}

char  remcomInBuffer[BUFMAX];
char  remcomOutBuffer[BUFMAX];
static short error;



/* Address of a routine to RTE to if we get a memory fault.  */
static volatile void (*_mem_fault_routine)(void) = NULL;

/* Indicate to caller of mem2hex or hex2mem that there has been an
   error.  */
static volatile int mem_err = 0;

void set_mem_err (void)
{
  mem_err = 1;
}

/* These are separate functions so that they are so short and sweet
   that the compiler won't save any registers (if there is a fault
   to mem_fault, they won't get restored, so there better not be any
   saved).  */
int get_char (char *addr)
{
  return *addr;
}

void set_long (long *addr,long val)
{
  *addr = val;
  cache_flush();
  
}
void set_short (short *addr,short val)
{
  *addr = val;
  cache_flush();
}
void set_char (char *addr,int val)
{
  *addr = val;
  cache_flush();
}

/* convert the memory pointed to by mem into hex, placing result in buf */
/* return a pointer to the last char put in buf (null) */
/* If MAY_FAULT is non-zero, then we should set mem_err in response to
   a fault; if zero treat a fault like any other fault in the stub.  */
char* mem2hex(char *mem,char *buf,int count,int may_fault)
{
      int i;
      unsigned char ch;

      if (may_fault)
	  _mem_fault_routine = (volatile void (*)(void)) set_mem_err;

      for (i=0;i<count;i++) {
          ch = get_char (mem++);
	  if (may_fault && mem_err)
	    return (buf);
          *buf++ = hexchars[ch >> 4];
          *buf++ = hexchars[ch % 16];
      }
      *buf = 0;
      if (may_fault)
	  _mem_fault_routine = NULL;
      return(buf);
}

/* convert the hex array pointed to by buf into binary to be placed in mem */
/* return a pointer to the character AFTER the last byte written */
char* hex2mem(char *buf,char *mem,int count,int may_fault)
{
      int i;
      unsigned char ch;

      if (may_fault)
	  _mem_fault_routine = (volatile void (*)(void))set_mem_err;
      for (i=0;i<count;i++) {
	  if ( ((long)mem & 3) == 0 && count >= 4 ) {
		/* write long */
		long sh;
		  sh  = (hex(*buf++) << 4 | hex(*buf++)) << 0  ;
		  sh |= (hex(*buf++) << 4 | hex(*buf++)) << 8  ;
		  sh |= (hex(*buf++) << 4 | hex(*buf++)) << 16  ;
		  sh |= (hex(*buf++) << 4 | hex(*buf++)) << 24  ;
		  set_long (((long*)mem)++, sh);
		  i += 3;
	  }else if ( ((long)mem & 1) == 0 && count >= 2 ) {
		/* write short */
		short sh;
		  sh  =  (hex(*buf++) << 4 | hex(*buf++)) << 0  ;
		  sh |=  (hex(*buf++) << 4 | hex(*buf++)) << 8  ;
		  set_short (((short*)mem)++, sh);
		  i += 1;
	  } else { 
		/* write char */
		  ch = hex(*buf++) << 4;
		  ch = ch + hex(*buf++);
		  set_char (mem++, ch);
	  }
	  if (may_fault && mem_err)
	    return (mem);
      }
      if (may_fault)
	  _mem_fault_routine = NULL;
      return(mem);
}

/* this function takes the 386 exception vector and attempts to
   translate this number into a unix compatible signal value */
int computeSignal(int exceptionVector )
{
  int sigval;
  switch (exceptionVector) {
    // case 0 	: sigval = ; break;  /* Reset */
    case 1 	: sigval = 5; break; /* BREakpoint break debug exception */
    case 2 	: sigval = 4; break;  /* SWI */
    case 3 	: sigval = 5; break;  /* Prefetch Abort */
    case 4 	: sigval = 11; break;  /* Data Abort */
    // case 5	: sigval = ; break;  /* Reserved */
    case 6	: sigval = 7; break;  /* IRQ */
    case 7 	: sigval = 7; break;  /* FIQ */

#if 0

 /* Not for ARM */
    case 0 : sigval = 8; break; /* divide by zero */
    case 1 : sigval = 5; break; /* debug exception */
    case 3 : sigval = 5; break; /* breakpoint */
    case 4 : sigval = 16; break; /* into instruction (overflow) */
    case 5 : sigval = 16; break; /* bound instruction */
    case 6 : sigval = 4; break; /* Invalid opcode */
    case 7 : sigval = 8; break; /* coprocessor not available */
    case 8 : sigval = 7; break; /* double fault */
    case 9 : sigval = 11; break; /* coprocessor segment overrun */
    case 10 : sigval = 11; break; /* Invalid TSS */
    case 11 : sigval = 11; break; /* Segment not present */
    case 12 : sigval = 11; break; /* stack exception */
    case 13 : sigval = 11; break; /* general protection */
    case 14 : sigval = 11; break; /* page fault */
    case 16 : sigval = 7; break; /* coprocessor error */

#if ! USE_LINUX_IRQ_STUB
    case 0x15: sigval =5; break; /* USER ABORT */
    case 15:   sigval =5; break; /* USER ABORT */
#endif	  
#endif	  
	  
    default:
      sigval = 7;         /* "software generated"*/
  }
  return (sigval);
}

/**********************************************/
/* WHILE WE FIND NICE HEX CHARS, BUILD AN INT */
/* RETURN NUMBER OF CHARS PROCESSED           */
/**********************************************/
int hexToInt(char **ptr, int *intValue)
{
    int numChars = 0;
    int hexValue;

    *intValue = 0;

    while (**ptr)
    {
        hexValue = hex(**ptr);
        if (hexValue >=0)
        {
            *intValue = (*intValue <<4) | hexValue;
            numChars ++;
        }
        else
            break;

        (*ptr)++;
    }

    return (numChars);
}

int connected=0;


void host_msg(char *msg)
{
    char *emsg;
    for(emsg=msg;*emsg;emsg++);
    remcomOutBuffer[0] = 'O';
   mem2hex(msg, remcomOutBuffer+1, emsg-msg, 0);
   if ( connected )
	   putpacket(remcomOutBuffer);
}

void gdb_outc(char c){
	static char buff[256];
	static int iptr=0;
	if ( ! initialized ) return;
	buff[iptr++]=c;
	buff[iptr]=0;
	
	if ( c == '\n' || c > 100) {
		host_msg(buff);
		iptr=0;
	}
}

/*
 * This function does all command procesing for interfacing to gdb.
 */



void _handle_exception(int exceptionVector)
{
  int    sigval;
  int    addr, length;
  char * ptr;
  int    newPC;
  int old_cache;
  extern int cache_status;

#if 0
  if ( exceptionVector == 6  ) { /* Detect which PC was on trigger event */
	  GEDR =  0x100;         /* a real software emulator !!!         */
	  if( int_hits < 1023 )
		  int_array[int_hits++]=_registers[PC];
	  else
                  disable_irq(0);

	  _returnFromException();

	if (! check_ether_break()) {
		_returnFromException();
	}
  }
#endif
  //cache_flush();
  old_cache=cache_off();
  swap_vectors();
  cli();  

  en_dev_irq(0);

  // gdb_i386vector = exceptionVector;
  

  if (remote_debug  ) 
	printk("vector=%d, sr=0x%x, pc=0x%x\n",
			    exceptionVector,
			    _registers[ PS ],
			    _registers[ PC ]);


  /* reply to host that an exception has occurred */
  sigval = computeSignal( exceptionVector );
  remcomOutBuffer[0] = 'S';
  remcomOutBuffer[1] =  hexchars[sigval >> 4];
  remcomOutBuffer[2] =  hexchars[sigval % 16];
  remcomOutBuffer[3] = 0;

  putpacket(remcomOutBuffer);


  while (1==1) {
    error = 0;
    remcomOutBuffer[0] = 0;
    while( !  getpacket(remcomInBuffer))
    {
	    remcomOutBuffer[0] = 'S';
	    remcomOutBuffer[1] =  hexchars[sigval >> 4];
	    remcomOutBuffer[2] =  hexchars[sigval % 16];
	    remcomOutBuffer[3] = 0;
	    putpacket(remcomOutBuffer);	    
    }    
    connected=1;

    switch (remcomInBuffer[0]) {
      case '?' :   remcomOutBuffer[0] = 'S';
                   remcomOutBuffer[1] =  hexchars[sigval >> 4];
                   remcomOutBuffer[2] =  hexchars[sigval % 16];
                   remcomOutBuffer[3] = 0;
                 break;
      case 'd' : remote_debug = !(remote_debug);  /* toggle debug flag */
                 break;
      case 'g' : /* return the value of the CPU registers */
		{ static char first=1;
		  if ( first ) { first=0;
			host_msg("GDB on Arnold (K&K) ready\n\r");
		  }
		}
                mem2hex((char*) _registers, remcomOutBuffer, NUMREGBYTES, 0);
                break;
      case 'G' : /* set the value of the CPU registers - return OK */
                hex2mem(&remcomInBuffer[1], (char*) _registers, NUMREGBYTES, 0);
                strcpy(remcomOutBuffer,"OK");
                break;

      /* mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
      case 'm' :
		    /* TRY TO READ %x,%x.  IF SUCCEED, SET PTR = 0 */
                    ptr = &remcomInBuffer[1];
                    if (hexToInt(&ptr,&addr))
                        if (*(ptr++) == ',')
                            if (hexToInt(&ptr,&length))
                            {
                                ptr = 0;
				mem_err = 0;
                                mem2hex((char*) addr, remcomOutBuffer, length, 1);
				if (mem_err) {
				    strcpy (remcomOutBuffer, "E03");
				    debug_error ("memory fault",0);
				}
                            }

                    if (ptr)
                    {
		      strcpy(remcomOutBuffer,"E01");
		      debug_error("malformed read memory command: %s",remcomInBuffer);
		    }
	          break;

      /* MAA..AA,LLLL: Write LLLL bytes at address AA.AA return OK */
      case 'M' :
		    /* TRY TO READ '%x,%x:'.  IF SUCCEED, SET PTR = 0 */
                    ptr = &remcomInBuffer[1];
                    if (hexToInt(&ptr,&addr))
                        if (*(ptr++) == ',')
                            if (hexToInt(&ptr,&length))
                                if (*(ptr++) == ':')
                                {
				    mem_err = 0;
                                    hex2mem(ptr, (char*) addr, length, 1);

				    if (mem_err) {
					strcpy (remcomOutBuffer, "E03");
					debug_error ("memory fault",0);
				    } else {
				        strcpy(remcomOutBuffer,"OK");
				    }

                                    ptr = 0;
                                }
                    if (ptr)
                    {
		      strcpy(remcomOutBuffer,"E02");
		      debug_error("malformed write memory command: %s",remcomInBuffer);
		    }
                break;

     /* cAA..AA    Continue at address AA..AA(optional) */
     /* sAA..AA   Step one instruction from AA..AA(optional) */
     case 'c' :
     case 's' :
          /* try to read optional parameter, pc unchanged if no parm */
         ptr = &remcomInBuffer[1];
         if (hexToInt(&ptr,&addr))
             _registers[ PC ] = addr;

          newPC = _registers[ PC];

#if 0  /* Single step not available */
          /* clear the trace bit */

          /* set the trace bit if we're stepping */
          if (remcomInBuffer[0] == 's') {
//		 __asm__ volatile ("mov r0,%0":: "r" (newPC+4));
		 __asm__ volatile ("mov r0,%0":: "r" (newPC));
		asm volatile (" bic r0,r0,#3");
		asm volatile (" orr r0,r0,#1");
		asm volatile (" mcr p15,0,r0,c14,c8,0");
		
	  }
#endif

#if 0
	  /* Don't really think we need this, except maybe for protection
	     exceptions.  */
                  /*
                   * invoke the previous handler.
                   */
                  if (oldExceptionHook)
                      (*oldExceptionHook) (frame->exceptionVector);
                  newPC = _registers[ PC ];    /* pc may have changed  */
#endif /* 0 */
		  if (remcomInBuffer[0] != 's')  en_dev_irq(1);
  swap_vectors();
#if KEITH
  cache_flush();  
#else
  asm volatile (" mcr p15,0,r0,c7,c7,0"); /* Flush I+D */
  asm volatile (" mcr p15,0,r0,c7,c10,4"); /* Drain Write buffer */
#endif
  cache_restore(old_cache);
		  _returnFromException(); /* this is a jump */
          break;

      /* kill the program */
      case 'k' :  /* do nothing */
#if 0
	/* Huh? This doesn't look like "nothing".
	   m68k-stub.c and sparc-stub.c don't have it.  */
		BREAKPOINT();
#endif
                break;
      } /* switch */

    /* reply to the request */
    putpacket(remcomOutBuffer);
    }
}
void enable_irq(int no)
{
 register int psr;
 __asm__ volatile ("mrs %0,cpsr": "=r" (psr));
 __asm__ volatile ("msr cpsr,%0":: "g" (psr&=~(1<<7)));   /* Enable Interrupts */
}

void disable_irq(int no)
{
 register int psr;
 __asm__ volatile ("mrs %0,cpsr": "=r" (psr));
 __asm__ volatile ("msr cpsr,%0":: "g" (psr|=~(1<<7)));   /* Disable Interrupts */
}

/* this function is used to set up exception handlers for tracing and
   breakpoints */
void set_debug_traps(void)
{
extern void _remcomHandler(void);
/*int exception;*/

  _stackPtr  = &remcomStack[STACKSIZE/sizeof(int) - 1];

#ifdef HANDLE_RESET  // that does not work !!??
  exceptionHandler (0, _catchException0);
#endif

#ifndef ANGEL_IN_USE
#ifdef HANDLE_UNDEF
  exceptionHandler (1, _catchException1);  /* Undefine Instr */
#endif
#ifdef HANDLE_SWI
  exceptionHandler (2, _catchException2);  /* Undefine Instr */
#endif
#ifdef HANDLE_PREABRT
  exceptionHandler (3, _catchException3);
#endif
#ifdef HANDLE_DATABRT
  exceptionHandler (4, _catchException4);
#endif
#ifdef HANDLE_RESERVED
  exceptionHandler (5, _catchException5);  /* Interrupt */
#endif
#endif

#ifdef HANDLE_IRQ
#warning("IRQs handled by gdb-stub")
  exceptionHandler (6, _catchException5);  /* Interrupt */
#endif

#ifdef HANDLE_FIQ
#warning("FIQs handled by gdb-stub")
  exceptionHandler (7, _catchException7);
#endif


  /* In case GDB is started before us, ack any packets (presumably
     "$?#xx") sitting there.  */
#if KEITH
  cache_flush();
#else
  asm volatile (" mcr p15,0,r0,c7,c7,0"); /* Flush I+D */
  asm volatile (" mcr p15,0,r0,c7,c10,4"); /* Drain Write buffer */
#endif  

  putDebugChar ('+');
  
  initialized = 1;

}

void remote_interruptible(int on)
{
	if( on )
		en_dev_irq(1);
	else
		en_dev_irq(0);	
}

/* This function will generate a breakpoint exception.  It is used at the
   beginning of a program to sync up with a debugger and can be used
   otherwise as a quick means to stop program execution and "break" into
   the debugger. */

volatile void start_debugsession(long pc,long ps)
{
	/* Set all registers to 0x8000 */
	int i;
	for(i=0;i<NUMREGBYTES/4;i++) _registers[i]=0x8000;
	_registers[ PC ] = pc;
	_registers[ PS ] = ps;
	_handle_exception(1); /* Start debugger */

}

#ifdef breakpoint
#undef breakpoint
#endif

volatile void breakpoint(void)
{
	en_dev_irq(0);	
	if (initialized){
#if 0
	_handle_exception(3);
#else
	BREAKPOINT();
#endif
}
}

void i_breakpoint(void)
{
	{ unsigned int i;
	 for(i=0;i<(sizeof(_registers)/sizeof(_registers[0]));i++){
		_registers[i]=0x8000+i;
	}
	}
	_handle_exception(1);
}
#include "hard.h"

int setup_gdbstub(void)
{
	int use_iface=ETHERNET_IF;
	extern void InitIo(void);
#ifndef KEITH
	InitIo();
#endif

#if 0
#if !defined(SSM_MT5) && !defined(PLATFORM_SULZER) && !defined(SMC_ADR) && !defined(SA1111)
        if ( (GPLR & PCMCIA_CD1) != 0 ) {
		/* No Card found */
		use_iface=SER_IF;
	}
#endif
#endif
	if (set_dbg_iface(use_iface)== -1 ) return -1;
	set_debug_traps();
	return 0;
}


int setup_catch_int(void)
{
  exceptionHandler (6, _catchException6);
  GPDR &= ~0x100;   /* CTS1    */
  GRER &= ~0x100;
  GFER |=  0x100;
  GEDR =   0x100;  
  ICLR &= ~0x100;   /* set FIQ */
  ICMR |=  0x100;
  enable_irq(1);
  return(0);  
}
