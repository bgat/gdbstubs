	.file	"sh2.c"
gcc2_compiled.:
	.text
	.align 4
	.type	 _get_stepi_dest,@function
_get_stepi_dest:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-8,r15
	mov	r15,r14
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(20,r6),r7
	mov.w	@r7,r6
	mov.w	r6,@r14
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(20,r6),r7
	mov	r7,r6
	add	#2,r6
	mov.l	r6,@(4,r14)
	mov.w	@r14,r7
	mov.l	.L29,r6
	and	r6,r7
	mov.l	.L30,r6
	cmp/eq	r6,r7
	bt	.L6
	mov.w	@r14,r7
	mov.l	.L29,r6
	and	r6,r7
	mov.l	.L31,r6
	cmp/eq	r6,r7
	bt	.L6
	bra	.L5
	nop
	.align 4
.L6:
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(24,r6),r5
	mov	#1,r7
	and	r5,r7
	tst	r7,r7
	bf	.L4
	bra	.L5
	nop
	.align 4
.L5:
	mov.w	@r14,r7
	mov.l	.L29,r6
	and	r6,r7
	mov.l	.L32,r6
	cmp/eq	r6,r7
	bt	.L7
	mov.w	@r14,r7
	mov.l	.L29,r6
	and	r6,r7
	mov.l	.L33,r6
	cmp/eq	r6,r7
	bt	.L7
	bra	.L3
	nop
	.align 4
.L7:
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(24,r6),r5
	mov	#1,r7
	and	r5,r7
	tst	r7,r7
	bf	.L3
	bra	.L4
	nop
	.align 4
.L4:
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(20,r6),r7
	mov	r7,r1
	add	#4,r1
	mov.w	@r14,r7
	mov.w	.L34,r6
	and	r7,r6
	exts.w	r6,r7
	tst	r7,r7
	bt	.L8
	mov.w	@r14,r7
	mov	#-128,r6
	or	r6,r7
	mov	r7,r6
	add	r6,r6
	mov	r1,r7
	add	r6,r7
	bra	.L9
	nop
	.align 4
.L8:
	mov.w	@r14,r6
	mov	#127,r5
	and	r6,r5
	exts.w	r5,r6
	mov	r6,r5
	add	r5,r5
	mov	r1,r7
	add	r5,r7
.L9:
	mov.l	r7,@(4,r14)
	bra	.L10
	nop
	.align 4
.L3:
	mov.w	@r14,r7
	mov.l	.L35,r6
	and	r6,r7
	mov.l	.L36,r6
	cmp/eq	r6,r7
	bf	.L11
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(20,r6),r7
	mov	r7,r2
	add	#4,r2
	mov.w	@r14,r7
	mov.w	.L37,r6
	and	r6,r7
	exts.w	r7,r6
	tst	r6,r6
	bt	.L12
	mov.w	@r14,r7
	mov.w	.L38,r6
	or	r6,r7
	mov	r7,r6
	add	r6,r6
	mov	r2,r7
	add	r6,r7
	bra	.L13
	nop
	.align 4
.L12:
	mov.w	@r14,r6
	mov.w	.L39,r5
	and	r5,r6
	exts.w	r6,r5
	mov	r5,r6
	add	r6,r6
	mov	r2,r7
	add	r6,r7
.L13:
	mov.l	r7,@(4,r14)
	bra	.L10
	nop
	.align 4
.L11:
	mov.w	@r14,r7
	mov.l	.L40,r6
	mov	r7,r0
	and	r6,r0
	cmp/eq	#35,r0
	bf	.L15
	mov.l	.L28,r6
	mov	r6,r7
	add	#64,r7
	mov.l	.L28,r6
	mov.w	@r14,r5
	mov.w	.L41,r4
	and	r4,r5
	exts.w	r5,r0
	mov	r0,r4
	mov.l	.L42,r5
	jsr	@r5
	nop
	mov	r4,r5
	exts.w	r5,r4
	mov	r4,r5
	mov	r5,r0
	shll2	r0
	add	#20,r6
	mov.l	@(r0,r6),r5
	mov	r5,r6
	add	#4,r6
	mov.l	@(20,r7),r7
	add	r7,r6
	mov.l	r6,@(4,r14)
	bra	.L10
	nop
	.align 4
.L15:
	mov.w	@r14,r7
	mov.l	.L35,r6
	and	r6,r7
	mov.l	.L43,r6
	cmp/eq	r6,r7
	bf	.L17
	mov.l	.L28,r7
	mov	r7,r6
	add	#64,r6
	mov.l	@(20,r6),r7
	mov	r7,r3
	add	#4,r3
	mov.w	@r14,r7
	mov.w	.L37,r6
	and	r6,r7
	exts.w	r7,r6
	tst	r6,r6
	bt	.L18
	mov.w	@r14,r7
	mov.w	.L38,r6
	or	r6,r7
	mov	r7,r6
	add	r6,r6
	mov	r3,r7
	add	r6,r7
	bra	.L19
	nop
	.align 4
.L18:
	mov.w	@r14,r6
	mov.w	.L39,r5
	and	r5,r6
	exts.w	r6,r5
	mov	r5,r6
	add	r6,r6
	mov	r3,r7
	add	r6,r7
.L19:
	mov.l	r7,@(4,r14)
	bra	.L10
	nop
	.align 1
.L34:
	.short	128
.L37:
	.short	2048
.L38:
	.short	-2048
.L39:
	.short	2047
.L41:
	.short	3840
.L44:
	.align 2
.L28:
	.long	_register_file
.L29:
	.long	65280
.L30:
	.long	35072
.L31:
	.long	36096
.L32:
	.long	35584
.L33:
	.long	36608
.L35:
	.long	61440
.L36:
	.long	40960
.L40:
	.long	61695
.L42:
	.long	___ashiftrt_r4_8
.L43:
	.long	45056
	.align 4
.L17:
	mov.w	@r14,r7
	mov.l	.L45,r6
	mov	r7,r0
	and	r6,r0
	cmp/eq	#3,r0
	bf	.L21
	mov.l	.L46,r6
	mov	r6,r7
	add	#64,r7
	mov.l	.L46,r6
	mov.w	@r14,r5
	mov	r5,r4
	mov.l	.L47,r5
	jsr	@r5
	nop
	mov	r4,r5
	mov	r5,r4
	mov	#15,r5
	and	r4,r5
	exts.w	r5,r4
	mov	r4,r5
	mov	r5,r0
	shll2	r0
	add	#20,r6
	mov.l	@(r0,r6),r5
	mov	r5,r6
	add	#4,r6
	mov.l	@(20,r7),r7
	add	r7,r6
	mov.l	r6,@(4,r14)
	bra	.L10
	nop
	.align 4
.L21:
	mov.w	@r14,r7
	mov.l	.L45,r6
	and	r6,r7
	mov.w	.L48,r6
	cmp/eq	r6,r7
	bf	.L23
	mov.l	.L46,r7
	mov.w	@r14,r6
	mov	r6,r4
	mov.l	.L47,r6
	jsr	@r6
	nop
	mov	r4,r6
	mov	r6,r5
	mov	#15,r6
	and	r5,r6
	exts.w	r6,r5
	mov	r5,r6
	mov	r6,r0
	shll2	r0
	add	#20,r7
	mov.l	@(r0,r7),r6
	mov.l	r6,@(4,r14)
	bra	.L10
	nop
	.align 4
.L23:
	mov.w	@r14,r7
	mov.l	.L45,r6
	and	r6,r7
	mov.w	.L49,r6
	cmp/eq	r6,r7
	bf	.L25
	mov.l	.L46,r7
	mov.w	@r14,r6
	mov	r6,r4
	mov.l	.L47,r6
	jsr	@r6
	nop
	mov	r4,r6
	mov	r6,r5
	mov	#15,r6
	and	r5,r6
	exts.w	r6,r5
	mov	r5,r6
	mov	r6,r0
	shll2	r0
	add	#20,r7
	mov.l	@(r0,r7),r6
	mov.l	r6,@(4,r14)
	bra	.L10
	nop
	.align 4
.L25:
	mov.w	@r14,r0
	cmp/eq	#11,r0
	bf	.L10
	mov.l	.L46,r7
	mov.l	@r7,r6
	mov.l	r6,@(4,r14)
.L27:
.L26:
.L24:
.L22:
.L20:
.L16:
.L14:
.L10:
	mov.l	@(4,r14),r7
	mov	r7,r0
	bra	.L2
	nop
.L2:
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
	.align 1
.L48:
	.short	16427
.L49:
	.short	16395
.L50:
	.align 2
.L45:
	.long	61695
.L46:
	.long	_register_file
.L47:
	.long	___ashiftrt_r4_8
.Lfe1:
	.size	 _get_stepi_dest,.Lfe1-_get_stepi_dest
	.align 4
	.global	_gdb_step
	.type	 _gdb_step,@function
_gdb_step:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-8,r15
	mov	r15,r14
	mov.l	r4,@r14
	mov.l	@r14,r1
	tst	r1,r1
	bt	.L52
	mov.l	.L53,r1
	mov	r1,r2
	add	#64,r2
	mov.l	@r14,r1
	mov.l	r1,@(20,r2)
.L52:
	mov	r15,r1
	mov.l	.L54,r8
	jsr	@r8
	nop
	mov.l	r0,@(4,r14)
	mov.l	.L55,r1
	mov.l	@(4,r14),r2
	mov.w	@r2,r3
	mov.w	r3,@r1
	mov.l	@(4,r14),r1
	mov.w	.L56,r2
	mov.w	r2,@r1
	mov	r15,r1
	mov.l	.L57,r8
	jsr	@r8
	nop
	bra	.L51
	nop
	.align 4
.L51:
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L56:
	.short	-15584
.L58:
	.align 2
.L53:
	.long	_register_file
.L54:
	.long	_get_stepi_dest
.L55:
	.long	_stepped_opcode
.L57:
	.long	_gdb_return_from_exception
.Lfe2:
	.size	 _gdb_step,.Lfe2-_gdb_step
	.align 4
	.global	_gdb_enter_monitor
	.type	 _gdb_enter_monitor,@function
_gdb_enter_monitor:
	mov.l	r14,@-r15
	mov	r15,r14
	mov.l	.L61,r1
	mov.w	@r1,r2
	tst	r2,r2
	bt	.L60
	mov.l	.L62,r1
	mov	r1,r2
	add	#64,r2
	mov.l	@(20,r2),r1
	mov.l	.L61,r2
	mov.w	@r2,r3
	mov.w	r3,@r1
	mov.l	.L61,r1
	mov	#0,r2
	mov.w	r2,@r1
.L60:
	bra	.L59
	nop
	.align 4
.L59:
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L63:
	.align 2
.L61:
	.long	_stepped_opcode
.L62:
	.long	_register_file
.Lfe3:
	.size	 _gdb_enter_monitor,.Lfe3-_gdb_enter_monitor
	.align 4
	.global	_gdb_peek_register_file
	.type	 _gdb_peek_register_file,@function
_gdb_peek_register_file:
	mov.l	r14,@-r15
	add	#-12,r15
	mov	r15,r14
	mov.l	r4,@r14
	mov.l	r5,@(4,r14)
	mov	#4,r1
	mov.l	r1,@(8,r14)
	mov.l	@r14,r1
	mov	#0,r2
	mov	#22,r3
	sub	r2,r1
	cmp/hi	r3,r1
	bf	.L95
	bra	.L89
	nop
.L95:
	mova	.L90,r0
	add	r1,r1
	mov.w	@(r0,r1),r1
	braf	r1
	nop
.L91:
	.align 2
	.align 2
.L90:
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L81-.L91
	.word	.L82-.L91
	.word	.L83-.L91
	.word	.L84-.L91
	.word	.L85-.L91
	.word	.L86-.L91
	.word	.L87-.L91
	.word	.L88-.L91
	.align 4
.L66:
.L67:
.L68:
.L69:
.L70:
.L71:
.L72:
.L73:
.L74:
.L75:
.L76:
.L77:
.L78:
.L79:
.L80:
.L81:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov.l	@r14,r3
	mov	r3,r7
	mov	r7,r0
	shll2	r0
	add	#20,r2
	mov.l	@(r0,r2),r3
	mov.l	r3,@r1
	bra	.L65
	nop
	.align 4
.L82:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov	r2,r3
	add	#64,r3
	mov.l	@(20,r3),r2
	mov.l	r2,@r1
	bra	.L65
	nop
	.align 4
.L83:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov.l	@r2,r3
	mov.l	r3,@r1
	bra	.L65
	nop
	.align 4
.L84:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov.l	@(4,r2),r3
	mov.l	r3,@r1
	bra	.L65
	nop
	.align 4
.L85:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov.l	@(8,r2),r3
	mov.l	r3,@r1
	bra	.L65
	nop
	.align 4
.L86:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov.l	@(12,r2),r3
	mov.l	r3,@r1
	bra	.L65
	nop
	.align 4
.L87:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov.l	@(16,r2),r3
	mov.l	r3,@r1
	bra	.L65
	nop
	.align 4
.L88:
	mov.l	@(4,r14),r1
	mov.l	.L92,r2
	mov	r2,r3
	add	#64,r3
	mov.l	@(24,r3),r2
	mov.l	r2,@r1
	bra	.L65
	nop
	.align 4
.L89:
	mov	#0,r1
	mov.l	r1,@(8,r14)
.L65:
	mov.l	@(8,r14),r1
	mov	r1,r0
	bra	.L64
	nop
	.align 4
.L64:
	add	#12,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L93:
	.align 2
.L92:
	.long	_register_file
.Lfe4:
	.size	 _gdb_peek_register_file,.Lfe4-_gdb_peek_register_file
	.align 4
	.global	_gdb_poke_register_file
	.type	 _gdb_poke_register_file,@function
_gdb_poke_register_file:
	mov.l	r14,@-r15
	add	#-12,r15
	mov	r15,r14
	mov.l	r4,@r14
	mov.l	r5,@(4,r14)
	mov	#4,r1
	mov.l	r1,@(8,r14)
	mov.l	@r14,r1
	mov	#0,r2
	mov	#22,r3
	sub	r2,r1
	cmp/hi	r3,r1
	bf	.L127
	bra	.L121
	nop
.L127:
	mova	.L122,r0
	add	r1,r1
	mov.w	@(r0,r1),r1
	braf	r1
	nop
.L123:
	.align 2
	.align 2
.L122:
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L113-.L123
	.word	.L114-.L123
	.word	.L115-.L123
	.word	.L116-.L123
	.word	.L117-.L123
	.word	.L118-.L123
	.word	.L119-.L123
	.word	.L120-.L123
	.align 4
.L98:
.L99:
.L100:
.L101:
.L102:
.L103:
.L104:
.L105:
.L106:
.L107:
.L108:
.L109:
.L110:
.L111:
.L112:
.L113:
	mov.l	.L124,r1
	mov.l	@r14,r2
	mov	r2,r3
	mov	r3,r0
	shll2	r0
	add	#20,r1
	mov.l	@(4,r14),r2
	mov.l	r2,@(r0,r1)
	bra	.L97
	nop
	.align 4
.L114:
	mov.l	.L124,r1
	mov	r1,r2
	add	#64,r2
	mov.l	@(4,r14),r1
	mov.l	r1,@(20,r2)
	bra	.L97
	nop
	.align 4
.L115:
	mov.l	.L124,r1
	mov.l	@(4,r14),r2
	mov.l	r2,@r1
	bra	.L97
	nop
	.align 4
.L116:
	mov.l	.L124,r1
	mov.l	@(4,r14),r2
	mov.l	r2,@(4,r1)
	bra	.L97
	nop
	.align 4
.L117:
	mov.l	.L124,r1
	mov.l	@(4,r14),r2
	mov.l	r2,@(8,r1)
	bra	.L97
	nop
	.align 4
.L118:
	mov.l	.L124,r1
	mov.l	@(4,r14),r2
	mov.l	r2,@(12,r1)
	bra	.L97
	nop
	.align 4
.L119:
	mov.l	.L124,r1
	mov.l	@(4,r14),r2
	mov.l	r2,@(16,r1)
	bra	.L97
	nop
	.align 4
.L120:
	mov.l	.L124,r1
	mov	r1,r2
	add	#64,r2
	mov.l	@(4,r14),r1
	mov.l	r1,@(24,r2)
	bra	.L97
	nop
	.align 4
.L121:
	mov	#0,r1
	mov.l	r1,@(8,r14)
.L97:
	mov.l	@(8,r14),r1
	mov	r1,r0
	bra	.L96
	nop
	.align 4
.L96:
	add	#12,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L125:
	.align 2
.L124:
	.long	_register_file
.Lfe5:
	.size	 _gdb_poke_register_file,.Lfe5-_gdb_poke_register_file
	.align 4
	.global	_gdb_continue
	.type	 _gdb_continue,@function
_gdb_continue:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-4,r15
	mov	r15,r14
	mov.l	r4,@r14
	mov.l	@r14,r1
	tst	r1,r1
	bt	.L129
	mov.l	.L130,r1
	mov	r1,r2
	add	#64,r2
	mov.l	@r14,r1
	mov.l	r1,@(20,r2)
.L129:
	mov	r15,r1
	mov.l	.L131,r8
	jsr	@r8
	nop
	bra	.L128
	nop
	.align 4
.L128:
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
.L132:
	.align 2
.L130:
	.long	_register_file
.L131:
	.long	_gdb_return_from_exception
.Lfe6:
	.size	 _gdb_continue,.Lfe6-_gdb_continue
	.align 4
	.global	_gdb_kill
	.type	 _gdb_kill,@function
_gdb_kill:
	mov.l	r14,@-r15
	mov	r15,r14
	mov #0, r0       
	mov.l @r0, r0    
	jmp @r0          
	nop              
.L133:
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.Lfe7:
	.size	 _gdb_kill,.Lfe7-_gdb_kill
	

/*
  Generic code to save processor context.
  Assumes the stack looks like this:

  sigval<-r15
  r1
  r0
  pc
  sr
*/
save_registers_handle_exception:

  /* find end of register_file */
  mov.l register_file_end, r0

  /* copy sr to register file */
  mov.l @(16, r15), r1
  mov.l r1, @r0

  /* copy pc to register file */
  mov.l @(12, r15), r1
  mov.l r1, @-r0

  /* sigval, r1, r0, pc, sr are already on the stack, */
  /* so r15 isn't the same as it was immediately before */
  /* we took the current exception.  We have to adjust */
  /* r15 in the register file so that gdb gets the right */
  /* stack pointer value */
  mov r15, r1
  add #20, r1
  mov.l r1, @-r0

  /* save r14-r2 */
  mov.l r14, @-r0
  mov.l r13, @-r0
  mov.l r12, @-r0
  mov.l r11, @-r0
  mov.l r10, @-r0
  mov.l r9, @-r0
  mov.l r8, @-r0
  mov.l r7, @-r0
  mov.l r6, @-r0
  mov.l r5, @-r0
  mov.l r4, @-r0
  mov.l r3, @-r0
  mov.l r2, @-r0

  /* copy r1 to register file */
  mov.l @(4, r15), r1
  mov.l r1, @-r0

  /* copy r0 to register file */
  mov.l @(8, r15), r1
  mov.l r1, @-r0

  /* save macl, mach, vbr, gbr, pr in register file */
  sts.l macl, @-r0
  sts.l mach, @-r0
  stc.l vbr, @-r0
  stc.l gbr, @-r0
  sts.l pr, @-r0

  /* call gdb_handle_exception */
  mov.l handle_exception, r0
  mov.l @r15, r4
  jsr @r0
  nop

  .align 2
  handle_exception: .long _gdb_handle_exception
  register_file_end: .long _register_file+88



/* unhandled exception isr */
.global _gdb_unhandled_isr
_gdb_unhandled_isr:

  /* put r0, r1 on the stack */
  mov.l r0, @-r15
  mov   #0xf0, r0
  ldc   r0, sr
  mov.l r1, @-r15

  /* put SIGUSR on stack */
  mov #30, r0
  mov.l r0, @-r15

  /* save registers, call gdb_handle_exception */
  bra save_registers_handle_exception
  nop



/*
  TRAPA #32 (breakpoint) isr.
  Sends a SIGTRAP to gdb_handle_exception().

  Because we always subtract 2 from the pc
  stacked during exception processing, this
  function won't permit compiled-in breakpoints.
  If you compile a TRAPA #32 into the code, we'll
  loop on it indefinitely.  Use TRAPA #33 instead.

  TODO: we could examine the opcode, and only back up PC on #32.

*/
.global _gdb_trapa32_isr
_gdb_trapa32_isr:

  /* put r0, r1 on the stack */
  mov.l r0, @-r15
  mov   #0xf0, r0
  ldc   r0, sr
  mov.l r1, @-r15

  /* put SIGTRAP on stack */
  mov #5, r0
  mov.l r0, @-r15

  /* fudge pc, so we re-execute the instruction replaced
     by the trap; this breaks compiled-in breakpoints! */
  mov.l @(12, r15), r0
  add #-2, r0
  mov.l r0, @(12, r15)

  /* save registers, call gdb_handle_exception */
  bra save_registers_handle_exception
  nop


.global _gdb_trapa33_isr
_gdb_trapa33_isr:

  /* put r0, r1 on the stack */
  mov.l r0, @-r15
  mov   #0xf0, r0
  ldc   r0, sr
  mov.l r1, @-r15

  /* put SIGTRAP on stack */
  mov #5, r0
  mov.l r0, @-r15

  /* save registers,
  call gdb_handle_exception */
  bra save_registers_handle_exception
  nop


.global _gdb_illegalinst_isr
_gdb_illegalinst_isr:

  /* put r0, r1 on the stack */
  mov.l r0, @-r15
  mov   #0xf0, r0
  ldc   r0, sr
  mov.l r1, @-r15

  /* put SIGILL on stack */
  mov #4, r0
  mov.l r0, @-r15

  /* save registers,
  call gdb_handle_exception */
  bra save_registers_handle_exception
  nop


.global _gdb_addresserr_isr
_gdb_addresserr_isr:

  /* put r0, r1 on the stack */
  mov.l r0, @-r15
  mov   #0xf0, r0
  ldc   r0, sr
  mov.l r1, @-r15

  /* put SIGSEGV on stack */
  mov #11, r0
  mov.l r0, @-r15

  /* save registers,
  call gdb_handle_exception */
  bra save_registers_handle_exception
  nop


/* Restores registers to the values specified in register_file.  */
.global _gdb_return_from_exception
_gdb_return_from_exception:

  /* find register_file */
  mov.l register_file, r0
  lds.l @r0+, pr
  ldc.l @r0+, gbr
  ldc.l @r0+, vbr
  lds.l @r0+, mach
  lds.l @r0+, macl

  /* skip r0 and r1 for now,
     since we're using them */
  add #8, r0

  mov.l @r0+, r2
  mov.l @r0+, r3
  mov.l @r0+, r4
  mov.l @r0+, r5
  mov.l @r0+, r6
  mov.l @r0+, r7
  mov.l @r0+, r8
  mov.l @r0+, r9
  mov.l @r0+, r10
  mov.l @r0+, r11
  mov.l @r0+, r12
  mov.l @r0+, r13
  mov.l @r0+, r14
  mov.l @r0+, r15

  /* put sr onto stack */
  mov.l @(4,r0), r1
  mov.l r1, @-r15

  /* put pc onto stack */
  mov.l @r0, r1
  mov.l r1, @-r15

  /* restore r1, r0 */
  add #-64, r0
  mov.l @(4,r0), r1
  mov.l @r0, r0

  rte
  nop
.align 2
  register_file: .long _register_file


	   .global _gdb_ubc_isr                
	   _gdb_ubc_isr:                       
	   /* put r0, r1 on the stack */       
	   mov.l r0, @-r15                     
	   mov   #0xf0, r0                     
	   ldc   r0, sr                        
	   mov.l r1, @-r15                     
	   /* put SIGTRAP on stack */          
	   mov #5, r0                          
	   mov.l r0, @-r15                     
	   /* zero UBBR to turn off UBC */     
	   mov.l ubc_ubbr, r0                  
	   mov #0, r1                          
	   mov.w r1, @r0                       
	   /* save registers,                  
	      call gdb_handle_exception */     
	   bra save_registers_handle_exception 
	   nop                                 
	   .align 2                            
	   ubc_ubbr: .long 0xffff8608          
.section	.gdbregfile,"aw",@progbits
	.align 2
	.type	 _register_file,@object
	.size	 _register_file,92
_register_file:
	.zero	92
	.align 1
	.type	 _stepped_opcode,@object
	.size	 _stepped_opcode,2
_stepped_opcode:
	.zero	2
	.ident	"GCC: (GNU) 2.95.3 20010315 (release)"
