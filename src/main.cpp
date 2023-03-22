#include "../include/swap.h"
#include "stdio.h"

int main(int argc, char** argv)
{
    swap myswap(10,100);
    myswap.printInfo();  //打印交换前数值
    myswap.run( );  //完成数据交换功能
    myswap.printInfo();

    return 0;
}
 