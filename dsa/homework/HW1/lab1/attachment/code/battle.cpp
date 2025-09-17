#include <cstdlib>
#include <cstdio>
int main() {
    //编译
    system("g++ -std=c++11 -o check_input check_input.cpp");
    system("g++ -std=c++11 -o rand_input rand_input.cpp");
    system("g++ -std=c++11 -o solution1 solution1.cpp");
    system("g++ -std=c++11 -o solution2 solution2.cpp");
    //开始测试
    while(1) {
        //输出测试数据
        system("./rand_input > rand.in");
        //检查测试数据
        if(system("./check_input < rand.in") != 0) {
            printf("测试数据错误\n");
            break;
        }
        //运行程序
        system("./solution1 < rand.in > s1.out");
        system("./solution2 < rand.in > s2.out");
        //比较
        if(system("diff s1.out s2.out") != 0) {
            printf("测试数据：\n\n");
            system("cat rand.in");
            printf("\ns1.out：\n\n");
            system("cat s1.out");
            printf("\ns2.out：\n\n");
            system("cat s2.out");
            break;
        }
    }
}