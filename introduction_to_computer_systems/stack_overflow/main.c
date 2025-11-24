#include <stdio.h>
void echo() {
    char buf[4];
    // gets(buf)会读入字符并且存入buf中，这一个读入是无限制的，如果读入的过多会发生溢出
    gets(buf);
    puts(buf);
}
// 未关闭栈保护时：输入字符小于等于4个时正常，否则会触发保护
// 关闭栈保护:略微超出buf的范围，此时没有超出缓冲区，不会产生段错误
// 超出缓冲区之后就会产生段错误
int main() {
    echo();
}