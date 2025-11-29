const int THRESHOLD = 4;

//实现一个队列类
class Queue {
public:
    unsigned int head;  // 头指针，从0开始
    unsigned int tail;  // 尾指针，下一个插入位置
    unsigned int size;  // 队列中元素数量
    unsigned int capacity;  // 数组容量
    unsigned int* data;  // 手动管理的数组

    Queue() : head(0), tail(0), size(0), capacity(1) {
        data = new unsigned int[capacity];
    }

    // 析构函数，释放内存
    ~Queue() {
        delete[] data;
    }

    // 扩容函数
    void resize(unsigned int newCapacity) {
        unsigned int* newData = new unsigned int[newCapacity];
        // 复制现有元素
        for (unsigned int i = 0; i < size; i++) {
            newData[i] = data[(head + i) % capacity];
        }
        delete[] data;
        data = newData;
        head = 0;
        tail = size;
        capacity = newCapacity;
    }

    // 当冗余空间过多时，将数据迁移并收缩内存
    void migrate() {
        if (capacity > size * THRESHOLD && size > 0) {
            unsigned int newCapacity = (size > 1) ? size : 1;
            resize(newCapacity);
        }
    }

    void qpush(unsigned int val) {
        // 检查是否需要扩容
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[tail] = val;
        tail = (tail + 1) % capacity;
        size++;
    }

    void qpop() {
        if (size == 0) {
            return;
        }
        head = (head + 1) % capacity;
        size--;
        migrate();  // 检查是否需要缩容
    }

    unsigned int query(unsigned int i) {
        if (size == 0) {
            return 0;
        }
        if (i > size) {
            // 返回队尾元素
            return data[(head + size - 1) % capacity];
        }
        // 确保i≥1（根据题目要求）
        return data[(head + i - 1) % capacity];
    }
};

//用于管理队列
class QueueManager{
public:
    //构造函数，创建m个队列，编号为1~m
    QueueManager(unsigned int m);
    //在第k个队列中插入元素x
    void push(unsigned int k, unsigned int x);
    //在第k个队列中弹出元素
    void pop(unsigned int k);
    //返回第k个队列中的第i个元素
    unsigned int query(unsigned int k, unsigned int i);
    ~QueueManager();
private:
    Queue* queues;  // 使用数组而不是vector
    unsigned int size;
};
