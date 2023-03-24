#include "stdio.h"
#include "States_Gen_Tree.h"
#include <memory>
#include "nodes.h"



int main(int argc, char** argv)
{
    // Get all legal state_trees
    States_Gen_Tree states_gen;
    states_gen.init_tree();
    printf("Total tree = [%ld]\n", states_gen.n);
    
    // Only ideal cases are considered, no request nesting, all block is clean
    
    return 0;
}
