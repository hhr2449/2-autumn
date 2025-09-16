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

//!优化：
//在version2的解法中，要求求解出整个二维的dp数组，所以时间复杂度是o(m*n)，会超时
//dp算法本身没有办法优化，需要利用给出的条件：当值大于k的时候直接输出-1，显然我们要想到**剪枝**
//在每一个行中，只有[i-k, i+k]的范围内可能出现小于等于k的数值，这样我们可以将行分成三段：
//1. [1, k] 2. (k, n-k) 3. [n-k, m]
//情况1：求解列的范围为[0, i+k],这里0列的值可以直接求得，i+k的值由于其上方的值一定会大于k，因此只有考虑左边和左上方的转移
//情况2：求解列的范围为[i-k, i+k],i-k的值只可能从左上方和上方转移过来，i+k的值只可能从左上方和左方转移过来
//情况3：求解列的范围为[i-k, m],i-k的值只可能从左上方和上方转移过来
#include <iostream>
using namespace std;
class minDistance {
    public:
        string a, b;
        int m, n, k;
        int **f;
        minDistance(string a, string b, int k) {
            this->a = a;
            this->b = b;
            this->k = k;
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
            for(int j = 0; j <= k; j++) {
                f[0][j] = j;
            }
            //开始递推
            //f[0]代表当前求解行的上一行,f[1]代表当前求解行
            //第[1, k]行需要求解[0, i + k]列的值
            for(int i = 1; i <= k; i++) {
                for(int j = 0; j <= i + k; j++) {
                    if(j == 0) {
                        f[1][j] = i;
                        continue;
                    }
                    if(a[i - 1] == b[j - 1]) {
                        f[1][j] = f[0][j - 1];
                    }
                    else {
                        if(j == i + k) {
                            f[1][j] = f[1][j - 1] + 1;
                        }
                        else {
                            f[1][j] = min(f[0][j], f[1][j - 1]) + 1;
                        }
                    }
                }
                swap(f[0], f[1]);
            } 
            //第(k, n - k)行需求解[i - k, i + k]
            for(int i = k + 1; i < n - k; i++) {
                for(int j = i - k; j <= i + k; j++) {
                    if(a[i - 1] == b[j - 1]) {
                        f[1][j] = f[0][j - 1];
                    }
                    else {
                        if(j == i - k) {
                            f[1][j] = f[0][j] + 1;
                        }
                        else if(j == i + k) {
                            f[1][j] = f[1][j - 1] + 1; 
                        }
                        else {
                            f[1][j] = min(f[0][j], f[1][j - 1]) + 1;
                        }

                    }
                }
                swap(f[0], f[1]);
            }
            //第[n - k, m]行需求解[i - k, n]
            for(int i = n - k; i <= m; i++) {
                for(int j = i - k; j <= n; j++) {
                    if(a[i - 1] == b[j - 1]) {
                        f[1][j] = f[0][j - 1];
                    }
                    else {
                        if(j == i - k) {
                            f[1][j] = f[0][j] + 1;
                        }
                        else {
                            f[1][j] = min(f[0][j], f[1][j - 1]) + 1;
                        }

                    }
                }
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
    minDistance md(a, b, k);
    int minDistance = md.getMinDistance();
    if(minDistance <= k) {
        cout << minDistance << endl;
    }
    else {
        cout << -1 << endl;
    }
}