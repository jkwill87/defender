#pragma once

#include "types.h"


// PGM Function Declarations ---------------------------------------------------

void pgm_init(char *path);
unsigned pgm_calc_ceil();
unsigned pgm_get_y_value(unsigned x, unsigned z);
void pgm_set_world_terrain();


// Unit Function Declarations --------------------------------------------------

Unit *unit_next();
Unit *unit_first();
Unit *unit_find(byte x, byte y, byte z);
bool unit_rm(byte x, byte y, byte z);
void unit_clear();
bool unit_add(Entity entitiy, byte x, byte y, byte z);
