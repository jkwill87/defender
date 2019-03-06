#include <stdbool.h>
#include "stdlib.h"
#include "world.h"

static Unit *units[UNIT_MAX] = {0};


// Eternal Variable Declarations -----------------------------------------------

extern World world;


// Static Function Definitions -------------------------------------------------

bool _unit_at_pos(const Unit *unit, byte x, byte y, byte z) {
    if (x < unit->position.x - 1 || x > unit->position.x + 1) return false;
    if (y < unit->position.y - 1 || y > unit->position.y + 1) return false;
    if (z < unit->position.z - 1 || z > unit->position.z + 1) return false;
    return true;
}

bool _unit_conflicts(const Unit *unit, byte x, byte y, byte z) {
    for (byte ix = (byte) (unit->position.x - 1);
         ix < unit->position.x + 1; ix++) {
        for (byte iy = (byte) (unit->position.y - 1);
             ix < unit->position.z + 1; ix++) {
            for (byte iz = (byte) (unit->position.y - 1);
                 ix < unit->position.z + 1;
                 ix++) {
                // assert_eq entity is in bounds of game world
                if (ix < 0 || ix >= WORLD_XZ) return true;
                if (iz < 0 || iz >= WORLD_XZ) return true;
                if (iy < 0 || iy >= WORLD_Y) return true;
                // assert_eq not already occupied by another unit or world
                if (world[ix][iy][iz] != TRANSPARENT) return true;
            }
        }
    }
    return false;
}

Unit *_unit_iter(bool reset) {
    static unsigned i = 0;
    if (reset) i = 0;
    for (; i < UNIT_MAX - 1; i++) {
        if (units[i] != NULL) break;
    }
    return units[i++];
}

Unit *_create_human(unsigned x, unsigned y, unsigned z) {
    Unit *human = calloc(1, sizeof(Unit));
    human->entity = HUMAN;
    human->position = (Position) {x, y, z};
    human->block[1][0][1] = GREEN;
    human->block[1][1][1] = RED;
    human->block[1][2][1] = ORANGE;
    human->durability = 50;
    return human;
}

Unit *_create_ufo(unsigned x, unsigned y, unsigned z) {
    Unit *ufo = calloc(1, sizeof(Unit));
    ufo->entity = UFO;
    ufo->position = (Position) {x, y, z};
    ufo->block[0][0][1] = BLUE;
    ufo->block[1][0][0] = BLUE;
    ufo->block[1][0][2] = BLUE;
    ufo->block[2][0][1] = BLUE;
    ufo->durability = 100;
    return ufo;
}


// Public Function Definitions -------------------------------------------------

Unit *unit_next() { return _unit_iter(false); }

Unit *unit_first() { return _unit_iter(true); }

Unit *unit_find(byte x, byte y, byte z) {
    for (int i = 0; i < UNIT_MAX; i++) {
        if (_unit_at_pos(units[i], x, y, z)) return units[i];
    }
    return NULL;  // signals unit not found
}

bool unit_rm(byte x, byte y, byte z) {
    for (int i = 0; i < UNIT_MAX; i++) {
        if (_unit_at_pos(units[i], x, y, z)) {
            free(units[i]);
            units[i] = NULL;
            return true;
        }
    }
    return false;  // unit does not exist
}

void unit_clear() {
    for (int i = 0; i < UNIT_MAX; i++) {
        if (units[i] != NULL) {
            free(units[i]);
            units[i] = NULL;
        }
    }
}

bool unit_add(Entity entitiy, byte x, byte y, byte z) {
    unsigned i = 0;
    do {
        if (units[i] == NULL) break;
    } while (++i < UNIT_MAX);
    if (units[i] == NULL) {
        switch (entitiy) {
            case HUMAN:
                units[i] = _create_human(x, y, z);
                break;
            case UFO:
                units[i] = _create_ufo(x, y, z);
                break;
        }
        return true;
    }
    return false;  // cannot add unit, exceeded UNIT_MAX
}
