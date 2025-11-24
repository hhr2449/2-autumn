#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
typedef unsigned char* byte_p;

void show_byte(byte_p p, int len) {
    for(int i = 0; i < len; i++) {
        printf("%02x ", p[i]);
    }
}
int main() {
    int *a;
    a = new int[10];
    //menset的作用是按照字节来设置值，比如设置1，实际上是将每个字节都设置为00000001
    memset(a, 0, 10 * sizeof(int));
    for(int i = 0; i < 10; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
    memset(a, 1, 10 * sizeof(int));
    for(int i = 0; i < 10; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
    show_byte((byte_p)a, 4);
    cout << endl;
    memset(a, -1, 10 * sizeof(int));
    //每个字节都是11111111，实际上就是ff
    show_byte((byte_p)a, 4);
    cout << endl;

    //malloc则是分配堆区上指定字节数的连续内存，并且返回指针
    int *b = (int *)malloc (10 * sizeof(int));
    show_byte((byte_p)b, 4);
    cout << endl;
    memset(b, 10, 10 * sizeof(int));
    show_byte((byte_p)b, 4);
    cout << endl;
}