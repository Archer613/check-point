#include "stdio.h"
#include "../include/tltree.h"
#include <memory>



int main(int argc, char** argv)
{
    Tree::TLTree tltree;
    tltree.init_tree();
    tltree.print();
    
    tltree.foreach(ID_L3);
    printf("Total tree = [%ld]\n", tltree.n);
    

    return 0;
}
 