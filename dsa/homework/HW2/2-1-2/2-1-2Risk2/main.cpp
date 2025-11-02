#include <cstdio>
//思路：使用单调队列
//时间靠前且人数更少的天数一定不可能成为追溯范围中的最大人数日，所以当一个节点入队列的时候可以淘汰掉队尾所有不大于他的数据
//队列从队尾到队首逐渐增大，队首元素就是当前最大值
//但是还要考察是否在范围中，计算某一天时，如果队首元素不在其追溯范围内，那么根据题目限制，
//该元素也不可能在之后的日期的追溯范围里面，此时可以直接将其出队
//通过这种方法，可以求出每一天的追溯范围内的最大感染人数
//对于不同的查询范围，可以先求出最大感染人数的数组，然后将其排序，使用二分查找来找到p,q的分界点
//找到第一个大于等于p的点i和第一个大于等于q的点j，那么[0,i)就是低风险,[i,j)就是高风险




//求每日最大值o(n),排序o(nlogn)，t次查询，每次使用logn，总共o(n+nlogn+tlogn)

//!快排最差会到O(n^2)，这里快排过不了，但是归并排序能过
int n;
class pair {
    public:
    //第一个存天数，第二个存人数
        int first;
        int second;
        pair(int first, int second) : first(first), second(second){

        }
        pair(){

        }
};

class MaxQueue {
    public:
        //head是队列中的第一个元素，tail是队列中下一个要写入的元素位置
        int head = 0, tail = 0;
        pair *data = new pair[n + 1];

        MaxQueue(){
        }
        void push(int date, int people) {
            //队尾元素如果小于x就要淘汰掉
            while(tail > head && data[tail - 1].second < people) {
                tail--;
            }
            data[tail].first = date;
            data[tail].second = people;
            tail++;
        }
        void pop() {
            head++;
        }
        pair top() {
            return data[head];
        }

};

//快速排序
//排序范围[left, right]
int partition(int *arr, int left, int right) {
    int pivot = arr[left];
    while(left < right) {
        while(left <right && arr[right] > pivot) {
            right--;
        }
        arr[left] = arr[right];
        while(left < right && arr[left] <= pivot) {
            left++;
        }
        arr[right] = arr[left];
    }
    arr[left] = pivot;
    return left;
}

void quickSort(int *arr, int left, int right) {
    if(left < right) {
        int pivot = partition(arr, left, right);
        quickSort(arr, left, pivot - 1);
        quickSort(arr, pivot + 1, right);
    }
}

//返回arr中[left, right)范围内大于等于target的最小值索引(如果有多个返回最靠左的索引)
//这里的查找思路：1. 保证不变量：[0, left - 1]中的元素一定小于target，[right, n)中的元素一定大于等于target
//2. 缩小范围：mid的取值可能：[left, right)，所以动left是变成mid + 1，动right变成mid可以保证每次范围都缩小
//3. 确定里不变量和left、right的变化情况，即可确定变化的条件

//实际上和二分查找一个特定的值是一样的，只是将等于也归入了范围中
int binarySearch(int *arr, int left, int right, int target) {
    while(left < right) {
        int mid = left + (right - left)/2;
        if(arr[mid] >= target) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }
    return right;
}


int main() {
    scanf("%d", &n);
    MaxQueue maxqueue;
    int *people_num = new int[n + 1];
    //people_num[i]表示第i天确诊的人数
    for(int i = 0; i < n; i++) {
        int num;
        scanf("%d", &num);
        people_num[i] = num;
    }
    //储存每一天的追溯天数
    int *date_num = new int[n + 1];
    for(int i = 0; i < n; i++) {
        int num;
        scanf("%d", &num);
        date_num[i] = num;
    }
    //储存范围
    int t;
    scanf("%d", &t);
    int *p = new int[t + 1];
    int *q = new int[t + 1];
    for(int i = 0; i < t; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        p[i] = a;
        q[i] = b;
    }
    //每一天的追溯范围内的最大感染人数
    int *max_num = new int[n + 1];
    //单独处理第0天
    max_num[0] = 0;
    //第i天应该让第i-1天的感染人数入队
    for(int i = 1; i < n; i++) {
        maxqueue.push(i - 1, people_num[i - 1]);
        pair top = maxqueue.top();
        //如果超出了追溯范围，则出队
        while(top.first < i - date_num[i]) {
            maxqueue.pop();
            top = maxqueue.top();
        }
        //此时的top就是追溯范围内最大感染人数
        int k = top.second;
        max_num[i] = k;
    }
    quickSort(max_num, 0, n - 1);
    for(int i = 0; i < t; i++) {
        int p_index = binarySearch(max_num, 0, n, p[i]);
        int q_index = binarySearch(max_num, p_index, n, q[i]);
        if(p_index == -1) {
            printf("%d 0\n", n);
            continue;
        }
        if(q_index == -1) {
            printf("%d %d\n", n, n - p_index);
            continue;
        }
        printf("%d %d\n", p_index, q_index - p_index);
    }

}