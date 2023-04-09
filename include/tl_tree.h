#ifndef CHECK_POINT_TL_TREE_H
#define CHECK_POINT_TL_TREE_H

#include "nodes.h"
#include "Common.h"

class tl_tree{
private:
    TLTreeNode_L1 node_l1[ID_L1_NUM];
    TLTreeNode_L2L3 node_l2[ID_L2_NUM];
    TLTreeNode_L2L3 node_l3;
    TLTreeNode_UL node_ul[ID_UL_NUM];
    int states_new[ID_CACHE_NUM];
    int self_dir[ID_CACHE_NUM][3];
    int client_dir[ID_CACHE_NUM][2];

public:
    tl_tree() = default;

    void init();
    void get_all_states();
    bool run(int op, int param, int *s, int id);
    void get_DIR_all_states(int *s);

    long int num;
};

#endif