#ifndef CHECK_POINT_CACHEUPDATECOMMON_H
#define CHECK_POINT_CACHEUPDATECOMMON_H

#include "const.h"
#include "Tool.h"

static int probe_next_state(int state, int param){
    int next_state;

    switch(state){
        case TIP:
            switch(param){
                case toT: next_state = state; break;
                case toB: next_state = BRANCH; break;
                case toN: next_state = INVALID; break;
            }
            break;
        case TRUNK:
            switch(param){
                case toT: next_state = state; break;
                case toB: next_state = BRANCH; break;
                case toN: next_state = INVALID; break;
            }
            break;
        case BRANCH:
            switch(param){
                case toT: Tool::cp_assert(false,"ilegal param"); break;
                case toB: next_state = BRANCH; break;
                case toN: next_state = INVALID; break;
            }
            break;
        case INVALID:
            switch(param){
                case toT: Tool::cp_assert(false,"ilegal param"); break;
                case toB: Tool::cp_assert(false,"ilegal param"); break;
                case toN: next_state = INVALID; break;
            }
            break;
        default: Tool::cp_assert(false,"ilegal state"); break;
    }

    return next_state;
}

#endif