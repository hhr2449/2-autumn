#include <cstdio>
const int MAX_N = 1000000;
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
        pair *data = new pair[MAX_N];

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

int main() {
    int n;
    scanf("%d", &n);
    MaxQueue maxqueue;
    int *people_num = new int[MAX_N];
    //people_num[i]表示第i天确诊的人数
    for(int i = 0; i < n; i++) {
        int num;
        scanf("%d", &num);
        people_num[i] = num;
    }
    //储存每一天的追溯天数
    int *date_num = new int[MAX_N];
    for(int i = 0; i < n; i++) {
        int num;
        scanf("%d", &num);
        date_num[i] = num;
    }
    //储存范围
    int t;
    scanf("%d", &t);
    int *p = new int[MAX_N];
    int *q = new int[MAX_N];
    for(int i = 0; i < t; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        p[i] = a;
        q[i] = b;
    }
    //储存每种范围对应的风险天数
    int *low_risk = new int[MAX_N];
    int *middle_risk = new int[MAX_N];

    for(int i = 0; i < t; i++) {
        low_risk[i] = 0;
        middle_risk[i] = 0;
    }
    //单独处理第0天
    //p可以为0，真他妈傻逼，他妈的改了2个小时
    for(int j = 0; j < t; j++) {
        if(p[j] == 0) {
            middle_risk[j] = 1;
        }
        else {
            low_risk[j] = 1;

        }
    }
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
        //判断对应范围的风险等级
        for(int j = 0; j < t; j++) {
            if(k < p[j]) {
                low_risk[j]++;
            }
            if(k < q[j] && k >= p[j]) {
                middle_risk[j]++;
            }
        }

    }
    for(int i = 0; i < t; i++) {
        printf("%d %d\n", low_risk[i], middle_risk[i]);
    }
}