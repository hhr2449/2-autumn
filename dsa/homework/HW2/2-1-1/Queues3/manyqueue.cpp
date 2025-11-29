#include "manyqueue.h"

// 构造函数，创建m个队列，编号为1~m
QueueManager::QueueManager(unsigned int m) : size(m) {
    queues = new Queue[m + 1];  // 索引从1开始，所以分配m+1个空间
}

// 在第k个队列中插入元素x
void QueueManager::push(unsigned int k, unsigned int x) {
    // 验证k的范围，k必须在1到size之间
    if (k >= 1 && k <= size) {
        queues[k].qpush(x);
    }
}

// 在第k个队列中弹出元素
void QueueManager::pop(unsigned int k) {
    // 验证k的范围，k必须在1到size之间
    if (k >= 1 && k <= size) {
        queues[k].qpop();
    }
}

// 返回第k个队列中的第i个元素
unsigned int QueueManager::query(unsigned int k, unsigned int i) {
    // 验证k的范围，k必须在1到size之间
    if (k >= 1 && k <= size) {
        return queues[k].query(i);
    }
    return 0;  // k超出范围时返回0
}

// 析构函数，释放所有队列的内存
QueueManager::~QueueManager() {
    delete[] queues;
}
