#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "stdio.h"


#define DEBUG

#ifdef DEBUG
    #define DBG_PRINTF
#endif

/********************************* 以下内容是根据以上定义内容自动做出的调整，无需修改 ******************/
/* 调试时输出  */
#ifdef      DBG_PRINTF
    #define     dbg_printf(fmt, args...)             (printf("\t%s,\t%s,%d >> "fmt"\n", __FILE__, __FUNCTION__, __LINE__, ##args))
#else
    #define     dbg_printf(fmt, arg...)              // nothing
#endif

#endif
