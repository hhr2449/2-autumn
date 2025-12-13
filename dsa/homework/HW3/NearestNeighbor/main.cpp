#include <stdio.h>
#include <cstdlib>
#define ll long long
const int N = 100005;

// 表示一个点
// 一个点最多5个维度，使用一个5维数组表示
// axis等于0,1,2,3,4分别表示第1,2,3,4,5个维度
// 坐标一定是整数

struct Point {
    int x[5];
};

// 使用一个大数组来储存所有的点
Point points[N];
// 排序使用的临时数组
Point tmp[N];

// 对points数组进行排序
// 指定以axis为标准

// 归并将points数组的[l, mid)和[mid,r)进行合并
void merge(int l, int mid, int r, int _axis) {
    // 左半数组和右半数组分别从l和mid开始
    int idx_l = l;
    int idx_r = mid;
    int idx_tmp = 0;
    // 宽度为r-l
    int width = r - l;
    while(idx_l < mid && idx_r < r) {
        if(points[idx_l].x[_axis] <= points[idx_r].x[_axis]) {
            tmp[idx_tmp++] = points[idx_l++];
        }
        else {
            tmp[idx_tmp++] = points[idx_r++];
        }
    }
    
    // 复制剩余元素
    while(idx_l < mid) {
        tmp[idx_tmp++] = points[idx_l++];
    }
    while(idx_r < r) {
        tmp[idx_tmp++] = points[idx_r++];
    }
    // 最后将tmp数组复制到points数组
    for(int i = 0; i < width; i++) {
        points[l + i] = tmp[i];
    }
}

// 进行归并排序
void merge_sort(int l, int r, int _axis) {
    // 长度为1，自然有序
    if(r - l <= 1) {
        return;
    }
    // 否则分别对左半数组和右半数组进行排序
    int mid = l + ((r - l) >> 1);
    merge_sort(l, mid, _axis);
    merge_sort(mid, r, _axis);
    // 合并左右数组
    merge(l, mid, r, _axis);
}

// 每一个KDTree节点需要存储的信息：
// 存点坐标(分割点)，左右孩子坐标，当前节点的划分维度（当前节点划分出子节点的维度）

// 使用数组来储存所有KDTree节点的信息
Point kd_tree[N];
int lc[N], rc[N];
int axis[N];

// 维护一个全局变量，作为最短距离的平方的记录
ll min_dist;

// kd树类
// 根节点从1开始，0表示空节点
struct KDTree{
    // 总节点个数，总的维度
    int n, d;
    // 现有节点个数
    int cnt = 0;
    KDTree(int n, int d) : n(n), d(d) {}


    // 建树
    // 以当前维度为标准，对[l, r)区间内的点进行建树，返回根节点坐标
    int build_tree(int l, int r, int cur_axis) {
        // 递归建树
        // 每次以当前维度为标准进行排序
        // 取中位数作为分割点
        // 递归建树左子树和右子树

        // 递归基
        // 空区间
        if(l >= r) {
            return 0;
        }
        // 区间长度为1，建立节点，返回
        if(r - l == 1) {
            // 建立节点
            cnt++;
            // 节点坐标就是points中第l个点
            kd_tree[cnt] = points[l];
            // 左右孩子默认为空
            lc[cnt] = rc[cnt] = 0;
            // 无需向下递归，所以axis也不用管
            return cnt;
        }


        // 否则进行递归建树
        // 对范围内的点进行排序，取中点作为分割点
        int mid = l + ((r - l) >> 1);
        // 以当前维度为标准进行排序
        merge_sort(l, r, cur_axis);
        // 建立节点
        cnt++;
        kd_tree[cnt] = points[mid];
        //! 注意，这里要先记录节点的编号，因为后面递归调用会修改cnt
        int cnt_old = cnt;
        // 递归建树
        // axis向后轮转一位
        //! 注意，当前已经使用了mid了，所以递归时不能将mid纳入范围
        lc[cnt_old] = build_tree(l, mid, (cur_axis + 1) % d);
        rc[cnt_old] = build_tree(mid + 1, r, (cur_axis + 1) % d);
        // 当前节点的划分维度就是当前维度
        axis[cnt_old] = cur_axis;
        return cnt_old;
    }

    // 查找，随时更新平方的最小值
    // 目标节点为x，当前节点为cur
    void search_best(Point x, int cur) {
        // 计算当前节点到目标节点距离的平方值
        ll dist = 0;
        for(int i = 0; i < d; i++) {
            dist += (ll)((ll)(x.x[i] - kd_tree[cur].x[i]) * (ll)(x.x[i] - kd_tree[cur].x[i]));
        }
        // 如果更优，则更新全局最小值
        if(dist < min_dist) {
            min_dist = dist;
        }

        // 接着进行递归查找
        // 先向较近分支递归（也就是目标节点所在区域对应的分支）
        // 再根据min_dist来判断是否要向较远分支进行递归
        // 实际执行的效果是：先沿着一条路径走到目标节点所在的区域对应的叶子节点，并且路上同时更新min_dist
        // 然后向上回溯，根据min_dist判断是否向较远分支进行递归

        int near, far;
        // 如果在当前节点的分割维度下，目标节点的值小于分割点的值，则左子树是较近分支
        if(x.x[axis[cur]] <= kd_tree[cur].x[axis[cur]]) {
            near = lc[cur];
            far = rc[cur];
        }
        else {
            near = rc[cur];
            far =lc[cur];
        }
        // 先递归查找较近分支
        if(near != 0) {
            search_best(x, near);
        }
        // 如果当前节点到分割线的距离大于等于min_dist，则说明远端分支的点到目标节点的距离也大于等于min_dist
        // 所以无需递归查找远端分支
        ll dist_to_split = (ll)((ll)(x.x[axis[cur]] - kd_tree[cur].x[axis[cur]]) * (ll)(x.x[axis[cur]] - kd_tree[cur].x[axis[cur]]));
        if(dist_to_split < min_dist) {
            if(far != 0) {
                search_best(x, far);
            }
        }
    }
};

int main() {
    // 输入所有的点
    int d, n;
    scanf("%d %d", &d, &n);
    // n个向量
    for (int i = 0; i < n; i++) {
        // d个维度
        for(int j = 0; j < d; j++) {
            scanf("%d", &points[i].x[j]);
        }
    }
    
    KDTree kdtree(n, d);
    // 建树
    int root = kdtree.build_tree(0, n, 0);
    
    int q;
    scanf("%d", &q);
    // q次查询
    while(q--) {
        // 输入查询点
        Point query;
        for(int j = 0; j < d; j++) {
            scanf("%d", &query.x[j]);
        }
        // 初始化min_dist为一个较大值
        min_dist = 0x3f3f3f3f3f3f3f3fLL;
        // 查找
        kdtree.search_best(query, root);
        // 输出
        printf("%lld\n", min_dist);
    }
}