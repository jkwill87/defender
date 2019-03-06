#pragma once

#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#endif

#include "types.h"


#ifdef __cplusplus
extern "C" {
#endif

// Materials Function Declarations ---------------------------------------------

Material *get_material(Colour colour);
Material *get_material_a(Colour colour, float alpha);


// Engine Function Declarations ------------------------------------------------

void glut_hook_default__display();
void frustrum_extract();
void start_game(int *argc, char **argv);
void tree(float bx, float by, float bz, float tx, float ty, float tz, int l);
void build_display_list();


// PGM Function Declarations ---------------------------------------------------

void pgm_init(const char *filename);
unsigned pgm_calc_ceil();
unsigned pgm_get_y_value(double x, double z);
void pgm_set_world_terrain();


// Hook Function Declarations --------------------------------------------------

void glut_hook_default__draw_2d();
void glut_hook_default__idle_update();
void glut_hook_default__keyboard(unsigned char key, int x, int y);
void glut_hook_default__motion(int x, int y);
void glut_hook_default__mouse(int button, int state, int x, int y);
void glut_hook_default__passive_motion(int x, int y);
void glut_hook_default__reshape(int w, int h);


// Map Function Declarations ---------------------------------------------------

void map_mode_toggle();
void map_pos_update();
void map_outline_layer();
void map_terrain_layer();
void map_player_layer();
void map_npc_layer();
void map_laser_layer();

// Movement Function Declarations ----------------------------------------------

void calc_player_move(Direction direction);
bool has_collided(Coordinate coord);
bool has_laser_collided();


#ifdef __cplusplus
}
#endif
