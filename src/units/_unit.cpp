#include <algorithm>
#include <iostream>
#include <random>
#include "debug.h"
#include "units.hpp"

using namespace std;

extern World world_terrain;
extern World world_units;
extern Config config;

static uint8 _gen_random(int min, int max) {
    static random_device rd;
    static mt19937 seed(rd());
    uniform_int_distribution<> distribution(min, max);
    int result = distribution(seed);
    assert_gte(result, 0, "underflow imminent");
    assert_lte(result, 255, "overflow imminent");
    return static_cast<uint8>(result);
}

vector<Unit *> Unit::units;
uint8 Unit::cycle = 0;

Unit::Unit(int x, int y, int z, string name) :
    id(units.size() + 1),
    is_colliding_ground(false),
    is_colliding_unit(false),
    target({x, max(y, WORLD_Y - MAP_CLEAR), z}),
    origin(target),
    as_str(name + " #" + to_string(units.size()))
{
    units.push_back(this);
    assert_gte(x, 0, "x out of bounds");
    assert_gte(y, 0, "y out of bounds");
    assert_gte(z, 0, "z out of bounds");
    assert_lt(x, WORLD_XZ, "x out of bounds");
    assert_lt(y, WORLD_Y, "y out of bounds");
    assert_lt(z, WORLD_XZ, "z out of bounds");
    log("%s placed at {%02d,%02d,%02d}", as_str.c_str(), x, y, z);
}

Unit::~Unit() {
    auto iter = find(units.begin(), units.end(), this);
    if (iter != units.end()) {
        log("%s destroyed", as_str.c_str());
        units.erase(iter);
    } else {
        assert_ok(false, "could not unregister unit");
    }
}

uint8 Unit::calc_min_y(int x, int z) {
    static int minimums[WORLD_XZ][WORLD_XZ] = {{-1}};
    static bool calculated = false;  // only need to compute once
    if (!calculated) {
        Coordinate c;
        for (c.x = 0; c.x < WORLD_XZ; c.x++) {
            for (c.z = 0; c.z < WORLD_XZ; c.z++) {
                for (c.y = WORLD_Y - 1; c.y >= 0; c.y--) {
                    if (world_terrain[c.x][c.y][c.z]) {
                        minimums[c.x][c.z] = c.y;
                        break;
                    }
                }
                assert_gt(minimums[c.x][c.z], -1, "couldn't determine min");
            }
        }
        calculated = true;
    }
    return static_cast<uint8>(minimums[x][z]);
}

uint8 Unit::calc_min_y() {
    static uint8 y_min = 0;
    if (y_min) return y_min;  // only needs to be calculated once
    for (int x = 0; x < WORLD_XZ; x++)
        for (int z = 0; z < WORLD_XZ; z++)
            y_min = max(y_min, calc_min_y(x, z));
    return y_min;
}

coordinate Unit::calc_random_coordinate(bool edge, bool above_terrain) {
    uint8 x = _gen_random(MAP_CLEAR, WORLD_XZ - MAP_CLEAR);
    uint8 z = _gen_random(MAP_CLEAR, WORLD_XZ - MAP_CLEAR);
    const uint8 y_from = above_terrain ? calc_min_y(x, z) : 1;
    const uint8 y_to =WORLD_Y - MAP_CLEAR;
    uint8 y = _gen_random(y_from, y_to);
    Coordinate c = {x, y, z};
    if (edge) {
        switch (_gen_random(0, 4)) {
            case 0:
                c.x = MAP_CLEAR;
                break;
            case 1:
                c.x = WORLD_XZ - MAP_CLEAR;
                break;
            case 2:
                c.z = MAP_CLEAR;
                break;
            default:
                c.z = WORLD_XZ - MAP_CLEAR;
                break;
        }
    }
    bool already_occupied = world_units[c.x][c.y][c.z] ||
        world_terrain[c.x][c.y][c.z];
    return already_occupied ? calc_random_coordinate(edge) : c;
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

Unit *Unit::find_unit(Coordinate coordinate) {
    for (Unit *unit: Unit::units) {
        if (unit->is_occupying(coordinate)) {
            return unit;
        }
    }
    return nullptr;
}

void Unit::ai() {
    if (config.pause_units) return;
    target.x = max(target.x, MAP_CLEAR);
    target.x = min(target.x, WORLD_XZ - 1);
    target.y = max(target.y, MAP_CLEAR);
    target.y = min(target.y, WORLD_Y - 1);
    target.z = max(target.z, MAP_CLEAR);
    target.z = min(target.z, WORLD_XZ - 1);
    if (origin.x - target.x < 0) origin.x++;
    else if (origin.x - target.x > 0) origin.x--;
    if (origin.y - target.y < 0) origin.y++;
    else if (origin.y - target.y > 0)origin.y--;
    if (origin.z - target.z < 0) origin.z++;
    else if (origin.z - target.z > 0)origin.z--;
}

void Unit::render() {
    is_colliding_ground = false;
    is_colliding_unit = false;
    for (auto const &mapping : layout) {
        // Determine colour
        Colour colour = mapping.second;
        // Determine position
        PositionArray positions = mapping.first;
        int x = origin.x + positions[0];
        int y = origin.y + positions[1];
        int z = origin.z + positions[2];
        // Determine if colliding
        if (world_terrain[x][y][z])is_colliding_ground = true;
        else if (world_units[x][y][z]) is_colliding_unit = true;
        // Draw unit
        world_units[x][y][z] = colour;
    }
}

void Unit::shoot() {
    cout << as_str + " shot down" << endl;
    delete this;
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
