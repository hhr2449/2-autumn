#include <cstdio>
#define ll long long
//思路：使用单调队列
//时间靠前且人数更少的天数一定不可能成为追溯范围中的最大人数日，所以当一个节点入队列的时候可以淘汰掉队尾所有不大于他的数据
//队列从队尾到队首逐渐增大，队首元素就是当前最大值
//但是还要考察是否在范围中，计算某一天时，如果队首元素不在其追溯范围内，那么根据题目限制，
//该元素也不可能在之后的日期的追溯范围里面，此时可以直接将其出队
//通过这种方法，可以求出每一天的追溯范围内的最大感染人数

//!求解落在某个区间内的元素数：
//使用前缀和，prefix[i]表示值为0，1，2……i的元素个数，然后每次就可以在常数时间内进行求解了

//!注意最大有2^32，应该使用ll
ll n;
class pair {
    public:
    //第一个存天数，第二个存人数
        ll first;
        ll second;
        pair(ll first, ll second) : first(first), second(second){

        }
        pair(){

        }
};

class MaxQueue {
    public:
        //head是队列中的第一个元素，tail是队列中下一个要写入的元素位置
        ll head = 0, tail = 0;
        pair *data = new pair[1000003];

        MaxQueue(){
        }
        void push(ll date, ll people) {
            //队尾元素如果小于x就要淘汰掉
            while(tail > head && data[tail - 1].second < people) {
                tail--;
            }
            data[tail].first = date;
            data[tail].second = people;
            tail++;
        }
        void pop() {
            if(tail > head) {
                head++;
            }
        }
        pair top() {
            return data[head];
        }
        bool empty() const { return tail <= head; }

};

int main() {
    scanf("%lld", &n);
    MaxQueue maxqueue;
    ll *people_num = new ll[n + 1];
    //people_num[i]表示第i天确诊的人数
    for(ll i = 0; i < n; i++) {
        ll num;
        scanf("%lld", &num);
        people_num[i] = num;
    }
    //储存每一天的追溯天数
    ll *date_num = new ll[n + 1];
    for(int i = 0; i < n; i++) {
        ll num;
        scanf("%lld", &num);
        date_num[i] = num;
    }
    //储存范围
    ll t;
    scanf("%lld", &t);
    ll *p = new ll[t + 1];
    ll *q = new ll[t + 1];
    for(ll i = 0; i < t; i++) {
        ll a, b;
        scanf("%lld %lld", &a, &b);
        p[i] = a;
        q[i] = b;
    }
    //每一天的追溯范围内的最大感染人数
    ll *max_num = new ll[n + 1];
    //单独处理第0天
    max_num[0] = 0;
    ll max_v = 0;
    //第i天应该让第i-1天的感染人数入队
    for(ll i = 1; i < n; i++) {
        maxqueue.push(i - 1, people_num[i - 1]);
        //如果超出了追溯范围，则出队
        ll left = (ll)i - date_num[i] < 0 ? 0 : (ll)i - date_num[i];
        while(!maxqueue.empty() && maxqueue.top().first < left ) {
            maxqueue.pop();
        }
        //此时的top就是追溯范围内最大感染人数
        ll k;
        if(maxqueue.empty()) {
            k = 0;
        }
        else {
            k = maxqueue.top().second;
        }
        max_num[i] = k;
        if(k > max_v) {
            max_v = k;
        }
    }
    //统计每个感染人数出现了多少天
    //然后求前缀和
    ll *cnt = new ll[max_v + 2];
    for(ll i = 0; i <= max_v + 1; i++) {
        cnt[i] = 0;
    }
    for(ll i = 0; i < n; i++) {
        cnt[max_num[i]]++;
    }
    ll *prefix = new ll[max_v + 2];
    //prefix[i]代表小于i人的天数
    prefix[0] = 0;
    for(ll i = 1; i <= max_v + 1; i++) {
        prefix[i] = prefix[i - 1] + cnt[i - 1];
    }
    for(ll i = 0; i < t; i++) {
        if(p[i] > max_v) {
            printf("%lld %lld\n", n, 0);
            continue;
        }
        else if(q[i] >max_v) {
            printf("%lld %lld\n", prefix[p[i]], prefix[max_v + 1] - prefix[p[i]]);
            continue;
        }
        printf("%lld %lld\n", prefix[p[i]], prefix[q[i]] - prefix[p[i]]);
    }
    //o(n)求每天最大值，o(n)求解前缀和，o(t)求解每一次查询，整体时间复杂度o(n + t)
}