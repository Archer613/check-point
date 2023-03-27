#ifndef CHECK_POINT_NODES_H
#define CHECK_POINT_NODES_H
#include "NodeBase.h"
#include "const.h"
#include "Tool.h"

class TLTreeNode_UL: public TLTreeNodeBase{
private:
  std::set<TLMes> mes_out;
public:
  void node_init(int p_id, std::set<int> c_id, NodeMes s);
  void reset();
  std::set<TLMes> control(int op);
};


class TLTreeNode_L1: public TLTreeNodeBase{
private:
  void input(TLMes in);
  void up_self();// update self
  TLMes mes_in;
  std::set<TLMes> mes_out;
public:
  void node_init(int p_id, std::set<int> c_id, NodeMes s);
  void up_states(int *s); // update all
  void reset();
  void handle(TLMes in);
  std::set<TLMes> control(int op, int param);
};



/*
    handle:

    input           :parse inpput tlmes
      |
    output          :build output to next nodes
      |
    state_update    :update state rule


*/
class TLTreeNode_L2L3: public TLTreeNodeBase{
private:
  void input(TLMes in);
  void up_self();// update self
  void output();
  TLMes mes_in;
  std::set<TLMes> mes_out;
  bool grant;
  bool wb_dir;
public:
  void node_init(int p_id, std::set<int> c_id, NodeMes s);
  void up_states(int *s); // update all
  void reset();
  std::set<TLMes> handle(TLMes in);
};


#endif