/**
 * init.c
 *
 * Global variables which store the game's shared state between its submodules.
 */

#include "graphics.h"


// Default Global State Definitions --------------------------------------------

Pgm terrain = {.x=0, .y=0, .z=0, .data={0}};

World world;

View view = {
    .cam_x = 360, .cam_y = 450, .old_x = 0, .old_y = 0, .count = 0
};

Position player_pos = {
    .x = -20, .y = -25, .z = -45
};

Config config = {
    .display_all_cubes = false,
    .fly_control = false,
    .full_screen = false,
    .show_fps = false,
    .test_world = false,
    .screen_width = 1280,
    .screen_height = 720,
    .map_mode = MINI
};

GlutHooks glut_hooks = {
    .display=glut_hook_default__display,
    .draw_2d = glut_hook_default__draw_2d,
    .idle_update=glut_hook_default__idle_update,
    .keyboard=glut_hook_default__keyboard,
    .motion=glut_hook_default__motion,
    .mouse=glut_hook_default__mouse,
    .passive_motion=glut_hook_default__passive_motion,
    .reshape=glut_hook_default__reshape
};

Laser laser = {
    .active=false,
    .from={0},
    .to={0}
};
