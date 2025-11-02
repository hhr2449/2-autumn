func1:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	nop
	popq	%rbp
	ret
func2:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%r8d, -20(%rbp)
	movl	%r9d, -24(%rbp)
	nop
	popq	%rbp
	ret
func3:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%r8d, -20(%rbp)
	movl	%r9d, -24(%rbp)
	nop
	popq	%rbp
	ret
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movl	$1, -32(%rbp)
	movl	$2, -28(%rbp)
	movl	$3, -24(%rbp)
	movl	$4, -20(%rbp)
	movl	$5, -16(%rbp)
	movl	$6, -12(%rbp)
	movl	$7, -8(%rbp)
	movl	$8, -4(%rbp)
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %ecx
	movl	-32(%rbp), %eax
	movl	%ecx, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	func1
	movl	-12(%rbp), %r8d
	movl	-16(%rbp), %edi
	movl	-20(%rbp), %ecx
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %esi
	movl	-32(%rbp), %eax
	movl	%r8d, %r9d
	movl	%edi, %r8d
	movl	%eax, %edi
	movl	$0, %eax
	call	func2
	movl	-12(%rbp), %r9d
	movl	-16(%rbp), %r8d
	movl	-20(%rbp), %ecx
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %esi
	movl	-32(%rbp), %eax
	movl	-4(%rbp), %edi
	pushq	%rdi
	movl	-8(%rbp), %edi
	pushq	%rdi
	movl	%eax, %edi
	movl	$0, %eax
	call	func3
	addq	$16, %rsp
	movl	$0, %eax
	leave
	ret