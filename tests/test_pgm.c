#include <stdio.h>
#include "graphics.h"
#include "debug.h"

// Macro Definitions -----------------------------------------------------------

#define YELLOW "\033[33m"
#define MEDIUM "\033[48;5;%dm"
#define RESET "\033[0m"

// External Variables ----------------------------------------------------------

extern Pgm terrain;
extern World world;

// Static Functions ------------------------------------------------------------

static void visualize_world() {
    for (int z = 0; z < WORLD_XZ; z++) {
        printf(YELLOW "\n%03d " RESET, z);
        for (int x = 0; x < WORLD_XZ; x++) {
            int tint = 232;
            for (int y = WORLD_Y - 1; y; y--) {
                if (world[x][y][z]) {
                    tint += (int) (y * (23.0 / WORLD_Y));
                    break;
                }
            }
            printf(MEDIUM "  " RESET, tint);
        }
    }
    puts("\n");
}

// Entrypoint ------------------------------------------------------------------

int main() {
    // pgm_init
    pgm_init("../assets/ground.pgm");
    assert_eq(terrain.x, 100, "unexpected x value");
    assert_eq(terrain.z, 100, "unexpected z value");
    assert_eq(terrain.y, 255, "unexpected y value");
    // pgm_get_y_value
    assert_eq(pgm_get_y_value(0, 0), 166, "data at 0,0");
    assert_eq(pgm_get_y_value(1, 0), 184, "data at 1,0");
    assert_eq(pgm_get_y_value(99, 99), 135, "data at 99,99");
    // pgm_calc_ceil
    assert_eq(pgm_calc_ceil(), 280, "incorrect ceil calculation");
    pgm_set_world_terrain();
    visualize_world();
}
