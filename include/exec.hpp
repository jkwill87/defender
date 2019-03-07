#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif


// Function Declarations -------------------------------------------------------

void unit_update_all();
void unit_rm_all();
void place_random_humans(byte count);
void place_random_aliens(byte count);
Coordinate gen_random_coord();
Coordinate gen_random_edge_coord();

#ifdef __cplusplus
}
#endif
