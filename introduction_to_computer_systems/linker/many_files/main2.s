
add:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	#使用绝对地址来访问全局变量a,b
	movl	a(%rip), %edx
	movl	b(%rip), %eax
	addl	%edx, %eax
	popq	%rbp
	ret