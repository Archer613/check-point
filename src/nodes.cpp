#include "../include/nodes.h"

//-------------------------L1_NODES-----------------------//

void TLTreeNode_L1::node_init(int p_id, std::set<int> c_id, NodeMes s){
    // init NodeBase
    init(p_id, c_id, s);
}

void TLTreeNode_L1::input(TLMes in){
    if(in.valid && in.opcode == Probe)
        mes_in = in;
    else    
        Tool::cp_assert(false,"L1 ilegal input");
}

void TLTreeNode_L1::up_self(){
    if(mes_in.opcode == Probe){
        self.state = probe_next_state(states[self.id], mes_in.param);
    }
}

void TLTreeNode_L1::reset(){
    mes_in.valid = false;
    mes_out.clear();
    self.state = states[self.id];
}

// up_states
void TLTreeNode_L1::up_states(int *s){

    for (int i = 0; i < ID_CACHE_NUM; i++)
    {
        states[i] = s[i];
    }

    reset();
}

// handle
void TLTreeNode_L1::handle(TLMes in){
    
    input(in);

    up_self();    

    
}

// control
std::set<TLMes> TLTreeNode_L1::control(int op, int param){
    // update self state
    int self_next_state;
    if(op == AcquireBlock || op == AcquirePerm){
        switch(param){
            case toT: self_next_state = TIP; break;
            case toB:  break; // TODO
            default: Tool::cp_assert(false,"control legal param");
        }
    }else if(op == ReleaseData){
        switch(param){
            case TtoN: self_next_state = INVALID; break;
            case BtoN: self_next_state = INVALID; break;
            default: Tool::cp_assert(false,"control ilegal param");
        }
    }else{
        Tool::cp_assert(false,"first_mes ilegal opcode");
    }
    self.state = self_next_state;

    // mes_out
    TLMes first_mes;
    first_mes.opcode = op;
    first_mes.param = param;
    first_mes.perfercache = (self.id == ID_CORE0_ICACHE || self.id == ID_CORE1_ICACHE);
    first_mes.valid = true;
    first_mes.id = parent_id;
    first_mes.src_id = self.id;
    mes_out.insert(first_mes);
    return mes_out;
    
}


//-------------------------L2L3_NODES-----------------------//

void TLTreeNode_L2L3::node_init(int p_id, std::set<int> c_id, NodeMes s){
    // init NodeBase
    init(p_id, c_id, s);
}

void TLTreeNode_L2L3::input(TLMes in){
    if(in.valid && !req_is_UL(in.opcode) && in.opcode != Release)
        mes_in = in;
    else    
        Tool::cp_assert(false,"L2/L3 ilegal input");
}

void TLTreeNode_L2L3::output(){
    // releaseData
    if(mes_in.opcode == ReleaseData){
        // nothing to do
    }
    // probe
    else if(mes_in.opcode == Probe){
        if((states[self.id] == BRANCH || states[self.id] == TIP) && mes_in.param == toB){
            // not probe up level
        }else{
            if (!fork_max_states_is_invalid(states, self.id, ID_NONE)){// at least one client hit
                // get fork states
                int fork[ID_FORK_NUM_MAX];
                int fork_num = get_fork_max_states(fork, states, self.id, ID_NONE);
                for (int i = 0; i < fork_num; i++)
                {
                    if(fork[i] != INVALID){
                        // set_probe
                        TLMes mes_be_sent = mes_in;
                        std::set<int>::iterator it = children_id.begin();
                        for (int j = 0; j < i; j++)
                            it++;
                        mes_be_sent.id = *it;
                        mes_out.insert(mes_be_sent);
                    }
                }  
            }
        }
    }
    // accquire*
    else if(mes_in.opcode == AcquireBlock || mes_in.opcode == AcquirePerm){
        // acquire down
        if(needT(mes_in.opcode, mes_in.param) && fork_states_no_more_than_TIP(states, self.id, mes_in.src_id)){
            TLMes mes_be_sent = mes_in;
            mes_be_sent.id = parent_id;
            mes_be_sent.src_id = self.id;
            mes_out.insert(mes_be_sent);
        }else if(!needT(mes_in.opcode, mes_in.param) && fork_max_states_is_invalid(states, self.id, mes_in.src_id) && states[self.id] == INVALID){
            TLMes mes_be_sent = mes_in;
            mes_be_sent.id = parent_id;
            mes_be_sent.src_id = self.id;
            mes_out.insert(mes_be_sent);
        }
        // probe up
        if(!fork_max_states_is_invalid(states, self.id, mes_in.src_id)){
            if(needT(mes_in.opcode, mes_in.param) || states[self.id] == INVALID || !fork_states_no_more_than_TIP(states, self.id, mes_in.src_id)){
                TLMes mes_be_sent;
                mes_be_sent.opcode = Probe;
                if(needT(mes_in.opcode, mes_in.param)){
                    mes_be_sent.param = toN;
                }else{
                    mes_be_sent.param = toB;
                }
                mes_be_sent.perfercache = false;
                mes_be_sent.src_id = self.id;
                mes_be_sent.valid = true;
                for(std::set<int>::iterator it = children_id.begin(); it != children_id.end(); it++){
                    if(*it != mes_in.src_id){
                        mes_be_sent.id = *it;
                        mes_out.insert(mes_be_sent);
                    }
                }
            }
        }
    }else{
        Tool::cp_assert(false,"L2/L3 output ilegal op");
    }
}

// void TLTreeNode_L1::up_self(){
//     switch(mes_in.opcode){
//         case AcquireBlock: 
//     }
// }

void TLTreeNode_L2L3::reset(){
    mes_in.valid = false;
    mes_out.clear();
    self.state = states[self.id];
}

// up_states
void TLTreeNode_L2L3::up_states(int *s){

    for (int i = 0; i < ID_CACHE_NUM; i++)
    {
        states[i] = s[i];
    }

    reset();
}

// handle
std::set<TLMes> TLTreeNode_L2L3::handle(TLMes in){
    
    input(in);

    output();

    return mes_out;

    // up_self();    

}
