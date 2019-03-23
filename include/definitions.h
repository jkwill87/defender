#pragma once

// Constant Definitions --------------------------------------------------------

// Configurable
#define GAME_SPEED 2         //  1..10
#define WORLD_XZ 100         // 50..150
#define WORLD_Y 50           // 50..150
#define LANDER_VISIBILITY 6  //  1..15
#define LETHAL_FALL_HEIGHT 5 //  1..50
#define LANDER_COUNT 1      //  1..25
#define HUMAN_COUNT 5        //  1..25

// Not so much...
#define LASER_DIST (WORLD_XZ * 1.5f)
#define MAP_CLEAR 5
#define MAX_CUBES 500000
#define PGM_MAX_DIGITS 10
#define PGM_MAX_DIM 1000
#define PI 3.14159265358979323846f
#define UNIT_COUNT (LANDER_COUNT + HUMAN_COUNT)
