#pragma once //防止头文件重复包含
#include <iostream>

class swap
{
public:
    //为私有变量赋值的构造函数
    swap(int a, int b)
    {
        this->_a = a;   //this指针，指示_a是类中的属性变量；
        this->_b = b;   //这样即使类外有重名的_a变量也不会搞混淆
    }
    //创建两个接口函数
    void run();       //交换两个数
    void printInfo(); //打印两个数
private:
    //定义两个私有变量
    int _a;
    int _b;
};