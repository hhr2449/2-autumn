//辅助函数copy
template <typename input_iterator, typename output_iterator>
//将[first, last)中的元素拷贝到另一个容器的[result, result + (last - first) + 1)中
//迭代器的意义：为遍历容器中的元素提供了一个统一的接口
//在vector这种连续容器中，可以直接使用for循环加上索引来访问，但是后面list这样不连续的容器，直接使用循环来访问就会比较复杂
//所以引入迭代器，无论什么数据结构，只用迭代器和循环即可遍历访问
void copy(input_iterator first, input_iterator last, output_iterator result) {
    for(; first != last; ++first, ++result) {
        *result = *first;
    }
}