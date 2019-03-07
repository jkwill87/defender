#include <sstream>

#include <units.hpp>
#include <exec.hpp>

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
    if (is_at_target()) {
        // Set a new search position
        target = gen_random_edge_coord();
    } else if (!captive) {
        // Attempt to find a human target
        Human *human;
        for (Unit *unit: Unit::units) {
            human = dynamic_cast<Human *>(unit);
            if (!human) continue;
            if (!human->available) continue;
            captive = human;
            human->available = false;
            state = PURSUING;
            log("%s targetted %s", as_str.c_str(), captive->as_str.c_str());
            break;
        }
    }
}

void Lander::ai_pursue() {
    assert_ok(captive, "no target to pursue");
    target.x = captive->origin.x;
    target.z = captive->origin.z;
    move_to_target();
    if (y_distance(captive)) {
        captive->action_lift();
        state = CAPTURING;
        log("%s capturing %s", as_str.c_str(), captive->as_str.c_str());
    }
}

void Lander::ai_capture() {
    assert_ok(captive, "no target to capture");
    if (has_captive()) {
        state = ESCAPING;
    } else if (!getting_captive()) {
        captive = nullptr;
        state = SEARCHING;
        log("%s escaping", as_str.c_str());
    }
}

void Lander::ai_escape() {
    assert_ok(captive, "no target to escape with");
    if (WORLD_Y - origin.y < 0) {
        captive->action_capture();
        state = KILLED;
        log("%s escaped", as_str.c_str());
    } else if (has_captive()) {
        ++target.y;
    } else if (!getting_captive()) {
        captive = nullptr;
        state = SEARCHING;
        log("%s escaping", as_str.c_str());
    }
}

void Lander::animate_rotation() {
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
}

bool Lander::has_captive() {
    return y_distance(captive) < 6;
}

bool Lander::getting_captive() {
    return y_distance(captive) != 0;
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
    animate_rotation();
    Unit::render();
}


void Lander::action_hit() {
    log("%s shot down", as_str.c_str());
    if (captive) {
        captive->action_drop();
    }
    state = KILLED;
}
