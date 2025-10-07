#include <cstdio>
#include <cstdlib>
//定义一个指向无符号字符的指针为byte_p，之后要使用这个指针来访问内存中的字节
typedef unsigned char* byte_p;
void show_Bytes(byte_p start, size_t len) {
    //输出指向的字节的十六进制表示
    for(size_t i = 0; i < len; i++) {
        printf("%.2x ", start[i]);
    }
}
void show_int(int x) {
    show_Bytes((byte_p) &x, sizeof(x));
}
//为什么这样可以展示字节
//各种数据是如何展示的：对于一个数据类型，其实规定了数据的存储结构，比如int的数据占据4个字节
//那么访问int类型的数据的时候就会根据指向的地址来访问后面四个字节的内容，并且按照int的方式来进行解释
//而unsigned char的数据类型占据一个字节，那么当访问unsigned char类型的数据的时候，就会直接输出该字节对应的内容
//这里要求其通过二位的十六进制进行输出，其实输出的就是该字节的十六进行编码
int main() {
    show_int(12345678);
}