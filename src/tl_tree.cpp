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

void tl_tree::get_DIR_all_states(int *s){
    // for CORE0_L2 CORE1_L2 L3
    for (int id = ID_CORE0_L2; id <= ID_L3; id++)
    {
        int dir_id = id - ID_CORE0_L2;

        int fork[ID_FORK_NUM_MAX];
        int fork_max_states = get_fork_max_states(fork, s, id, ID_NONE);
        if(fork[0] == TRUNK || fork[1] == TRUNK)
            Tool::cp_assert(false,"fork max states cant be trunk");

        self_dir[dir_id][0] = s[id];
        self_dir[dir_id][1] = fork[0];
        self_dir[dir_id][2] = fork[1];

        client_dir[dir_id][0] = fork[0];
        client_dir[dir_id][1] = fork[1];
    }
    
}


bool tl_tree::run(int op, int param, int *s, int id){
    std::set<TLMes> mes_in;
    std::set<TLMes> mes_temp;
    std::set<TLMes> mes_out;
    bool has_mes_valid = true;
    bool first_mes_legal = true;
    static long int total_num = 0;
    static bool mode = true;

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

    if(!mode) HLOG(true, "NUM: %ld\n", total_num);
    total_num++;
    if(!mes_in.begin()->valid){
        if(!mode) HLOG(true, "Ilegal input!\n\n");
        first_mes_legal = false;
    }else{
        if(!mode) HLOG(true, "legal input:\n\n");
        first_mes_legal = true;
        num++;
        // print output
        if(mode){
            // HLOG(true, "%d  %d  %d  %d  %d\n", Tool::opToChnl(op), Tool::opToTLop(op), param, Tool::idToTLid(id), Tool::idToCore(id));// chnl op param src core
            // for (int i = 0; i < ID_CACHE_NUM; i++)
            // {
            //     HLOG(true, "%d  ", s[i]);
            // }
            // HLOG(true, "\n");

            // test case
            HLOG(true, "%d %d %d %d %d\n", Tool::idToTLid(id), Tool::idToCore(id), Tool::opToChnl(op), Tool::opToTLop(op), param);
        }   
    }
    
    if(first_mes_legal && !mode){
        HLOG(true, "[%s]->[%s] [%s %s] pe[%d] va[%d]\n", Tool::idTostring(mes_in.begin()->src_id).c_str()
                                                , Tool::idTostring(mes_in.begin()->id).c_str()
                                                , Tool::opTostring(mes_in.begin()->opcode).c_str()
                                                , Tool::paramTostring(mes_in.begin()->opcode, mes_in.begin()->param).c_str()
                                                , mes_in.begin()->perfercache, mes_in.begin()->valid);
    }

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
                if(first_mes_legal && !mode){
                    HLOG(true, "[%s]->[%s] [%s %s] pe[%d] va[%d]\n", Tool::idTostring(it_temp->src_id).c_str()
                                                , Tool::idTostring(it_temp->id).c_str()
                                                , Tool::opTostring(it_temp->opcode).c_str()
                                                , Tool::paramTostring(it_temp->opcode, it_temp->param).c_str()
                                                , it_temp->perfercache, it_temp->valid);
                }
            }
            mes_temp.clear();
        }
        mes_in.swap(mes_out);
        mes_out.clear();
    } 

    // get states
    get_all_states();
    if(first_mes_legal && !mode){
        Tool::print(s);
        Tool::print(states_new);
    }

    // get all states
    // Self DIR : self client0 client1
    // Client DIR : client0 client1
    get_DIR_all_states(s);
    if(first_mes_legal && !mode){
        for (int id = ID_CORE0_L2; id <= ID_L3; id++)
        {
            int dir_id = id - ID_CORE0_L2;

            // printf Self Dir
            HLOG(true, "cache[%s]:\n Self DIR: [%s] [%s] [%s]\n", Tool::idTostring(id).c_str()
                                                                , Tool::stateTostring(self_dir[dir_id][0]).c_str()
                                                                , Tool::stateTostring(self_dir[dir_id][1]).c_str()
                                                                , Tool::stateTostring(self_dir[dir_id][2]).c_str());

            // printf Client Dir
            HLOG(true, " Client DIR: [%s] [%s]\n", Tool::stateTostring(client_dir[dir_id][0]).c_str()
                                                , Tool::stateTostring(client_dir[dir_id][1]).c_str());
        }
    }
    if(!mode) HLOG(true, "\n--------------------------------\n");

    // print output
    if(first_mes_legal && mode){
        // states
        // for (int i = 0; i < ID_CACHE_NUM; i++)
        // {
        //     HLOG(true, "%d  ", states_new[i]);
        // }
        // HLOG(true, "\n");
        
        // testcase DIR 
        for (int id = ID_CORE0_L2; id <= ID_L3; id++)
        {
            int dir_id = id - ID_CORE0_L2;
            HLOG(true, "%d %d %d ", self_dir[dir_id][0], self_dir[dir_id][1], self_dir[dir_id][2]);
            HLOG(true, "%d %d\n", client_dir[dir_id][0], client_dir[dir_id][1]);
        }
        
    }

    return false;
}