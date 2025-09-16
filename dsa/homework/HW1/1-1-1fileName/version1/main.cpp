// 描述
// 助教在批改作业时发现同学们提交的文件名都很任性，所以决定统一规范一下。在动手之前他想知道，将文件名 A 转换为 B 至少需要多少时间；如果超过 K 个时间单位，便干脆放弃。

// 为了度量转换的时间成本，我们约定仅允许以下两种编辑操作，且它们各需一个单位的时间：

// 1.在第i个位置插入一个字符 

// 2.删除第i个字符

// 输入
// 共包含 3 行。

// 第 1 行包含三个整数 N、M、K：文件名 A 的长度、文件名 B 的长度、时间成本的阈值。

// 接下来的2行，分别给出原始字符串 A 和目标字符串 B。

// 输出
// 仅1行，给出将文件名由A转换为B所需的最小时间成本；如果超过K，则输出-1。

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
            //构建二维数组
            this->f = new int*[m + 1];
            for(int i = 0; i <= m; i++) {
                this->f[i] = new int[n + 1];
            }
        }

        //返回值是a,b之间的编辑距离
        int getMinDistance() {
            //初始化边界
            //注意变量的含义是长度，而不是索引
            for(int i = 0; i <= m; i++) {
                f[i][0] = i;
            }
            for(int j = 0; j <= n; j++) {
                f[0][j] = j;
            }
            //开始递推
            for(int i = 1; i <= m; i++) {
                for(int j = 1; j <= n; j++) {
                    if(a[i - 1] == b[j - 1]) {
                        f[i][j] = f[i - 1][j - 1];
                    }
                    else {
                        f[i][j] = min(f[i - 1][j], f[i][j - 1]) + 1;
                    }
                }
            } 
            return f[m][n];
        }
        ~minDistance() {
            for(int i = 0; i <= m; i++) {
                delete[] f[i];
            }
            delete[] f;
        }
};
int main() {
    int m, n, k;
    cin >> m >> n >> k;
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