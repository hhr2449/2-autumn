#include <stdio.h>
int greater(int a, int b) {
        int res = 0;
        if(a < b) {
            goto el;
        }
        res = a;
        goto done;
    el:
        res = b;
    done:
        return res;

}
int main() {
    printf("%d", greater(1, 2));
}