	.file	"relax.cc"
	.text
	.p2align 4,,15
	.globl	_Z5plainv
	.type	_Z5plainv, @function
_Z5plainv:
.LFB341:
	.cfi_startproc
	movl	V(%rip), %eax
	movl	%eax, A(%rip)
	movl	%eax, B(%rip)
	ret
	.cfi_endproc
.LFE341:
	.size	_Z5plainv, .-_Z5plainv
	.p2align 4,,15
	.globl	_Z4atomv
	.type	_Z4atomv, @function
_Z4atomv:
.LFB342:
	.cfi_startproc
	movl	VV(%rip), %eax
	movl	%eax, A(%rip)
	movl	VV(%rip), %eax
	movl	%eax, B(%rip)
	ret
	.cfi_endproc
.LFE342:
	.size	_Z4atomv, .-_Z4atomv
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB343:
	.cfi_startproc
	movl	V(%rip), %eax
	movl	%eax, A(%rip)
	movl	%eax, B(%rip)
	call	_Z4atomv
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE343:
	.size	main, .-main
	.globl	VV
	.bss
	.align 4
	.type	VV, @object
	.size	VV, 4
VV:
	.zero	4
	.globl	V
	.align 4
	.type	V, @object
	.size	V, 4
V:
	.zero	4
	.globl	B
	.align 4
	.type	B, @object
	.size	B, 4
B:
	.zero	4
	.globl	A
	.align 4
	.type	A, @object
	.size	A, 4
A:
	.zero	4
	.ident	"GCC: (Ubuntu 8.4.0-1ubuntu1~18.04) 8.4.0"
	.section	.note.GNU-stack,"",@progbits
