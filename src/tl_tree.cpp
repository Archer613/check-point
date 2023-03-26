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
    // CORE1
    for (int i = ID_CORE0_L1_NUM; i < ID_CORE0_L1_NUM + ID_CORE1_L1_NUM; i++)
    {
        self.id = i;
        node_l1[i].init(ID_CORE0_L2, c_id, self);
        c_id.clear();
    }

    //----------- L2 ---------------//
    // CORE0        
    c_id.insert(ID_CORE0_DCACHE);
    c_id.insert(ID_CORE0_ICACHE);
    self.id = ID_CORE0_L2;
    node_l2[0].init(ID_L3, c_id, self);
    c_id.clear();
    // CORE1
    c_id.insert(ID_CORE1_DCACHE);
    c_id.insert(ID_CORE1_ICACHE);
    self.id = ID_CORE1_L2;
    node_l2[1].init(ID_L3, c_id, self);
    c_id.clear();

    //----------- L3 ---------------//
    c_id.insert(ID_CORE0_L2);
    c_id.insert(ID_CORE1_L2);
    self.id = ID_L3;
    node_l3.init(ID_NONE, c_id, self);
    c_id.clear();
}

bool tl_tree::run(int op, int param, int *s, int id){
    std::set<TLMes> mes_in;
    std::set<TLMes> mes_temp;
    std::set<TLMes> mes_out;
    bool has_mes_valid = true;
    
    // input first
    node_l1[id].up_states(s);
    mes_in = node_l1[id].control(op, param);
    printf("scr_id_:%d id:%d op:%d, param:%d pe:%d valid:%d\n", mes_in.begin()->src_id, mes_in.begin()->id, mes_in.begin()->opcode, mes_in.begin()->param
                                            , mes_in.begin()->perfercache, mes_in.begin()->valid);

    // run
    while(has_mes_valid){
        has_mes_valid = false;
        for(std::set<TLMes>::iterator it = mes_in.begin(); it != mes_in.end(); it++){
            if(it->valid && it->id != ID_NONE){
                if(it->id < ID_L1_NUM){
                    node_l1[it->id].handle(*it);
                }else if(it->id < ID_L1_NUM + ID_L2_NUM){
                    mes_temp = node_l2[it->id-ID_L1_NUM].handle(*it);
                }else{
                    mes_temp = node_l3.handle(*it);
                }
            }
            for(std::set<TLMes>::iterator it_temp = mes_temp.begin(); it_temp != mes_temp.end(); it_temp++){
                if(it_temp->valid && it_temp->id != ID_NONE)
                    has_mes_valid = true;
                mes_out.insert(*it_temp);
                printf("scr_id_:%d id:%d op:%d, param:%d pe:%d valid:%d\n", it_temp->src_id, it_temp->id, it_temp->opcode, it_temp->param
                                            , it_temp->perfercache, it_temp->valid);
            }
            mes_temp.clear();
        }
        mes_in.swap(mes_out);
        mes_out.clear();
    } 

}