#include <sstream>

#include "debug.h"
#include "units.hpp"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern World world_terrain;
extern World world_units;


// Constructor Definition ------------------------------------------------------

Lander::Lander(int x, int y, int z) : Unit(x, y, z, "lander") {
    layout[{-2, -1, +0}] = COLOUR_GREEN;
    layout[{+2, -1, +0}] = COLOUR_GREEN;
    layout[{+0, -1, -2}] = COLOUR_GREEN;
    layout[{+0, -1, +2}] = COLOUR_GREEN;
    layout[{+0, +1, +0}] = COLOUR_GREEN;
    layout[{-1, +1, +0}] = COLOUR_GREEN;
    layout[{+1, +1, +0}] = COLOUR_GREEN;
    layout[{+0, +2, +0}] = COLOUR_YELLOW;
    origin.y = max(origin.y, calc_min_y() + MAP_CLEAR);
    target = calc_random_coordinate(true);
}

Lander::Lander(Coordinate coordinate) :
    Lander(coordinate.x, coordinate.y, coordinate.z) {}


Lander::Lander() : Lander(calc_random_coordinate()) {}


// Private Method Definitions --------------------------------------------------

void Lander::look() {
    if (WORLD_Y - origin.y < 3) {
        state = EXITED;
        return;
    }
    int captive_distance = y_distance(captive);
    if (captive_distance > 0 && captive_distance < MAP_CLEAR * 2) {
        state = ESCAPING;
        return;
    }
    if (captive_distance > 0) {
        state = CAPTURING;
        return;
    }
    Coordinate idx1;  // coordinate from
    idx1.x = max(origin.x - LANDER_VISIBILITY, 0);
    idx1.z = max(origin.z - LANDER_VISIBILITY, 0);
    idx1.y = 0;
    Coordinate idx2;  // coordinate to
    idx2.x = min(origin.x + LANDER_VISIBILITY, WORLD_XZ - 1);
    idx2.z = min(origin.z + LANDER_VISIBILITY, WORLD_XZ - 1);
    idx2.y = origin.y;
    Coordinate idx;  // coordinate counter
    for (idx.x = idx1.x; idx.x < idx2.x; idx.x++) {
        for (idx.z = idx1.z; idx.z < idx2.z; idx.z++) {
            for (idx.y = idx1.y; idx.y < idx2.y; idx.y++) {
                if (world_units[idx.x][idx.y][idx.z]) {
                    auto human = dynamic_cast<Human *>(find_unit(idx));
                    if (human && human->available) {
                        state = PURSUING;
                        captive = human;
                        return;
                    }
                }
            }
        }
    }
    state = SEARCHING;
}


// Public Method Definitions ---------------------------------------------------

void Lander::ai() {
    switch (state) {
        case SEARCHING:
            if (
                origin.x <= MAP_CLEAR || origin.x >= WORLD_XZ - MAP_CLEAR ||
                origin.z <= MAP_CLEAR || origin.z >= WORLD_XZ - MAP_CLEAR
                ) {
                log("%s searching elsewhere", as_str.c_str());
                target = calc_random_coordinate(true);
                target.y = min(origin.y + 1, WORLD_Y - MAP_CLEAR);
            }
            break;
        case PURSUING:
            target.x = captive->origin.x;
            target.z = captive->origin.z;
            target.y = min(captive->origin.y + MAP_CLEAR, WORLD_Y - MAP_CLEAR);
            break;
        case CAPTURING:
            target.y = min(captive->origin.y + MAP_CLEAR, WORLD_Y - MAP_CLEAR);
            break;
        case ESCAPING:
            if (cycle % 10 == 0) {
                ++target.y;
                captive->action_lift();
            }
            break;
        case EXITED:
            captive->action_capture();
            to_remove=true;
            log("%s escaped", as_str.c_str());
            return;
        case KILLED:
            to_remove=true;
            log("%s killed", as_str.c_str());
            return;
    }
    look();
    Unit::ai();
}

void Lander::render() {
    if (cycle % 2) {
        layout[{+0, +0, +1}] = COLOUR_GREEN;
        layout[{+0, +0, -1}] = COLOUR_GREEN;
        layout[{-1, +0, +0}] = COLOUR_YELLOW;
        layout[{+1, +0, +0}] = COLOUR_YELLOW;
    } else {
        layout[{+0, +0, +1}] = COLOUR_YELLOW;
        layout[{+0, +0, -1}] = COLOUR_YELLOW;
        layout[{-1, +0, +0}] = COLOUR_GREEN;
        layout[{+1, +0, +0}] = COLOUR_GREEN;
    }
    Unit::render();
}

void Lander::shoot() {
    if (captive) captive->action_drop();
    state = KILLED;
    Unit::shoot();
}
