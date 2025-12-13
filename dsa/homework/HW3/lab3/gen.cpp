// 测试数据生成器
// 使用用户名为key，做题量为value
// 取500000条数据，读入到数组中，然后每次随机选择起点并且进行打乱
// 这样就实现了随机性：起点随机，并且进行打乱

// 接受命令行参数，前面一个是插入次数，后面一个是查询次数
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <string>
#include <cstring>
using namespace std;
const int N = 500000;

// 条目类
struct hash_entry {
    string my_string;
    int my_data;
} entry[N + 5];
int main(int argc, char* argv[]) {
    // 生成随机种子
    srand(time(NULL));
    // 获取命令行参数
    // 分别是插入次数，查询次数，文件编号
    int n, m, num;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    num = atoi(argv[3]);
    // 使用变量来表示文件名
    string filename = "data";
    filename += to_string(num);
    filename += ".in";
    // 重定位输入输出
    freopen("poj.txt", "r", stdin);
    freopen(filename.c_str(), "w", stdout);

    // _作为占位符，表示忽略
    int _;
    for(int i = 0; i < N; i++) {
        cin >> _ >> entry[i].my_string >> _ >> entry[i].my_data;
    }

    // 随机选择起点
    int start = rand() % N;
    if(start > N - n - 5) {
        start -= n;
    }
    // 进行打乱
    shuffle(entry + start, entry + start + n, default_random_engine(rand()));
    // 生成插入数据
    for(int i = 0; i < n; i++) {
        cout << 0 << ' ' << entry[start + i].my_string << ' ' << entry[start + i].my_data << endl;
    }
    // 再次进行打乱
    shuffle(entry + start, entry + start + n, default_random_engine(rand()));
    // 生成查询数据
    for(int i = 0; i < m; i++) {
        cout << 1 << ' ' << entry[start + i].my_string << endl;
    }
    cout << 2;
}