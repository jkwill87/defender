#include <cstring>
#include <cmath>
#include <vector>

#include "debug.h"
#include "exec.h"
#include "units.hpp"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern Laser laser;
extern Position player_pos;
extern View view;
extern World world_units;

typedef vector<Unit *> UnitVector;


// Static Variable Declarations ----------------------------------------------

static vector<Lander*> landers;
static vector<Human*> humans;


// Static Function Definitions -------------------------------------------------

static UnitVector _combine_units() {
    vector<Unit *> units;
    units.reserve(landers.size() + humans.size());
    units.insert(units.end(), humans.begin(), humans.end());
    units.insert(units.end(), landers.begin(), landers.end());
    return units;
}

static void _render(UnitVector &units) {
    memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);
    for (long i = units.size(); i > 0; i--) {
        units[i - 1]->render();
    }
}

static void _damage(UnitVector &units) {
    if (laser.active) {
        float rot_x = (view.cam_x / 180.0f * PI);
        float rot_y = (view.cam_y / 180.0f * PI);
        for (int i = 0; i < WORLD_XZ * WORLD_XZ; i++) {
            Position pt = {
                (player_pos.x - sinf(rot_y) * i) * -1,
                (player_pos.y + sinf(rot_x) * i) * -1,
                (player_pos.z + cosf(rot_y) * i) * -1
            };
            for (long j = units.size(); j > 0; j--) {
                Unit *unit = units[j - 1];
                if (
                    fabs(unit->origin.x - pt.x) <= 2 &&
                    fabs(unit->origin.y - pt.y) <= 2 &&
                    fabs(unit->origin.z - pt.z) <= 2
                    )
                    unit->shoot();
            }
        }
    }
}

static void _react(UnitVector &units) {
    for (long i = units.size(); i > 0; i--) {
        units[i - 1]->ai();
    }
}

static void _remove(UnitVector &units){
    //for (long i = units.size(); i > 0; i--) {
    //    unit = units[i - 1];
    //    if (unit->to_remove){
    //
    //    }
    //}
}

// Function Definitions --------------------------------------------------------

void unit_cycle() {
    ++Unit::cycle;
    UnitVector units = _combine_units();
    memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);
    for (long i = units.size(); i > 0; i--) {
        units[i - 1]->render();
    }
    if (laser.active) {
        float rot_x = (view.cam_x / 180.0f * PI);
        float rot_y = (view.cam_y / 180.0f * PI);
        for (int i = 0; i < WORLD_XZ * WORLD_XZ; i++) {
            Position pt = {
                (player_pos.x - sinf(rot_y) * i) * -1,
                (player_pos.y + sinf(rot_x) * i) * -1,
                (player_pos.z + cosf(rot_y) * i) * -1
            };
            for (long j = units.size(); j > 0; j--) {
                Unit *unit = units[j - 1];
                if (
                    fabs(unit->origin.x - pt.x) <= 2 &&
                    fabs(unit->origin.y - pt.y) <= 2 &&
                    fabs(unit->origin.z - pt.z) <= 2
                    )
                    unit->shoot();
            }
        }
    }
    for (long i = units.size(); i > 0; i--) {
        units[i - 1]->ai();
    }
    Unit * unit;
    for (long i = units.size(); i > 0; i--) {
        unit = units[i - 1];
        if (unit->to_remove){

        }
    }
}

void unit_init() {
    for (int i = 0; i < HUMAN_COUNT; i++) {
        landers.emplace_back(new Lander());
    }
    for (int i = 0; i < LANDER_COUNT; i++) {
        humans.emplace_back(new Human());
    }
}

void unit_purge() {
    for (long i = humans.size(); i > 0; i--) {
        delete humans[i - 1];  // remove in reverse to avoid read violation
    }
    for (long i = landers.size(); i > 0; i--) {
        delete landers[i - 1];  // remove in reverse to avoid read violation
    }
    landers.clear();
    humans.clear();
}

Human * find_human(Coordinate coordinate){
    Human*human;
    for (long i = humans.size(); i > 0; i--) {
         human = humans[i - 1];
        if(human->is_occupying(coordinate)){
            return human;
        }
    }
    return nullptr;
}