#include "../include/swap.h"

//实现两个数的交换
void swap::run( )
{
    int temp;
    temp=_a;
    _a=_b;
    _b=temp;
}
//实现两个数的值打印
void swap:: printInfo( )
{
    std::cout<<"_a= "<<_a<<std::endl;
     std::cout<<"_b= "<<_b<<std::endl;
} 