#ifndef CHECK_POINT_TOOL_H
#define CHECK_POINT_TOOL_H

#include <string>
#include "const.h"

namespace Tool{

//------------------------------------Tool---------------------------------------------//    
    using namespace std;

    static string stateTostring(uint8_t state){
        string s;
        switch (state)
        {
        case TIP:
            s = "TT";
            break;
        case TRUNK:
            s = "T";
            break;
        case BRANCH:
            s = "B";
            break;
        case INVALID:
            s = "N";
            break;
        default: s = "error";
            break;
        }
        return s;
    }


}
#endif