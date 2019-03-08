#include <random>

#include "debug.h"
#include "exec.h"
#include "units.hpp"

extern World world_terrain;
extern World world_units;

using namespace std;

static int minimums[WORLD_XZ][WORLD_XZ] = {{-1}};

byte calc_y_min(int x, int z) {
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

byte calc_y_min_global() {
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

byte gen_random(int min, int max) {
    static random_device rd;
    static mt19937 seed(rd());
    uniform_int_distribution<> distribution(min, max);
    int result = distribution(seed);
    assert_gte(result, 0, "underflow imminent");
    assert_lte(result, 255, "overflow imminent");
    return static_cast<byte>(result);
}

Coordinate gen_random_coord() {
    byte x = gen_random(MAP_CLEAR, WORLD_XZ - MAP_CLEAR);
    byte z = gen_random(MAP_CLEAR, WORLD_XZ - MAP_CLEAR);
    byte y = gen_random(calc_y_min(x, z) + MAP_CLEAR, WORLD_Y - MAP_CLEAR);
    Coordinate coordinate = {x, y, z};
    bool already_occupied =
        world_units[coordinate.x][coordinate.y][coordinate.z] ||
        world_terrain[coordinate.x][coordinate.y][coordinate.z];
    return already_occupied ? gen_random_coord() : coordinate;
}

Coordinate gen_random_edge_coord() {
    Coordinate coordinate = gen_random_coord();
    switch (gen_random(0, 4)) {
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
    bool already_occupied =
        world_units[coordinate.x][coordinate.y][coordinate.z] ||
        world_terrain[coordinate.x][coordinate.y][coordinate.z];
    return already_occupied ? gen_random_edge_coord() : coordinate;
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
