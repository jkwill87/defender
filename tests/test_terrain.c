
#include "graphics.h"

extern Pgm terrain;

int main() {
    terrain.z = 5;
    terrain.x = 5;
    terrain.y = 25;
    for (unsigned i = 0; i < 25; i++) terrain.data[i] = i + 1;
    // test pgm_set_world_terrain
    pgm_set_world_terrain();
}
