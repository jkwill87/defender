#include <cstring>

#include "debug.h"
#include "exec.h"
#include "units.hpp"

extern World world_units;
extern Laser laser;

using namespace std;

void unit_cycle() {
    // Increment timer cycle count
    ++Unit::cycle;
    // Update unit position buffer
    memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);
    for (long i = Unit::units.size(); i > 0; i--) {
        Unit::units[i - 1]->render();
    }
    // Process unit ai
    for (long i = Unit::units.size(); i > 0; i--) {
        Unit::units[i - 1]->ai();
    }
    // Check if laser intersects any units
    if (!laser.active) return;
}

void unit_rm_all() {
    for (long i = Unit::units.size(); i > 0; i--) {
        delete Unit::units[i - 1];  // remove in reverse to avoid read violation
    }
}
