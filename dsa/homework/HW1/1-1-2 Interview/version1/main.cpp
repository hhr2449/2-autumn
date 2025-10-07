#include <cstdio>
#include <cstdlib>
//双向链表节点
//规定顺时针为next，逆时针为prev
struct Node {
    int val;
    Node *prev;
    Node *next;
    Node(int val) : val(val), prev(NULL), next(NULL) {}
};
//在指定节点的前面插入一个节点，并且返回新的节点
Node* insert_prev(Node *node, int val) {
    Node *new_node = new Node(val);
    new_node->next = node;
    new_node->prev = node->prev;
    node->prev->next = new_node;
    node->prev = new_node;
    return new_node;
}
int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int val;
    Node *head;
    Node *p;
    //i即为当前人数
    for(int i = 0; i < n; i++) {
        scanf("%d", &val);
        if(i == 0) {
            head = new Node(val);
            head->prev = head;
            head->next = head;
            continue;
        }
        //前进个数
        //当移动多圈的时候可以取模
        int cnt_move = m;
        if(m < i) {
            cnt_move = m % i;
        }
        //注意当前节点就是移动的第一个
        //逆时针移动
        p = head;
        for(int j = 1; j < cnt_move; j++) {
            p = p->prev;
        }
        //注意，每次插入新的节点要更新head，下一次移动插入就是从head开始
        head = insert_prev(p, val);
    }
    p = head;
    for(int i = 0; i < n; i++) {
        printf("%d", p->val);
        if(i != n - 1) {
            printf(" ");
        }
        p = p->next;
    }
}