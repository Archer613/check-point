#ifndef CHECK_POINT_COMMON_H
#define CHECK_POINT_COMMON_H

#include "stdio.h"
#define __HLOG__  

#ifdef __HLOG__  
#define HLOG(format,...) printf(format, ##__VA_ARGS__)
#else  
#define HLOG(format,...)  
#endif  


#endif