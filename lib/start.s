	.globl	_start
	.align	4
	.text
_start:
	lih	r0, 0x400
	srspw	r0
	movepc	rret, 8
	br	start, #al
	br	0, #al
