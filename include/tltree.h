#ifndef CHECK_POINT_TLTREE_H
#define CHECK_POINT_TLTREE_H

#include "const.h"

class NodeMes{
public:
    int level;
    int state;
    int id;
};

class TLTreeNodeBase{
private:
    NodeMes parents[];
    NodeMes self;
    NodeMes children[];
public:
    
    
};


#endif