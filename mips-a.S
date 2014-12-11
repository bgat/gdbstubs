#include "mips.h"
	.section .text

breakpoint:	
	addiu	sp, sp, -4
	sw	k0, (sp)
	la	k0, gdb_register_file
	.set noat
	sw	$1, 1*4(k0)
	.set at
	sw	v0, 2*4(k0)
	sw	v1, 3*4(k0)
	sw	a0, 4*4(k0)
	sw	a1, 5*4(k0)
	sw	a2, 6*4(k0)
	sw	a3, 7*4(k0)
	sw	t0, 8*4(k0)
	sw	t1, 9*4(k0)
	sw	t2,10*4(k0)
	sw	t3,11*4(k0)
	sw	t4,12*4(k0)
	sw	t5,13*4(k0)
	sw	t6,14*4(k0)
	sw	t7,15*4(k0)
	sw	s0,16*4(k0)
	sw	s1,17*4(k0)
	sw	s2,18*4(k0)
	sw	s3,19*4(k0)
	sw	s4,20*4(k0)
	sw	s5,21*4(k0)
	sw	s6,22*4(k0)
	sw	s7,23*4(k0)
	sw	t8,24*4(k0)
	sw	t9,25*4(k0)

	/* TODO: from here down is wrong */
	sw	k0,26*4(k0)
	sw	k1,27*4(k0)
	sw	gp,28*4(k0)
	sw	sp,29*4(k0)
	sw	s8,30*4(k0)
	sw	ra,31*4(k0)

	/* TODO: something involving EPC, I bet */
	j	mips_handle_exception
	nop


	.global gdb_return_from_exception
gdb_return_from_exception:
	/* TODO: finish this */

	

#if defined(CRT0)

.global start
.global _start
_start:
start:
	/* (assume uMON has given us a stack) */

	/* install breakpoint handler */
	/* ... */

	jal gdb_startup
	
	/* force a breakpoint */
	break	5
	nop

#if 0
	/* some (ARM) test code */
top:	mov r0, #0
	mov r1, #1
	mov r2, #2
	mov r3, #3
	mov r4, #4
	mov r5, #5
	mov r6, #6
	mov r7, #7
	b 1f
	nop
	nop
1:	nop
	mov lr, pc
	add lr, lr, #12
	mov pc, lr
	nop
	nop
	nop
	nop
	nop
	nop
	mov lr, pc
	add lr, lr, #20
	stmfd sp!, {r1,lr}
	mov r1, #0
	ldmfd sp!, {r1,pc}
	nop
	nop
	nop
	nop
	nop
	b top
#endif
#endif
		

