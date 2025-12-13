#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
int main() {
    freopen("result.txt", "w", stdout);
    int cnt = 1;
    for(int k = 1; k <= 3; k++) {
        cout << "\n===== 测试 data" << k << ".in =====\n";
        for(int i = 1; i <= 2; i++) {
            for(int j = 1; j <= 3; j++) {     
                cout << "第" << cnt << "次测试" << endl;
                if(i == 1) {
                    cout << "使用bad_hashing" << endl;
                }
                else {
                    cout << "使用good_hashing" << endl;
                }

                if(j == 1) {
                    cout << "使用linear_probe" << endl;
                }
                else if(j == 2) {
                    cout << "使用Bid_square_probe" << endl;
                }
                else {
                    cout << "使用Overflow_probe" << endl;
                }

                cout << "使用data" << k << ".in" << endl;

                string s = "./main ";
                s += to_string(i);
                s += " ";
                s += to_string(j);
                s = s + " < " + "data" + to_string(k) + ".in";
                auto start = chrono::steady_clock::now();
                system(s.c_str());
                auto end = chrono::steady_clock::now();
                double t = chrono::duration<double>(end - start).count();
                cout << "运行时间：" << t << "s" << endl << endl;
                cnt++;
            }
        }
    }

}
