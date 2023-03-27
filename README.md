# check_point

```

check_point is a project which can generate all TL-link states transformation relations for xiangshan nanhu-v2 

For example:

[C0 ICACHE]->[C0 L2] [AcquireBlock NtoB] pe[1] va[1]
[C0 L2]->[L3] [AcquireBlock NtoB] pe[1] va[1]
[L3]->[ID_NONE] [AcquireBlock NtoB] pe[1] va[1]

Original:

[N]-[N]   [N]-[N]
  [N]       [N]
       [N]

New:

[N]-[TT]   [N]-[N]
  [T]       [N]
       [T]


```

# Compile check_point

```
> git clone https://github.com/Archer613/check-point.git
> mkdir build
> cd build
> cmake ..
For debug, add -DCMAKE_BUILD_TYPE=Debug

> make
```