#include <iostream>
using namespace std;

int push_back(int x[]) {
    if(x == nullptr) {
        return 0;
    }
    int size_x = sizeof(x);
    return size_x;
}

int main() {
    int x[] = {1, 2, 3, 4, 5};
    cout << push_back(x);
}

