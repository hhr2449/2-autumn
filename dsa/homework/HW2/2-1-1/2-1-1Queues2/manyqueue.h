#include <vector>
const int THRESHOLD = 4;
//实现一个队列类
class Queue {
    //使用向量来构建队列
    //维护一个头指针
    //添加元素时直接将元素插入向量的尾部
    //弹出元素时直接维护尾指针
    //当总的空间超过队列使用空间的4倍的时候推倒重建
    public:
        int head;
        std::vector<int> data;
        //capacity是总共占据了的空间，size是实际使用了的空间
        int capacity;
        int size = 0;
        //从1开始索引
        Queue() : head(1) {
            data.resize(1);
            capacity = 1;
        }
        //当冗余空间过多时，将数据迁移到一个新的vector，销毁原本的
        void migrate() {
            if(capacity < size * THRESHOLD) {
                return;
            }
            std::vector<int> newData(data.begin() + head - 1, data.end());
            data.swap(newData);
            head = 1;
        }
        void qpush(int val) {
            //添加元素时直接插入向量的尾部
            data.push_back(val);
            size++;
            capacity++;
            migrate();
        }
        void qpop() {
            //弹出时直接移动头指针
            if(size == 0) {
                return;
            }
            head++;
            size--;
        }
        int query(int r) {
            if(r < 1 || r > size) {
                if(size == 0) {
                    return 0;
                }
                else {
                    return data.back();
                }
            }
            return data[head + r - 1];
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
    //TODO
    //需要一个数组来保存队列
    //数组中每个元素是一个队列的指针
    std::vector<Queue> queues;
    int size = 0;
};