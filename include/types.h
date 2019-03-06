#pragma once

#include <stdbool.h>

#include "definitions.h"


// Value Type Definitions ------------------------------------------------------

typedef unsigned char byte;


// Enum Type Definitions -------------------------------------------------------

typedef enum direction {
    COAST = 0,
    FORWARDS,
    BACKWARDS,
    STRAFE_LEFT,
    STRAFE_RIGHT
} Direction;

typedef enum colour {
    // shades
    TRANSPARENT = 0,
    BLACK,
    DARK,
    MEDIUM,
    LIGHT,
    WHITE,
    // colours
    BLUE,
    GREEN,
    RED,
    YELLOW,
    ORANGE
} Colour;

typedef enum entity {
    HUMAN,
    UFO
} Entity;

typedef enum map_mode {
    HIDDEN = 0,
    MINI = 1,
    FULL = 2
} MapMode;


// Struct Type Definitions -----------------------------------------------------

typedef struct pgm {
    unsigned x;
    unsigned y;
    unsigned z;
    unsigned data[PGM_MAX_DIM * PGM_MAX_DIM];
} Pgm;

typedef struct config {
    bool display_all_cubes;
    bool fly_control;
    bool full_screen;
    bool show_fps;
    bool test_world;
    int screen_width;
    int screen_height;
    MapMode map_mode;
} Config;

typedef struct view {
    int count;
    float cam_x;
    float cam_y;
    float old_x;
    float old_y;
} View;

typedef struct position {
    float x;
    float y;
    float z;
} Position;

typedef struct point {
    int x;
    int y;
} Point;

typedef struct unit {
    enum entity entity;
    enum colour block[3][3][3];
    struct position position;
    unsigned durability;
} Unit;

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

typedef struct laser {
    bool active;
    Position from;
    Position to;
}Laser;

typedef float Material[4];

typedef byte World[WORLD_XZ][WORLD_Y][WORLD_XZ];
