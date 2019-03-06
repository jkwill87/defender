#include <cstring>
#include <random>

#include "debug.h"
#include "exec.hpp"
#include "units.hpp"

extern World world_units;
extern World world_terrain;

using namespace std;

static byte calc_y_min() {
    static byte y_min = 0;
    if (y_min) return y_min;  // only needs to be calculated once
    for (int x = 0; x < WORLD_XZ; x++) {
        for (int z = 0; z < WORLD_XZ; z++) {
            for (int y = y_min + 1; y < WORLD_Y; y++) {
                if (world_terrain[x][y][x]) y_min = static_cast<byte>(y);
                break;
            }
        }
    }
    return y_min;
}

static Coordinate gen_random_coord() {
    static random_device rd;
    static mt19937 seed(rd());
    uniform_int_distribution<> xz_dist(3, WORLD_XZ - 3);
    uniform_int_distribution<> y_dist(calc_y_min() + 3, WORLD_Y - 3);
    Coordinate coordinate = {xz_dist(seed), y_dist(seed), xz_dist(seed)};
    return coordinate;
}

void unit_update_all() {
    memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);  // clear units
    for (float i = Unit::units.size(); i > 0; i--) {
        Unit::units[i - 1]->render();
    }
}

void unit_rm_all() {
    for (float i = Unit::units.size(); i > 0; i--) {
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

void place_random_humans(byte count) {
    for (int i = 0; i < count; i++) {
        new Human(gen_random_coord());
    }

}

void place_random_aliens(byte count) {
    for (int i = 0; i < count; i++) {
        new Lander(gen_random_coord());
    }
}
