#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 12

int vlevel = 0;

void Gets(char* buf) {
    // 模拟不安全的输入函数
    fgets(buf, 100, stdin);  // 故意允许溢出
}

void validate(int level) {
    printf("验证级别 %d 成功!\n", level);
}

void touch1() {
    vlevel = 1;
    printf("Touch1!: You called touch1()\n");
    validate(1);
    exit(0);  // 成功退出
}

unsigned getbuf() {
    char buf[BUFFER_SIZE];
    
    printf("\n=== 调试信息 ===\n");
    printf("buf地址: %p\n", (void*)buf);
    printf("需要覆盖的偏移量: %lu 字节\n", 
           (unsigned long)(sizeof(buf) + sizeof(void*)));
    printf("touch1地址: %p\n", (void*)touch1);
    printf("==================\n\n");
    
    printf("请输入数据: ");
    Gets(buf);
    return 1;
}

void test() {
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val);
}

int main() {
    printf("开始测试...\n");
    test();
    printf("程序正常结束\n");
    return 0;
}
