#ifndef CHECK_POINT_CONST_H
#define CHECK_POINT_CONST_H

// Message
enum {
  PutFullData = 0,
  PutPartialData,
  ArithmeticData,
  LogicalData,
  Get,
  Hint,
  AcquireBlock,//6
  AcquirePerm//7
};
enum { 
  // Probe = 6 
  Probe = 8 // Reordering
};
enum {
  AccessAck = 0,
  AccessAckData,
  HintAck,
  ProbeAck = 4,
  ProbeAckData,
  // Release, //5
  // ReleaseData //6 
  Release = 9, // Reordering
  ReleaseData= 10 // Reordering
};


enum { Grant = 4, GrantData, ReleaseAck };
enum { GrantAck = 0 };

// Param
enum { toT = 0, toB, toN };
enum { NtoB = 0, NtoT, BtoT };
enum { TtoB = 0, TtoN, BtoN, TtoT, BtoB, NtoN };

// State
enum { INVALID = 0, BRANCH, TRUNK, TIP };

// Others
enum { CLEAN = 0, DIRTY };
enum { INCLUSIVE = 0, NONINCLUSIVE };
enum { promoteT_safe = true };


// ID
// ID be storage by set ,rule is D$ < I$, CORE0 < CORE1
enum {
    ID_NONE = -1,
    ID_CORE0_DCACHE = 0,
    ID_CORE0_ICACHE = 1,
    ID_CORE1_DCACHE = 2,
    ID_CORE1_ICACHE = 3,
    ID_CORE0_L2 = 4,
    ID_CORE1_L2 = 5,
    ID_L3 = 6,
    ID_CACHE_NUM,

    ID_CORE0_L1_NUM = 2,
    ID_CORE1_L1_NUM = 2,
    ID_L1_NUM = ID_CORE0_L1_NUM + ID_CORE1_L1_NUM,
    ID_L2_NUM = 2,

    ID_FORK_NUM_MAX = 2,
};


// level
enum{
    L1 = 0,
    L2,
    L3,
};

#endif