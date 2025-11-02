long move(long x, long y) {
    long a, b, c, d;
    
    // 直接赋值（立即数到寄存器）
    a = 100;
    
    // 寄存器到寄存器的移动
    b = x;
    
    // 寄存器到内存的移动
    c = y;
    
    // 内存到寄存器的移动
    d = c;
    
    // 通过内存间接访问
    long *ptr = &a;
    long e = *ptr;
    
    // 返回计算结果
    return a + b + c + d + e;
}