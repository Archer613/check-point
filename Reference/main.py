from functools import reduce
from typing import List
import graphviz
import itertools
from enum import Enum, unique
from anytree import Node, RenderTree, NodeMixin

from const import *
from rule import *
from tltree import *


def state_collect(tltree: TLTreeNode):
    if not tltree.children:
        return [tltree.state]
    state = [tltree.state]
    for child in tltree.children:
        state = state + state_collect(child)
    return state


def get_max_priv(state):
    if TLState.TIP in state:
        return TLState.TIP
    if TLState.TRUNK in state:
        return TLState.TRUNK
    if TLState.BRANCH in state:
        return TLState.BRANCH
    return TLState.INVALID


def leaf_filter(tltree: TLTreeNode):
    rules = [
        leaf_is_not_trunk
    ]
    if not tltree.children:
        for r in rules:
            if not r(tltree.state):
                return False
    for child in tltree.children:
        if not leaf_filter(child):
            return False
    return True


def incl_inter_level_filter(tltree: TLTreeNode):
    if not tltree.children:
        return True
    rules = [
        invalid_not_have_valid_child_r,
        trunk_not_have_branch_child_r,
        branch_not_have_t_child_r,
        trunk_have_t_child_r
    ]
    for r in rules:
        if not r(tltree, tltree.children):
            return False
    for child in tltree.children:
        if not incl_inter_level_filter(child):
            return False
    return True


def global_filter(tltree: TLTreeNode):
    state = state_collect(tltree)
    if state.count(TLState.TIP) > 1:
        return False
    return True


def nine_fork_filter(tltree: TLTreeNode):
    if not tltree.children:
        return True
    child_states = []
    for child in tltree.children:
        child_states += state_collect(child)
    max_priv = get_max_priv(child_states)

    for child in tltree.children:
        state = state_collect(child)
        priv = get_max_priv(state)
        if priv == TLState.TRUNK:
            return False
        if (priv == TLState.BRANCH and max_priv == TLState.TIP):
            return False
    if tltree.state == TLState.BRANCH and max_priv == TLState.TIP:
        return False
    if tltree.state == TLState.TRUNK and max_priv != TLState.TIP:
        return False
    for child in tltree.children:
        if not nine_fork_filter(child):
            return False
    return True

if __name__ == '__main__':

    # Parameters
    TOPO = Topo.DUAL
    INCLUSION = Inclusion.NONINCLUSIVE

    # Traverse all possible states
    states = [TLState] * 4 if TOPO == Topo.SINGLE else [TLState] * 7
    all_trees = []
    for x in itertools.product(*states):
        tlTree = TLTree(x)
        all_trees.append(tlTree.tree)
    print("Total states: " + str(len(all_trees)))

    # Filters
    incl_checkers = [
        leaf_filter,
        incl_inter_level_filter,
        global_filter
    ]

    nine_checkers = [
        leaf_filter,
        nine_fork_filter,
        global_filter
    ]

    # Filter all illegal tree states
    if INCLUSION == Inclusion.INCLUSIVE:
        for c in incl_checkers:
            all_trees = list(filter(c, all_trees))
            print(f"filter: {retrieve_name(c)} states: {len(all_trees)}")
    else:
        for c in nine_checkers:
            all_trees = list(filter(c, all_trees))
            print(f"filter: {retrieve_name(c)} states: {len(all_trees)}")

    # Visualization
    for tree in all_trees:
        for pre, fill, node in RenderTree(tree):
            print("%s%s" % (pre, node.state))
        print()