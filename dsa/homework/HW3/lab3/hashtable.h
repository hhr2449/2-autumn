#include <cstring>
#include <cstdio>
// 表长
// 表长取做素数M = 4n + 3的形式，可以保证试探链的前面M项互异
const int TABLE_SIZE = 499883; // = 124970 * 4 + 3


// 哈希表项
// 注意要储存键值对，key使用字符串进行储存
// 如果使用像是多槽位，开放散列，试探链之类的方式，则需要通过key来进行具体的判断
struct hash_entry{
    char* my_string;
    int my_data; 
    // 初始化函数
    // 默认初始化为一个空条目
    hash_entry(){
        my_string = NULL;my_data = 0;
    }
    // 传入一个字符串和一个数据
    // 注意这里使用深拷贝，为my_string分配一个新的内存，再将传入的字符串复制过去
    hash_entry(char* str, int data):my_data(data){
        my_string = new char[strlen(str)+1];
        strcpy(my_string,str);
    }
};

// 哈希策略接口，里面包含一个纯虚函数，用于计算哈希值
// 重载了()，传入一个key和表长N，返回一个哈希值
// 可以实现不同的哈希策略类，然后继承这个接口，这样通过统一的接口hashing_stategy* my_hashing, 可以传入不同的哈希策略类
struct hashing_strategy{
    virtual int operator()(char* str, int N)=0;
};

// 最基础的哈希策略，实现再hashtable.cpp中
struct naive_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};

// 坏的哈希函数
struct bad_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};

// 好的哈希函数
struct good_hashing:public hashing_strategy{
    int operator()(char* str, int N) override;
};

// 冲突处理接口
// 重载运算符()，传入一个哈希表，表长N，上一次选择的位置last_choice，按照具体的冲突处理策略来选择一个位置用于安置新的条目
// 包含一个纯虚函数init()，用于初始化冲突处理策略
struct collision_strategy{
    // 策略初始化
    // 冲突处理策略可能需要维护一些状态
    // 比如使用平方试探，需要维护一个试探的步数step，然后每次step加1
    // 这个函数就是要在第一次初始化冲突处理策略的时候调用
    virtual void init()=0;// pure virtual function
    virtual int operator()(hash_entry* Table, int table_size, int last_choice)=0;
};


// 线性探测解决冲突
struct linear_probe: public collision_strategy{
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};

// 双向平方
struct Bid_square_probe: public collision_strategy{
    // 试探长度
    int len;
    // 试探的方向(1向右，-1向左)
    int dir;
    // 是否首次
    bool first;
    // 原始位置
    int origin;
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};

// 溢出区
// 将哈希表划分为两个部分，前面350771个元素用于储存数据，后面的元素用于溢出区
struct Overflow_probe: public collision_strategy{
    int real_size = 350771;
    int cur;
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
// 哈希表结构体
struct hashtable{
    // 其中维护了一个哈希表项数组Table
    hash_entry* Table;
    int table_size;
    // 哈希策略和冲突解决策略
    // 相当于一个接口，可以传入不同的策略实现
    // 这里使用指针，从而实现多态调用，如果使用对象的话会发生切片，无法实现多态
    hashing_strategy* my_hashing; // 如果改为hashing_strategy my_hashing, 即不是用指针作为hashtable的成员, 而是让hashing_strategy结构体直接作为hashtable的成员. 会发生什么bug?
    collision_strategy* my_collision;

    // 传入表长，策略，并且根据表长分配哈希表数组
    hashtable(int size, hashing_strategy* hashing, collision_strategy* collision)
        :table_size(size),my_hashing(hashing),my_collision(collision)
    {
        Table = new hash_entry[table_size];
    }

    // 插入一个元素
    bool insert(hash_entry entry){ 
        // 使用dynamic_cast，如果转换成功（父类指针指向的对象确是指定的类型），则返回转换后的指针，否则返回nullptr
        // 判断是否是溢出区策略，如果是的话则要更改table_size为实际的大小
        int size = table_size;
        if(dynamic_cast<Overflow_probe*>(my_collision)) {
            size = ((Overflow_probe*)my_collision)->real_size;
        }
        
        // 计算哈希值
       int last_choice = (*my_hashing)(entry.my_string,size);
       my_collision->init();
       // 如果计算出来的位置已经有了
       // 则反复使用冲突处理来计算下一个位置，直到不冲突为止
       while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
           last_choice = (*my_collision)(Table, size, last_choice);
       }
       // 存入
       Table[last_choice] = entry;
       return true;
    }

    // 根据key进行查询，返回数据
    int query(char* query_string){
        // 无需特别处理溢出区的情况，因为溢出区策略只会影响实际的存储区长度从而影响哈希值的计算
        // table_size不会影响冲突处理函数
        
        // 先使用哈希函数计算出位置
        int last_choice = (*my_hashing)(query_string,table_size);
        my_collision->init();
        // 如果算出的位置不是要找的
        // 沿着试探链进行查找，直到找到对应的位置或是找到空条目结束
        // 使用插入时的冲突处理函数就可以复原出插入时的路径
        while(Table[last_choice].my_string!=NULL && 
            strcmp(Table[last_choice].my_string, query_string)!=0){ // 未处理的情况: 哈希表已满?
            last_choice = (*my_collision)(Table, table_size, last_choice);
        }
        if(Table[last_choice].my_string == NULL){
            return -1;
        }
        else {
            return Table[last_choice].my_data;
        }
    }
};
