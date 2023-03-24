#ifndef CHECK_POINT_TLTREE_H
#define CHECK_POINT_TLTREE_H

#include "const.h"
#include "NodeBase.h"
#include "Tool.h"
#include "string.h"
#include "rule.h"



class States_Gen_Tree{
public:
    States_Gen_Tree() = default;

    enum{
        TREE_NUM = 224,
    };
    long int n = 0;// tree_num;
    int state_trees[TREE_NUM][ID_CACHE_NUM];
    
    void init_tree(void);
    void print(int *states);
private:
    TLTreeNodeBase Nodes[ID_CACHE_NUM];
    rule rules;
    void foreach(int cache_id);
    bool check(void);
    
};


#endif