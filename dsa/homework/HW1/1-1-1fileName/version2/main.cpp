//思路：看到求解最值（编辑距离）问题，想到使用动态规划
//1. 设定变量并且确定含义
//2. 设定f(n)/或f(m,n)及其更高维度的含义，一般用于表示特定变量条件下的最优解
//3. 确定状态转移方程

//1.这里看到涉及字符串，想到一般来说涉及字符串的动态规划会设变量为字符串的长度，这里有两个字符串，
//所以设定变量m,n用于表示字符串A,B的长度
//2.f(m,n)可以表示：将字符串A[:m]转化为B[:n]的编辑距离（这里是一个二维的动态规划，明显一个变量是没有办法描述的）
//3.状态转移方程：
//想办法使用更小的f来表示出f(m,n)
//如果A[m] == B[n],那么f(m,n) == f(m-1, n-1)
//如果A[m] != B[n],那么f(m,n) == min(f(m-1, n), f(m, n-1)) + 1 (要不然删去A[m],要不然在A[m]后面插入B[n])
//4.初始条件：
//f(0,0) = 0
//f(0,n) = n
//f(m,0) = m
//5.返回结果：
//f(m,n)

//构建代码：
//设定一个二维数组f[][]用于表示编辑距离
//初始化f,利用初始条件
//设定双重循环用于表示求解f[i][j]
//循环中，利用转移方程进行求解

//!优化：
//按照刚刚的思路，需要开一个大小为m*n的数组来进行储存，这样占用内存会很大
//观察求解过程发现，求解f[i][j]只需要知道上面的和左边的值即可，我们采用一行一行往下推的方法，
//!那么只需要维护当前行和上一行即可，不需要维护整个二维数组
//!及时释放掉没用的数据

#include <iostream>
using namespace std;
class minDistance {
    public:
        string a, b;
        int m, n;
        int **f;
        minDistance(string a, string b) {
            this->a = a;
            this->b = b;
            this->m = a.size();
            this->n = b.size();
            //只储存两行数据
            this->f = new int*[2];
            for(int i = 0; i < 2; i++) {
                this->f[i] = new int[n + 1];
            }
        }

        //返回值是a,b之间的编辑距离
        int getMinDistance() {
            //初始化边界
            //注意变量的含义是长度，而不是索引
            for(int j = 0; j <= n; j++) {
                f[0][j] = j;
            }
            //开始递推
            //f[0]代表当前求解行的上一行,f[1]代表当前求解行
            for(int i = 1; i <= m; i++) {
                //求解第i行的时候要先确定第一个元素
                f[1][0] = i;
                for(int j = 1; j <= n; j++) {
                    if(a[i - 1] == b[j - 1]) {
                        f[1][j] = f[0][j - 1];
                    }
                    else {
                        f[1][j] = min(f[0][j], f[1][j - 1]) + 1;
                    }
                }
                //交换f[0]和f[1]，f[1]在下一次循环中会被覆盖
                swap(f[0], f[1]);
            } 
            return f[0][n];
        }
};
int main() {
    int m, n, k;
    cin >> m >> n >> k;
    if(m - n > k || n - m > k) {
        cout << -1 << endl;
        return 0;
    }
    string a, b;
    cin >> a >> b;
    minDistance md(a, b);
    int minDistance = md.getMinDistance();
    if(minDistance <= k) {
        cout << minDistance << endl;
    }
    else {
        cout << -1 << endl;
    }
}