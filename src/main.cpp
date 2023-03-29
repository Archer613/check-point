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
    // for (int n = 0; n < states_gen.n; n++)
    // {
    //     // acquirePerm
    //     for (int param = NtoT; param <= BtoT; param++)
    //     {
    //         model.run( AcquirePerm, param, states_gen.state_trees[n], ID_CORE0_DCACHE);
    //         model.run( AcquirePerm, param, states_gen.state_trees[n], ID_CORE1_DCACHE);
    //     }  
        
    //     // acquireBlock
    //     model.run( AcquireBlock, NtoB, states_gen.state_trees[n], ID_CORE0_ICACHE);
    //     model.run( AcquireBlock, NtoB, states_gen.state_trees[n], ID_CORE1_ICACHE);
    //     for (int param = NtoB; param <= BtoT; param++)
    //     {
    //         model.run( AcquireBlock, param, states_gen.state_trees[n], ID_CORE0_DCACHE);
    //         model.run( AcquireBlock, param, states_gen.state_trees[n], ID_CORE1_DCACHE);
    //     }   

    //     // releaseData
    //     for (int param = TtoN; param <= BtoN; param++)
    //     {
    //         for (int cache = 0; cache < ID_L1_NUM; cache++)
    //         {
    //              model.run( ReleaseData, param, states_gen.state_trees[n], cache);
    //         }
    //     }
    // }
    // printf("Total legal trans = [%ld]\n", model.num);

    // TODO: Test TL_UL Probe
    // model.run(PutFullData, 0, states_gen.state_trees[1883], ID_DMA);
    


    return 0;
}
