// 二叉树节点模板类
// 简化版，只实现基本的节点结构和连接操作
template <typename T>
class binNode {
    public:
    // 节点数据，指向父亲，右孩子，左孩子的指针
    T data;
    binNode *parent, *lc, *rc;

    // 构造函数
    binNode(T _data) : data(_data), parent(nullptr), lc(nullptr), rc(nullptr) {}
    // 插入左儿子
    binNode* insertLC(const T& _data) {
        binNode* lc = new binNode(_data);
        lc->parent = this;
        this->lc = lc;
        return lc;
    }
    // 插入右儿子
    binNode* insertRC(const T& _data) {
        binNode* rc = new binNode(_data);
        rc->parent = this;
        this->rc = rc;
        return rc;
    }
    // 接入左子树（或是节点）
    void attachLC(binNode* _lc) {
        if(_lc == nullptr) {
            this->lc = nullptr;
            return;
        }
        if(this->lc != nullptr) {
            this->lc->parent = nullptr;
        }
        _lc->parent = this;
        this->lc = _lc;
    }
    // 接入右子树（或是节点）
    void attachRC(binNode* _rc) {
        if(_rc == nullptr) {
            this->rc = nullptr;
            return;
        }
        if(this->rc != nullptr) {
            this->rc->parent = nullptr;
        }
        _rc->parent = this;
        this->rc = _rc;
    }
};

// 简单的二叉树
template <typename T>
class binTree {
    public:
    // 根节点指针
    binNode<T>* root;
    // 构造函数
    binTree() : root(nullptr) {}

    // 插入根节点
    void insertRoot(const T& _data) {
        root = new binNode(_data);
    }
    // 插入左儿子，e作为x的左儿子
    binNode<T>* insertLC(binNode<T>* x, const T& _data) {
        // 直接调用节点的插入函数
        return x->insertLC(_data);
    }
    // 插入右儿子，e作为x的右儿子
    binNode<T>* insertRC(binNode<T>* x, const T& _data) {
        // 直接调用节点的插入函数
        return x->insertRC(_data);
    }

    // 接入左子树
    void attachLC(binNode<T>* x, binNode<T>* _lc) {
        x->attachLC(_lc);
    }

    // 接入右子树
    void attachRC(binNode<T>* x, binNode<T>* _rc) {
        x->attachRC(_rc);
    }

};

// 实现一个简单的伸展树
template <typename T>
class splayTree :public binTree<T>{
    public:
    // _x, _p, _r 分别表示搜索到的节点，父亲节点，替换节点
    // 其实是为了BST的结构
    binNode<T> *_x, *_p, *_r;

    // 构造函数
    splayTree() : binTree<T>() {}
    
    // 伸展
    // 将节点x伸展到根
    // 需要同时考虑父亲和祖父节点
void splay(binNode<T>* v) {
    if(!v) {
        return;
    }
    binNode<T>* p = v->parent;
    binNode<T>* g = nullptr;
    if(p != nullptr) {
        g = p->parent;
    }
    // 只要父亲不为空就要一直伸展，直到到达根节点
    while(p) {
        // 如果g是非空的，进行双旋转
        if(g) {
            // 将v和gg相连
            binNode<T>* gg = g->parent;
            if(gg != nullptr) {
                if(g == gg->lc) {
                    gg->attachLC(v);
                }
                else {
                    gg->attachRC(v);
                }
            }
            else {
                // 如果gg为空，伸展完后v会成为根节点
                v->parent = nullptr;
            }
            // p是g的左儿子
            if(p == g->lc) {
                // v是p的左儿子
                // 对应zig-zig的情况
                if(v == p->lc) {
                    g->attachLC(p->rc);
                    p->attachRC(g);
                    p->attachLC(v->rc);
                    v->attachRC(p);
                }
                // 对应zag-zig
                else {
                    g->attachLC(v->rc);
                    p->attachRC(v->lc);
                    v->attachRC(g);
                    v->attachLC(p);
                }
            }
            // p是g的右儿子
            else {
                // v是p的右儿子
                //对应zag-zag
                if(v == p->rc) {
                    g->attachRC(p->lc);
                    p->attachLC(g);
                    p->attachRC(v->lc);
                    v->attachRC(p);
                }
                // 对应zig-zag
                else {
                    g->attachRC(v->lc);
                    p->attachLC(v->rc);
                    v->attachRC(g);
                    v->attachLC(p);
                }
            }

        }
        // 否则只用进行单旋转
        else {
            if(v == p->lc) {
                p->attachLC(v->rc);
                v->attachRC(p);
            }
            else {
                p->attachRC(v->lc);
                v->attachLC(p);
            }
            v->parent = nullptr;
        }

        // 结束一轮伸展，更新父亲和祖父
        p = v->parent;
        if(p != nullptr) {
            g = p->parent;
        }
    }
    // 循环结束，v成为根节点
    this->root = v;

}

// 依照关键码搜索节点
// 补充BST的搜索函数
binNode<T>* BSTsearch(const T& _data) {
    // 空树情况特殊处理
    if(root == nullptr) {
        return nullptr;
    }
    // _x为当前节点，_p为父亲节点
    _x = root;
    _p = nullptr;
    // 只要_x不是空节点就一直搜索
    // 左子树不大于根节点，右子树不小于根节点
    while(_x != nullptr) {
        if(_data < _x->data) {
            _p = _x;
            _x = _x->lc;
        }
        else if(_data > _x->data) {
            _p = _x;
            _x = _x->rc;
        }
        // 搜索成功
        else {
            return _x;
        }
    }
    // 如果搜索失败，返回最后一个访问的节点
    // 注意失败也需要伸展
    return _p;
}

binNode<T>* search(const T& _data) {
    // 调用BST的搜索函数
    binNode<T>* v = BSTsearch(_data);
    // 如果搜索成功，伸展到根
    if(v != nullptr) {
        splay(v);
    }
    return v;
}

// 插入一个新的节点
binNode<T>* insert(const T& _data) {
    // 空树直接插入
    if(root == nullptr) {
        root = new binNode<T>(_data);
        return root;
    }
    // 进行查找，判断是否已存在
    binNode<T>* t = search(_data);
    // 如果已存在，直接返回根节点
    if(_data == t->data) {
        return root;
    }
    // 如果不存在
    // 此时t已经处于了根节点，只需要考虑如何将v变成根节点即可
    root = new binNode<T>(_data);
    if(t->data < _data) {
        root->attachRC(t->rc);
        root->attachLC(t);
        t->rc = nullptr;
    }
    else {
        root->attachRC(t);
        root->attachLC(t->lc);
        t->lc = nullptr;
    }
    return root;
}

bool remove(const T& _data) {
    // 进行搜索
    binNode<T>* t = search(_data);
    // 如果空树或是没有指定节点，删除失败
    if(t == nullptr || t->data != _data) {
        return false;
    }
    // 找到了指定节点
    // 如果没有左子树或是右子树，则可以直接删除，将根节点位置传递给孩子
    if(t->lc == nullptr) {
        root = t->rc;
        if(root != nullptr) {
            root->parent = nullptr;
        }
        delete t;
    }
    else if(t->rc == nullptr) {
        root = t->lc;
        if(root != nullptr) {
            root->parent = nullptr;
        }
        delete t;
    }
    //否则要找到继承人
    else {
        // 将节点删除
        binNode<T>* l = t->lc;
        binNode<T>* r = t->rc;
        delete t;
        // 接下来在右子树中进行一次搜索，找到新的根
        root = r;
        root = search(r->data);
        // 然后将左子树链接
        root->attachLC(l);
    }   
    return true;
    
}

 
};


int main() {

    
    return 0;
}