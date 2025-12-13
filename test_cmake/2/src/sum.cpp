#include "sum.h"
Sum::Sum(int x, int y ) : a(x), b(y) {
    cout << "Parameterized Constructor called" << endl;
}
Sum::Sum() {
    cout << "Constructor called" << endl;
}
int Sum::sum(int a, int b) {
    cout << "Sum of " << a << " and " << b << " is " << a + b << endl;
    return a + b;
}
int Sum::sum() {
    cout << "Sum of member variables " << a << " and " << b << "is " << a + b << endl;
    return a + b;
}