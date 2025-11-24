#include <cstdlib>
#include <cstdio>
#define ll long long
const int MAX_N = 1e6 +10;
//孩子节点，传承路径全部使用编号来表示
struct Node {
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
    int pred = 0;
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


//求传承路径// 使用DFS计算传承路径
void get_path(int id) {
    if(tree[id].child == nullptr) {
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
    
    // 为所有弟子设置路径
    for(int j = 0; j < tree[id].child_num; j++) {
        int son_index = tree[id].child[j];
        
        if(son_index == max_index) {
            // 正统弟子：上家是父亲，路径长加1
            tree[son_index].pred = id;
            tree[son_index].path_len = tree[id].path_len + 1;
        } else {
            // 非正统弟子：路径为空,上家设为0
            tree[son_index].pred = 0;
            tree[son_index].path_len = 0;
        }
        
        // 递归处理子节点
        get_path(son_index);
    }
}

int main() {
    scanf("%d", &n);
    tree = new Node[n + 10];
    //填充child,child_num
    //i表示的就是节点i
    for(int i = 1; i <= n; i++) {
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
    get_path(1);
    //计算节点的引路人
    int *res = new int[n + 1];
    //节点i的引路人
    for(int i = 1; i <= n; i++) {
        res[i] = -1;
        //记录节点i的值
        int val = tree[i].val;
        //沿着路径向上走
        int cur = tree[i].pred;
        while(cur != 0) { 
            if(tree[cur].val > val) {
                res[i] = cur;
                break;
            }
            cur = tree[cur].pred;
        }
    }
    for(int i = 1; i <= n; i++) {
        printf("%d\n", res[i]);
    }

}