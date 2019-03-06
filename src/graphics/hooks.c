#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "graphics.h"

extern View view;
extern Config config;
extern Laser laser;


// Hook Callback Definitions ---------------------------------------------------

void glut_hook_default__draw_2d() {
    map_player_pos();
    map_human_pos();
    if (laser.active){
        map_laser();
    }
    map_outline();
    map_terrain();
}

void glut_hook_default__idle_update() {
    static int laser_base = 0;
    static int timer_base = 0;
    static int frame = 0;
    // calculate time delta
    int time = glutGet(GLUT_ELAPSED_TIME);
    frame++;
    bool next_tick = time - timer_base > 1000;
    // log profiling information
    if (next_tick && config.show_fps) {
        printf("FPS:%4.2f\n", frame * 1000.0 / (time - timer_base));
    }
    // reset laser cooldown
    bool laser_cooldown = time - laser_base > 350;
    if (!laser.active){
        laser_base = time;
    } else if (laser_cooldown){
        laser.active=false;
        laser_base = time;
    }
    // apply player movement
    calc_player_move(COAST);
    if (!next_tick) return;
    // reset time base
    timer_base = time;
    frame = 0;
    // apply unit movement
    Unit *unit = unit_first();
    while (unit != NULL) {
        calc_unit_move(unit);
        unit = unit_next();
    }
}

void glut_hook_default__keyboard(unsigned char key, int x, int y) {
    Direction direction = COAST;
    switch (key) {
        case 'q':
        case 27:
            puts("exiting.");
            exit(0);
        case 'f':
            config.fly_control = !config.fly_control;
            printf(
                "fly controls set to %s\n", config.fly_control ? "ON" : "OFF"
            );
            break;
        case 'w':
            direction = FORWARDS;
            break;
        case 's':
            direction = BACKWARDS;
            break;
        case 'a':
            direction = STRAFE_LEFT;
            break;
        case 'd':
            direction = STRAFE_RIGHT;
            break;
        case 'm':
            map_mode_toggle();
            break;
        case ' ':
            laser.active = true;  // in class prof. said to activate w/ space
            break;
        default:
            break;
    }
    calc_player_move(direction);
}

void glut_hook_default__motion(int x, int y) {
    view.old_x = x;
    view.old_y = y;
}

void glut_hook_default__mouse(int button, int state, int x, int y) {
    laser.active=true;  // spec says to use mouse
}

void glut_hook_default__passive_motion(int x, int y) {
    view.cam_x += (float) y - view.old_y;
    view.cam_y += (float) x - view.old_x;
    view.old_x = x;
    view.old_y = y;
    glutPostRedisplay();
    if(laser.active && laser_collision_check()){
        puts("hit human!");
    }
}

void glut_hook_default__reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 0.1, WORLD_XZ * 1.5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    config.screen_width = w;
    config.screen_height = h;
    map_pos_update();
}
