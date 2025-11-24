#include <stdio.h>
struct prob {
    int *p;
    struct {
        int x;
        int y;
    } s;
    struct prob *next;
};
int main() {
    struct prob *sp;
    printf("%lld\n", &(sp->s) == &(sp->s.x));
}