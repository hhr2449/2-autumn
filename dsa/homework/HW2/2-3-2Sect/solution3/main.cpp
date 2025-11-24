#include <cstdlib>
#include <cstdio>
#define ll long long
const int MAX_N = 1e6 +10;


struct pair {
    int idx;
    int val;
};

//单调栈
//首先先模拟动态数组，然后加上一个top来作为栈顶，再修改插入逻辑成为栈
//单调栈中的元素应该同时储存值和索引
struct mono_stack {
    pair *s = new pair[10];
    //top指向的是当前栈顶
    int top = -1;
    int capacity = 10;
    int size = 0;
    void expand(int d) {
        if(size + d <= capacity) {
            return;
        }
        pair *tmp = s;
        s = new pair[2*(capacity + d)];
        for(int i = 0; i <= top; i++) {
            s[i].val = tmp[i].val;
            s[i].idx = tmp[i].idx;
        }
        capacity = 2*(capacity + d);
        delete [] tmp;
    }
    void push(int idx, int val) {
        expand(1);
        //添加元素的过程中要不断淘汰不如插入元素的
        int insert_pos = top + 1;
        while(insert_pos > 0 && s[insert_pos - 1].val < val) {
            insert_pos--;
            size--;
        }
        s[insert_pos].val = val;
        s[insert_pos].idx = idx;
        top = insert_pos;
        size++;
    }
    //析构函数
    ~mono_stack() {
        delete [] s;
    }
};
//孩子节点，传承路径全部使用编号来表示
struct Node {
    //编号
    int id;
    //自己的内力值
    int val;
    //以该节点为根数的内力值
    //求和，使用long long防止溢出
    ll sum = 0;
    // 子节点的编号
    int *child = nullptr;
    // 子节点的个数
    int child_num = 0;
    //传承路径(从前向后对应从上到下)
    //!有问题，如果储存完整的路径的话空间复杂度是o(n^2)会空间超限
    //!优化：储存传承路径的上家

    //!之前的思路可以正确执行，但是找引路人一步需要O(n^2)的复杂度，会时间超限
    //!优化思路：对于一个传承序列来说，又老又差的肯定不能成为引路人，这个特点符合单调栈
    //!考虑边找传承路径边找引路人，参数中添加一个单调栈
    //传承路径的长度
    int path_len = 0;
    Node() {}
    Node(int id, int val) : val(val) {}

};

//使用一个数组来存储所有的节点
//节点编号从1开始
Node *tree;
//节点总数n
int n;
//结果
int res[MAX_N];


//获取以id为根数的内力值之和
ll get_sum(int id) {
    //初始化为自身内力值
    tree[id].sum = (ll)tree[id].val;
    //若为叶子节点，则自身的内力值就是sum
    if(tree[id].child == nullptr) {
        return tree[id].sum;
    }
    //否则，让子节点递归调用求得sum，然后再加起来
    for(int i = 0; i < tree[id].child_num; i++) {
        tree[id].sum += get_sum(tree[id].child[i]);
    }
    return tree[id].sum;
}

//寻找引路人
//id表示当前正在寻找编号为id的引路人，栈stk是id节点的传承路径的单调栈
//找完id的引路人后，将id加入栈中，对于正统弟子，可以继承id的单调栈，非正统弟子则是空栈
void find_path(int id, mono_stack *stk) {

    res[id]= -1;
    //id是当前这一个路径的起点
    //没有引路人
    if(stk->size == 0) {
        stk->push(id, tree[id].val);
    }
    //id不是起点
    //从当前的栈顶开始遍历，引路人就是第一个大于他的节点
    else {
        for(int i = stk->top; i >= 0; i--) {
            if(stk->s[i].val > tree[id].val) {
                res[id] = stk->s[i].idx;
                break;
            }
        }
        stk->push(id, tree[id].val);

    }
    ///递归调用子节点
    //递归基：叶子节点
    if(tree[id].child_num == 0) {
        return;
    }

    // 找到正统弟子
    ll max_sum = -1, max_index = -1;
    for(int j = 0; j < tree[id].child_num; j++) {
        ll son_index = tree[id].child[j];
        //当内力值更大或是内力值相等但是编号更小时更新
        if(tree[son_index].sum > max_sum || 
           (tree[son_index].sum == max_sum && son_index < max_index)) {
            max_sum = tree[son_index].sum;
            max_index = son_index;
        }
    }
    //正统弟子可以继承父节点的传承路径
    find_path(max_index, stk);
    //其余的弟子新建栈
    for(int j = 0; j < tree[id].child_num; j++) {
        ll son_index = tree[id].child[j];
        if(son_index != max_index) {
            find_path(son_index, new mono_stack());
        }
    }
}
int main() {
    scanf("%d", &n);
    tree = new Node[n + 10];
    //填充child,child_num
    //i表示的就是节点i
    for(int i = 1; i <= n; i++) {
        //编号
        tree[i].id = i;
        int num;
        scanf("%d", &num);
        tree[i].child_num = num;
        //没有子节点，跳过,child为nullptr
        if(num == 0) {
            continue;
        }
        tree[i].child = new int[num];
        //填充child
        for(int j = 0; j < num; j++) {
            int id;
            scanf("%d", &id);
            tree[i].child[j] = id;
        }
    }
    //填充内力值
    for(int i = 1; i <= n; i++) {
        int a;
        scanf("%d", &a);
        tree[i].val = a;
    }
    //计算根数和
    get_sum(1);
    //计算传承路径
    find_path(1, new mono_stack());
    for(int i = 1; i <= n; i++) {
        printf("%d\n", res[i]);
    }

}