#include <stack>
#include <cstdio>
#include <algorithm>
#include <vector>
#define ll long long
using namespace std;

struct Point {
    ll x, y;
    //0为上链，1为下链
    int chain = 0;
    
};
struct triangle {
    Point a, b, c;
    triangle(Point a, Point b, Point c):a(a),b(b),c(c) {}
};
bool operator<(const Point &a, const Point &b) {
    return a.x < b.x;
}

//计算叉积
//中心节点是o
ll cross(const Point& o, const Point& a, const Point& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int main() {
    int n;
    scanf("%d", &n);
    //数组存点
    vector<Point> points(n);
    //最左端点和最右端点的索引
    ll leftMost = 0, rightMost = 0;
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &points[i].x, &points[i].y);
        if(points[i].x < points[leftMost].x) {
            leftMost = i;
        }
        if(points[i].x > points[rightMost].x) {
            rightMost = i;
        }
    }
    //对每个点的chain属性进行标注
    //数据是逆时针输入的，所以如果左端点索引在右端点索引的左边
    //则左端点到右端点之间是下链
    if(leftMost < rightMost) {
        for(int i = leftMost; i <= rightMost; i++) {
            points[i].chain = 1;
        }
        //chain默认为0，不用单独处理上链。
    }
    else {
        //否则右端点以右，左端点以左应该是下链
        for(int i = leftMost; i < n; i++) {
            points[i].chain = 1;
        }
        for(int i = 0; i <= rightMost; i++) {
            points[i].chain = 1;
        }
    }
    //标注完成后对点进行排序，排序后的顺序就是扫描到的顺序
    sort(points.begin(), points.end());

    //扫描过程
    //第一种情况：当前点和栈中点不是同一个链
    //设s中点为p_0,p_1……p_k，当前点为vi，那么构成的三角形是(p_0,p_1,vi),(p_1,p_2,vi),……,(p_k-1,p_k,vi)
    //然后让vi和上一个节点入栈
    //第二种情况：当前点和栈中点是同一个链
    //判断p_i,p_i-1,vi构成的以p_i为顶点的角的大小
    //a.如果是劣的，则(p_i-1, p_i, vi)构成一个三角形，p_i出栈，p_i-1保留在栈中，继续循环，直到进入b或是栈空
    //b.如果是优的，则直接vi入栈
    //特殊点：最后一个点，他既可以属于上链，也可以属于下链，所以不能用之前的情况处理，
    //单独处理，其和栈中节点构成三角形

    //三角形
    vector<triangle> triangles;
    //存节点的索引而不是直接存节点
    stack<int> s;
    //第一第二个节点先入栈
    s.push(0);
    s.push(1);
    //然后依次扫描
    for(int i = 2; i < n; i++) {
        //最后一个节点特殊处理
        if(i == n - 1) {
            vector<int> tmp;
            //全部出栈
            while(!s.empty()) {
                tmp.push_back(s.top());
                s.pop();
            }
            //(p_0,p_1,vi),(p_1,p_2,vi),……,(p_k-1,p_k,vi)构成三角形
            for(int j = 0; j < tmp.size() - 1; j++) {
                triangles.push_back(triangle(points[tmp[j]], points[tmp[j + 1]], points[i]));
            }
        }
        else {
            //当前扫描到的节点
            Point vi = points[i];
            //如果当前节点和栈顶节点不是同一个链
            //栈中节点和当前节点构成三角形
            if(vi.chain != points[s.top()].chain) {

                vector<int> tmp;
                //取出栈中所有节点
                while(!s.empty()) {
                    tmp.push_back(s.top());
                    s.pop();
                }
                for(int j = 0; j < tmp.size() - 1; j++) {
                    triangles.push_back(triangle(points[tmp[j]], points[tmp[j + 1]], points[i]));
                }

                //当前节点和栈顶节点索引入栈
                s.push(i - 1);
                s.push(i);
            }
            //如果属于同链
            else {
                while(s.size() >= 2) {
                    //取出栈顶节点
                    ll p_i_idx = s.top();
                    Point p_i = points[s.top()];
                    s.pop();
                    //取出p_i-1
                    Point p_i_1 = points[s.top()];
                    //上面这部分出去了两个点，同时不改变栈
                    //计算叉积（以p_i_1为中心）
                    ll cross_val = cross(p_i_1, p_i, vi);
                    //判断是否是劣节点
                    bool is_good = false;
                    //叉积大于0（夹角小于派）且在上链
                    is_good = (cross_val > 0 && p_i.chain == 0) || (cross_val < 0 && p_i.chain == 1);
                    //优节点,结束循环
                    if(is_good) {
                        s.push(p_i_idx);
                        break;
                    }
                    else {
                        //否则得到三角形
                        triangles.push_back(triangle(p_i_1, p_i, vi));
                    }
                }
                //将当前节点入栈
                s.push(i);
            }
        }
    }

    //输出结果
    for(int i = 0; i < triangles.size(); i++) {
        printf("%lld %lld %lld %lld %lld %lld\n", triangles[i].a.x, triangles[i].a.y, triangles[i].b.x, triangles[i].b.y, triangles[i].c.x, triangles[i].c.y);
    }

    
}