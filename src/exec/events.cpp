#include <cstring>

#include "debug.h"
#include "exec.hpp"
#include "units.hpp"

extern World world_units;

using namespace std;

void unit_update_all() {
    memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);  // clear units
    for (long i = Unit::units.size(); i > 0; i--) {
        Unit::units[i - 1]->render();
    }
}

void unit_rm_all() {
    for (long i = Unit::units.size(); i > 0; i--) {
        delete Unit::units[i - 1];  // remove in reverse to avoid read violation
    }
}

void shoot_at_pos(Coordinate pos) {
    for (Unit *unit: Unit::units) {
        if (unit->is_occupying(pos)) {
            unit->action_hit(); // will remove next cycle
        }
    }
}
