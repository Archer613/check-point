#include "../include/States_Gen_Tree.h"

void States_Gen_Tree::init_tree(void){
        NodeMes self;
        std::set<int> c_id;

        //----------- L1 -------------//   
        // CORE0
        for (int i = 0; i < ID_CORE0_L1_NUM; i++)
        {
            self.id = i;
            Nodes[i].init(ID_CORE0_L2, c_id, self);
            c_id.clear();
        }
        // CORE1
        for (int i = ID_CORE0_L1_NUM; i < ID_CORE0_L1_NUM + ID_CORE1_L1_NUM; i++)
        {
            self.id = i;
            Nodes[i].init(ID_CORE1_L2, c_id, self);
            c_id.clear();
        }

        //----------- L2 ---------------//
        // CORE0        
        c_id.insert(ID_CORE0_DCACHE);
        c_id.insert(ID_CORE0_ICACHE);
        self.id = ID_CORE0_L2;
        Nodes[ID_CORE0_L2].init(ID_L3, c_id, self);
        c_id.clear();
        // CORE1
        c_id.insert(ID_CORE1_DCACHE);
        c_id.insert(ID_CORE1_ICACHE);
        self.id = ID_CORE1_L2;
        Nodes[ID_CORE1_L2].init(ID_L3, c_id, self);
        c_id.clear();

        //----------- L3 ---------------//
        c_id.insert(ID_CORE0_L2);
        c_id.insert(ID_CORE1_L2);
        self.id = ID_L3;
        Nodes[ID_L3].init(ID_NONE, c_id, self);
        c_id.clear();

        
        // get all legal state_trees
        foreach(ID_L3);
    }


void States_Gen_Tree::foreach(int cache_id){
    int next_cache_id = cache_id-1;
    for (int state = INVALID; state <= TIP; state++)
    {
        Nodes[cache_id].self.state = state;

        if(cache_id == 0){
            // check rules
            if(check()){
                for (size_t i = 0; i < ID_CACHE_NUM; i++)
                {
                    state_trees[n][i] = Nodes[i].self.state;
                }
                // Tool::print(state_trees[n]);
                n++;   
            }
        }else if(cache_id > 0){
            foreach(next_cache_id);
        }   
    }
}

bool States_Gen_Tree::check(void){
    bool ck = true;

    for (int id = 0; id < ID_CACHE_NUM; id++)
    {
        
        // Get Self State
        int self = Nodes[id].self.state;
        // Get Child State
        int n_child = Nodes[id].children_id.size();
        int child[n_child];
        if(n_child > 0){
            int k = 0;
            for(std::set<int>::iterator i = Nodes[id].children_id.begin(); i!=Nodes[id].children_id.end(); i++){
                child[k++] = Nodes[*i].self.state;
            }
        }
        // Get parent State
        int parent = INVALID;
        if(Nodes[id].parent_id != ID_NONE){
            parent = Nodes[Nodes[id].parent_id].self.state;
        }
        // Get all State
        int s[ID_CACHE_NUM];
        for (size_t i = 0; i < ID_CACHE_NUM; i++)
        {
            s[i] = Nodes[i].self.state;
        }
        // Get upper_state;
        int upper_state;
        if(self == INVALID && parent != INVALID)
            upper_state = parent;
        else
            upper_state = self;
        // Get fork max State 
        // TODO：Only supports up to three levels
        int n_fork = n_child;
        int fork_max_state[n_fork];
        for (int i = 0; i < n_fork; i++)//init
        {
            fork_max_state[i] = INVALID;
        }
        if(n_fork > 0){// Get
            int k = 0;
            for(std::set<int>::iterator i = Nodes[id].children_id.begin(); i != Nodes[id].children_id.end(); i++){
                if(Nodes[*i].children_id.size() > 0){//L2
                    for(std::set<int>::iterator j = Nodes[*i].children_id.begin(); j!= Nodes[*i].children_id.end(); j++){
                        if(Nodes[*j].self.state > fork_max_state[k]){//L1
                            fork_max_state[k] = Nodes[*j].self.state;
                        }
                    }
                }
                if(Nodes[*i].self.state > fork_max_state[k]){//L1
                    fork_max_state[k] = Nodes[*i].self.state;
                }
                k += 1;
            }
        }
        

        // trunk_not_have_branch_child_r
        ck = rules.trunk_not_have_branch_child_r(upper_state, child, n_child);
        if(ck == false)
            return ck;
        
        // check branch_not_have_t_child_r
        ck = rules.branch_not_have_t_child_r(upper_state, child, n_child);
        if(ck == false)
            return ck;

        // check trunk_have_t_fork_r
        ck = rules.trunk_have_t_fork_r(self, fork_max_state, n_fork);
        if(ck == false)
            return ck;

        // one_fork_tip_and_other_invalid
        ck = rules.one_fork_tip_and_other_invalid(n_fork, fork_max_state);
        if(ck == false)
            return ck;

        // check leaf_is_not_trunk
        if (id < ID_L1_NUM)
        {
            ck = rules.leaf_is_not_trunk(Nodes[id].self.state);
            if(ck == false)
                return ck;
        }

        // no_more_than_one_tip
        ck = rules.no_more_than_one_tip(s);
        if(ck == false)
            return ck;

        
        
        
    }

    return ck;
}