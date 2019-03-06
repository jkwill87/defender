#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

#ifndef NDEBUG
#define assert_eq(a, b, msg) if((a)!=(b)){fprintf(stderr,"error: %s:%d %s(): %s\n",__FILE__,__LINE__,__func__,msg);abort();}
#define assert_ok(a, msg) if(!(a)){fprintf(stderr,"error: %s:%d %s(): %s\n",__FILE__,__LINE__,__func__,msg);abort();}

#else
#define assert_eq(conf,msg) ;
#define assert_ok(conf,msg) ;
#endif
