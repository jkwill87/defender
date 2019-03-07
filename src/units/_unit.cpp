#include <algorithm>
#include <units.hpp>


#include "debug.h"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern World world_terrain;
extern World world_units;


// Constructor Definitions -----------------------------------------------------

vector<Unit *> Unit::units;

Unit::Unit(int x, int y, int z, string name) :
    id(units.size()),
    target({x, y, z}),
    origin({x, y, z}),
    as_str(name + " id:" + to_string(units.size())) {
    units.push_back(this);
    assert_gte(x, 0, "x out of bounds");
    assert_gte(y, 0, "y out of bounds");
    assert_gte(z, 0, "z out of bounds");
    assert_lt(x, WORLD_XZ, "x out of bounds");
    assert_lt(y, WORLD_Y, "y out of bounds");
    assert_lt(z, WORLD_XZ, "z out of bounds");
    log("%s placed at {%02d,%02d,%02d}", as_str.c_str(), x, y, z);
}

Unit::Unit(Coordinate coordinate, string name) : Unit(
    coordinate.x,
    coordinate.y,
    coordinate.z,
    name
) {}


Unit::~Unit() {
    auto iter = find(units.begin(), units.end(), this);
    if (iter != units.end()) {
        log("%s destroyed", as_str.c_str());
        units.erase(iter);
    } else {
        assert_ok(false, "could not unregister unit");
    }
}


// Method Definitions ----------------------------------------------------------

void Unit::render() {
    for (auto const &mapping : layout) {
        PositionArray positions = mapping.first;
        int x = origin.x + positions[0];
        int y = origin.y + positions[1];
        int z = origin.z + positions[2];
        Colour colour = mapping.second;
        world_units[x][y][z] = colour;
    }
    ++cycle;
}

void Unit::remove() {
    const auto vec_addr = std::find(units.begin(), units.end(), this);
    units.erase(vec_addr);
}

int Unit::y_distance(const Unit *target) {
    int distance = 0;
    if (origin.x == target->origin.x && origin.z == target->origin.z &&
        origin.y > target->origin.y) {
        return origin.y - target->origin.y;
    }
    return distance;
}

bool Unit::is_at_target() {
    return origin.x == target.z && origin.y == target.y && origin.z == target.z;
}

void Unit::move_to_target() {
    if (origin.x - target.x < 0) origin.x++;
    else if (origin.x - target.x > 0) origin.x--;
    if (origin.z - target.z < 0) origin.z++;
    else if (origin.z - target.z > 0)origin.z--;
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
