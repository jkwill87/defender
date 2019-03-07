#include <random>

#include "debug.h"
#include "exec.hpp"
#include "units.hpp"

using namespace std;
extern World world_terrain;
extern World world_units;

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

static int gen_random(int min, int max) {
    static random_device rd;
    static mt19937 seed(rd());
    uniform_int_distribution<> distribution(min, max);
    int result = distribution(seed);
    return result;
}

Coordinate gen_random_coord() {
    int x = gen_random(5, WORLD_XZ - 5);
    int y = gen_random(calc_y_min() + 5, WORLD_Y - 5);
    int z = gen_random(5, WORLD_XZ - 5);
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
            coordinate.x = 5;
            break;
        case 1:
            coordinate.x = WORLD_XZ - 5;
            break;
        case 2:
            coordinate.z = 5;
            break;
        default:
            coordinate.z = WORLD_XZ - 5;
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
