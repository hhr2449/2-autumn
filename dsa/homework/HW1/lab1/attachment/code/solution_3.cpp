#include <cstdlib>
#include <cstdio>
#define ll long long
//思路：可以构建一个二维前缀和数组，也就是一个mn的二维数组prefixsum，prefixsum[i][j]是第i行j列的元素和。
//的矩形的和，那么对于任意一个起点为(x, y)，大小为(a, b)的子数组，其和应该为
//prefixsum[x + a - 1][y + b - 1] - prefixsum[x + a - 1][y - 1] - prefixsum[x - 1][y + b - 1] + prefixsum[x - 1][y - 1]
//前缀和数组的构建时间复杂度为O(mn)，查询的时间复杂度为O(1)，则整体的时间复杂度为O(mn+p)

//注意看数据范围，使用long long
//long long使用%lld 输出
ll prefixsum[2000][2000];
int main() {
    ll n, m, p;
    scanf("%lld %lld", &n, &m);
    //!应该将下标从1算起，这样可以规避掉边界情况处理（常见边界处理技巧）
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            ll x;
            scanf("%lld", &x);
            prefixsum[i][j] = 0;
            if(i != 0) {
                prefixsum[i][j] += prefixsum[i - 1][j];
            }
            if(j != 0) {
                prefixsum[i][j] += prefixsum[i][j - 1];
            }
            if(i != 0 && j != 0) {
                prefixsum[i][j] -= prefixsum[i - 1][j - 1];
            }
            prefixsum[i][j] += x;
        }
    }
    scanf("%lld", &p);
    for(int i = 0; i < p; i++) {
        int x, y, a, b;
        ll sum;
        scanf("%d %d %d %d", &x, &y, &a, &b);
        x--;
        y--;
        sum = prefixsum[x + a - 1][y + b - 1];
        if(x != 0) {
            sum -= prefixsum[x - 1][y + b - 1];
        }
        if(y != 0) {
            sum -= prefixsum[x + a - 1][y - 1];
        }
        if(x != 0 && y != 0) {
            sum += prefixsum[x - 1][y - 1];
        }
        printf("%lld\n", sum);
    }
}