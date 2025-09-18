#include <cstdio>
#include <cstdlib>
#include <ctime>

//1. O(abp)
//2. O(mn+ap)
//3. O(mn+p)
//1.矩阵规模  2.子矩阵规模   3.查询次数
//当子矩阵规模大，查询次数多的时候3有优势，当原矩阵大，子矩阵小，查询次数少的收反而是1有优势
int main() {
    //使用当前的时间作为种子
    srand(time(0));
    //生成矩阵规模
    int n, m;
    //手动输入矩阵规模
    scanf("%d %d", &n, &m);
    //输出
    printf("%d %d\n", n, m);
    //生成矩阵元素
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ", rand() %1001);
        }
        printf("\n");
    }
    //输入查询次数
    int p;
    scanf("%d", &p);
    printf("%d \n", p);
    //生成查询元素
    //控制子矩阵大小
    int x = 1;
    int y = 1;
    int a, b;
    scanf("%d %d", &a, &b);
    for(int i = 0; i < p; i++) {
        printf("%d %d %d %d\n", x, y, a, b);
    }
    return 0;
}