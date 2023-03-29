#ifndef CHECK_POINT_TOOL_H
#define CHECK_POINT_TOOL_H

#include <string>
#include "const.h"
#include <cassert>

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

    static void cp_assert(bool b, string s){
        if(b == false){
            printf("\nERROR: %s\n", s.c_str());
        }
        assert(b == true);

        return;

    }

    static string idTostring(int id){
        string s;
        switch(id){
            case ID_NONE: s = "ID_NONE"; break;
            case ID_CORE0_DCACHE: s = "C0 DCACHE"; break;
            case ID_CORE0_ICACHE: s = "C0 ICACHE"; break;
            case ID_CORE1_DCACHE: s = "C1 DCACHE"; break;
            case ID_CORE1_ICACHE: s = "C1 ICACHE"; break;
            case ID_CORE0_L2: s = "C0 L2"; break;
            case ID_CORE1_L2: s = "C1 L2"; break;
            case ID_L3: s = "L3"; break;
            case ID_CORE0_PTW: s = "ID_CORE0_PTW"; break;
            case ID_CORE1_PTW: s = "ID_CORE1_PTW"; break;
            case ID_DMA: s = "ID_DMA"; break;
            default: cp_assert(false, "ilegal id");
        }
        return s;
    }

    static string opTostring(uint8_t op){
        string s;
        switch(op){
            case PutFullData: s = "PutFullData"; break;
            case PutPartialData: s = "PutPartialData"; break;
            case Get: s = "Get"; break;
            case AcquireBlock: s = "AcquireBlock"; break;
            case AcquirePerm: s = "AcquirePerm"; break;
            case Probe: s = "Probe"; break;
            case Release: s = "Release"; break;
            case ReleaseData: s = "ReleaseData"; break;
            default: cp_assert(false, "ilegal opcode");
        }
        return s;
    }

    static string paramTostring(uint8_t op, uint8_t param){
        string s;
        switch(op){
            case AcquireBlock: 
                switch(param){
                    case NtoB: s = "NtoB"; break; 
                    case NtoT: s = "NtoT"; break; 
                    case BtoT: s = "BtoT"; break; 
                    default: cp_assert(false, "ilegal param");
                }
                break;
            case AcquirePerm: 
                switch(param){
                    case NtoB: s = "NtoB"; break; 
                    case NtoT: s = "NtoT"; break; 
                    case BtoT: s = "BtoT"; break; 
                    default: cp_assert(false, "ilegal param");
                }
                break;
            case Probe: 
                switch(param){
                    case toN: s = "toN"; break; 
                    case toB: s = "toB"; break; 
                    case toT: s = "toT"; break; 
                    default: cp_assert(false, "ilegal param");
                }
                break;
            case Release: 
                switch(param){
                    case TtoN: s = "TtoN"; break; 
                    case BtoN: s = "BtoN"; break; 
                    default: cp_assert(false, "ilegal param");
                }
                break;
            case ReleaseData:
                switch(param){
                    case TtoN: s = "TtoN"; break; 
                    case BtoN: s = "BtoN"; break; 
                    default: cp_assert(false, "ilegal param");
                }
                break;
            default: s = "0";
        }
        return s;
    }

    static void print(int *states){
        string s[ID_CACHE_NUM];

        for (size_t i = 0; i < ID_CACHE_NUM; i++)
        {
            s[i] = stateTostring(states[i]);
        }

        printf("[%s]-[%s]   [%s]-[%s]\n"  , s[0].c_str(), s[1].c_str(), s[2].c_str(), s[3].c_str() );
        printf("  [%s]       [%s]\n"      , s[4].c_str(), s[5].c_str());
        printf("       [%s]\n"            , s[6].c_str());
    }

    static int opToChnl(int op){
        if(op <= AcquirePerm)
            return CHNLA;
        else if(op <= Probe)
            return CHNLB;
        else if(op <= ReleaseData)
            return CHNLC;
        
        cp_assert(false, "opToChnl ilegal op");
        return -1;
    }

    static int opToTLop(int op){
        if(op <= AcquirePerm)
            return op;
        else if(op <= Probe)
            return (op-2);
        else if(op <= ReleaseData)
            return (op-3);

        cp_assert(false, "opToTLop ilegal op");
        return -1;
    }

    static int idToTLid(int id){
        enum{
            DCACHE_BUS_TYPE = 0,
            ICACHE_BUS_TYPE = 1,
            TILE_BUS_TYPE = 2,
            L3_BUS_TYPE = 3,
            DMA_BUS_TYPE = 4,
            PTW_BUS_TYPE = 5,
        };
        switch(id){
            case ID_NONE: cp_assert(false, "idToTLid ilegal id"); break;
            case ID_CORE0_DCACHE: return DCACHE_BUS_TYPE;
            case ID_CORE0_ICACHE: return ICACHE_BUS_TYPE;
            case ID_CORE1_DCACHE: return DCACHE_BUS_TYPE;
            case ID_CORE1_ICACHE: return ICACHE_BUS_TYPE;
            case ID_CORE0_L2: return TILE_BUS_TYPE;
            case ID_CORE1_L2: return TILE_BUS_TYPE;
            case ID_L3: return L3_BUS_TYPE;
            case ID_DMA: return DMA_BUS_TYPE;
            case ID_CORE0_PTW: return PTW_BUS_TYPE;
            case ID_CORE1_PTW: return PTW_BUS_TYPE;
            default: cp_assert(false, "idToTLid ilegal id"); break;
        }
        return -1;
    }

    static int idToCore(int id){
        enum{
            CORE0 = 0,
            CORE1,
        };
        switch(id){
            case ID_NONE: cp_assert(false, "idToCore ilegal id"); break;
            case ID_CORE0_DCACHE: return CORE0;
            case ID_CORE0_ICACHE: return CORE0;
            case ID_CORE1_DCACHE: return CORE1;
            case ID_CORE1_ICACHE: return CORE1;
            case ID_CORE0_L2: return CORE0;
            case ID_CORE1_L2: return CORE1;
            case ID_L3: return 0;
            case ID_DMA: return 0;
            case ID_CORE0_PTW: return CORE0;
            case ID_CORE1_PTW: return CORE1;
            default: cp_assert(false, "idToCore ilegal id"); break;
        }
        return -1;
    }


}
#endif