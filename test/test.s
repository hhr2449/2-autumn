    .globl main
main:
    # 假设这是 test 函数中的部分代码
# 目标：从 getbuf 栈帧中取出 cookie，并传给 touch3

    movq %rsp, %rax           # 获取当前栈顶地址（getbuf 的栈顶）
    movq %rax, %rdi
    popq %rax                 # 弹出 0x48（作为偏移量，或栈上保存的值）
    # 此时 %rax = 0x48

    movl %eax, %edx           # 将 %eax（低32位）复制到 %edx
    movl %edx, %ecx           # 将 %ecx 复制到 %esi
    movl %ecx, %esi           # 将 %edx 复制到 %ecx

    leaq (%rdi,%rsi,1), %rax   # 计算 cookie 地址：%rdi + %rsi
    movq %rax, %rdi           # 把 cookie 地址传给 %rdi
    ret
