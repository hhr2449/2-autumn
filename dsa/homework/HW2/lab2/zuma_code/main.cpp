#include <iostream>
#include <cstring>

using namespace std;

const int MAX_LEN = 10000;  // 最大长度限制

// 模拟珠子序列的数组
char sequence[MAX_LEN];
int length = 0;  // 当前序列的长度

// 消除操作
void eliminate() {
    bool changed = true;  // 是否发生了消除
    while (changed) {
        changed = false;
        int i = 0;
        while (i < length) {
            int j = i;
            // 找到连续的相同珠子
            while (j < length && sequence[i] == sequence[j]) {
                j++;
            }

            // 如果找到3个或更多的相同珠子
            if (j - i >= 3) {
                changed = true;
                // 消除这些珠子，将剩下的部分前移
                for (int k = j; k < length; k++) {
                    sequence[k - (j - i)] = sequence[k];
                }
                length -= (j - i);  // 更新序列长度
                i = 0;  // 从头开始重新检查
            } else {
                i = j;  // 否则跳到下一个位置
            }
        }
    }
}

// 插入操作
void insert(int position, char ball) {
    // 将所有元素后移一位
    for (int i = length; i > position; i--) {
        sequence[i] = sequence[i - 1];
    }
    sequence[position] = ball;  // 插入新珠子
    length++;
}

int main() {
    // 输入初始珠子序列
    char initial[MAX_LEN];
    cin >> initial;
    length = strlen(initial);  // 获取初始长度
    strcpy(sequence, initial);  // 复制到珠子序列中

    // 输入操作次数
    int n;
    cin >> n;

    // 处理每次操作
    for (int i = 0; i < n; i++) {
        int k;
        char ball;
        cin >> k >> ball;

        // 执行插入操作
        insert(k, ball);

        // 执行消除操作
        eliminate();

        // 输出当前的珠子序列
        if (length == 0) {
            cout << "-" << endl;  // 如果序列为空，输出"-"
        } else {
            for (int j = 0; j < length; j++) {
                cout << sequence[j];
            }
            cout << endl;
        }
    }

    return 0;
}
