/**
 * init.c
 *
 * Global variables which store the game's shared state between its submodules.
 */

#include "graphics.h"

Config config = {
    .display_all_cubes = false,
    .fly_control = false,
    .full_screen = false,
    .overhead_view=false,
    .pause_units=false,
    .show_fps = false,
    .test_world = false,
    .timer_unlock=false,
    .traction=false,
    .map_mode = MAP_MINI,
    .screen_height = 720,
    .screen_width = 1280,
};

Laser lasers[UNIT_COUNT + 1] = {{0}};

Pgm terrain = {
    .x = 0,
    .y = 0,
    .z = 0,
    .data = {0}
};

Position player_pos = {
    .x = -1.0f * WORLD_XZ / 2.0f,
    .y = -1.0f * WORLD_Y + MAP_CLEAR,
    .z = -1.0f * WORLD_XZ / 2.0f
};

GlutHooks glut_hooks = {
    .display = glut_hook_default__display,
    .draw_2d = glut_hook_default__draw_2d,
    .idle_update = glut_hook_default__idle_update,
    .keyboard = glut_hook_default__keyboard,
    .motion = glut_hook_default__motion,
    .mouse = glut_hook_default__mouse,
    .passive_motion = glut_hook_default__passive_motion,
    .reshape = glut_hook_default__reshape
};

View view = {
    .cam_x = 360,
    .cam_y = 450,
    .old_x = 0,
    .old_y = 0,
    .count = 0
};

World world_terrain = {{{COLOUR_NONE}}};
World world_units = {{{COLOUR_NONE}}};
