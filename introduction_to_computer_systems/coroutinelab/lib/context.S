.global coroutine_entry

coroutine_entry:
    movq %r13, %rdi
    callq *%r12

.global coroutine_switch
coroutine_switch:
    # TODO: Task 1
    # 保存 callee-saved 寄存器到 %rdi 指向的上下文
    # 对于caller-saved寄存器，如果有用的话在调用协程切换函数的时候就会进行保留了
    # 保存的上下文中 rip 指向 ret 指令的地址（.coroutine_ret）
    # 第一个参数中保存当前的状态
    # 根据寄存器枚举类来确定各个寄存器的保存顺序
    movq %rsp, 64(%rdi)
    movq %rbx, 72(%rdi)
    movq %rbp, 80(%rdi)
    movq %r12, 88(%rdi)
    movq %r13, 96(%rdi)
    movq %r14, 104(%rdi)
    movq %r15, 112(%rdi)
    # rip保存为 .coroutine_ret的地址(这里是rip相对寻址)
    leaq .coroutine_ret(%rip), %rax
    movq %rax, 120(%rdi)


    # 从 %rsi 指向的上下文恢复 callee-saved 寄存器
    # 最后 jmpq 到上下文保存的 rip
    movq 64(%rsi), %rsp
    movq 72(%rsi), %rbx
    movq 80(%rsi), %rbp
    movq 88(%rsi), %r12
    movq 96(%rsi), %r13
    movq 104(%rsi), %r14
    movq 112(%rsi), %r15
    # jmp到保存的rip处（其实就是entry）
    jmpq *120(%rsi)

.coroutine_ret:
    ret
