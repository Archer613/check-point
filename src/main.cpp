#include "stdio.h"
#include "States_Gen_Tree.h"
#include <memory>
#include "nodes.h"
#include "tl_tree.h"



int main(int argc, char** argv)
{
    // Get all legal state_trees
    States_Gen_Tree states_gen;
    states_gen.init_tree();
    printf("Total tree = [%ld]\n", states_gen.n);
    
    // Only ideal cases are considered, no request nesting, all block is clean
    // TODO: support TL-UL
    tl_tree model;
    model.init();
    model.run( AcquireBlock, NtoT, states_gen.state_trees[0], ID_CORE0_DCACHE);


    return 0;
}
