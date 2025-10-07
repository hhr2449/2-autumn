#include "Vector.h"
#include <iostream>
#include <sstream>
int main()
{
    Vector<int> myVector; // 保持你的类名 Vector
    int N;
    std::cin >> N;
    getchar(); // 吸收 N 后的换行符，避免影响后续 getline 读取

    std::string line;
    for (int i = 0; i < N; ++i)
    {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        // 1. push 命令：push [element]，无输出
        if (command == "push")
        {
            int element;
            if (iss >> element) // 确保输入有效元素
            {
                myVector.push_back(element);
            }
        }
        // 2. size 命令：输出当前元素个数，独占一行
        else if (command == "size")
        {
            std::cout << myVector.getSize() << std::endl;
        }
        // 3. get 命令：get [index]，无效索引输出 -1，有效输出元素
        else if (command == "get")
        {
            int index;
            if (iss >> index)
            {
                // 索引有效条件：index ≥0 且 index < 元素个数
                if (index < 0 || static_cast<size_t>(index) >= myVector.getSize())
                {
                    std::cout << -1 << std::endl;
                }
                else
                {
                    std::cout << myVector[static_cast<size_t>(index)] << std::endl;
                }
            }
            else // 无有效索引输入，视为无效
            {
                std::cout << -1 << std::endl;
            }
        }
        // 4. insert 命令：insert [index] [element]，无效索引忽略，无输出
        else if (command == "insert")
        {
            int index, element;
            if (iss >> index >> element)
            {
                // 插入索引有效条件：index ≥0 且 index ≤ 元素个数（可插末尾）
                if (index >= 0 && static_cast<size_t>(index) <= myVector.getSize())
                {
                    myVector.insert(static_cast<size_t>(index), element);
                }
            }
            // 缺参数或无效索引时忽略，不输出
        }
        // 5. pop 命令：删除末尾元素，空向量时忽略，无输出
        else if (command == "pop")
        {
            if (!myVector.empty())
            {
                myVector.pop_back();
            }
        }
        // 6. clear 命令：清空元素，无输出
        else if (command == "clear")
        {
            myVector.clear();
        }
        // 7. print 命令：空向量输出 empty，非空元素后跟空格，独占一行
        else if (command == "print")
        {
            if (myVector.empty())
            {
                std::cout << "empty" << std::endl;
            }
            else
            {
                for (size_t i = 0; i < myVector.getSize(); ++i)
                {
                    std::cout << myVector[i] << " ";
                }
                std::cout << std::endl;
            }
        }
        // 8. iterator 命令：通过 begin()/end() 遍历，格式同 print
        else if (command == "iterator")
        {
            if (myVector.empty())
            {
                std::cout << "empty" << std::endl;
            }
            else
            {
                for (auto it = myVector.begin(); it != myVector.end(); ++it)
                {
                    std::cout << *it << " ";
                }
                std::cout << std::endl;
            }
        }
        // 9. foreach 命令：范围 for 遍历，格式同 print
        else if (command == "foreach")
        {
            if (myVector.empty())
            {
                std::cout << "empty" << std::endl;
            }
            else
            {
                for (const auto& elem : myVector)
                {
                    std::cout << elem << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    return 0;
}