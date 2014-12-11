/*
  Copyright (c) 2003 by      William A. Gatliff
  All rights reserved.      bgat@billgatliff.com

  See the file COPYING for details.

  This file is provided "as-is", and without any express or implied
  warranties, including, without limitation, the implied warranties of
  merchantability and fitness for a particular purpose.

  The author welcomes feedback regarding this file.

  **

  $Id: mips.c,v 1.1 2003/09/13 04:39:33 bgat Exp $

*/

#include "gdb.h"
#include "mips.h"
#if defined(CSB250)
#include "csb250.h"
#endif

/* caution: this enumeration is used both by gdb to identify
   registers, and by gdbstubs to map register numbers in machine
   instructions to register names; changes to this enumeration will
   break a lot of stuff! */
typedef enum
  {
    ZERO, AT, V0, V1, A0, A1, A2, A3,
    T0, T1, T2, T3, T4, T5, T6, T7,
    S0, S1, S2, S3, S4, S5, S6, S7,
    T8, T9, K0, K1, GP, SP, S8, RA,
    SR, LO, HI, BAD, CAUSE, PC, FSR, FIR, FP
  } register_id_e;


typedef struct
{
  long at;
  long v0;
  long v1;
  long a0;
  long a1;
  long a2;
  long a3;
  long t0;
  long t1;
  long t2;
  long t3;
  long t4;
  long t5;
  long t6;
  long t7;
  long s0;
  long s1;
  long s2;
  long s3;
  long s4;
  long s5;
  long s6;
  long s7;
  long t8;
  long t9;
  long k0;
  long k1;
  long gp;
  long sp;
  long s8;
  long ra;
  long sr;
  long lo;
  long hi;
  long bad;
  long cause;
  long pc;
  long fsr;
  long fir;
  long _stack[256];
} register_file_S;

register_file_S gdb_register_file;

typedef unsigned long mips_instr_t;
mips_instr_t mips_stepped_instr;
mips_instr_t *mips_stepped_addr;
int mips_stepping;

void mips_handle_exception (int exception)
{
  if (mips_stepping)
    {
      *mips_stepped_addr = mips_stepped_instr;
      mips_stepping = 0;
    }
  gdb_handle_exception(exception);
}

int gdb_peek_register_file (int id, long *val)
{
  int retval = sizeof(long);

  switch (id)
    {
    case ZERO: *val = 0; break;
    case AT: *val = gdb_register_file.at; break;
    case V0: *val = gdb_register_file.v0; break;
    case V1: *val = gdb_register_file.v1; break;
    case A0: *val = gdb_register_file.a0; break;
    case A1: *val = gdb_register_file.a1; break;
    case A2: *val = gdb_register_file.a2; break;
    case A3: *val = gdb_register_file.a3; break;
    case T0: *val = gdb_register_file.t0; break;
    case T1: *val = gdb_register_file.t1; break;
    case T2: *val = gdb_register_file.t2; break;
    case T3: *val = gdb_register_file.t3; break;
    case T4: *val = gdb_register_file.t4; break;
    case T5: *val = gdb_register_file.t5; break;
    case T6: *val = gdb_register_file.t6; break;
    case T7: *val = gdb_register_file.t7; break;
    case S0: *val = gdb_register_file.s0; break;
    case S1: *val = gdb_register_file.s1; break;
    case S2: *val = gdb_register_file.s2; break;
    case S3: *val = gdb_register_file.s3; break;
    case S4: *val = gdb_register_file.s4; break;
    case S5: *val = gdb_register_file.s5; break;
    case S6: *val = gdb_register_file.s6; break;
    case S7: *val = gdb_register_file.s7; break;
    case T8: *val = gdb_register_file.t8; break;
    case T9: *val = gdb_register_file.t9; break;
    case K0: *val = gdb_register_file.k0; break;
    case K1: *val = gdb_register_file.k1; break;
    case GP: *val = gdb_register_file.gp; break;
    case SP: *val = gdb_register_file.sp; break;
    case S8: *val = gdb_register_file.s8; break;
    case RA: *val = gdb_register_file.ra; break;
    case SR: *val = gdb_register_file.sr; break;
    case LO: *val = gdb_register_file.lo; break;
    case HI: *val = gdb_register_file.hi; break;
    case BAD: *val = gdb_register_file.bad; break;
    case CAUSE: *val = gdb_register_file.cause; break;
    case PC: *val = gdb_register_file.pc; break;
    case FSR: *val = 0; break;
    case FIR: *val = 0; break;
    case FP: *val = gdb_register_file.fp; break;
    default: retval = 0;
    }

  return retval;
}


int gdb_poke_register_file (int id, long val)
{
  int retval = sizeof(long);

  switch (id)
    {
    case ZERO: break;
    case AT: gdb_register_file.at = val; break;
    case V0: gdb_register_file.v0 = val; break;
    case V1: gdb_register_file.v1 = val; break;
    case A0: gdb_register_file.a0 = val; break;
    case A1: gdb_register_file.a1 = val; break;
    case A2: gdb_register_file.a2 = val; break;
    case A3: gdb_register_file.a3 = val; break;
    case T0: gdb_register_file.t0 = val; break;
    case T1: gdb_register_file.t1 = val; break;
    case T2: gdb_register_file.t2 = val; break;
    case T3: gdb_register_file.t3 = val; break;
    case T4: gdb_register_file.t4 = val; break;
    case T5: gdb_register_file.t5 = val; break;
    case T6: gdb_register_file.t6 = val; break;
    case T7: gdb_register_file.t7 = val; break;
    case S0: gdb_register_file.s0 = val; break;
    case S1: gdb_register_file.s1 = val; break;
    case S2: gdb_register_file.s2 = val; break;
    case S3: gdb_register_file.s3 = val; break;
    case S4: gdb_register_file.s4 = val; break;
    case S5: gdb_register_file.s5 = val; break;
    case S6: gdb_register_file.s6 = val; break;
    case S7: gdb_register_file.s7 = val; break;
    case T8: gdb_register_file.t8 = val; break;
    case T9: gdb_register_file.t9 = val; break;
    case K0: gdb_register_file.k0 = val; break;
    case K1: gdb_register_file.k1 = val; break;
    case GP: gdb_register_file.gp = val; break;
    case SP: gdb_register_file.sp = val; break;
    case S8: gdb_register_file.s8 = val; break;
    case RA: gdb_register_file.ra = val; break;
    case SR: gdb_register_file.sr = val; break;
    case LO: gdb_register_file.lo = val; break;
    case HI: gdb_register_file.hi = val; break;
    case BAD: gdb_register_file.bad = val; break;
    case CAUSE: gdb_register_file.cause = val; break;
    case PC: gdb_register_file.pc = val; break;
    case FSR: break;
    case FIR: break;
    case FP: gdb_register_file.fp = val; break;
    default: retval = 0;
    }

  return retval;
}


void gdb_startup (void)
{
  mips_stepping = 0;
#if defined(CSB250)
  mips_csb250_startup();
#endif
}


int mips_cond_true (mips_instr_t op)
{
  int ret = 0;
  return ret;
}

mips_instr_t *mips_instr_dest (mips_instr_t op)
{
  return (mips_instr_t*)0;
}

void gdb_step (long addr)
{
#if 1
  /* test code, won't work on branch instructions */
  unsigned long *pc = (unsigned long*)(gdb_register_file.pc + 4);
  mips_stepped_addr = pc;
  mips_stepped_instr = *pc;
  mips_stepping = 1;
  *pc = MIPS_STEPI_OPCODE;
  gdb_return_from_exception();  

#else
  mips_instr_t *dest;
  mips_instr_t op;

  if (addr)
    gdb_register_file.pc = addr;

  mips_stepping = 1;

  op = *(mips_instr_t*)gdb_register_file.pc;
  dest = mips_instr_dest(op);

  mips_stepped_addr = (mips_instr_t *)dest;
  mips_stepped_instr = *mips_stepped_addr;

  *dest = MIPS_STEPI_OPCODE;
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





