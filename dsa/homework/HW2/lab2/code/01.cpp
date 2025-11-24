#include <iostream>
#include <string>

using namespace std;
string a;

void play(int rank) {
    int left = rank;
    int right = rank;
    char color = a.at(rank);
    // 从插入位置开始向左向右延申
    // 如果颜色相同就延伸，直到遇到异色，最终就可以得到[left, right)区间，该区间内的颜色一样
    // 然后删除
    while (left >= 0 && a.at(left) == color) --left;
    left += 1;
    while (right < a.size() && a.at(right) == color) ++right;

    int size = right - left;
    if (size >= 3) {
        //如果同色的超过了3个，则消除
        a.erase(left, size);
        //递归调用，处理连续消除的情况
        play(left - 1);
    }
}

int main() {
    getline(cin, a);
    int m = 0;
    cin >> m;

    int rank; char color;
    for(int i = 0; i < m; ++i) {
        cin >> rank >> color;
        a.insert(a.cbegin() + rank, color);
        play(rank);
        if(a.size() == 0) {
            cout << '-' << endl;
        }
        else {
            cout << a << endl;
        }
    }
    
    //cout << a << endl;

    return 0;
}