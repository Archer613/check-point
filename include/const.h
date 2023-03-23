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
  AcquireBlock,
  AcquirePerm
};
enum { Probe = 6 };
enum {
  AccessAck = 0,
  AccessAckData,
  HintAck,
  ProbeAck = 4,
  ProbeAckData,
  Release,
  ReleaseData
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


// ID
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
};


// level
enum{
    L1 = 0,
    L2,
    L3,
};

#endif