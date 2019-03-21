/**
 * lander.cpp
 *
 * Lander class derived from the Unit base class.
 */

#include <algorithm>
#include "debug.h"
#include "units.hpp"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern World world_terrain;
extern World world_units;
extern Position player_pos;


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
    origin.y = min(origin.y, (int) calc_min_y());
    new_search_path();
}

Lander::Lander(Coordinate coordinate)
    : Lander(coordinate.x, coordinate.y, coordinate.z) {
}


Lander::Lander() : Lander(calc_random_coordinate()) {
}

Lander::~Lander() {
    if (captive) captive->action_drop();
}


// Private Method Definitions --------------------------------------------------

void Lander::new_search_path() {
    log("%s searching elsewhere", as_str.c_str());
    abandon_captive();
    target = calc_random_coordinate(true);  // along edge
    target.y = origin.y;
}

void Lander::set_captive(Human *human) {
    assert_ok(human, "unable to set captive");
    captive = human;
    captive->available = false;
}

void Lander::decide_next() {
    if (state >= ATTACKING) return;
    Human *human;
    if (can_exit()) {
        state = EXITED;
    } else if (can_escape()) {
        state = ESCAPING;
    } else if (can_capture()) {
        state = CAPTURING;
    } else if (can_pursue(&human)) {
        state = PURSUING;
        set_captive(human);
    } else {
        state = SEARCHING;
    }
}

// Actions
void Lander::action_search() {
    if (origin.x <= MAP_CLEAR || origin.x >= WORLD_XZ - MAP_CLEAR ||
        origin.z <= MAP_CLEAR || origin.z >= WORLD_XZ - MAP_CLEAR)
        new_search_path();
}

void Lander::action_bounce_ground() {
    log("%s hitting ground", as_str.c_str());
    origin.y++;
    target.y += 5;
    if (captive) captive->action_lift();
}

void Lander::action_bounce_unit() {
    log("%s hitting unit", as_str.c_str());
    target.x = WORLD_XZ - target.x;
    target.z = WORLD_XZ - target.z;
    target.y++;
}

void Lander::action_pursue() {
    target.x = captive->origin.x;
    target.z = captive->origin.z;
    target.y = captive->origin.y + MAP_CLEAR;
}

void Lander::action_capture() {
    target.y = captive->origin.y + MAP_CLEAR;
}

void Lander::action_escape() {
    if (cycle % 10 != 0) return;
    ++target.y;
    captive->action_lift();
}

void Lander::action_exit() {
    log("%s escaped with %s", as_str.c_str(), captive->as_str.c_str());
    captive->action_capture();
    abandon_captive();
    state = ATTACKING;
    layout[{-2, -1, +0}] = COLOUR_RED;
    layout[{+2, -1, +0}] = COLOUR_RED;
    layout[{+0, -1, -2}] = COLOUR_RED;
    layout[{+0, -1, +2}] = COLOUR_RED;
    layout[{+0, +1, +0}] = COLOUR_RED;
    layout[{-1, +1, +0}] = COLOUR_RED;
    layout[{+1, +1, +0}] = COLOUR_RED;
}

void Lander::action_attack() {
    if (origin.x==target.x&&origin.y==target.y&&origin.z==target.z)
        target=calc_random_coordinate();
    if (origin.x+player_pos.x<LANDER_VISIBILITY && origin.z+player_pos.z<LANDER_VISIBILITY){
        log("%s attacked player", as_str.c_str());
    }
}

void Lander::action_kill() {
    log("%s killed", as_str.c_str());
    delete this;
}

// Deciders
bool Lander::can_escape() {
    int captive_distance = y_distance(captive);
    return captive && captive_distance > 0 && captive_distance < MAP_CLEAR * 2;
}

bool Lander::can_capture() {
    return y_distance(captive) > 0;
}

bool Lander::can_pursue(Human **human) {
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
                    *human = dynamic_cast<Human *>(find_unit(idx));
                    if (*human && (*human)->available) return true;
                }
            }
        }
    }
    return false;
}

bool Lander::can_shoot_player() {
    return false;
}

bool Lander::can_exit() {
    return captive && WORLD_Y - origin.y < MAP_CLEAR * 2;
}


// Public Method Definitions ---------------------------------------------------

void Lander::ai() {
    switch (state) {
        case SEARCHING:
            action_search();
            break;
        case PURSUING:
            action_pursue();
            break;
        case CAPTURING:
            action_capture();
            break;
        case ESCAPING:
            action_escape();
            break;
        case EXITED:
            action_exit();
            break;
        case ATTACKING:
            action_attack();
            break;
        case KILLED:
            action_kill();
            return;
    }
    decide_next();
    if (is_colliding_ground) action_bounce_ground();
    if (is_colliding_unit) action_bounce_unit();
    Unit::ai();
}

void Lander::render() {
    Colour base = state == ATTACKING ? COLOUR_RED : COLOUR_GREEN;
    if (cycle % 2) {
        layout[{+0, +0, +1}] = base;
        layout[{+0, +0, -1}] = base;
        layout[{-1, +0, +0}] = COLOUR_YELLOW;
        layout[{+1, +0, +0}] = COLOUR_YELLOW;
    } else {
        layout[{+0, +0, +1}] = COLOUR_YELLOW;
        layout[{+0, +0, -1}] = COLOUR_YELLOW;
        layout[{-1, +0, +0}] = base;
        layout[{+1, +0, +0}] = base;
    }
    Unit::render();
}

void Lander::abandon_captive() {
    captive = nullptr;
}
