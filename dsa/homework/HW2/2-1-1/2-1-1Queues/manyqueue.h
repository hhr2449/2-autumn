//实现列表类
class ListNode {

    public:
        int data;
        ListNode *succ;
        ListNode *pred;
        ListNode(int data, ListNode *succ = nullptr, ListNode *pred = nullptr) {
            this->data = data;
            this->succ = succ;
            this->pred = pred;
        }
        ListNode() : data(0), succ(nullptr), pred(nullptr) {}
        ~ListNode() {
        }
};
class List {        
    public:
    //头尾指针，不存放数据，方便操作
        ListNode *head;
        ListNode *tail;
        int size;
        List() {
            head = new ListNode();
            tail = new ListNode();
            head->succ = tail;
            tail->pred = head;
            size = 0;
        }
        //前插
        void insertPred(int data, ListNode *target) {
            ListNode *node = new ListNode(data);
            node->pred = target->pred;
            target->pred->succ = node;
            node->succ = target;
            target->pred = node;
            size++;
        }
        //后插
        void insertSucc(int data, ListNode *target) {
            ListNode *node = new ListNode(data);
            node->succ = target->succ;
            target->succ->pred = node;
            node->pred = target;
            target->succ = node;
            size++;
        }
        ListNode* operator[](int r) {
            //这里从1开始排序
            if(r < 1 || r > size) {
                return nullptr;
            }
            ListNode *p = head;
            while(r--) {
                p = p->succ;
            }
            return p;
        }
        bool empty() {
            return size == 0;
        }
        void insertHead(int data) {
            insertSucc(data, head);
        }
        void insertTail(int data) {
            insertPred(data, tail);
        }
        int getSize() {
            return size;
        }
        void deleteNode(ListNode *node) {
            node->pred->succ = node->succ;
            node->succ->pred = node->pred;
            delete node;
            size--;
        }
        ~List() {
            ListNode *p = head;
            
            while(p != nullptr) {
                ListNode *tmp = p;
                p = p->succ;
                delete tmp;
            }
        }
};


//实现一个队列类
class Queue {
    private:
        //使用列表作为底层数据结构
        List* list;
    public:
        Queue() {
            list = new List();
        }
        //插入列表的尾部
        void qpush(int data) {
            list->insertTail(data);
        }
        //删除列表的头部
        void qpop() {
            //如果列表为空，怎么都不做
            if(size() == 0) {
                return;
            }
            list->deleteNode(list->head->succ);
        }
        bool empty() {
            return list->empty();
        }
        int size() {
            return list->getSize();
        }
        //获取第r个节点中的数据
        int query(int r) {
            //如果超过范围
            if(r < 1 || r > size()) {
                //返回末尾元素
                if(size() != 0) {
                    return (*list)[size()]->data;
                }
                else {
                    return 0;
                }
            }
            return (*list)[r]->data;
        }
        ~Queue() {
            delete list;
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
    Queue** queues;
    int size = 0;
};