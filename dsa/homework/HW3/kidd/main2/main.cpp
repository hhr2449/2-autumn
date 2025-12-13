// 使用一个线段树来维护区间和
// 使用数组存树，节点中维护区间内的总翻转次数信息，左儿子下标和右儿子下标
// 实际上就是一个区间修改+区间查询的线段树
// 建树：区间二分，建左右儿子，递归进行
// 修改：翻转操作其实就是让区间内的每个叶子节点的翻转次数加1，对应到区间在翻转范围内的一个节点，假设区间为[s,t)，则该节点应该加(t-s)
// 使用懒标记，懒标记的含义是：该节点的区间内每个叶子节点的翻转次数都应该加上lazyTag
// 如果一个节点完全在修改区间内，直接修改sum并且打上懒标记（注意，这个节点必须直接修该，这样才能正确更新“部分在翻转区间内”的区间）
// 如果只是部分在区间内，则先进行下推，然后递归调用左右儿子，经历了下推和递归之后，可以确信他的左右儿子的sum都是正确的，这样就可以更新当前节点的sum了
// 所以v的懒标记是针对v的根树中除了v以外的节点的，v本身直接就进行修改了
// 查询：先进行标记下传，如果一个区间被完全包含，则直接加上返回该节点的翻转次数信息
//       如果完全不包含，返回
//       否则递归查询左右儿子


#include <stdio.h>
#include <cstdlib>
#define ll long long

struct Node{
    // 使用指针来指向左右节点
    Node* lc = nullptr;
    Node* rc = nullptr;
    // 该节点表示的区间范围
    // 范围是[l, r)
    int r, l;
    // 当前区间的总翻转次数
    ll sum = 0;
    // 懒标记，代表当前节点的sum应该加上lazyTag
    ll lazyTag = 0;

};

struct SegmentTree{
    // 只储存一个根节点
    Node* tree;
    int n;
    SegmentTree(int n) {
        this->n = n;
        // 只初始化根节点
        tree = new Node();
        tree->l = 1;
        tree->r = n + 1;
    }
    
    // 不主动建树，只有需要访问相关节点的时候才建立节点

    // 下传懒标记
    // 将下标为rank的懒标记进行处理，然后下推懒标记
    void push_down(Node* cur) {
        // 如果是叶子节点，可以退出了
        if(cur->l == cur->r - 1) {
            return;
        }
        // 不是叶子节点的话要将懒标记下传，并且更新左右儿子
        Node* rc = cur->rc;
        Node* lc = cur->lc;

        // 如果没有，则现场建立
        int mid = (cur->r + cur->l) / 2;
        if(rc == nullptr) {
            rc = new Node();
            cur->rc = rc;
            rc->l = mid;
            rc->r = cur->r;
        }
        if(lc == nullptr) {
            lc = new Node();
            cur->lc = lc;
            lc->l = cur->l;
            lc->r = mid;
        }
        lc->sum += cur->lazyTag*(lc->r - lc->l);
        lc->lazyTag += cur->lazyTag;
        rc->sum += cur->lazyTag*(rc->r - rc->l);
        rc->lazyTag += cur->lazyTag;
    
        // 清空标记
        cur->lazyTag = 0;
    }

    // 区间修改
    // 这里的修改其实就是翻转，也就是叶子节点的sum加1
    // 对应到区间节点上，就是让该节点的sum加上区间内的元素个数
    // 对区间[l, r)进行修改,当前处于rank节点
    void modify(Node* cur, int l, int r) {
        // 如果rank节点对应的区间完全和目标区间没有交点，直接退出
        if(cur->r <= l || cur->l >= r) {
            return;
        }
        // 如果完全包含在目标范围中，直接修改,并且打上标记
        if(cur->l >= l && cur->r <= r) {
            cur->sum += cur->r - cur->l;
            cur->lazyTag += 1;
            return;
        }
        // 否则要向孩子进行递归
        // 因为进行了下传，下传函数中已经验证并且确保了有孩子，所以可以放心地使用
        push_down(cur);
        Node* lc = cur->lc;
        Node* rc = cur->rc;
        modify(lc, l, r);
        modify(rc, l, r);
        
        // 最后要更新当前节点的sum
        cur->sum = lc->sum + rc->sum;
    }

    // 区间查询
    // 对区间[l, r)进行查询，当前处于rank节点
    // 返回总的sum值
    // 注意返回值要使用ll
    ll query(Node* cur, int l, int r) { 
        // 如果rank节点对应的区间完全和目标区间没有交点，直接退出，返回0
        if(cur->r <= l || cur->l >= r) {
            return 0;
        }
        // 如果完全包含在目标范围中，直接返回区间的sum
        // 一个节点，只要他的祖先节点没有懒标记，这个节点的sum就是准确的
        // 在我们的query模式中，一定是从整数的根节点开始query的，所以递归到当前节点的时候一定已经将
        // 所有祖先节点的懒标记下传完毕，所以当前节点的sum可以放心的使用    
        if(cur->l >= l && cur->r <= r) {
            return cur->sum;
        }
        // 否则要向孩子进行递归
        // 先进行下传
        // 因为进行了下传，下传函数中已经验证并且确保了有孩子，所以可以放心地使用
        push_down(cur);
        ll _sum = 0;
        Node* lc = cur->lc;
        Node* rc = cur->rc;
        _sum += query(lc, l, r);
        _sum += query(rc, l, r);
        return _sum;
    }

};

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    SegmentTree segTree(n);
    char op;
    int a, b;
    while(m--) {
        scanf(" %c", &op);
        if(op == 'H') {
            scanf("%d %d", &a, &b);
            segTree.modify(segTree.tree, a, b + 1);

        }
        else if(op == 'Q') {
            scanf("%d %d", &a, &b);
            ll ans = segTree.query(segTree.tree, a, b + 1);
            printf("%lld\n", ans);
        }
    }
}