#ifndef CHECK_POINT_TLTREE_H
#define CHECK_POINT_TLTREE_H

#include "const.h"
#include "common.h"
#include "Tool.h"
#include "string.h"
#include "rule.h"
#include <set>

namespace Tree{

    /*
        children
            |
        self
            |
        parent
    */

    // Include conntion and state
    class TLTreeNodeBase{
    public:
        std::set<int> children_id;
        NodeMes self;
        int parent_id;

        TLTreeNodeBase() =default;
        void init(int p_id, std::set<int> c_id, NodeMes s){
            this->children_id = c_id;
            this->parent_id = p_id;
            this->self = s;
            this->self.state = INVALID;// init all state INVALID
        }

        int get_state(void){
            return self.state;
        }
    };


    class TLTreeNode_L1: public TLTreeNodeBase{
    private:

    public:

    };
    
    


    class TLTree{
    public:
        TLTree() = default;

        enum{
            TREE_NUM = 224,
        };
        long int n = 0;// tree_num;
        
        void init_tree(void);
        void print(int *states);
    private:
        TLTreeNodeBase Nodes[ID_CACHE_NUM];
        rule rules;
        void foreach(int cache_id);
        bool check(void);
        int state_trees[TREE_NUM][ID_CACHE_NUM];
    };

}

#endif