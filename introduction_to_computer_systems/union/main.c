#include<stdio.h>
// 同一个内存，可以用作int，也可以用作char数组
union IntBytes {
    int i;
    char b[4];
};
int main() {
    union IntBytes u;
    u.i = 1;
    // 可以发现此为小端模式
    for(int i = 0; i < 4; i++) {
        printf("%02x ", u.b[i]);
    }
}