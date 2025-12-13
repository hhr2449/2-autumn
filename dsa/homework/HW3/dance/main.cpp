#include <stdio.h>
#include <cstdlib>
#include <ctime>
using namespace std;
const int N = 5e5 + 5;

// 分析：
// 如果使用普通的BST或是相关的结构的话，都要考虑一个问题：以那个属性作为关键码？如果以节点的序号作为关键码，则很多
// 操作都会使序号改变，这是不行的
// 于是想到了使用Treap:
// 1.当rnd取随机数的时候，可以实现监禁平衡，从而防止退化的情况，而且实现远比AVL等简单
// 2.treap很好的支持了按排名分裂操作和合并操作，适合于这种讲求相对次序的情况

// 1.插入：进行两次按排名分裂，然后将节点于其中一个分裂树合并，再将两棵树进行合并
// 2.交换：只需要进行按排名查找，找到两个节点将节点的id进行交换即可
// 3.旋转：注意里面的序是“相对的”，所以其实可以建立两套序：树中的序和实际的序，只需要存一个offset就可以建立起这两套序之间的映射关系
// 4.区间翻转：实际上就是让这个区间对应的树进行整体的反转
// 如何对一棵树进行反转：只需要对里面的所有节点进行左右孩子交换即可；
// 这里可以使用懒标记：不立刻进行交换，而是给根节点打上一个交换标记（如果原本已经有标记了就去除），
// 等到最后中序遍历输出的时候再进行标记的下传，即交换左右孩子，并且为左右孩子打上交换标记

// 节点结构体
struct Node {
    // lc, rc分别是左右儿子在节点数组中的下标
    // size是节点的根树的大小
    // id是代号，实际上代号在操作过程中没有作用
    // rnd是随机的一个数，用于维持树的平衡
    int lc, rc, size, id, rnd;
    bool isRev;
};


// treap类
struct Treap{
    // 使用数组来储存节点
    // 0节点代表空节点，从1开始存
    Node tree[N];
    // 根节点的下标
    int root;
    // 临时变量（用于分裂等）
    int x, y, z;
    // 节点总数，用于最后的遍历输出
    int cnt = 0;

    // 创建节点（rnd要随机化）
    int new_Node(int _id) {
        tree[++cnt].id = _id;
        // 随机生成rnd
        tree[cnt].rnd = rand();
        tree[cnt].isRev = false;
        tree[cnt].size = 1;
        return cnt;
    }

    // 更新节点的size
    void update_height(int rank) {
        // 节点的size就是左右儿子的size加1
        int lc = tree[rank].lc;
        int rc = tree[rank].rc;
        tree[rank].size = tree[lc].size + tree[rc].size + 1;
    }

    // 交换标记的下传
    // 遍历的时候，如果节点需要交换，对其调用该函数即可
    void push_down(int rank) {
        int lc = tree[rank].lc;
        int rc = tree[rank].rc;
        // 对本身进行交换
        tree[rank].lc = rc;
        tree[rank].rc = lc;
        tree[rank].isRev = false;
        // 下传交换标记
        // 如果真的存在，则将其标记反转
        if(lc != 0) {
            tree[lc].isRev ^= 1;
        }
        if(rc != 0) {
            tree[rc].isRev ^= 1;
        }
        
    }


//-------------------------这部分参考了一些资料，由本人重新实现---------------------------
//https://www.luogu.com.cn/article/ifj4ute5
//https://oi-wiki.org/ds/treap/#%E6%8C%89%E6%8E%92%E5%90%8D%E5%88%86%E8%A3%82

    // 在以_root为根的子树中寻找树中排位为k的节点
    // 返回下标
    int find_kth(int k, int _root) {
        if(_root == 0) {
            return 0;
        }
        // 如果存在反转标记，先进行反转，否则左右子树就错了
        if(tree[_root].isRev) {
            push_down(_root);
        }
        // 获取左右儿子
        int lc = tree[_root].lc;
        int rc = tree[_root].rc;
        // _root在以他为根的根树中的排位即为左子树大小+1
        // 刚好为k，则该节点排位就是k
        if(tree[lc].size + 1 == k) {
            return _root;
        }
        // 大于k，则目标节点在左子树
        else if(tree[lc].size + 1 > k) {
            return find_kth(k, lc);
        }
        // 小于k，深入右子树，注意此时要减去
        else {
            return find_kth(k - tree[lc].size - 1, rc);
        }
    }

    // 将以now为根的根数以k为指标划分为[0,k)和[k,n)两个子树
    // 两个子树的根就是最初传进来的x,y（因为这里使用引用）
    // now是原本的树中当前遍历到的节点的下标
    // 类似于之前找值得思路，只是这次如果分割点在右子树，应该要将当前now和左子树接到左区间上，反过来也是
    // x,y是左右区间树下一个要接得地方
    void split(int now, int k, int& x, int& y) {
        // now为空，结束
        if(now == 0) {
            x = 0;
            y = 0;
            return;
        }
        // 如果有反转标记，先进行反转
        if(tree[now].isRev) {
            push_down(now);
        }
        int lc = tree[now].lc;
        int rc = tree[now].rc;
        // 分割点在左子树，则当前节点和右子树接入右区间树，深入左子树递归
        // 此时接入的是一个含有右节点的子树，所以y应该还有左节点空着，则取左节点为下一个要接入的节点
        if(tree[lc].size + 1 > k) {
            y = now;
            // 断开另一个子树
            tree[y].lc = 0;
            split(lc, k, x, tree[y].lc);
        }
        else {
            x = now;
            // 断开另一个子树
            tree[x].rc = 0;
            split(rc, k - tree[lc].size - 1, tree[x].rc, y);
        }
        // 更新高度
        // 到达此处时递归已经完成，树已经完备，可以从下到上更新高度
        update_height(now);

    }

    // 合并两棵子树
    // 将u,v合并，返回根节点，其中u为左区间树，v为右区间树
    // 根据rnd的约束条件，要么u为根，v在u的右子树；要么是对称的情况
    // 两者中rnd小的作为根
    // 以u为根为例，只需要将u的右子树和v进行合并，将合并后的根作为u的右儿子即可
    int merge(int u, int v) {
        // 一棵树为空，直接将另一颗树插入
        if(u == 0) {
            return v;
        }
        if(v == 0) {
            return u;
        }

        if(tree[u].rnd < tree[v].rnd) {
            // 先下推标记
            if(tree[u].isRev) {
                push_down(u);
            }
            // 递归合并
            tree[u].rc = merge(tree[u].rc, v);
            // 递归结束后更新高度
            update_height(u);
            return u;
        }
        else {
            if(tree[v].isRev) {
                push_down(v);
            }
            tree[v].lc = merge(u, tree[v].lc);
            update_height(v);
            return v;
        }
    }

//-------------------------这部分参考了一些资料---------------------------

    // 插入节点，插入后树中编号为rank
    void insert(int rank, int id) {
        int n = new_Node(id);
        x = 0;
        y = 0;
        // 分割为[0, rank)和[rank, n)
        split(root, rank, x, y);
        // 左区间树就是x，右区间树是y
        z = merge(x, n);
        root = merge(z, y);

    }

    // 交换两个节点的代号
    void swap(int i, int j) {
        if(i == j) {
            return;
        }
        int node_i = find_kth(i + 1, root);
        int node_j = find_kth(j + 1, root);
        int tmp = tree[node_i].id;
        tree[node_i].id = tree[node_j].id;
        tree[node_j].id = tmp;
    }

    // 整体旋转
    void rotate(int r) {
        if(cnt == 0) {
            return;
        }
        // 取模计算旋转的位数
        r = ((r%cnt) + cnt) % cnt;
        // 如果位数为0，直接退出
        if(r == 0) {
            return;
        }

        // 整体旋转r位
        // 相当于[0, cnt) = [0, cnt - r) + [cnt - r, cnt)变成[cnt - r, cnt) + [0, cnt - r)
        // 只需要将其拆开，在换过来合并即可
        split(root, cnt - r, x, y);
        root = merge(y, x); 
    }

    // 翻转
    // 找到节点区间对应的子树，然后给根节点打上反转标记即可
    // 可以通过两次split来实现划分

    // [l, r]区间反转
    void reverse(int l, int r) {
        if(l == r) {
            return;
        }

        x = 0;
        y = 0;
        z = 0;
        // 单个区间
        // 分出[0, l), [l, r], (r, n)
        if(l < r) {
            split(root, l, x, y);
            // x:[0, l), y:[l, n)
            // 在对y进行分割
            int tmp = 0;
            split(y, r - l + 1, tmp, z);
            tree[tmp].isRev ^= 1;
            root = merge(x, merge(tmp ,z));
        }
        // 双区间
        // 此时应该进行分割，然后连接起来反转，再进行合并
        else {
            // 循环区间 [l, n) ∪ [0, r]
            // 步骤1：旋转，把 [l,n) 移到开头
            split(root, l, x, y);     
            root = merge(y, x);       
            
            // 步骤2：现在 [l,r] 变成了连续的 [0, len-1]
            int len = cnt - l + r + 1;  // 循环区间长度
            split(root, len, x, y);
            tree[x].isRev ^= 1;
            root = merge(x, y);
            
            // 步骤3：旋转回去，把前 (n-l) 个移到后面
            split(root, cnt - l, x, y);
            root = merge(y, x);
        }
    }

    // 中序遍历
    // 注意树的顺序和真实的顺序不一样，应该从树中编号为offset(或offset + n)的开始输出
    void traver(int now) {
        
        if(now == 0) {
            return;
        }
        if(tree[now].isRev) {
            push_down(now);
        }
        traver(tree[now].lc);
        printf("%d ", tree[now].id);
        traver(tree[now].rc);

    }


} treap;
int main() {
    srand(time(NULL));
    int n, m;
    scanf("%d%d", &n, &m);
    for(int i = 0; i < m; i++) {
        char op;
        scanf(" %c", &op);
        if(op == 'I') {
            int rank, id;
            scanf("%d%d", &id, &rank);
            treap.insert(rank, id);

        }
        else if(op == 'S') {
            int a, b;
            scanf("%d%d", &a, &b);
            treap.swap(a, b);
        }
        else if(op == 'R') {
            int r;
            scanf("%d", &r);
            treap.rotate(r);
        }
        else if(op == 'F') {
            int a, b;
            scanf("%d%d", &a, &b);
            treap.reverse(a, b);
        }
    }
    // 处理完成后，中序遍历输出答案
    treap.traver(treap.root);


}