#include <stdio.h>
#include <cmath>
int division(int a, int k) {
    int res = a / pow(2, k);
    return res;
}
int main() {
    int a, k;
    scanf("%d %d", &a, &k);
    printf("%d\n", division(a, k));
}