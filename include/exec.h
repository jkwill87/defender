#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif


// Function Declarations -------------------------------------------------------

void unit_cycle();
void unit_purge();
void unit_init();
Human * find_human(Coordinate coordinate);

#ifdef __cplusplus
}
#endif
