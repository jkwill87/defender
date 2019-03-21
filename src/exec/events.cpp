/**
 * events.cpp
 *
 * Controls the game's event loop.
 */

#include <cmath>
#include <cstring>
#include "debug.h"
#include "exec.h"
#include "units.hpp"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern Laser laser;
extern Position player_pos;
extern View view;
extern World world_units;


// Static Function Definitions 0------------------------------------------------

static void _render() {
    memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);
    for (long i = Unit::units.size(); i > 0; i--) {
        Unit::units[i - 1]->render();
    }
}

static void _damage() {
    if (laser.active) {
        float rot_x = (view.cam_x / 180.0f * PI);
        float rot_y = (view.cam_y / 180.0f * PI);
        for (int i = 0; i < WORLD_XZ * WORLD_XZ; i++) {
            Position pt = {
                (player_pos.x - sinf(rot_y) * i) * -1,
                (player_pos.y + sinf(rot_x) * i) * -1,
                (player_pos.z + cosf(rot_y) * i) * -1
            };
            for (long j = Unit::units.size(); j > 0; j--) {
                Unit *unit = Unit::units[j - 1];
                if (fabs(unit->origin.x - pt.x) <= 2 &&
                    fabs(unit->origin.y - pt.y) <= 2 &&
                    fabs(unit->origin.z - pt.z) <= 2)
                    unit->shoot();
            }
        }
    }
}

static void _react() {
    for (long i = Unit::units.size(); i > 0; i--) {
        Unit::units[i - 1]->ai();
    }
}


// Function Definitions --------------------------------------------------------

void unit_cycle() {
    ++Unit::cycle;
    _render();
    _damage();
    _react();
}

void unit_rm_all() {
    for (long i = Unit::units.size(); i > 0; i--) {
        delete Unit::units[i - 1];  // remove in reverse to avoid read violation
    }
}
