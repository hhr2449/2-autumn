// 使用一个线段树来维护区间和
// 使用数组存树，节点中维护区间内的总翻转次数信息，左儿子下标和右儿子下标
// 实际上就是一个区间修改+区间查询的线段树
// 建树：区间二分，建左右儿子，递归进行
// 修改：翻转操作其实就是让区间内的每个叶子节点的翻转次数加1，对应到区间在翻转范围内的一个节点，假设区间为[s,t)，则该节点应该加(t-s)
// 使用懒标记，懒标记的含义是：该节点的区间内每个叶子节点的翻转次数都应该加上lazyTag
// 如果一个节点完全在修改区间内，直接修改sum并且打上懒标记（注意，这个节点必须直接修该，这样才能正确更新“部分在翻转区间内”的区间）
// 如果只是部分在区间内，则先进行下推，然后递归调用左右儿子，经历了下推和递归之后，可以确信他的左右儿子的sum都是正确的，这样就可以更新当前节点的sum了
// 查询：先进行标记下传，如果一个区间被完全包含，则直接加上返回该节点的翻转次数信息
//       如果完全不包含，返回
//       否则递归查询左右儿子


#include <stdio.h>
#include <cstdlib>
#define ll long long
const int N = 2147483647;

struct Node{
    // 左右儿子下标
    // 左右儿子下标分别是2*i和2*i+1
    // 初始化为0，代表没有孩子
    int lc = 0, rc = 0;
    // 该节点表示的区间范围
    // 范围是[l, r)
    int r, l;
    // 当前区间的总翻转次数
    ll sum = 0;
    // 懒标记，代表当前节点的sum应该加上lazyTag
    ll lazyTag = 0;

};

struct SegmentTree{
    // 使用数组来存树
    // 0节点不存东西，当作空标记
    Node* tree;
    int n;
    SegmentTree(int n) {
        this->n = n;
        tree = new Node[4 * n + 5];
    }
    // 建树
    // 为树添加一个节点，节点下标rank，表示的区间是[l, r)
    void build(int rank, int l, int r) {
        // 表示的区间是[l, r)
        tree[rank].l = l;
        tree[rank].r = r;
        // 如果是叶子节点
        // 结束
        if(l == r - 1) {
            return;
        }
        // 否则要对区间进行二分，分别递归建树
        // 将左右儿子连接
        tree[rank].lc = 2 * rank;
        tree[rank].rc = 2 * rank + 1;
        // 递归建树
        int mid = (l + r) / 2;
        build(2 * rank, l, mid);
        build(2 * rank + 1, mid, r);
    }

    // 下传懒标记
    // 将下标为rank的懒标记进行处理，然后下推懒标记
    void push_down(int rank) {
        // 如果是叶子节点，可以退出了
        if(tree[rank].l == tree[rank].r - 1) {
            return;
        }
        // 不是叶子节点的话要将懒标记下传，并且更新左右儿子
        int rc = tree[rank].rc;
        int lc = tree[rank].lc;
        // 如果有左右儿子的话，则将其标记加上区间内的元素数
        if(lc != 0) {
            tree[lc].sum += tree[rank].lazyTag*(tree[lc].r - tree[lc].l);
            tree[lc].lazyTag += tree[rank].lazyTag;
        }
        if(rc != 0) {
            tree[rc].sum += tree[rank].lazyTag*(tree[rc].r - tree[rc].l);
            tree[rc].lazyTag += tree[rank].lazyTag;
        }
        // 清空标记
        tree[rank].lazyTag = 0;
    }

    // 区间修改
    // 这里的修改其实就是翻转，也就是叶子节点的sum加1
    // 对应到区间节点上，就是让该节点的sum加上区间内的元素个数
    // 对区间[l, r)进行修改,当前处于rank节点
    void modify(int rank, int l, int r) {
        // 如果rank节点对应的区间完全和目标区间没有交点，直接退出
        if(tree[rank].r <= l || tree[rank].l >= r) {
            return;
        }
        // 如果完全包含在目标范围中，直接修改,并且打上标记
        if(tree[rank].l >= l && tree[rank].r <= r) {
            tree[rank].sum += tree[rank].r - tree[rank].l;
            tree[rank].lazyTag += 1;
            return;
        }
        // 否则要向孩子进行递归
        push_down(rank);
        int lc = tree[rank].lc;
        int rc = tree[rank].rc;
        if(lc != 0) {
            modify(lc, l, r);
        }
        if(rc != 0) {
            modify(rc, l, r);
        }
        // 最后要更新当前节点的sum
        tree[rank].sum = tree[lc].sum + tree[rc].sum;
    }

    // 区间查询
    // 对区间[l, r)进行查询，当前处于rank节点
    // 返回总的sum值
    // 注意返回值要使用ll
    ll query(int rank, int l, int r) { 
        // 先进行下传
        push_down(rank);
        // 如果rank节点对应的区间完全和目标区间没有交点，直接退出，返回0
        if(tree[rank].r <= l || tree[rank].l >= r) {
            return 0;
        }
        // 如果完全包含在目标范围中，直接返回区间的sum
        if(tree[rank].l >= l && tree[rank].r <= r) {
            return tree[rank].sum;
        }
        // 否则要向孩子进行递归
        ll _sum = 0;
        int lc = tree[rank].lc;
        int rc = tree[rank].rc;
        if(lc != 0) {
            _sum += query(lc, l, r);
        }
        if(rc != 0) {
            _sum += query(rc, l, r);
        }
        return _sum;
    }

};

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    SegmentTree segTree(n);
    segTree.build(1, 1, n + 1);
    char op;
    int a, b;
    while(m--) {
        scanf(" %c", &op);
        if(op == 'H') {
            scanf("%d %d", &a, &b);
            segTree.modify(1, a, b + 1);

        }
        else if(op == 'Q') {
            scanf("%d %d", &a, &b);
            ll ans = segTree.query(1, a, b + 1);
            printf("%lld\n", ans);
        }
    }
}