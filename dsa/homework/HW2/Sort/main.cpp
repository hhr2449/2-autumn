#include <iostream>
using namespace std;

//定义函数指针
typedef void (*SortFunc) (int[], int, int);
//对[lo, hi)的元素进行冒泡排序


//!冒泡排序
//时间O(n^2),空间O(1)，非常稳定
void bubble_sort(int arr[], int lo, int hi) {
    //内层进行一趟循环，就可以使有序后缀增长1
    //无序前缀是[lo, hi)，每次hi要减1
    while(hi > lo) {
        //最大下标是hi-1，不能超过

        //优化：如果最后一次交换的是last和last - 1,那么[last, hi)一定是有序的
        //所以之后只需要对[lo, last)进行排序即可
        int last = lo;
        for(int i = lo; i < hi - 1; i++) {
            if(arr[i] > arr[i+1]) {
                swap(arr[i], arr[i+1]);
                last = i + 1;
            }
        }
        hi = last;
    }
}


//!归并排序
//!T(n) = 2T(n/2) + O(n),时间复杂度为O(nlogn)，空间复杂度为O(n)
//稳定，相同元素的相对位置不变，时间也是稳定的nlogn
//将数组划分成前后两部分，然后分别对两部分进行归并排序
//两部分分别有序后，再进行merge合并

//要合并的数组的范围是[lo, mid),[mid, hi)
void merge(int arr[], int lo, int mid, int hi) {
    int n = hi - lo;
    int *tmp = new int[n];
    int idx = 0;
    int i = lo, j = mid;
    //i, j分别是前后两个数组的索引
    //循环比较，将小的放进去
    //当一个数组用完时(i == mid或是j == hi)，退出
    while(i < mid && j < hi) {
        if(arr[i] < arr[j]) {
            tmp[idx++] = arr[i++];
        }
        else {
            tmp[idx++] = arr[j++];
        }
    }
    //将空的数组全部填入
    while(i < mid) {
        tmp[idx++] = arr[i++];
    }
    while(j < hi) {
        tmp[idx++] = arr[j++];
    }

    //然后将tmp填入arr
    for(int i = 0; i < n; i++) {
        arr[lo + i] = tmp[i];
    }
    delete[] tmp;
}

//范围[lo, hi)
void merge_sort(int arr[], int lo, int hi) { 
    //递归边界：当数组长度为1时，自然有序，退出
    if(hi - lo < 2) {
        return;
    } 
    //划分
    int mid = lo + ((hi - lo) >> 1);
    //前后两部分分别进行排序，最后merge在一起
    merge_sort(arr, lo, mid);
    merge_sort(arr, mid, hi);
    merge(arr, lo, mid, hi);
}

//!快速排序
//!最好情况：T(n) = 2T(n/2) + O(n),此时时间复杂度为O(nlogn)
//!平均情况也有O(nlogn)
//!最差情况：每次选取的枢值都是最大值，此时退化为冒泡排序，T(n) = O(n-1) + O(n),此时时间复杂度为O(n^2)
//!空间复杂度：最好空间复杂度O(logn),最差空间复杂度O(n)
//不稳定，相同元素的相对位置可能改变


int partition(int arr[], int lo, int hi) {
    //选取枢值，将其放在首位
    //这里选择了第一个元素，也可以选择随机的元素或是在首、中、尾中选取中间的元素
    int pivot = arr[lo];
    int i = lo;
    int j = hi - 1;
    while(i < j) {
        //注意内部的循环也必须要加上i < j的判断条件，因为内部移动的时候也可能会造成i >= j
        while(i < j && arr[j] >= pivot) {
            j--;
        }
        arr[i] = arr[j];
        while(i < j && arr[i] < pivot) {
            i++;
        }
        arr[j] = arr[i];
    }
    arr[i] = pivot;
    return i;
}
void quick_sort(int arr[], int lo, int hi) { 
    if(hi - lo < 2) {
        return;
    }
    int p = partition(arr, lo, hi);
    quick_sort(arr, lo, p);
    quick_sort(arr, p + 1, hi);
}

//!选择排序
//!T(n) = n^2,空间复杂度O(1)，不稳定
//!选择排序每次选择最小的元素，放在当前位置
//!相对于冒泡排序来说，其比较更多，写更少，如果写入消耗大，则选择排序更适合（但是选择和冒泡都是比较差的排序方法）
void select_sort(int arr[], int lo, int hi) {
    //每次确定一个最小的元素
    for(int i = lo; i < hi; i++) {
        //寻找最小元素
        int min = i;
        for(int j = i + 1; j < hi; j++) {
            if(arr[j] < arr[min]) {
                min = j;
            }
        }
        //交换
        swap(arr[i], arr[min]);
    }
}

//!插入排序
//将arr[j]插入到arr[0...j-1]中
//0~j-1已经经过排序是升序的，想要将j插入到这个有序的序列中，只需要从j-1~0进行比较
//如果值小于arr[j]，则后移一个单位给arr[j]腾位置，知道遇到了小于或等于arr[j]的元素，此时可以插入arr[j]
//!时间复杂度：O(n^2)，空间复杂度：O(1)
//好处是：在线算法，可以一边接受数据，一边进行排序

void insert_sort(int arr[], int lo, int hi) {
    //第一个元素本身肯定是有序的，不需要进行插入，从第二个元素开始
    for(int i = lo + 1; i < hi; i++) {
        //记录下要插入的值
        int key = arr[i];
        //从i-1开始进行比较
        int j = i - 1;
        //如果arr[j]小于key，则后移一个单位给key腾位置
        //知道遇到一个<=key的元素或是移到了尽头，则插入
        while(j >= lo && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--; 
        }
        arr[j + 1] = key;
    }
}




//测试函数
void test_one(SortFunc func, int arr[], int lo, int hi) {
    func(arr, lo, hi);
    for(int i = lo; i < hi; i++) {
        cout << arr[i] << " ";
    }
}

void test(SortFunc func) {
    // 一组用于测试排序算法的数组定义
    int arr1[] = {5, 3, 8, 4, 2};                // 随机数组
    int arr2[] = {1, 2, 3, 4, 5};                // 已经有序（最好情况）
    int arr3[] = {5, 4, 3, 2, 1};                // 逆序（最坏情况）
    int arr4[] = {4, 2, 2, 8, 3, 3, 1};          // 含重复元素
    int arr5[] = {10};                           // 单元素数组
    int arr6[] = {};                             // 空数组（注意长度为0时需特殊处理）
    int arr7[] = {0, -1, 5, -10, 3};             // 含负数
    int arr8[] = {1, 3, 2, 3, 1, 2, 3, 2, 1};    // 含重复子序列
    int arr9[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};    // 逆序大样本
    int arr10[] = {7, 2, 5, 3, 9, 1, 4, 6, 8};   // 随机中等规模数组

    // 每个测试数组的元素个数
    int sizes[] = {
        sizeof(arr1) / sizeof(arr1[0]),
        sizeof(arr2) / sizeof(arr2[0]),
        sizeof(arr3) / sizeof(arr3[0]),
        sizeof(arr4) / sizeof(arr4[0]),
        sizeof(arr5) / sizeof(arr5[0]),
        sizeof(arr6) / sizeof(arr6[0]),
        sizeof(arr7) / sizeof(arr7[0]),
        sizeof(arr8) / sizeof(arr8[0]),
        sizeof(arr9) / sizeof(arr9[0]),
        sizeof(arr10) / sizeof(arr10[0])
    };
    int *arr[] = {arr1, arr2, arr3, arr4, arr5, arr6, arr7, arr8, arr9, arr10};
    for(int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        cout << "test " << i << ": ";
        test_one(func, arr[i], 0, sizes[i]);
        cout << endl;
    }
}
int main() {
    cout << "bubble_sort:" << endl;
    test(bubble_sort);
    cout << "merge_sort:" << endl;
    test(merge_sort);
    cout << "quick_sort:" << endl;
    test(quick_sort);
    cout << "select_sort:" << endl;
    test(select_sort);
    cout << "insert_sort:" << endl;
    test(insert_sort);
}