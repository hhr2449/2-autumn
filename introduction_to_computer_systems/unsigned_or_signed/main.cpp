#include <iostream>
int main() {
    int a;
    unsigned int b;
    unsigned char* p = (unsigned char*)&a;
    for(int i = 0; i < 4; i++) {
        p[i] = 0xAA;
    }
    p = (unsigned char*)&b;
    for(int i = 0; i < 4; i++) {
        p[i] = 0xAA;
    }
    char c = 0xAA;
    unsigned char d = 0xAA;
    //! 同样的字节编码，a是使用有符号数的方式来解释的，输出的是-1431655766
    //! 而b是使用无符号数来解释的，输出的是2863311530
    //! 注意，char和unsigned char也受解释方式的影响
    //! 在强制转换为int时，char转成的是有符号数，而unsigned char转成无符号数
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "(int)c = " << (int)c << std::endl;
    std::cout << "(int)d = " << (int)d << std::endl;
}