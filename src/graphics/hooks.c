#include "exec.h"
#include "debug.h"
#include "graphics.h"

extern View view;
extern Config config;
extern Laser laser;

// Hook Callback Definitions ---------------------------------------------------

void glut_hook_default__draw_2d() {
    // note: layers overlay in the reverse order
    if (laser.active) map_laser_layer();  // e.g. laser is drawn above terrain
    map_npc_layer();  // same with npcs, etc...
    map_player_layer();
    map_outline_layer();
    map_terrain_layer();
}

void glut_hook_default__idle_update() {
    static int laser_base = 0;
    static int timer_base = 0;
    static int frame = 0;
    // calculate time delta
    int time = glutGet(GLUT_ELAPSED_TIME);
    frame++;
    bool next_tick = time - timer_base > 100 / GAME_SPEED;
    // log profiling information
    if (next_tick && config.show_fps) log_fps(frame, time, timer_base);
    // reset laser cooldown
    bool laser_cooldown = time - laser_base > 350;
    if (!laser.active) {
        laser_base = time;
    } else if (laser_cooldown) {
        laser.active = false;
        laser_base = time;
    }
    // apply player movement
    calc_player_move(DIRECTION_COAST);
    if (!next_tick) return;
    // reset time base
    timer_base = time;
    frame = 0;
    // trigger unit movement
    unit_cycle();
}

void glut_hook_default__keyboard(unsigned char key, int x, int y) {
    Direction direction = DIRECTION_COAST;
    switch (key) {
        case 'q':
        case 27:
        log("exiting");
            unit_rm_all();
#ifdef __APPLE__
        glutDestroyWindow(glutGetWindow());
        exit(0);
#else
            glutLeaveMainLoop();
            break;
#endif
        case 'f':
            config.fly_control = !config.fly_control;
            printf(
                "fly controls set to %s\n", config.fly_control ? "ON" : "OFF"
            );
            break;
        case 'w':
            direction = DIRECTION_FORWARD;
            break;
        case 's':
            direction = DIRECTION_BACK;
            break;
        case 'a':
            direction = DIRECTION_LEFT;
            break;
        case 'd':
            direction = DIRECTION_RIGHT;
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
    calc_player_move(direction); // applies drifting
}

void glut_hook_default__motion(int x, int y) {
    // render camera position
    view.cam_x += y - view.old_y;
    view.cam_y += x - view.old_x;
    view.old_x = x;
    view.old_y = y;
}

void glut_hook_default__mouse(int button, int state, int x, int y) {
    if (button != 0 || state != 0) return;
    laser.active = true;  // spec says to use mouse so that's used too
}

void glut_hook_default__passive_motion(int x, int y) {
    // render camera position
    view.cam_x += y - view.old_y;
    view.cam_y += x - view.old_x;
    view.old_x = x;
    view.old_y = y;
    // check for laser collisions
    if (laser.active && has_laser_collided()) {
        log("hit human!");
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
