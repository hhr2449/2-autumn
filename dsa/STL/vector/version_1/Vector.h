//简单版本的vector
//使用模板来适配不同的类型数据
//成员变量：T* elements用来储存数据，在堆上进行内存分配；size_t size当前数据个数；size_t capacity当前的容量
//成员函数：
//构造函数：1.默认构造函数 2.拷贝构造函数
//析构函数
//赋值运算符重载
//访问：1.获取size 2.获取capacity 3.判断是否为空 4.通过[]重载来实现循秩访问
//      5.for-each循环访问：需要实现类中的begin()和end()函数（获取指向首尾的指针，这里是简化版，实际的iterator是一个类，但是里面的核心其实也是一个指针，只是实现了更多的操作）
//          （实际上for-each的底层就是通过这两个函数来获取首位迭代器，然后进行遍历的）
//增：1.push_back 2.按照指定位置插入
//删：1.pop_back 2.清空
//容量控制：实现resize

//知识点：模板类不能够分离声明和实现，必须统一写在.h文件中
//因为模板类的原理是：当使用到了某个类型的模板类对象时，编译器会根据模板参数的类型来生成对应的代码（比如说用到了vector<int>,则编译器会生成一套vector<int>的代码）
//c++使用的编译模式是：分别编译，最后链接，这样如果将模板类的实现放在.cpp文件中，在编译vector.cpp的时候由于不知道使用了那些
//模板参数所以无法编译，而main.cpp中虽然知道了模板参数和类声明，但是缺少定义，所以也没办法进行编译
//而直接全部写在.h文件中，然后可以将其引入main.cpp中，这样就可以直接全部在main.cpp中进行编译了

#include <stdio.h>
#include <iostream>
#include "copy.h"

template <typename T>
class Vector {
    private:
        T* elements;
        size_t size;
        size_t capacity;
    public:
        //普通构造
        Vector();
        //拷贝构造
        Vector(const Vector& v);
        //析构
        ~Vector();
        //赋值运算符重载
        Vector& operator=(const Vector& v);
        //循秩访问
        T& operator[](size_t index);
        //常量版本的循秩访问
        const T& operator[](size_t index) const;
        //获取
        size_t getSize();
        size_t getCapacity();
        //判空
        bool empty();
        //for-each循环的迭代器
        T* begin();
        T* end();
        //增
        void push_back(const T& e);
        void insert(size_t index, const T& e);
        //删
        void pop_back();
        void clear();
        void print();
    private:
        //容量控制
        void resize(size_t newCapacity);
};

//实现
//可以在头文件中分离声明和定义，但是在实现每个函数的时候都需要添加template<typename T>
template <typename T>
Vector<T>::Vector() {
    elements = nullptr;
    size = 0;
    capacity = 0;
}
//拷贝构造函数
template <typename T>
Vector<T>::Vector(const Vector& v) {
    //创建相同大小的空间
    elements = new T[v.getCapacity()];
    //使用copy函数
    copy(v.begin(), v.end(), this->begin());
    //capacity和size也要改变
    capacity = v.getCapacity();
    size = v.getSize();
}
//析构函数，将内存释放掉
template <typename T>
Vector<T>:: ~Vector() {
    delete[] elements;
}
//赋值运算符重载
//使用resize将容量大小改成和v一样大
//使用copy
//改变size
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& v) {
    if(&v == this) {
        return *this;
    }
    resize(v.getCapacity());
    copy(v.begin(), v.end(), this->begin());
    size = v.getSize();
    return *this;
}
//循秩访问
template <typename T>
T& Vector<T>::operator[](size_t index) {
    return elements[index];
}
template <typename T>
const T& Vector<T>::operator[](size_t index) const {
    return elements[index];
}
template <typename T>
size_t Vector<T>::getSize() {
    return size;
}
template <typename T>
size_t Vector<T>::getCapacity() {
    return capacity;
}
template <typename T>
bool Vector<T>::empty() {
    return size == 0;
}
//迭代器
template <typename T>
T* Vector<T>::begin() {
    return elements;
}
template <typename T>
T* Vector<T>::end() {
    return elements + size;
}
//增
template <typename T>
void Vector<T>::push_back(const T& e) {
    //不够空间则扩容
    if(size == capacity) {
        resize(capacity == 0 ? 1 : 2 * capacity);
    }
    elements[size++] = e;
}
template <typename T>
void Vector<T>::insert(size_t index, const T& e) {
    if(index > size) {
        throw "index out of range";
    }
    if(size == capacity) {
        resize(capacity == 0 ? 1 : 2 * capacity);
    }
    for(size_t i = size; i > index; i--) {
        elements[i] = elements[i - 1];
    }
    elements[index] = e;
    size++;
}
template <typename T>
void Vector<T>::pop_back() {
    if(size == 0) {
        throw "vector is empty";
    }
    size--;
}
template <typename T>
void Vector<T>::clear() {
    size = 0;
}
template <typename T>
void Vector<T>::resize(size_t newCapacity) {
    // 1. 容量不变时直接返回，避免无意义操作
    if (newCapacity == capacity) {
        return;
    }

    // 2. 分配新内存（无论是否为空向量，都先分配）
    T* newElements = new T[newCapacity];

    // 3. 拷贝有效数据（只有非空向量且有有效数据时才拷贝）
    size_t copyLen = 0;
    if (elements != nullptr && size > 0) {
        copyLen = (size < newCapacity) ? size : newCapacity; // 有效数据长度
        copy(elements, elements + copyLen, newElements); // 拷贝有效数据
    }

    // 4. 释放旧内存（空向量时elements是nullptr，delete[] nullptr是安全的！）
    delete[] elements;

    // 5. 更新指针、容量和大小（缩容时size不能超过新容量）
    elements = newElements;
    capacity = newCapacity;
    if (size > newCapacity) {
        size = newCapacity;
    }
}
template <typename T>
void Vector<T>::print() {
    for(size_t i = 0; i < size; i++) {
        std::cout << elements[i] << " ";
    }
    printf("\n");
}
