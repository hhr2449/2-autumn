	.file	"main.c"
	.text
	.globl	move
	.type	move, @function
move:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)  //寄存器到内存
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax		 
	movq	$100, -56(%rbp)  //立即数到内存
	movq	-72(%rbp), %rax  //内存到寄存器
	movq	%rax, -48(%rbp)
	movq	-80(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -32(%rbp)
	leaq	-56(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	movq	-56(%rbp), %rdx
	movq	-48(%rbp), %rax
	addq	%rax, %rdx
	movq	-40(%rbp), %rax
	addq	%rax, %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	move, .-move
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
