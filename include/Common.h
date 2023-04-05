#ifndef CHECK_POINT_COMMON_H
#define CHECK_POINT_COMMON_H

#include "stdio.h"
#define __HLOG__  

#ifdef __HLOG__  
#define HLOG(sw,format,...) if(sw) printf(format, ##__VA_ARGS__)
#else  
#define HLOG(sw,format,...)  
#endif  


#endif