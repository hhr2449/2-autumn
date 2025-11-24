#include <iostream>
#include <string>
#include <chrono>

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


        if (left < a.size()) {
            play(left);
        }
    }
}

int main() {
    getline(cin, a);
    int m = 0;
    cin >> m;
    auto start = chrono::high_resolution_clock::now();

    int rank; char color;
    for(int i = 0; i < m; ++i) {
        cin >> rank >> color;
        a.insert(a.cbegin() + rank, color);
        play(rank);
    }

    cout << a << endl;
    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

    return 0;
}