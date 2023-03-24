#ifndef CHECK_POINT_RULE_H
#define CHECK_POINT_RULE_H

#include "const.h"
#include "NodeBase.h"
#include "Tool.h"

class rule{
private:
    enum{
        HAVE = true,
        NOT_HAVE = false,
    };
public:
    bool invalid_not_have_valid_child_r(int up, int *child, int n_child);


    // child and parent
    bool trunk_not_have_branch_child_r(int up, int *child, int n_child);
    bool branch_not_have_t_child_r(int up, int *child, int n_child);

    // fork
    bool trunk_have_t_fork_r(int up, int *fork_max_state, int n_fork);
    bool one_fork_tip_and_other_invalid(int n_fork, int *fork_max_state);

    // leaf
    bool leaf_is_not_trunk(int self);  
    // global
    bool no_more_than_one_tip(int state[ID_CACHE_NUM]);
};

#endif