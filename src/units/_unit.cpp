#include <algorithm>
#include <random>
#include <units.hpp>

#include "debug.h"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern World world_terrain;
extern World world_units;


// Static Function Definitions -------------------------------------------------

static byte _calc_y_min(int x, int z) {
    static int minimums[WORLD_XZ][WORLD_XZ] = {{-1}};
    static bool calculated = false;  // only need to compute once
    if (!calculated) {
        Coordinate c;
        for (c.x = 0; c.x >= WORLD_XZ; c.x++) {
            for (c.z = 0; c.z >= WORLD_XZ; c.z++) {
                for (c.y = WORLD_Y - 1; c.y >= 0; c.y--) {
                    if (world_terrain[c.x][c.y][c.z]) {
                        minimums[c.x][c.z] = c.y;
                        break;
                    }
                    assert_gt(minimums[c.x][c.z], -1, "couldn't determine min");
                }
            }
        }
    }
    return static_cast<byte>(minimums[x][z]);
}

static byte _gen_random(int min, int max) {
    static random_device rd;
    static mt19937 seed(rd());
    uniform_int_distribution<> distribution(min, max);
    int result = distribution(seed);
    assert_gte(result, 0, "underflow imminent");
    assert_lte(result, 255, "overflow imminent");
    return static_cast<byte>(result);
}


// Static Variable Definitions -------------------------------------------------

byte Unit::cycle = 0;
long Unit::id_counter=0;

// Constructor Definitions -----------------------------------------------------

Unit::Unit(int x, int y, int z) :
    id(id_counter++),
    target({x, max(y, WORLD_Y - MAP_CLEAR), z}),
    origin(target)
{
    assert_gte(x, 0, "x out of bounds");
    assert_gte(y, 0, "y out of bounds");
    assert_gte(z, 0, "z out of bounds");
    assert_lt(x, WORLD_XZ, "x out of bounds");
    assert_lt(y, WORLD_Y, "y out of bounds");
    assert_lt(z, WORLD_XZ, "z out of bounds");
}


// Protected Method Definitions ------------------------------------------------

byte Unit::calc_min_y() {
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

coordinate Unit::calc_random_coordinate(bool edge) {
    byte x = _gen_random(MAP_CLEAR, WORLD_XZ - MAP_CLEAR);
    byte z = _gen_random(MAP_CLEAR, WORLD_XZ - MAP_CLEAR);
    byte y = _gen_random(_calc_y_min(x, z) + MAP_CLEAR, WORLD_Y - MAP_CLEAR);
    Coordinate coordinate = {x, y, z};
    if (edge) {
        switch (_gen_random(0, 4)) {
            case 0:
                coordinate.x = MAP_CLEAR;
                break;
            case 1:
                coordinate.x = WORLD_XZ - MAP_CLEAR;
                break;
            case 2:
                coordinate.z = MAP_CLEAR;
                break;
            default:
                coordinate.z = WORLD_XZ - MAP_CLEAR;
                break;
        }
    }

    bool already_occupied =
        world_units[coordinate.x][coordinate.y][coordinate.z] ||
        world_terrain[coordinate.x][coordinate.y][coordinate.z];
    return already_occupied ? calc_random_coordinate(edge) : coordinate;
}

int Unit::y_distance(const Unit *target) {
    int distance = 0;
    if (target && origin.x == target->origin.x &&
        origin.z == target->origin.z &&
        origin.y > target->origin.y) {
        return origin.y - target->origin.y;
    }
    return distance;
}


// Public Method Definitions ---------------------------------------------------

void Unit::ai() {
    if (origin.x - target.x < 0) origin.x++;
    else if (origin.x - target.x > 0) origin.x--;
    if (origin.y - target.y < 0) origin.y++;
    else if (origin.y - target.y > 0)origin.y--;
    if (origin.z - target.z < 0) origin.z++;
    else if (origin.z - target.z > 0)origin.z--;
}

void Unit::render() {
    for (auto const &mapping : layout) {
        PositionArray positions = mapping.first;
        int x = origin.x + positions[0];
        int y = origin.y + positions[1];
        int z = origin.z + positions[2];
        Colour colour = mapping.second;
        world_units[x][y][z] = colour;
    }
}

void Unit::shoot() {
    log("%s shot down", as_str().c_str());
}

bool Unit::is_occupying(Coordinate &pos) {
    for (auto const &mapping : layout) {
        PositionArray positions = mapping.first;
        if (pos.x != origin.x + positions[0]) continue;
        if (pos.y != origin.y + positions[1]) continue;
        if (pos.z != origin.z + positions[2]) continue;
        return true;
    }
    return false;
}
