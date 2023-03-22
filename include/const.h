#ifndef CHECK_POINT_CONST_H
#define CHECK_POINT_CONST_H

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

enum { toT = 0, toB, toN };
enum { NtoB = 0, NtoT, BtoT };
enum { TtoB = 0, TtoN, BtoN, TtoT, BtoB, NtoN };
enum { INVALID = 0, BRANCH, TRUNK, TIP };

enum { CLEAN = 0, DIRTY };
enum { INCLUSIVE = 0, NONINCLUSIVE };


#endif