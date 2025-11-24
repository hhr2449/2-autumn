#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

//未分块的珠子序列，最大长度为2^22
char a[1 << 22];  // 不分块的珠子
//总长度
int alen;      // 珠子总长度

//分块时每一块的目标长度2^11(取根号)
const int plen_target = 1 << 11;  // 分块时每一块的目标长度
//最大长度为2^12
const int plen_bound = 1 << 12;   // 每一块的最大长度

//p是分块数组，共有2^12个块，每个块的长度为plen_bound（最大长度）
char p[1 << 12][plen_bound];  // 分块数组
//每一个块的实际长度
size_t plen[1 << 12];         // 每一块的长度
//块数
size_t pn;                    // 块数

struct Rank {
    int first, second;
};

// a to p：将 a 切分成 p
// 将未分块的原序列a分块为p
void a2p() {
    // 特殊情况，没有珠子，此时将第一个块长度赋值为0，退出
    if (alen == 0) {
        pn = 1;
        plen[0] = 0;
        return;
    }
    int i = 0, j = 0;
    // i用于索引块，j用于索引原本的数组a
    // 每一次将plen_target长度的数据从a中移动到p中对应的块
    // 如果不够，则全部移动完
    for (; j < alen; i++, j += plen_target) {
        int m = alen - j < plen_target ? alen - j : plen_target;
        //使用memcpy复制内存，目标地址为p[i],即为第i块，起始地址为&a[j]，长度为m
        memcpy(p[i], &a[j], m);
        plen[i] = m;
    }
    pn = i;
}

// p to a：将 p “组装”回 a
void p2a() {
    int copied = 0;
    // 将p放入a中
    // 类似的思路，将p的每个块复制到a中对应地址
    // 注意此时每次复制的长度是p[i]的长度plen[i]
    for (int i = 0; i < pn; i++) {
        memcpy(&a[copied], p[i], plen[i]);
        copied += plen[i];
    }
}

// 调试用：打印所有块，每块一行
void viewp() {
    for (int i = 0; i < pn; i++) {
        for (int j = 0; j < plen[i]; j++)
            putchar(p[i][j]);
        putchar('\n');
    }
}

// 找到对应rank的元素
// 返回其所处的块和块中的索引
// 查询时每次走过一个块的长度，复杂度为O(sqrt(n))（最差情况也只是走过所有的块，在走完对应的块，小于块数加上块大小）
// 优于链表，劣于向量
Rank find(int rank) {
    int group = 0;
    // 已知每个块的长度，可以从头开始进行遍历，直到rank剩余长度小于当前的块的长度
    while (group < pn - 1 && rank >= plen[group]) {
        rank -= plen[group];
        group++;
    }
    return {group, rank};
}

// 获取对应元素
// Rank类对象中描述了元素所处的块和块中的索引，可以直接获取
inline char &get(Rank pos) {
    return p[pos.first][pos.second];
}

// 这里的思路是，如果块过长了，则将整个分块数组放回a中，然后重新组合
// 在 p 上计算珠子的插入和消除
void play(int rank, char ch) {
    Rank pos = find(rank);
    char *cur = &get(pos);
    int succ_len = plen[pos.first] - pos.second;
    if (succ_len > 0) {
        memmove(cur + 1, cur, succ_len);
    }
    *cur = ch;
    alen++;
    plen[pos.first]++;

	// 块过长，重组
    if (plen[pos.first] >= plen_bound) {
        p2a();
        a2p();
        pos = find(rank);
    }

	// 计算需要消除的开区间 (l, r)
    Rank l = pos, r = pos;
    Rank lbound, rbound;
	int dis = 0;
    int eliminated = 0;
    while (1) {
        while (l.first >= 0 && get(l) == ch) {
            l.second--;
            dis++;
            while (l.second < 0 && l.first >= 0) {
                l.first--;
                if (l.first >= 0)
                    l.second += plen[l.first];
            }
        }
        while (r.first < pn && get(r) == ch) {
            r.second++;
            dis++;
            while (r.second >= plen[r.first] && r.first < pn) {
                r.second -= plen[r.first];
                r.first++;
            }
        }
        if (dis > 3) {
            eliminated += dis - 1;
            lbound = l;
            rbound = r;
            if (l.first >= 0) {
                ch = get(l);
                dis = 1;
            } else {
                break;
            }
        } else {
            break;
        }
    }

	// 执行消除
    if (eliminated > 0) {
        alen -= eliminated;
        l = lbound;
        r = rbound;
        cout << l.first << " " << r.first << " " << eliminated << endl;
        if (l.first == r.first) {
            int len = plen[r.first] - r.second;
            if (len > 0) {
                memmove(&p[l.first][l.second + 1], &p[r.first][r.second], len);
            }
            plen[l.first] -= eliminated;
        } else {
            if (l.first >= 0) {
                plen[l.first] = l.second + 1;
            }
            if (r.first < pn) {
                int len = plen[r.first] - r.second;
                if (len > 0) {
                    memmove(&p[r.first][0], &p[r.first][r.second], len);
                }
                plen[r.first] = len;
            }
            //!------------------------------------------------
            //for循环少了个1
            //本来的作用是删除中间的块，比如左边界在第一块，右边界在第3块，那么第二块直接删除
            //这里会直接将左边界所在的块删掉
            for (int i = l.first; i < r.first; i++)
                plen[i] = 0;
            //!------------------------------------------------
        }
    }
}

int main() {
    int n;
	fgets(a, sizeof(a), stdin);
    alen = strlen(a);
	if (alen > 0 && a[alen - 1] == '\n')
		a[--alen] = '\0';
    while (alen > 0 && (a[alen - 1] == '\n' || a[alen - 1] == '\r'))
        alen--;
    a2p();
	scanf("%d", &n);
	while (n--) {
		char ch;
		int rank;
		scanf("%d %c", &rank, &ch);
        play(rank, ch);
	}
    p2a();
    a[alen] = '\0';
    puts(a);
	return 0;
}
