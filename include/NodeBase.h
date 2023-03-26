#ifndef CHECK_POINT_NODEBASE_H
#define CHECK_POINT_NODEBASE_H

#include <set>
#include "const.h"
#include "CacheUpdateCommon.h"

class NodeMes{
public:
    int state;
    int id;

    NodeMes() = default;
    ~NodeMes() = default;

};

class TLMes{
public:
    // TL-link
    int opcode;
    int param;
    int perfercache;
    int valid;
    // input/output node id
    int id;
    // source
    int src_id;

    bool operator<(const TLMes& value) const{
        return id < value.id;
    }
};


/*
    children
        |
    self
        |
    parent
*/
// Include conntion and states
class TLTreeNodeBase{
public:
    NodeMes self;
    int states[ID_CACHE_NUM];
    std::set<int> children_id;
    int parent_id;

    TLTreeNodeBase() = default;
    void init(int p_id, std::set<int> c_id, NodeMes s){
        this->children_id = c_id;
        this->parent_id = p_id;
        this->self = s;
        this->self.state = INVALID;// init all state INVALID
    }
};


#endif