## 哈希策略的实现
### 坏的哈希
$$badhashing(s) = (\sum_{i = 0}^{|s| - 1}s_i*(i+1)) mod N$$
这种哈希策略有利用了字符串的每一位，改变某个字符，哈希值会发生改变
但是只是对字符做了线性递增权重的加权求和，权重增长过慢导致不同位置的区分度不足，不同的字符串容易出现不同位互相抵消导致映射到同一个值

### 好的哈希
使用多项式哈希
$$goodhashing(s) = (\sum_{i = 0}^{|s| - 1}s_i*p^{|s|-1})mod N$$
这里权重随位置指数级增长，高位和低位的贡献差别巨大。这样不同位置的字符几乎不可能产生彼此抵消的效应，使得分布的均匀性比较好

### 双向平方

第一次进入的时候记录初始位置，之后以此为中心进行平方试探

```cpp
if(first) {
first = false;
origin = last_choice;
}
```

成员变量len,dir分别表示长度（每次试探长度的平方）和方向，每次试探后都将方向进行翻转，如果上一次是向左试探，则将len加一

```
    int next = origin + dir * len * len;
    if(dir == -1) {
        len++;
    }
    dir *= -1;
```

### 溢出区策略

选用一个比整体表长要小的素数：350771作为实际存储区长度，剩下作为溢出区

维护cur作为溢出区的指针，初始化为溢出区的起始位置。每次返回指针指向的位置并且后移指针即可

```cpp
void Overflow_probe::init() {
    cur = real_size;
}
int Overflow_probe::operator()(hash_entry* Table, int table_size, int last_choice) {
    return cur++;
}
```





## 对框架的改变

insert函数：

使用变量size来储存大小，初始化为table_size

最前面，增加了一个对溢出区策略的判断和处理

```cpp
    bool insert(hash_entry entry){ 
        // 使用dynamic_cast，如果转换成功（父类指针指向的对象确是指定的类型），则返回转换后的指针，否则返回nullptr
        // 判断是否是溢出区策略，如果是的话则要更改table_size为实际的大小
        int size = table_size;
        if(dynamic_cast<Overflow_probe*>(my_collision)) {
            size = ((Overflow_probe*)my_collision)->real_size;
        }
        
        // 计算哈希值
       int last_choice = (*my_hashing)(entry.my_string,size);
       my_collision->init();
       // 如果计算出来的位置已经有了
       // 则反复使用冲突处理来计算下一个位置，直到不冲突为止
       while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
           last_choice = (*my_collision)(Table, size, last_choice);
       }
       // 存入
       Table[last_choice] = entry;
       return true;
    }
```

如果采用了溢出区策略，则动态转换会成功，此时进入if分支，将size修改为实际的存储数据区域的大小



为了方便脚本测试，修改main函数，添加了命令行参数，使其能够根据命令行参数来具体确定采用的策略

参数1代表哈希策略，参数2代表冲突策略

```cpp
int main(int argc, char* argv[]){
    int type;
    char buffer[1000];int data;
    // 1:坏哈希,2:好哈希
    // 1：线性探测，2：平方探测，3：溢出区
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    hashing_strategy* hashing;
    collision_strategy* collision;
    if(m == 1) {
        hashing = new bad_hashing();
    }
    else {
        hashing = new good_hashing();
    }
    if(n == 1) {
        collision = new linear_probe();
    }
    else if(n == 2) {
        collision = new Bid_square_probe();
    }
    else {
        collision = new Overflow_probe();
    }
    hashtable table(10000,hashing, collision);
```



## 进行测试

三组测试数据如下：

设插入次数为n,查询次数为m

`data1.in`：n = 1000, m = 1000,生成命令`./gen 1000 1000 1`

`data2.in`：n = 50000, m = 25000,生成命令`./gen 50000 25000 2`

`data3.in`：n = 100000, m = 100000，生成命令`./gen 100000 100000 3`

数据集之间的差别主要体现在数据量方面，构造三个数据集操作总数分别为2000, 75000, 200000

构造方法：先读取所有的数据，然后随机一个起始位置，从起始位置开始调用shuffle将n个元素进行随机打乱，然后构造n条插入指令，再进行一次打乱，构造m条查询指令

测试结果储存在result.txt中,result.md中绘制了表格



## 分析结果

1. 数据较小时，区别较小；数据较大时，好哈希明显更优

   数据较小的时候，无论采用哪种，都不容易发生冲突；数据较大的时候，坏哈希冲突的概率更高，排解冲突的时间更多

2. 使用好哈希时效率差距不明显；使用坏哈希的时候，双向平方试探明显更优

   双向平方试探的试探范围增长显著快于线性试探，在遇到冲突的区域的时候可以更快的离开。冲突较少的情况下，性能差距不明显；冲突较多的情况下（大数据坏哈希），双向平方明显更优

3. 大致上双向平方试探>公共溢出区>线性探测。在数据量较小且冲突较多时，使用公共溢出区可以减少试探，更优

4. 可能造成实际使用哈希函数的时候分布不均匀，冲突增多，效率降低

5. 计算负载因子，在负载因子达到某个阈值的时候将原有哈希表中的元素取出来，重新插入一个更大的哈希表中
