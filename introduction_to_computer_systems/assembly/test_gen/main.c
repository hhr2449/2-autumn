long gt (long x, long y) {
    return x > y;
}

/** 	
 * 源文件名称
    .file	"main.c"
 * 段指令
	.text
 * 对齐方式
	.p2align 4
 * 编译器指令：gt为全局，gt的类型是函数
	.globl	gt
	.type	gt, @function

 * 函数开始
gt:
 * 内部标签，标识函数开始
.LFB0:
 * 栈帧信息开始
	.cfi_startproc
	endbr64
 * 真正的汇编指令
	xorl	%eax, %eax
	cmpq	%rsi, %rdi
	setg	%al
 * 函数返回
	ret
 * 栈帧信息结束
	.cfi_endproc
 * 内部标签，表示函数结束
.LFE0:
	.size	gt, .-gt
 * 编译器信息
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
  **/