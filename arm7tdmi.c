/*
  Copyright (c) 2003 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **

  $Id: arm7tdmi.c,v 1.8 2003/12/11 04:10:14 bgat Exp $

*/

#include "gdb.h"
#include "arm7tdmi.h"
#if defined(KS32C50100)
#include "arm7tdmi-ks32c50100.h"
#endif
#if defined(LH79520)
#include "arm7tdmi-lh79520.h"
#endif

/* caution: this enumeration is used both by gdb to identify
   registers, and by gdbstubs to map register numbers in machine
   instructions to register names; changes to this enumeration will
   break a lot of stuff! */
typedef enum
  {
    R0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, R10, R11, R12, SP, LR, PC,
    unk10, unk11, unk12, unk13, unk14, unk15,
    unk16, unk17, unk18, CPSR=0x19
  } register_id_e;

#define ARM7TDMI_BCC    0x0a000000UL
#define ARM7TDMI_BLCC   0x0b000000UL
#define ARM7TDMI_MOV    0x01a00000UL

#define ARM7TDMI_LDM    0x08100000UL
#define ARM7TDMI_LDM_P  0x01000000UL
#define ARM7TDMI_LDM_U  0x00800000UL
#define ARM7TDMI_LDM_L  0x00100000UL

typedef enum {
  CC_EQ = 0, CC_NE = 1, CC_CS = 2, CC_CC = 3,
  CC_MI = 4, CC_PL = 5, CC_VS = 6, CC_VC = 7,
  CC_HI = 8, CC_LS = 9, CC_GE = 10, CC_LT = 11,
  CC_GT = 12, CC_LE = 13, CC_AL = 14, CC_NV = 15
} arm7tdmi_cc_e;

typedef enum {
  CPSR_N = 1 << 31, CPSR_Z = 1 << 30,
  CPSR_C = 1 << 29, CPSR_V = 1 << 28,
  CPSR_Q = 1 << 27,
  CPSR_I = 1 << 7, CPSR_F = 1 << 6,
  CPSR_T = 1 << 5,
  CPSR_M4 = 1 << 4, CPSR_M3 = 1 << 3,
  CPSR_M2 = 1 << 2, CPSR_M1 = 1 << 1,
  CPSR_M0 = 1 << 0,
} arm7tdmi_cpsr_e;

typedef struct
{
  long und_stack[128];
  long pc;
  long cpsr;
  long r0;
  long r1;
  long r2;
  long r3;
  long r4;
  long r5;
  long r6;
  long r7;
  long r8;
  long r9;
  long r10;
  long r11;
  long r12;
  long sp;
  long lr;
} register_file_S;

register_file_S gdb_register_file;

char arm7tdmi_stepping;

typedef unsigned long arm7tdmi_instr_t;
arm7tdmi_instr_t arm7tdmi_stepped_instr;
arm7tdmi_instr_t *arm7tdmi_stepped_addr;


void arm7tdmi_handle_exception (int exception)
{
  if (arm7tdmi_stepping)
    {
      *arm7tdmi_stepped_addr = arm7tdmi_stepped_instr;
      arm7tdmi_stepping = 0;
    }
  gdb_handle_exception(exception);
}

void gdb_startup (void)
{
  arm7tdmi_stepping = 0;
#if defined(KS32C50100)
  arm7tdmi_ks32c50100_startup();
#endif
#if defined(LH79520)
  arm7tdmi_lh79520_startup();
#endif

}

int gdb_peek_register_file (int id, long *val)
{
  int retval = sizeof(long);

  switch (id)
    {
    case R0:   *val = gdb_register_file.r0;   break;
    case R1:   *val = gdb_register_file.r1;   break;
    case R2:   *val = gdb_register_file.r2;   break;
    case R3:   *val = gdb_register_file.r3;   break;
    case R4:   *val = gdb_register_file.r4;   break;
    case R5:   *val = gdb_register_file.r5;   break;
    case R6:   *val = gdb_register_file.r6;   break;
    case R7:   *val = gdb_register_file.r7;   break;
    case R8:   *val = gdb_register_file.r8;   break;
    case R9:   *val = gdb_register_file.r9;   break;
    case R10:  *val = gdb_register_file.r10;  break;
    case R11:  *val = gdb_register_file.r11;  break;
    case R12:  *val = gdb_register_file.r12;  break;
    case SP:   *val = gdb_register_file.sp;   break;
    case LR:   *val = gdb_register_file.lr;   break;
    case PC:   *val = gdb_register_file.pc;   break;
    case CPSR: *val = gdb_register_file.cpsr; break;
    case unk10: case unk11: case  unk12: /* *val = 0; break; */
    case unk13: case unk14: case unk15:  /* *val = 0; break; */
    case unk16: case unk17: case unk18:  /* *val = 0; break; */
    default: retval = 0;
    }

  return retval;
}


int gdb_poke_register_file (int id, long val)
{
  int retval = sizeof(long);

  switch (id)
    {
    case R0:   gdb_register_file.r0   = val; break;
    case R1:   gdb_register_file.r1   = val; break;
    case R2:   gdb_register_file.r2   = val; break;
    case R3:   gdb_register_file.r3   = val; break;
    case R4:   gdb_register_file.r4   = val; break;
    case R5:   gdb_register_file.r5   = val; break;
    case R6:   gdb_register_file.r6   = val; break;
    case R7:   gdb_register_file.r7   = val; break;
    case R8:   gdb_register_file.r8   = val; break;
    case R9:   gdb_register_file.r9   = val; break;
    case R10:  gdb_register_file.r10  = val; break;
    case R11:  gdb_register_file.r11  = val; break;
    case R12:  gdb_register_file.r12  = val; break;
    case SP:   gdb_register_file.sp   = val; break;
    case LR:   gdb_register_file.lr   = val; break;
    case PC:   gdb_register_file.pc   = val; break;
    case CPSR: gdb_register_file.cpsr = val; break;
    case unk10: case unk11: case unk12: break;
    case unk13: case unk14: case unk15: break;
    case unk16: case unk17: case unk18: break;
    default: retval = 0;
    }

  return retval;
}


int arm7tdmi_cond_true (arm7tdmi_instr_t op)
{
  int ret = 0;
  arm7tdmi_cpsr_e cpsr = (arm7tdmi_cpsr_e)gdb_register_file.cpsr;

  switch ((op & 0xf0000000) >> 28)
    {
    case CC_EQ: /* Z set */
      ret = (cpsr & CPSR_Z) ? 1 : 0;
      break;
    case CC_NE: /* Z clear */
      ret = (cpsr & CPSR_Z) ? 0 : 1;
      break;
    case CC_CS: /* C set */
      ret = (cpsr & CPSR_C) ? 1 : 0;
      break;
    case CC_CC: /* C clear */
      ret = (cpsr & CPSR_C) ? 0 : 1;
      break;
    case CC_MI: /* N set */
      ret = (cpsr & CPSR_N) ? 1 : 0;
      break;
    case CC_PL: /* N clear */
      ret = (cpsr & CPSR_N) ? 0 : 1;
      break;
    case CC_VS: /* V set */
      ret = (cpsr & CPSR_V) ? 1 : 0;
      break;
    case CC_VC: /* V clear */
      ret = (cpsr & CPSR_V) ? 0 : 1;
      break;
    case CC_HI: /* C set and Z clear */
      ret = ((cpsr & CPSR_C) && !(cpsr & CPSR_Z)) ? 1 : 0;
      break;
    case CC_LS: /* C clear or Z set */
      ret = (!(cpsr & CPSR_C) || (cpsr & CPSR_Z)) ? 1 : 0;
      break;
    case CC_GE: /* N == V */
      ret = (((cpsr & CPSR_N) && (cpsr & CPSR_V))
	     || (!(cpsr & CPSR_N) && !(cpsr & CPSR_V))) ? 1 : 0;
	break;
    case CC_LT: /* N != V */
      ret = (((cpsr & CPSR_N) && !(cpsr & CPSR_V))
	     || (!(cpsr & CPSR_N) && (cpsr & CPSR_V))) ? 1 : 0;
    case CC_GT: /* Z == 0 and N == V */
      ret = (!(cpsr & CPSR_Z) && 
	     (((cpsr & CPSR_N) && (cpsr & CPSR_V))
	     || (!(cpsr & CPSR_N) && !(cpsr & CPSR_V)))) ? 1 : 0;
      break;
    case CC_LE: /* Z == 1 or N != V */
      ret = ((cpsr & CPSR_Z) || 
	     (((cpsr & CPSR_N) && !(cpsr & CPSR_V))
	     || (!(cpsr & CPSR_N) && (cpsr & CPSR_V)))) ? 1 : 0;
      break;
    case CC_AL: /* always */
      ret = 1;
      break;
    case CC_NV: /* never (deprecated) */
      ret = 0;
      break;
    }
  return ret;
}

arm7tdmi_instr_t *arm7tdmi_instr_dest (arm7tdmi_instr_t op)
{
  unsigned long pc = gdb_register_file.pc;

  /* There are several classes of branch instructions:
     Bcc  and BLcc -- Conditional branch, branch with link
     MOV PC, ...   -- load PC with subroutine address
     LDR PC, ...   -- load PC with subroutine address
     LDM ..., {..., PC, ...} --- load PC with value from stack
     ADD, SUB, etc. with the PC as the destination register

     TODO: add support for all branch instruction classes
     TODO: macroize opcode determination, to reduce risk of errors
     TODO: non-T architectures can use T bit for instruction stepping (easier)
     TODO: simplify by first checking for PC as the destination register?
  */
  if (!arm7tdmi_cond_true(op))
    pc = pc + 4;
  
  else if ((ARM7TDMI_BCC == (op & 0x0f000000UL)
	    || ARM7TDMI_BLCC == (op & 0x0f000000UL)))
    {
      signed long signed_immed_24 = (op & 0x00ffffffUL);
      if (signed_immed_24 & 0x800000UL)
	signed_immed_24 |= 0xff000000UL;
      pc = pc + 8 + (signed_immed_24 << 2);
    }
  
  else if ((ARM7TDMI_MOV == (op & 0x0fa00000UL))
	   && (PC == ((op >> 12) & 0xf)))
    gdb_peek_register_file(op & 0xf, &pc);

  else if ((ARM7TDMI_LDM == (op & 0x08100000UL))
	   && ((1 << PC) & op))
      {
	/* TODO: this has not been thoroughly tested */
	unsigned long *newpc;
	int rl;
	gdb_peek_register_file((op >> 16) & 0xf, (long*)&newpc);

	if (ARM7TDMI_LDM_P & op)
	  {
	    if (ARM7TDMI_LDM_U & op)
	      ++newpc;
	    else
	      --newpc;		     
	  }
	for (rl = 0; rl < 15; rl++)
	  {
	    if ((1 << rl) & op)
	      {
		if (ARM7TDMI_LDM_U & op)
		  ++newpc;
		else
		  --newpc;
	      }
	  }
	pc = *newpc;
      }

  else
    pc = pc + 4;

  /* TODO: other branch instructions */
  
  return (arm7tdmi_instr_t *)pc;
}

void gdb_step (long addr)
{
#if 0
  /* test code, won't work on branch instructions */
  unsigned long *pc = (unsigned long*)(gdb_register_file.pc + 4);
  arm7tdmi_stepped_addr = pc;
  arm7tdmi_stepped_instr = *pc;
  arm7tdmi_stepping = 1;
  *pc = ARM7TDMI_STEPI_OPCODE;
  gdb_return_from_exception();  

#else
  arm7tdmi_instr_t *dest;
  arm7tdmi_instr_t op;

  if (addr)
    gdb_register_file.pc = addr;

  arm7tdmi_stepping = 1;

  op = *(arm7tdmi_instr_t*)gdb_register_file.pc;
  dest = arm7tdmi_instr_dest(op);

  arm7tdmi_stepped_addr = (arm7tdmi_instr_t *)dest;
  arm7tdmi_stepped_instr = *arm7tdmi_stepped_addr;

  *dest = ARM7TDMI_STEPI_OPCODE;
  gdb_return_from_exception();
#endif
}


void gdb_continue (long addr)
{
  if (addr)
    gdb_register_file.pc = addr;
  gdb_return_from_exception();
}


void gdb_flush_cache (void *start, int len)
{
}


void gdb_monitor_onentry (void){}
void gdb_monitor_onexit (void){}





