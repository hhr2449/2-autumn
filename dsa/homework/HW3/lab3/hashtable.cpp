#include "hashtable.h"

// 基础哈希策略的实现
// 使用key的第一个字符的ASCII码对表长N取模作为哈希值，极易发生冲突
int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}

// 坏的哈希函数
int bad_hashing::operator()(char* str, int N) {
    if(str == NULL) return 0;
    else {
        int sum = 0;
        for(int i = 0; str[i] != '\0'; i++) {
            sum += str[i] * (1 + i);
        }
        return sum % N;
    }
}

// 好的哈希函数
int good_hashing::operator()(char* str, int N) {
    if(str == NULL) return 0;
    unsigned long long sum = 0;
    unsigned long long b = 233;
    // 递推计算幂次
    for(int i = 0; str[i] != '\0'; i++) {
        sum = b * sum + str[i];
    }
    return (int)(sum % N);

}

// 线性探测的实现
// 其实就是移动到下一个位置
int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}

void linear_probe::init(){
    return;// do nothing
}

// step初始化为1，每次走step^2
void Bid_square_probe::init(){
    len = 1;
    dir = 1;
    first = true;
    origin = 0;
}

int Bid_square_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    // 获取初始位置，依次为中心进行平方试探
    if(first) {
        first = false;
        origin = last_choice;
    }
    // 在原始位置的基础上进行平方试探
    int next = origin + dir * len * len;
    // 增加长度
    if(dir == -1) {
        len++;
    }
    // 翻转方向
    dir *= -1;
    return (next + table_size) % table_size;
}

// 初始化，cur指向溢出区的起始下标
void Overflow_probe::init() {
    cur = real_size;
}

int Overflow_probe::operator()(hash_entry* Table, int table_size, int last_choice) {
    // 只需要返回溢出区的下一个空的位置就可以了
    return cur++;
}