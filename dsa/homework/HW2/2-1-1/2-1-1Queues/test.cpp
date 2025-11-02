#include <stdlib.h>
int main() {
    system("cls");
    system("g++ main.cpp manyqueue.cpp -o main");
    system("./main < input.in > output.out");
}