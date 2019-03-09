#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "types.h"


#ifndef NDEBUG
    #define assert_ok(a, msg)if(!(a)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define assert_not(a, msg)if((a)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define assert_eq(a, b, msg)if((a)!=(b)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define assert_gt(a, b, msg)if((a)<=(b)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define assert_gte(a, b, msg)if((a)<(b)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define assert_lt(a, b, msg)if((a)>=(b)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define assert_lte(a, b, msg)if((a)>(b)){fprintf(stderr,"error: %s:%d %s()\n\t%s\n\n",__FILE__,__LINE__,__func__,(msg));abort();}
    #define log(...)fprintf(stderr,"notice: %s:%d %s()\n\t",__FILE__,__LINE__,__func__);fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n\n");
    #define log_fps(frame,time,base){printf("FPS: %4.2f\n",frame*1000.0f/(time-base));}
#else
    #define assert_ok(a, msg)((void)0)
    #define assert_not(a, msg)((void)0)
    #define assert_eq(a, b, msg)((void)0)
    #define assert_gt(a, b, msg)((void)0)
    #define assert_gte(a, b, msg)((void)0)
    #define assert_lt(a, b, msg)((void)0)
    #define assert_lte(a, b, msg)((void)0)
    #define log(...)((void)0)
    #define log_fps(frame, time, base){printf("\rFPS: %4.2f",frame*1000.0f/(time-base));fflush(stdout);}
#endif
