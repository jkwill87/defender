#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include "definitions.h"


// Type Declarations -----------------------------------------------------------

typedef uint_fast8_t uint8;
typedef uint8 World[WORLD_XZ][WORLD_Y][WORLD_XZ];
typedef float Material[4];


// Type Definitions ------------------------------------------------------------

typedef enum colour {
    COLOUR_NONE = 0,
    COLOUR_WHITE,
    COLOUR_GREY,
    COLOUR_GREY2,
    COLOUR_GREY3,
    COLOUR_BLACK,
    COLOUR_BLUE,
    COLOUR_GREEN,
    COLOUR_ORANGE,
    COLOUR_RED,
    COLOUR_YELLOW,
} Colour;

typedef enum direction {
    DIRECTION_COAST = 0,
    DIRECTION_FORWARD,
    DIRECTION_BACK,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
} Direction;

typedef enum map_mode {
    MAP_HIDDEN = 0,
    MAP_MINI,
    MAP_FULL,
} MapMode;

typedef struct config {
    bool display_all_cubes;
    bool fly_control;
    bool full_screen;
    bool show_fps;
    bool test_world;
    bool overhead_view;
    int screen_height;
    int screen_width;
    enum map_mode map_mode;
} Config;

typedef struct position {
    float x;
    float y;
    float z;
} Position;

typedef struct coordinate {
    int x;
    int y;
    int z;
} Coordinate;

typedef struct glut_hooks {
    void (*display)();
    void (*draw_2d)();
    void (*idle_update)();
    void (*keyboard)(unsigned char, int, int);
    void (*motion)(int, int);
    void (*mouse)(int, int, int, int);
    void (*passive_motion)(int, int);
    void (*reshape)(int, int);
} GlutHooks;

typedef struct lasers {
    bool active;
    Position from;
    Position to;
} Laser;

typedef struct pgm {
    int x;
    int y;
    int z;
    unsigned long data[PGM_MAX_DIM * PGM_MAX_DIM];
} Pgm;

typedef struct view {
    int count;
    int cam_x;
    int cam_y;
    int old_x;
    int old_y;
} View;
