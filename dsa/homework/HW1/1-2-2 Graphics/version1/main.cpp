//1. 如何连接线段：将两个点数组进行排序，对应位置的点就是相连的点
//2. 判断某个点是否经过线段：设x轴上的点为p，y轴上的点为q，那么让给定点s对向量pq,进行toleft检测，如果结果为正，说明
//   s在向量pq的左侧，也就是不经过pq
//3. 如何进行查找：只要确定点在那两个线段之间即可，可以使用binary search，如果判断该点在某个线段的左边则向左边查找，右边同理

#include <iostream>
using namespace std;
class Graphics {
    public:
        //属性：点的个数，点x坐标，点y坐标，这些属性可以构造出要求的图
        int pointNum;
        int *pointX;
        int *pointY;
        Graphics(int n, int *x, int *y) {
            pointNum = n;
            pointX = x;
            pointY = y;
            //构造的时候直接进行排序
            quickSort(pointX, 0, pointNum - 1);
            quickSort(pointY, 0, pointNum - 1);
        }
        //toleft test
        //如果点q在向量(x2-x1, y2-y1)的左边，则返回true
        bool toleft(int x1, int y1, int x2, int y2, int qx, int qy) {
            //!注意这里面的减法也可能会导致溢出的问题，必须要先转成longlong
            long long a = (long long)(x2 - x1) * (long long)(qy - y1);
            long long b = (long long)(qx - x1) * (long long)(y2 - y1);
            return a > b;
        }
        //快排的辅助划分函数，可以将指定的区间内的元素划分成两个部分，并且返回pivot的索引
        //划分的范围是[left, right]
        int partition(int *arr, int left, int right) {
            //选取第一个元素为pivot
            int pivot = arr[left];
            //使用双指针，左指针寻找大于pivot的元素，放在右指针指向的位置，右指针寻找小于的元素，放在左指针指向的位置
            while(left < right) {
                while(left < right && arr[right] > pivot) {
                    right--;
                }
                arr[left] = arr[right];
                while(left < right && arr[left] <= pivot) {
                    left++;
                }
                arr[right] = arr[left];
            }
            //相遇的位置就是pivot
            arr[left] = pivot;
            return left;
        }
        //sort
        //对点进行排序
        //使用快速排序
        void quickSort(int *arr, int left, int right) {
            if(left < right) {
                int pivot = partition(arr, left, right);
                quickSort(arr, left, pivot - 1);
                quickSort(arr, pivot + 1, right);
            }
        }

        //search，get the answer
        //经过排序，线段应该是从内向外排
        //返回点左边的线段下标
        //范围是[left, right]
        int getAns(int x, int y) {
            int left = 0, right = pointNum - 1;
            //在所有线段的左边
            if(toleft(pointX[0], 0, 0, pointY[0], x, y)) {
                return 0;
            }
            //在所有线段的右边
            if(!toleft(pointX[pointNum - 1], 0, 0, pointY[pointNum - 1], x, y)) {
                return pointNum;
            }
            //只有一个线段
            if(pointNum == 1) {
                if(toleft(pointX[0], 0, 0, pointY[0], x, y)) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            //两个线段
            if(pointNum == 2) {
                int ans = 0;
                if(!toleft(pointX[0], 0, 0, pointY[0], x, y)) {
                    ans++;
                }
                if(!toleft(pointX[1], 0, 0, pointY[1], x, y)) {
                    ans++;
                }
                return ans;
            }
            //多于两个线段
            //这里只处理多于两个线段并且点在线段内部的情况，其余情况已经在上面处理
            //可以这样想：线段序列可以看作这样一个序列：1 1……1 0 0……0 0 
            //这里1代表经过的线段，0代表没有经过的线段，现在要做的事情就是找到1和0的交汇点
            //那么left和right分别位于两端，进行二分查找，当mid指向0是就将right移动到mid，当mid指向1时将left移动到mid
            //这样left和right的距离一定是单调递减并且最后一定会到达1，此时left和right分别位于1和0的交汇点
            while (right - left > 1) {
                int mid = left + (right - left) / 2;
                if (toleft(pointX[mid], 0, 0, pointY[mid], x, y)) {
                    right = mid;
                } else {
                    left = mid;
                }
            }
            return left + 1;
        }
        //析构函数
        ~Graphics() {
            delete[] pointX;
            delete[] pointY;
        }
};
int main() {
    int n;
    int *x;
    int *y;
    cin >> n;
    x = new int[n];
    y = new int[n];
    for(int i = 0; i < n; i++) {
        scanf("%d", &x[i]);
    }
    for(int i = 0; i < n; i++) {
        scanf("%d", &y[i]);
    }
    Graphics g(n, x, y);
    int queryCnt;
    scanf("%d", &queryCnt);
    for(int i = 0; i < queryCnt; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        printf("%d\n", g.getAns(x, y));
    }
}