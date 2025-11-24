#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;
int add(int a, int b) {
    a = 5;
    b = 10;
    int c = a + b;
    a = c + 888;
    return a + b;

}
int main() {
    int E[10];
    int a = 5;
    int b = 10;
    int c = a + b;
    c = add(a, b);

}