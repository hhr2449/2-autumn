#include <stdio.h>
#include <stdlib.h>
int main() {
    system("g++ -o rand_input rand_input.cpp");
    system("g++ -o main main.cpp");
    system("g++ -o test test.cpp");
    for(int i = 0; i < 1000; i++) {
        system("./rand_input > input.in");
        system("./main < input.in > output1.out");
        system("./test < input.in > output2.out");
        if(system("diff output1.out output2.out") != 0) {
            printf("测试数据：\n");
            system("cat input.in");
            printf("输出1：\n");
            system("cat output1.out");
            printf("输出2：\n");
            system("cat output2.out");
            return 0;
        }

    }
    printf("AC\n");
    
}