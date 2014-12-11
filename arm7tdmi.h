#ifndef ARM7TDMI_H
#define ARM7TDMI_H

/* TODO: simplify, simplify; what about big endian? */
#define ARM7TDMI_STEPI_OPCODE 0xe7ffdefeUL
#define breakpoint() volatile __asm__(".word 0xe7ffdefe");

extern void arm7tdmi_handle_exception(int exception);

#endif
