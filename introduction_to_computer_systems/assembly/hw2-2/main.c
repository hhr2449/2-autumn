#include <stdio.h>
int main() {
    int x = -1;
    unsigned ux = (unsigned) x;
    printf("%d\n", (x == ux));
}