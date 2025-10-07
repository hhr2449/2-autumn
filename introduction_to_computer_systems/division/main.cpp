#include <stdio.h>
//使用移位运算来实现a/2^k的计算
int division(int a, int k) {
    int bias = (a >> (sizeof(int) * 8 - 1)) & ((1 << k) - 1);
    return (a + bias) >> k;
}

int main() {
    //除法向0舍去
    // printf("%d\n", division(10, 2));
    // printf("%d\n", division(-10, 2));
    int a, k;
    scanf("%d %d", &a, &k);
    printf("%d\n", division(a, k));

}