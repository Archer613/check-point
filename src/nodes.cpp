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
    mes_out.insert(first_mes);
    return mes_out;
    
}

