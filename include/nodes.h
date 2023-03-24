#ifndef CHECK_POINT_NODES_H
#define CHECK_POINT_NODES_H
#include "NodeBase.h"
#include "const.h"
#include "Tool.h"


/*
    handle:

    input           :parse inpput tlmes
      |
    state_update    :update state rule
      |
    output          :build output to next nodes


*/
class TLTreeNode_L1: public TLTreeNodeBase{
private:
  void input(TLMes in);
  void up_self();// update self
  void reset();
  TLMes mes_in;
  std::set<TLMes> mes_out;
public:
  void node_init(int p_id, std::set<int> c_id, NodeMes s);
  void up_states(int *s); // update all
  void handle(TLMes in);
  std::set<TLMes> control(int op, int param);
};


#endif