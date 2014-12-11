.org 0x9000

.global _start
_start:	
	mov r0, #0
	add r0, r0, #1
	.word 0xe7ffdefe
	add r0, r0, #1
	add r0, r0, #1
	.word 0xe7ffdefe
	add r0, r0, #1
	add r0, r0, #1
	.word 0xe7ffdefe
	add r0, r0, #1
	add r0, r0, #1
	.word 0xe7ffdefe
	mov r0, #0
	add r0, r0, #1
	.word 0xe7ffdefe
	add r0, r0, #1
	add r0, r0, #1
	.word 0xe7ffdefe
	add r0, r0, #1
	add r0, r0, #1
	.word 0xe7ffdefe
	add r0, r0, #1
	add r0, r0, #1
	.word 0xe7ffdefe

	b _start			
