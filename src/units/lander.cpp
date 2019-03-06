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
    if (y_distance(target)) {
        target->action_lift();
        state = CAPTURING;
        log("%s capturing %s", as_str.c_str(), target->as_str.c_str());
    }
}

void Lander::ai_capture() {
    assert_ok(target, "no target to capture");
    const int target_distance = y_distance(target);
    if (target_distance < 6) {
        state = ESCAPING;
    } else if (!target_distance) {
        target = nullptr;
        state = SEARCHING;
        log("%s escaping", as_str.c_str());
    }
}

void Lander::ai_escape() {
    assert_ok(target, "no target to escape with");
    if (WORLD_Y - origin.y < 0) {
        target->action_capture();
        state = KILLED;
        log("%s escaped", as_str.c_str());
    } else {
        ++origin.y;
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
        case ESCAPING:
            ai_escape();
            break;
        case KILLED:
            remove();
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
