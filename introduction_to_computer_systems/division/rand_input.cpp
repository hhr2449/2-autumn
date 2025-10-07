#include <cstdio>
#include <cstdlib>
#include <ctime>
int main() {
    srand(time(NULL));
    int a, k;
    a = rand() % 10000;
    k = rand() % 31;
    int is_nagtive = rand() % 2;
    if(is_nagtive == 1) {
        a = -a;
    }
    printf("%d %d\n", a, k);
}