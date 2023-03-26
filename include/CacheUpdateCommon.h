#ifndef CHECK_POINT_CACHEUPDATECOMMON_H
#define CHECK_POINT_CACHEUPDATECOMMON_H

#include "const.h"
#include "Tool.h"

//-------------------------------Base Tool-------------------------//

static bool req_is_UL(int op){
    bool flag = false;
    switch(op){
        case PutFullData: flag = true; break;
        case PutPartialData: flag = true; break;
        case Get: flag = true; break;
        default: flag = false; break;
    }
    return flag;

}

/*
    TODO: Adaptive Architecture
    I$ - D$  I$ - D$  
       L2       L2
            L3
*/
// return : fork_num
static int get_fork_max_states(int *fork, int *states, int id, bool except_id){
    int fork_num = 0;
    // L1
    if (id == ID_CORE0_ICACHE || id == ID_CORE0_DCACHE || id == ID_CORE1_ICACHE || id == ID_CORE1_DCACHE){
        return fork_num;
    }
    // init fork for L2/L3
    fork_num = 2;
    fork[0] = INVALID;
    fork[1] = INVALID;
    // L2
    if (id == ID_CORE0_L2 || id == ID_CORE1_L2){
        if (id == ID_CORE0_L2){
            if(except_id != ID_CORE0_DCACHE)
                fork[0] = states[ID_CORE0_DCACHE];
            if(except_id != ID_CORE0_ICACHE)
                fork[1] = states[ID_CORE0_ICACHE];
            return fork_num;
        }else if (id == ID_CORE1_L2){
            if(except_id != ID_CORE1_DCACHE)
                fork[0] = states[ID_CORE1_DCACHE];
            if(except_id != ID_CORE1_ICACHE)
                fork[1] = states[ID_CORE1_ICACHE];
            return fork_num;
        }
    }
    // L3
    if (id == ID_L3){
        fork_num = 2;
        fork[0] = INVALID;
        fork[1] = INVALID;
        // core0
        if(except_id != ID_CORE0_L2){
            if (states[ID_CORE0_DCACHE] > fork[0]){
                fork[0] = states[ID_CORE0_DCACHE];
            }else if (states[ID_CORE0_ICACHE] > fork[0]){
                fork[0] = states[ID_CORE0_ICACHE];
            }else if (states[ID_CORE0_L2] > fork[0]){
                fork[0] = states[ID_CORE0_L2];
            }
        }
        // core1
        if(except_id != ID_CORE1_L2){
            if (states[ID_CORE1_DCACHE] > fork[1]){
                fork[1] = states[ID_CORE1_DCACHE];
            }else if (states[ID_CORE1_ICACHE] > fork[1]){
                fork[1] = states[ID_CORE1_ICACHE];
            }else if (states[ID_CORE1_L2] > fork[1]){
                fork[1] = states[ID_CORE1_L2];
            }
        }
        return fork_num;
    }

    Tool::cp_assert(false, "ilegal id");
    return -1;
}

static bool fork_max_states_is_invalid(int *states, int id, bool except_id){
    // get fork states
    bool fork_is_invalid = true;
    int fork[ID_FORK_NUM_MAX];
    int fork_num = get_fork_max_states(fork, states, id, except_id);
    for (int i = 0; i < fork_num; i++)
    {
        if(fork[i] != INVALID)
            fork_is_invalid = false;
    } 

    return fork_is_invalid;
}

static bool fork_states_no_more_than_TIP(int *states, int id, bool except_id){
    // get fork states
    bool fork_is_tip = true;
    int fork[ID_FORK_NUM_MAX];
    int fork_num = get_fork_max_states(fork, states, id, except_id);
    for (int i = 0; i < fork_num; i++)
    {
        if(fork[i] == TIP)
            fork_is_tip = false;
    } 

    return fork_is_tip;
}



// only for req form Channel A
static bool needT(int op, int param){
    bool flag = false;
    if(req_is_UL(op) && op != Get){
        flag = true;
    }else if((op == AcquireBlock || op == AcquirePerm) && (param == NtoT || param == BtoT)){
        flag == true;
    }
    return flag;
}

static bool gotT(int *states){
    int n = 0;
    for (int i = 0; i < ID_CACHE_NUM; i++)
    {
        if (states[i] == TIP)
            n++;
    }
    if(n == 0){
        return true;
    }else{
        return false;
    }
}

static bool promoteT(int *states, int id){
    // get fork states
    bool fork_is_invalid = true;
    int fork[ID_FORK_NUM_MAX];
    int fork_num = get_fork_max_states(fork, states, id, ID_NONE);
    for (int i = 0; i < fork_num; i++)
    {
        if(fork[i] != INVALID)
            fork_is_invalid = false;
    }
    
    // rule
    bool flag = false;
    if(states[id] != INVALID){
        if(states[id] == TIP && fork_is_invalid){
            flag = true;
        }
    }else{
        // if(gotT)
    }
}


//-------------------------------State Update Rules-------------------------//

//TODO: only support op == acquire*
static int req_a_next_state(int *states ,int id, int op, int param){
    if(op != AcquireBlock || op != AcquirePerm)
        Tool::cp_assert(false, "req_a_next_state: ilegaa op");
    
    int next_state = INVALID;
    // needT
    if(needT(op,param)){
        if(op == AcquireBlock || op == AcquirePerm){
            next_state = TRUNK;
        }else{
            next_state = TIP;
        }
    }
    // no needT
    else{
        // self == INVALID
        // if(){

        // }
    }
}


static int probe_next_state(int state, int param){
    int next_state;

    switch(state){
        case TIP:
            switch(param){
                case toT: next_state = state; break;
                case toB: next_state = BRANCH; break;
                case toN: next_state = INVALID; break;
            }
            break;
        case TRUNK:
            switch(param){
                case toT: next_state = state; break;
                case toB: next_state = BRANCH; break;
                case toN: next_state = INVALID; break;
            }
            break;
        case BRANCH:
            switch(param){
                case toT: Tool::cp_assert(false,"ilegal param"); break;
                case toB: next_state = BRANCH; break;
                case toN: next_state = INVALID; break;
            }
            break;
        case INVALID:
            switch(param){
                case toT: Tool::cp_assert(false,"ilegal param"); break;
                case toB: Tool::cp_assert(false,"ilegal param"); break;
                case toN: next_state = INVALID; break;
            }
            break;
        default: Tool::cp_assert(false,"ilegal state"); break;
    }

    return next_state;
}

#endif