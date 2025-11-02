#include <iostream>
#include <functional>
#include <climits>
using namespace std;

class Vector {
    public:
        int *T;
        int size, capacity;
        Vector(int n) {
            T = new int[n];
            size = 0;
            capacity = n;
        }
        void expand(int d) {
            if(d + size < capacity) {
                return;
            }
            int *newT = T;
            T = new int[2 * (size + d)];
            capacity = 2 * (size + d);
            for (int i = 0; i < size; i++) {
                T[i] = newT[i];
            }
            delete[] newT;
        }
        void push_back(int x[], int size_x) {
            if(x == nullptr) {
                return;
            }
            expand(size_x);
            for (int i = 0; i < size_x; i++) {
                T[size + i] = x[i];
            }
            size += size_x;
        }
        //传入一个函数对象
        void traver(function<void(int&)> f) {
            //遍历数组，对于每个元素都调用这个函数对象
            for(int i = 0; i < size; i++) {
                f(T[i]);
            }
        }

};

void inc(int& x) {
    x++;
}

class compare {
    public:
        int pred = INT_MIN;
        int& i;
        compare(int& sum) : i(sum) {}
        void operator() (int& x) {
            if(pred > x) {
                i++;
            }
            pred = x;
        }
};

int main() {
    Vector v(10);
    int a[] = {4, 2, 1, 4, 2, -1};
    v.push_back(a, 6);
    v.traver(inc);
    for(int i = 0; i < v.size; i++) {
        cout << v.T[i] << " ";
    }
    int sum = 0;
    compare cmp(sum);
    v.traver(cmp);
    cout << endl << sum;
}