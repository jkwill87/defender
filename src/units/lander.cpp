#include <sstream>

#include <units.hpp>

#include "debug.h"

using namespace std;

// External Variable Declarations ----------------------------------------------

extern World world_terrain;


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
}

Lander::Lander(Coordinate coordinate) :
    Lander(coordinate.x, coordinate.y, coordinate.z) {}


// Private Methods -------------------------------------------------------------

void Lander::ai_search() {
    // Attempt to find a human target
    Human *human;
    for (Unit *unit: Unit::units) {
        human = dynamic_cast<Human *>(unit);
        if (!human) continue;
        if (!human->available) continue;
        target = human;
        human->available = false;
        state = PURSUING;
        log("%s targetted %s", as_str.c_str(), target->as_str.c_str());
        break;
    }
}

void Lander::ai_pursue() {
    assert_ok(target, "no target to pursue");
    move_towards(target);
    if (is_over(target)) {
        target->action_lift();
        state = CAPTURING;
    }
}

void Lander::ai_capture() {
    assert_ok(target, "no target to capture");
    if (is_intersecting(target)) {
        target->action_capture();
        target = nullptr;
        state = SEARCHING;
    }
}


// Public Methods --------------------------------------------------------------

void Lander::render() {
    switch (state) {
        case SEARCHING:
            ai_search();
            break;
        case PURSUING:
            ai_pursue();
            break;
        case CAPTURING:
            ai_capture();
            break;
        case KILLED:
            delete this;
            break;
    }
    if (frame_a) {
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
    frame_a = !frame_a;
    Unit::render();

}

void Lander::action_hit() {
    log("%s shot down", as_str.c_str());
    if (target) {
        target->action_drop();
    }
    state = KILLED;
}
