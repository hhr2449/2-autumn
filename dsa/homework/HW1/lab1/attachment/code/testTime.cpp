#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <sys/time.h>
//获取时间
//timeval类：
// struct timeval {
//     time_t      tv_sec;   // 秒数（自1970-01-01 00:00:00 UTC起）
//     suseconds_t tv_usec;  // 微秒数（0 ~ 999999）
// };
//有两个变量，tv_sec表示Unix纪元到现在的秒数，tv_usec表示当前秒钟的微秒数。
//使用gettimeofday(timeval *tv, NULL)可以获取当前时间并且储存在tv中
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    //将时间转换为毫秒
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;;
}
void test_s1() {
    //测试s1
    double start1 = get_time();
    system("./s1 < input.in > 1.out");
    double t1 = get_time() - start1;
    printf("s1: %.3lf ms\n", t1);
}
void test_s2() {
    //测试s2
    double start2 = get_time();
    system("./s2 < input.in > 2.out");
    double t2 = get_time() - start2;
    printf("s2: %.3lf ms\n", t2);
}
void test_s3() {
    //测试s3
    double start3 = get_time();
    system("./s3 < input.in > 3.out");
    double t3 = get_time() - start3;
    printf("s3: %.3lf ms\n", t3);
}
int main() {
    system("g++ -std=c++11 solution_1.cpp -o s1");
    system("g++ -std=c++11 solution_2.cpp -o s2");
    system("g++ -std=c++11 solution_3.cpp -o s3");
    system("g++ -std=c++11 rand_input_2.cpp -o rand_input2");
    //将设定好的参数输入进rand_input2中，产生测试数据
    system("./rand_input2 < parameter_rand_input.in > input.in");
    test_s1();
    test_s2();
    test_s3();

}