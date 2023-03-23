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

    class TLTreeNode{
    private:
        
    public:
        std::set<int> children_id;
        NodeMes self;
        int parent_id;

        TLTreeNode() =default;
        void init(int p_id, std::set<int> c_id, NodeMes s){
            this->children_id = c_id;
            this->parent_id = p_id;
            this->self = s;
            this->self.state = INVALID;// init all state INBALID
        }

        int get_state(void){
            return self.state;
        }
    };


    class TLTree{
    private:
        rule rules;
    public:
        TLTreeNode Nodes[ID_CACHE_NUM];
        long int n = 0;

        TLTree() = default;

        void init_tree(void);
        void print(void);
        void foreach(int cache_id);
        bool check(void);
    };

}

#endif