#include <iostream>
#include <string>

using namespace std;
string a;

void play(int rank) {
    int left = rank;
    int right = rank;
    char color = a.at(rank);

    while (left >= 0 && a.at(left) == color) --left;
    left += 1;
    while (right < a.size() && a.at(right) == color) ++right;

    int size = right - left;
    if (size >= 3) {
        a.erase(left, size);

        // 避免了调用play(-1)的情况
        // 但是，当消除完成，a变成空字符串时，仍然会发生越界访问
        int next = left;
        if (left - 1 >= 0) next = left - 1;
        play(next);
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