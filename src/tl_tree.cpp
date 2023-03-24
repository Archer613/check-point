#include "../include/tl_tree.h"

void tl_tree::init(){
    NodeMes self;
    std::set<int> c_id;
    //----------- L1 -------------//   
    // CORE0
    for (int i = 0; i < ID_CORE0_L1_NUM; i++)
    {
        self.id = i;
        node_l1[i].init(ID_CORE0_L2, c_id, self);
        c_id.clear();
    }

}

bool tl_tree::run(int op, int param, int *s, int id){
    std::set<TLMes> mes;
    
    node_l1[id].up_states(s);
    mes = node_l1[id].control(op, param);

    printf("id:%d op:%d, param:%d pe:%d valid:%d\n", mes.begin()->id, mes.begin()->opcode, mes.begin()->param
                                            , mes.begin()->perfercache, mes.begin()->valid);

}