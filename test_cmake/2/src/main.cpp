#include <iostream>
#include "sum.h"
int main(int, char**){
    std::cout << "Hello, from test2!\n";
    Sum s;
    Sum s2(3, 4);
    std::cout << "Sum: " << s.sum(1, 2) << "\n";
    std::cout << "Sum: " << s2.sum() << "\n";
    return 0;
}
