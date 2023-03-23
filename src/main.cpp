#include "stdio.h"
#include "../include/tltree.h"
#include <memory>



int main(int argc, char** argv)
{
    // Get all legal state_trees
    Tree::TLTree tltree;
    tltree.init_tree();
    printf("Total tree = [%ld]\n", tltree.n);
    
    // Only ideal cases are considered, no request nesting, all block is clean

    
    return 0;
}
 