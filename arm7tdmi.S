	.section .text

.global arm7tdmi_fiq_handler
arm7tdmi_fiq_handler:
	b arm7tdmi_fiq_handler
	
	
	.global arm7tdmi_und_handler
arm7tdmi_und_handler:
	ldr sp, __und_stack
	stmfd sp, {r0-r12,sp,lr}^
	nop
	sub sp, sp, #(15*4)
	mrs r0, spsr
	stmfd sp!, {r0}
	sub lr, lr, #4
	stmfd sp!, {lr}
	mov r0, #5
	b arm7tdmi_handle_exception

__und_stack:		.word gdb_register_file + ((128+17)*4)
__register_file:	.word gdb_register_file

.global gdb_return_from_exception
gdb_return_from_exception:
	ldr sp, __register_file
	add sp, sp, #(130*4)
	ldmfd sp, {r0-r12,sp,lr}^
	nop
	ldmea sp!, {lr}
	msr spsr, lr
	ldmea sp!, {lr}
	subs pc, lr, #0


#if defined(CRT0)

.global start
.global _start
_start:
start:
	/* disable interrupts (assume we're in user/system mode) */
	mrs r0, cpsr
	orr r0, r0, #0xc0
	msr cpsr, r0

	/* put arm7tdmi_und_handler in the exception vector table */
	ldr r0, __bl_opcode
	ldr r1, __handler
	mov r1, r1, lsr #2
	sub r1, r1, #3
	orr r1, r1, r0
	mov r0, #4
	str r1, [r0]

	/* initialize the stack pointer */
	ldr r0, __sys_stack
	mov sp, r0

	/* initialize gdbstubs */
	bl gdb_startup
	
	/* load some registers, force a "breakpoint" */
	mov r0, #0
	mov r1, #1
	mov r2, #2
	mov r3, #3
	mov r4, #4
	mov r5, #5
	mov r6, #6
	mov r7, #7
	.word 0xe7ffdefe

#if 1
	/* some test code */
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

__sys_stack:		.word _stack
__bl_opcode:		.word 0xea000000
__handler:		.word arm7tdmi_und_handler

#endif
		

