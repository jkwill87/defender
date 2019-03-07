#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif


// Function Declarations -------------------------------------------------------

void unit_cycle();
void unit_rm_all();
void place_random_humans(byte count);
void place_random_aliens(byte count);
byte gen_random(int min, int max);
Coordinate gen_random_coord();
Coordinate gen_random_edge_coord();
byte calc_y_min(int x, int y);

#ifdef __cplusplus
}
#endif
