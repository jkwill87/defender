#pragma once

#ifdef __APPLE__

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Materials
Material *get_material(Colour colour);
Material *get_material_a(Colour colour, float alpha);

// Engine
void build_display_list();
void frustrum_extract();
void glut_hook_default__display();
void shoot_laser();
void start_game(int *argc, char **argv);
void tree(float bx, float by, float bz, float tx, float ty, float tz, int l);

// PGM
unsigned pgm_calc_ceil();
unsigned pgm_get_y_value(double x, double z);
void pgm_init(const char *filename);
void pgm_set_world_terrain();

// Hooks
void glut_hook_default__draw_2d();
void glut_hook_default__idle_update();
void glut_hook_default__keyboard(unsigned char key, int x, int y);
void glut_hook_default__motion(int x, int y);
void glut_hook_default__mouse(int button, int state, int x, int y);
void glut_hook_default__passive_motion(int x, int y);
void glut_hook_default__reshape(int w, int h);

// Map
void map_laser_layer();
void map_mode_toggle();
void map_npc_layer();
void map_outline_layer();
void map_player_layer();
void map_pos_update();
void map_terrain_layer();

#ifdef __cplusplus
}
#endif
