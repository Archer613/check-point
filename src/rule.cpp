#include "../include/rule.h"

// General Methods
bool Methods(int up, int *child, int n_child, int up_S, int child_S1, int child_S2, bool have){
    if(n_child == 0){
        return true;
    }

    bool flag = true;
    if(up == up_S){
        for (int i = 0; i < n_child; i++)
        {
            if(child[i] == child_S1 || child[i] == child_S2){
                flag = have;
                return flag;
            }else{
                flag = !have;
            }
        }
    }

    return flag;
}

bool rule::invalid_not_have_valid_child_r(int up, int *child, int n_child){
    return Methods(up, child, n_child, INVALID, INVALID, INVALID, HAVE);
}

bool rule::trunk_not_have_branch_child_r(int up, int *child, int n_child){
    return Methods(up, child, n_child, TRUNK, BRANCH, BRANCH, NOT_HAVE);
}

bool rule::branch_not_have_t_child_r(int up, int *child, int n_child){
    return Methods(up, child, n_child, BRANCH, TIP, TRUNK, NOT_HAVE);
}

bool rule::trunk_have_t_fork_r(int up, int *fork_max_state, int n_fork){
    return Methods(up, fork_max_state, n_fork, TRUNK, TIP, TIP, HAVE);
}

bool rule::one_fork_tip_and_other_invalid(int n_fork, int *fork_max_state){
    for (int i = 0; i < n_fork; i++)
    {
        if(fork_max_state[i] == TIP){
            for (int j = 0; j < n_fork; j++)
            {
                if(i != j){
                    if(fork_max_state[j] != INVALID){
                        return false;
                    }
                }
            }            
        }
    }

    return true;
}

bool rule::leaf_is_not_trunk(int self){
    return self != TRUNK;
}

bool rule::no_more_than_one_tip(int state[ID_CACHE_NUM]){
    int n = 0;
    for (int i = 0; i < ID_CACHE_NUM; i++)
    {
        if (state[i] == TIP)
            n++;
    }
    if(n <= 1){
        return true;
    }else{
        return false;
    }
}