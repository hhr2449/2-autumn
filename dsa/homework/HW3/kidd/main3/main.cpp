// 使用一个线段树来维护区间和
// 节点中维护区间内的总翻转次数信息
// 实际上就是一个区间修改+区间查询的线段树
// 修改：翻转操作其实就是让区间内的每个叶子节点的翻转次数加1，对应到区间在翻转范围内的一个节点，假设区间为[s,t)，则该节点应该加(t-s)
// 使用懒标记，懒标记的含义是：该节点的区间内每个叶子节点的翻转次数都应该加上lazyTag
// 如果一个节点完全在修改区间内，直接修改sum并且打上懒标记（注意，这个节点必须直接修该，这样才能正确更新“部分在翻转区间内”的区间）
// 如果只是部分在区间内，则先进行下推，然后递归调用左右儿子，经历了下推和递归之后，可以确信他的左右儿子的sum都是正确的，这样就可以更新当前节点的sum了
// 所以v的懒标记是针对v的根树中除了v以外的节点的，v本身直接就进行修改了
// 查询：先进行标记下传，如果一个区间被完全包含，则直接加上返回该节点的翻转次数信息
//       如果完全不包含，返回
//       否则递归查询左右儿子

// 使用动态开点，只有在需要访问到某个节点的时候才实际创建这个节点，从而节约空间

// 使用指针来连接节点，每个节点里面需要额外的储存两个指针，而且容易产生内存碎片
// 所以我们可以使用数组来储存，可以开六个数组，分别存储r、l、sum、lazyTag、lc、rc
// cnt代表当前的节点数，当创建一个新的节点的时候，cnt++，然后r[cnt],l[cnt],……，rc[cnt]就是这个新建节点的属性
// 注意还要去找他的父亲节点，然后将父亲节点的lc\lc给进行赋值
// 如何估算数组的大小：每一次操作都有可能创建新的节点，考虑最坏的情况，访问了一条从来没有走过的路径
// 此时会一路新建节点，新建节点数就是层数乘2
// 这一颗树是一个完全二叉树，最大层数是logn，所以只需要2*m*logn个节点即可
// 带入数据计算，理论上最大只需要12400000个
// 空间限制256,一个节点最多只可以使用21字节


#include <stdio.h>
#include <cstdlib>
#define ll long long
#define ui unsigned int
const ll N = 13150000;

// 六个数组储存节点属性
// 节点cnt的区间范围是[l[cnt], r[cnt])
ui lc[N], rc[N];
ll sum[N];
// 将懒标记使用unsigned储存，节约空间
ui lazyTag[N];
// 完全可以不用存储节点的区间范围，可以使用参数来进行传递，在递归的时候就可以知晓区间范围了

// 使用数组进行储存
struct SegmentTree{
    // cnt是实际存储的节点数
    ui cnt;
    // 从1开始储存节点，0留空，作为空节点标识
    SegmentTree(int n) {
        // 先初始化根节点
        cnt = 1;
    }
    
    // 不主动建树，只有需要访问相关节点的时候才建立节点

    // 下传懒标记
    // 将下标为cur的懒标记进行处理，然后下推懒标记
    void push_down(ui cur, ui l, ui r) {
        // 如果是叶子节点，可以退出了
        if(l == r - 1) {
            return;
        }
        if(lazyTag[cur] == 0) {
            return;
        }
        // 不是叶子节点的话要将懒标记下传，并且更新左右儿子
        ui rc_idx = rc[cur];
        ui lc_idx = lc[cur];

        // 如果没有，则现场建立
        ui mid = (r + l) / 2;
        if(lc_idx == 0) {
            cnt++;
            lc_idx = cnt;
            lc[cur] = lc_idx;
        }
        if(rc_idx == 0) {
            // 新增一个节点,cnt后移一位
            // 先进行后移
            cnt++;
            rc_idx = cnt;
            // 父节点连接
            rc[cur] = rc_idx;
        }

        sum[lc_idx] += (ll)lazyTag[cur] * (ll)(mid - l);
        lazyTag[lc_idx] += lazyTag[cur];
        sum[rc_idx] += (ll)lazyTag[cur] * (ll)(r - mid);
        lazyTag[rc_idx] += lazyTag[cur];
    
        // 清空标记
        lazyTag[cur] = 0;
    }

    // 区间修改
    // 这里的修改其实就是翻转，也就是叶子节点的sum加1
    // 对应到区间节点上，就是让该节点的sum加上区间内的元素个数
    // 对区间[_l, _r)进行修改,当前处于rank节点,节点区间为[l, r)
    void modify(ui cur, ui l, ui r, ui _l, ui _r) {
        // 如果完全包含在目标范围中，直接修改,并且打上标记
        if(l >= _l && r <= _r) {
            sum[cur] += r - l;
            lazyTag[cur] += 1;
            return;
        }
        // 否则要向孩子进行递归
        push_down(cur, l, r);
        ui mid = (l + r) / 2;
        // 于左区间有交集，则向左递归
         if (_l < mid) {
            if (lc[cur] == 0) {
                lc[cur] = ++cnt;
            }
            modify(lc[cur], l, mid, _l, _r);
        }
        if (_r > mid) {
            if (rc[cur] == 0) {
                rc[cur] = ++cnt;
            }
            modify(rc[cur], mid, r, _l, _r);
        }
        // 最后要更新当前节点的sum
        sum[cur] = sum[lc[cur]] + sum[rc[cur]];
    }

    // 区间查询
    // 对区间[l, r)进行查询，当前处于rank节点
    // 返回总的sum值
    // 注意返回值要使用ll
    ll query(ui cur, ui l, ui r, ui _l, ui _r) { 
        // 如果完全包含在目标范围中，直接返回区间的sum
        // 一个节点，只要他的祖先节点没有懒标记，这个节点的sum就是准确的
        // 在我们的query模式中，一定是从整数的根节点开始query的，所以递归到当前节点的时候一定已经将
        // 所有祖先节点的懒标记下传完毕，所以当前节点的sum可以放心的使用    
        if(l >= _l && r <= _r) {
            return sum[cur];
        }
        // 否则要向孩子进行递归
        // 先进行下传
        // 因为进行了下传，下传函数中已经验证并且确保了有孩子，所以可以放心地使用
        push_down(cur, l, r);
        ll _sum = 0;
        ui lc_idx = lc[cur];
        ui rc_idx = rc[cur];
        ui mid = (l + r) / 2;
        // 与左区间有交集，并且存在左孩子，则向左递归
        // 注意这里只有存在孩子才递归，如果不存在，说明该节点没有承载任何sum
        if(_l < mid && lc_idx != 0) {
            _sum += query(lc_idx, l, mid, _l, _r);
        }
        // 右区间同理
        if(_r > mid && rc_idx != 0) {
            _sum += query(rc_idx, mid, r, _l, _r);
        } 
        
        return _sum;
    }

};

int main() {
    ui n, m;
    scanf("%u %u", &n, &m);
    SegmentTree segTree(n);
    char op;
    ui a, b;
    while(m--) {
        scanf(" %c", &op);
        if(op == 'H') {
            scanf("%u %u", &a, &b);
            segTree.modify(1, 0, n, a - 1, b);

        }
        else if(op == 'Q') {
            scanf("%u %u", &a, &b);
            ll ans = segTree.query(1, 0, n, a - 1, b);
            printf("%lld\n", ans);
        }
    }
}