#include "../include/tl_tree.h"

void tl_tree::init(){
    num = 0;

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
        node_l1[i].init(ID_CORE1_L2, c_id, self);
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

    //----------- UL -------------//   
    // PTW CORE0
    self.id = ID_CORE0_PTW;
    node_ul[0].init(ID_CORE0_L2, c_id, self);
    c_id.clear();
    // PTW CORE1
    self.id = ID_CORE1_PTW;
    node_ul[1].init(ID_CORE1_L2, c_id, self);
    c_id.clear();
    // DMA
    self.id = ID_DMA;
    node_ul[2].init(ID_L3, c_id, self);
    c_id.clear();
    

}


void tl_tree::get_all_states(){
    for (int id = 0; id < ID_CACHE_NUM; id++)
    {
        if(id < ID_L1_NUM){
            states_new[id] = node_l1[id].self.state;
        }else if(id < ID_L1_NUM + ID_L2_NUM){
            states_new[id] = node_l2[id-ID_L1_NUM].self.state;
        }else{
            states_new[id] = node_l3.self.state;
        }
    }
    
            
}


bool tl_tree::run(int op, int param, int *s, int id){
    std::set<TLMes> mes_in;
    std::set<TLMes> mes_temp;
    std::set<TLMes> mes_out;
    bool has_mes_valid = true;
    bool first_mes_legal = false;

    // update states
    for (int i = 0; i < ID_CACHE_NUM; i++)
    {
        if(i < ID_L1_NUM){
            node_l1[i].up_states(s);
            node_l1[i].reset();
        }else if(i < ID_L1_NUM + ID_L2_NUM){
            node_l2[i-ID_L1_NUM].up_states(s);
            node_l2[i-ID_L1_NUM].reset();
        }else{
            node_l3.up_states(s);
            node_l3.reset();
        }
    }
    for (int id = ID_CACHE_NUM; id < ID_CACHE_NUM + ID_UL_NUM; id++)
    {
        node_ul[id-ID_CACHE_NUM].reset();
    }
    

    // input first
    if(!req_is_UL(op)){
        mes_in = node_l1[id].control(op, param);
    }else{
        mes_in = node_ul[id-ID_CACHE_NUM].control(op);
    }

    if(!mes_in.begin()->valid){
        // printf("Ilegal input!\n");
    }else{
        // printf("legal input:\n");
        first_mes_legal = true;
        num++;
        // print output
        printf("%d  %d  %d  %d  %d\n", Tool::opToChnl(op), Tool::opToTLop(op), param, Tool::idToTLid(id), Tool::idToCore(id));// chnl op param src core
        for (int i = 0; i < ID_CACHE_NUM; i++)
        {
            printf("%d  ", s[i]);
        }
        printf("\n"); 
    }
    
    // printf("[%s]->[%s] [%s %s] pe[%d] va[%d]\n", Tool::idTostring(mes_in.begin()->src_id).c_str()
    //                                         , Tool::idTostring(mes_in.begin()->id).c_str()
    //                                         , Tool::opTostring(mes_in.begin()->opcode).c_str()
    //                                         , Tool::paramTostring(mes_in.begin()->opcode, mes_in.begin()->param).c_str()
    //                                         , mes_in.begin()->perfercache, mes_in.begin()->valid);


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
                // printf("[%s]->[%s] [%s %s] pe[%d] va[%d]\n", Tool::idTostring(it_temp->src_id).c_str()
                //                             , Tool::idTostring(it_temp->id).c_str()
                //                             , Tool::opTostring(it_temp->opcode).c_str()
                //                             , Tool::paramTostring(it_temp->opcode, it_temp->param).c_str()
                //                             , it_temp->perfercache, it_temp->valid);
            }
            mes_temp.clear();
        }
        mes_in.swap(mes_out);
        mes_out.clear();
    } 

    // get states
    get_all_states();
    // Tool::print(s);
    // Tool::print(states_new);
    // printf("\n--------------------------------\n");

    // print output
    if(first_mes_legal){
        for (int i = 0; i < ID_CACHE_NUM; i++)
        {
            printf("%d  ", states_new[i]);
        }
        printf("\n"); 
    }
    

    return false;
}