#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

struct Point {
    long long x, y;
    int id;
    int chain; // 0: 下链, 1: 上链
};

struct Triangle {
    Point p1, p2, p3;
};

// 叉积
long long cross(const Point& o, const Point& a, const Point& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int main() {
    int n;
    cin >> n;
    
    vector<Point> polygon(n);
    for (int i = 0; i < n; i++) {
        cin >> polygon[i].x >> polygon[i].y;
        polygon[i].id = i;
    }
    
    // 找到最左和最右的点
    int leftmost = 0, rightmost = 0;
    for (int i = 1; i < n; i++) {
        if (polygon[i].x < polygon[leftmost].x) leftmost = i;
        if (polygon[i].x > polygon[rightmost].x) rightmost = i;
    }
    
    // 构建两条链
    vector<Point> upperChain, lowerChain;
    
    // 从leftmost到rightmost（逆时针方向）
    int idx = leftmost;
    upperChain.push_back(polygon[idx]);
    while (idx != rightmost) {
        idx = (idx + 1) % n;
        upperChain.push_back(polygon[idx]);
    }
    
    // 从rightmost到leftmost（继续逆时针）
    lowerChain.push_back(polygon[idx]);
    while (idx != leftmost) {
        idx = (idx + 1) % n;
        lowerChain.push_back(polygon[idx]);
    }
    
    // 判断哪个是上链（通过第二个点的y坐标）
    bool upperIsTop = (upperChain.size() > 1 && upperChain[1].y > upperChain[0].y);
    if (!upperIsTop) {
        swap(upperChain, lowerChain);
    }
    
    // 标记链并合并
    vector<Point> vertices;
    for (auto& p : upperChain) {
        p.chain = 1;
        vertices.push_back(p);
    }
    for (int i = 1; i < lowerChain.size(); i++) {
        lowerChain[i].chain = 0;
        vertices.push_back(lowerChain[i]);
    }
    
    // 按x坐标排序
    sort(vertices.begin(), vertices.end(), 
         [](const Point& a, const Point& b) { return a.x < b.x; });
    
    // 扫描线算法
    vector<Triangle> triangles;
    stack<int> S; // 存储索引
    S.push(0);
    S.push(1);
    
    for (int i = 2; i < n; i++) {
        Point& vi = vertices[i];
        
        if (i == n - 1) {
            // 最后一个顶点，连接所有剩余的
            // 最后一个顶点既可以属于上链，又可以属于下链，所以不能统一处理
            int prev = S.top();
            S.pop();
            while (!S.empty()) {
                int curr = S.top();
                S.pop();
                triangles.push_back({vertices[curr], vertices[prev], vi});
                prev = curr;
            }
        } else {
            Point& top = vertices[S.top()];
            
            if (vi.chain != top.chain) {
                // 情况1：不同链
                vector<int> temp;
                while (!S.empty()) {
                    temp.push_back(S.top());
                    S.pop();
                }
                
                for (int j = 0; j < (int)temp.size() - 1; j++) {
                    triangles.push_back({vertices[temp[j]], vertices[temp[j+1]], vi});
                }
                
                S.push(i - 1);
                S.push(i);
            } else {
                // 情况2：同一链
                int last = S.top();
                S.pop();
                
                while (!S.empty()) {
                    int second = S.top();
                    Point& p_second = vertices[second];
                    Point& p_last = vertices[last];
                    
                    // 判断能否连接
                    long long cr = cross(p_second, p_last, vi);
                    
                    // 对于上链，叉积应该<0（顺时针转向）
                    // 对于下链，叉积应该>0（逆时针转向）
                    bool canConnect = (vi.chain == 1) ? (cr < 0) : (cr > 0);
                    
                    if (canConnect) {
                        triangles.push_back({p_second, p_last, vi});
                        last = second;
                        S.pop();
                    } else {
                        break;
                    }
                }
                
                S.push(last);
                S.push(i);
            }
        }
    }
    
    // 输出
    for (const auto& t : triangles) {
        cout << t.p1.x << " " << t.p1.y << " "
             << t.p2.x << " " << t.p2.y << " "
             << t.p3.x << " " << t.p3.y << "\n";
    }
    
    return 0;
}