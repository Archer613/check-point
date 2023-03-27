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

}

// handle
void TLTreeNode_L1::handle(TLMes in){
    
    input(in);

    up_self();    

    
}

// control
std::set<TLMes> TLTreeNode_L1::control(int op, int param){
    // Input legality check
    bool legal = false;
    if(op == AcquireBlock || op == AcquirePerm){
        switch(param){
            case NtoB: if(states[self.id] == INVALID) legal = true; break;
            case NtoT: if(states[self.id] == INVALID) legal = true; break;
            case BtoT: if(states[self.id] == BRANCH) legal = true; break;
            default: Tool::cp_assert(false, "control ilegal param");
        }
    }else if(op == Release || op == ReleaseData){
        switch(param){
            case TtoN: if(states[self.id] == TIP) legal = true; break;
            case BtoN: if(states[self.id] == BRANCH) legal = true; break;
            default: Tool::cp_assert(false, "control ilegal param");
        }
    }   

    // update self state
    int self_next_state;
    if(op == AcquireBlock || op == AcquirePerm){
        switch(param){
            case NtoT: self_next_state = TIP; break;
            case NtoB: 
                if(promoteT_model(states, self.id)){
                    self_next_state = TIP;
                }else{
                    self_next_state = BRANCH;
                }
                break;
            case BtoT: self_next_state = TIP; break;
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
    if(legal)
        self.state = self_next_state;

    // mes_out
    TLMes first_mes;
    first_mes.opcode = op;
    first_mes.param = param;
    first_mes.perfercache = (self.id == ID_CORE0_ICACHE || self.id == ID_CORE1_ICACHE);
    first_mes.valid = legal;
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
        // nothing to output

        // wb_dir
        wb_dir = true;
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

        // wb_dir
        if(states[self.id] != INVALID && (mes_in.param == toN || (states[self.id] == TIP && mes_in.param == toB))){
            wb_dir = true;
        }
    }
    // accquire*
    else if(mes_in.opcode == AcquireBlock || mes_in.opcode == AcquirePerm){
        // probe up
        if(needT(mes_in.opcode, mes_in.param) && fork_states_no_more_than_TIP(states, self.id, mes_in.src_id)){
            TLMes mes_be_sent = mes_in;
            mes_be_sent.id = parent_id;
            mes_be_sent.src_id = self.id;
            mes_out.insert(mes_be_sent);
            grant = true;
            // wb_dir
            if(mes_in.perfercache || (states[self.id] != INVALID && mes_in.opcode != Get)){
                wb_dir = true;
            }
        }else if(!needT(mes_in.opcode, mes_in.param) && fork_max_states_is_invalid(states, self.id, mes_in.src_id) && states[self.id] == INVALID){
            TLMes mes_be_sent = mes_in;
            mes_be_sent.id = parent_id;
            mes_be_sent.src_id = self.id;
            mes_out.insert(mes_be_sent);
            grant = true;
            // probe up wb_dir
            if(mes_in.perfercache || (states[self.id] != INVALID && mes_in.opcode != Get)){
                wb_dir = true;
            }
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
                // probe up wb_dir
                if(states[self.id] == INVALID)
                    wb_dir = true;
            }
        }
        // receive acquire* wb_dir
        if(mes_in.perfercache || states[self.id] != INVALID){
            if(mes_in.opcode == AcquireBlock){
                wb_dir = true;
            }else if(mes_in.opcode == AcquirePerm && states[self.id] != INVALID){
                wb_dir = true;
            }
        }
    }else{
        Tool::cp_assert(false,"L2/L3 output ilegal op");
    }
}

void TLTreeNode_L2L3::up_self(){
    if(wb_dir){
        switch(mes_in.opcode){
            // Channel A
            case AcquireBlock: self.state = req_a_next_state(states,self.id,mes_in.opcode,mes_in.param,grant); break;
            case AcquirePerm: self.state = req_a_next_state(states,self.id,mes_in.opcode,mes_in.param,grant); break;
            // Channel B
            case Probe: self.state = probe_next_state(states[self.id],mes_in.param); break;
            // Channel C
            case ReleaseData: self.state = release_next_state(states[self.id],mes_in.param); break;
            default: Tool::cp_assert(false, "up_self ilegal op");
        }
    }else{
        // no change self state
    }

}

void TLTreeNode_L2L3::reset(){
    mes_in.valid = false;
    mes_out.clear();
    self.state = states[self.id];
    grant = false;
    wb_dir = false;
}

// up_states
void TLTreeNode_L2L3::up_states(int *s){

    for (int i = 0; i < ID_CACHE_NUM; i++)
    {
        states[i] = s[i];
    }

}

// handle
std::set<TLMes> TLTreeNode_L2L3::handle(TLMes in){
    
    input(in);

    output();

    up_self();    

    return mes_out;

}
