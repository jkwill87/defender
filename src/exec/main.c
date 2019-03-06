/**
 * main.c
 *
 * The game's entry point.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "graphics.h"


// External Variables ----------------------------------------------------------

extern Config config;


// Static Functions ------------–-----------------------------------------------

static void _arg_parse(int argc, char **argv, Config *config) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (!strcmp(arg, "-drawall")) {
            config->display_all_cubes = !config->display_all_cubes;
        } else if (!strcmp(arg, "-fps")) {
            config->show_fps = !config->show_fps;
        } else if (!strcmp(arg, "-full")) {
            config->full_screen = !config->full_screen;
        } else if (!strcmp(arg, "-testworld")) {
            config->test_world = !config->test_world;
        } else {
            puts("usage: a1 [-drawall] [-testworld] [-fps] [-full]");
            exit(1);
        }
    }
}

static void _load_map() {
    printf("loading pgm... ");
    pgm_init("ground.pgm");
    puts("OK!");
    printf("initializing world... ");
    pgm_set_world_terrain();
    puts("OK!");
}

void _seed_units() {
    printf("adding humans... ");
    unit_add(HUMAN, WORLD_XZ - 5, 5, WORLD_XZ - 5);
    unit_add(HUMAN, WORLD_XZ - 10, WORLD_Y - 10, WORLD_XZ - 15);
    unit_add(HUMAN, WORLD_XZ / 2 + 10, WORLD_Y - 20, WORLD_XZ / 2 - 10);
    unit_add(HUMAN, WORLD_XZ / 2 + 15, WORLD_Y - 15, WORLD_XZ / 2 - 5);
    unit_add(HUMAN, WORLD_XZ / 2 + 20, WORLD_Y - 10, WORLD_XZ / 2);
    puts("OK!");
}


// Entrypoint ------------------------------------------------------------------

int main(int argc, char **argv) {
    _arg_parse(argc, argv, &config);
    _load_map();
    _seed_units();
    start_game(&argc, argv);
    unit_clear();
    return 0;
}
