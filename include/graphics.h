#pragma once

#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "types.h"
#include "world.h"


// Materials Function Declarations ---------------------------------------------

const Material *get_material(Colour colour);
const Material *get_material_a(Colour colour, float alpha);


// Engine Function Declarations ------------------------------------------------

void glut_hook_default__display();
void frustrum_extract();
void start_game(int *argc, char **argv);
void tree(float bx, float by, float bz, float tx, float ty, float tz, int l);
void build_display_list();
bool collision_detect(Position next);
void calc_player_move(Direction direction);
void calc_unit_move(Unit *unit);
void calc_line_of_sight();
void laser_draw();
bool laser_collision_check();

// Map Function Declarations ---------------------------------------------------

void map_mode_toggle();
void map_pos_update();
void map_outline();
void map_terrain();
void map_player_pos();
void map_human_pos();
void map_laser();


// Hook Function Declarations --------------------------------------------------

void glut_hook_default__draw_2d();
void glut_hook_default__idle_update();
void glut_hook_default__keyboard(unsigned char key, int x, int y);
void glut_hook_default__motion(int x, int y);
void glut_hook_default__mouse(int button, int state, int x, int y);
void glut_hook_default__passive_motion(int x, int y);
void glut_hook_default__reshape(int w, int h);
