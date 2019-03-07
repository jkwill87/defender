#include <units.hpp>
#include "debug.h"

using namespace std;


// External Variable Declarations ----------------------------------------------

static const int origin_offset = 2;


// External Variable Declarations ----------------------------------------------

extern World world_terrain;


// Constructor Definition ------------------------------------------------------

Human::Human(int x, int y, int z) : Unit(x, y, z, "human") {
    layout[{+0, -1, +0}] = COLOUR_GREEN;
    layout[{+0, +0, +0}] = COLOUR_RED;
    layout[{+0, +1, +0}] = COLOUR_ORANGE;
    terrain_height = origin_offset;
    for (; y > origin_offset; y--) {
        if (world_terrain[x][y][z]) {
            terrain_height += y;
            break;
        }
    }
    origin.y=target.y = terrain_height;
}

Human::Human(Coordinate coordinate) :
    Human(coordinate.x, coordinate.y, coordinate.z) {}


// Private Methods -------------------------------------------------------------

void Human::ai_settled() {
    if (fall_height >= 5) {
        state = KILLED;
    }
}

void Human::ai_floating() {
    animate_floating();
    ++target.y;
    assert_lte(target.y - origin_offset, WORLD_Y, "out of bounds");
}

void Human::ai_falling() {
    if (origin.y == terrain_height) {
        state = SETTLED;
    } else {
        ++fall_height;
        --target.y;
        assert_gte(origin.y, 0, "out of bounds");
    }
}

void Human::animate_floating() {
    Layout next_layout;
    next_layout[{+0, -1, +0}] = layout[{+0, +0, +0}];
    next_layout[{+0, +0, +0}] = layout[{+0, +1, +0}];
    next_layout[{+0, +1, +0}] = layout[{+0, -1, +0}];
    layout.swap(next_layout);
    if (cycle % 2) return; // elevate once every n cycles
}


// Public Methods --------------------------------------------------------------

void Human::render() {
    switch (state) {
        case SETTLED:
            ai_settled();
            break;
        case FALLING:
            ai_falling();
            break;
        case FLOATING:
            ai_floating();
            break;
        case KILLED:
            remove();
            return;
    }
    Unit::render();
}

void Human::action_capture() {
    log("%s captured", as_str.c_str());
    available = false;
    state = KILLED;
}

void Human::action_drop() {
    log("%s dropped", as_str.c_str());
    fall_height = 0;
    available = true;
    state = FALLING;
}

void Human::action_lift() {
    log("%s lifted", as_str.c_str());
    available = false;
    state = FLOATING;
}

void Human::action_hit() {
    log("%s shot down", as_str.c_str());
    available = false;
    state = KILLED;
}
