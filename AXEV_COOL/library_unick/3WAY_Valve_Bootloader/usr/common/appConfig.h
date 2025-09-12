#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

#include <clock_device.h>
#include <string.h>

#define PRAGMA(x) _Pragma(#x)
#define BEGIN_PACK PRAGMA(pack(push, 1))
#define END_PACK   PRAGMA(pack(pop))

/* ************************hardware config ********************************/
#define MAIN_CPU_CLOCK          16U       /*MHz*/

#if MAIN_CPU_CLOCK == 16U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_1
#else
    #error MAIN_CPU_CLOCK MUST BE equal to 16MHz!*/
#endif

/* ************************System config ******************************/
#define WATCH_DOG_EN                            (0U)
/* ************************debug config *******************************/
#define CODE_DEBUG_EN    0

#if CODE_DEBUG_EN == 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #define DEBUG_OUT(...) printf(__VA_ARGS__)
#else
  #define DEBUG_OUT(...)      
#endif


#endif