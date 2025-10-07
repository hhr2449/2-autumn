#include <stdio.h>
#include <stdlib.h>
int main() {
    double a = 1e20;
    float b = 1;
    float c = (a + b) - a;
    if (c == b) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
    }
}