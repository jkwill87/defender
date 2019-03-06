/**
 * engine.c
 *
 * A refactoring and customization of cis4820's starting OpenGL rendering code.
 */

#include <math.h>
#include <stdio.h>

#include "debug.h"
#include "graphics.h"


// External Variable Declarations ----------------------------------------------

extern Config config;
extern GlutHooks glut_hooks;
extern Position player_pos;
extern View view;
extern World world;
extern Laser laser;


// Static Variable Declarations ------------------------------------------------

static float f[6][4];
static int display_list[MAX_CUBES][3];
static Material viewpoint_light = {-50.0f, -50.0f, -50.0f, 1.0};


// Static Function Definitions -------------------------------------------------

static void _draw_cube(int i, int j, int k) {
    glMaterialfv(GL_FRONT, GL_SPECULAR, *get_material(WHITE));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, *get_material(BLACK));
    glMaterialfv(GL_FRONT, GL_AMBIENT, *get_material(DARK));
    glPushMatrix();
    glTranslatef(i + 0.5f, j + 0.5f, k + 0.5f);
    glutSolidCube(1.0);
    glPopMatrix();
}

static void _draw_unit(Unit *unit) {
    Colour colour;
    Position pos;
    for (int x = 0; x < 3; x++)
        for (int y = 0; y < 3; y++)
            for (int z = 0; z < 3; z++) {
                colour = unit->block[x][y][z];
                if (colour == TRANSPARENT)
                    continue;
                pos.x = unit->position.x + x - 1;
                pos.y = unit->position.y + y - 1;
                pos.z = unit->position.z + z - 1;
                glMaterialfv(
                    GL_FRONT,
                    GL_AMBIENT_AND_DIFFUSE,
                    *get_material(colour)
                );
                glPushMatrix();
                glTranslatef(pos.x, pos.y, pos.z);
                glutSolidCube(1.0);
                glPopMatrix();
            }
}

static bool _cube_in_frustrum(float x, float y, float z, float n) {
    for (int p = 0; p < 6; p++) {
        if (f[p][0] * (x - n) + f[p][1] * (y - n) + f[p][2] * (z - n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x + n) + f[p][1] * (y - n) + f[p][2] * (z - n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x - n) + f[p][1] * (y + n) + f[p][2] * (z - n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x + n) + f[p][1] * (y + n) + f[p][2] * (z - n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x - n) + f[p][1] * (y - n) + f[p][2] * (z + n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x + n) + f[p][1] * (y - n) + f[p][2] * (z + n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x - n) + f[p][1] * (y + n) + f[p][2] * (z + n) +
            f[p][3] > 0)
            continue;
        if (f[p][0] * (x + n) + f[p][1] * (y + n) + f[p][2] * (z + n) +
            f[p][3] > 0)
            continue;
        return false;
    }
    return true;
}

// Function Definitions --------------------------------------------------------

void start_game(int *argc, char **argv) {
    printf("starting game... ");
    fflush(stdout);
    // exec display
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    if (config.full_screen) {
        glutGameModeString("1280x720:32@60");
        glutEnterGameMode();
        // glutSetCursor(GLUT_CURSOR_NONE); // hide the cursor
    } else {
        glutInitWindowSize(config.screen_width, config.screen_height);
        glutCreateWindow(argv[0]);
    }
    // exec lighting
    Material light_position = {0.0, 50.0, 0.0, 0.0};
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, *get_material(MEDIUM));
    glLightfv(GL_LIGHT0, GL_DIFFUSE, *get_material(LIGHT));
    glLightfv(GL_LIGHT0, GL_SPECULAR, *get_material(BLACK));
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, viewpoint_light);
    glLightfv(GL_LIGHT1, GL_AMBIENT, *get_material(MEDIUM));
    glLightfv(GL_LIGHT1, GL_DIFFUSE, *get_material(LIGHT));
    glLightfv(GL_LIGHT1, GL_SPECULAR, *get_material(MEDIUM));
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.075);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    // register hooks
    glutReshapeFunc(glut_hooks.reshape);
    glutDisplayFunc(glut_hooks.display);
    glutKeyboardFunc(glut_hooks.keyboard);
    glutPassiveMotionFunc(glut_hooks.passive_motion);
    glutMotionFunc(glut_hooks.motion);
    glutMouseFunc(glut_hooks.mouse);
    glutIdleFunc(glut_hooks.idle_update);
    puts("OK!");
    fflush(stdout);
    // start game loop
    map_pos_update();
    glutMainLoop();
}

void glut_hook_default__display() {
    view.count = 0;
    build_display_list();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(view.cam_x, 1.0, 0.0, 0.0);
    glRotatef(view.cam_y, 0.0, 1.0, 0.0);
    glTranslatef(player_pos.x, player_pos.y, player_pos.z);
    viewpoint_light[0] = -player_pos.x;
    viewpoint_light[1] = -player_pos.y;
    viewpoint_light[2] = -player_pos.z;
    glLightfv(GL_LIGHT1, GL_POSITION, viewpoint_light);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, *get_material(BLACK));
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, *get_material(DARK));
    glPushMatrix();
    glTranslatef(WORLD_XZ / 2.0f, WORLD_Y / 2.0f, WORLD_XZ / 2.0f);
    glutSolidCube(WORLD_XZ);
    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, *get_material(BLACK));
    // draw terrain
    if (config.display_all_cubes) {
        for (int x = 0; x < WORLD_XZ; x++) {
            for (int y = 0; y < WORLD_Y; y++) {
                for (int z = 0; z < WORLD_XZ; z++) {
                    if (world[x][y][z] != TRANSPARENT) {
                        _draw_cube(x, y, z);
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < view.count; i++) {
            _draw_cube(
                display_list[i][0],
                display_list[i][1],
                display_list[i][2]
            );
        }
    }
    // draw laser
    if (laser.active) {
        calc_line_of_sight();
        laser_draw();
    }
    // draw units
    Unit *unit = unit_first();
    while (unit != NULL) {
        _draw_unit(unit);
        unit = unit_next();
    }
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, config.screen_width, 0, config.screen_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glNormal3f(0.0, 0.0, -1.0f);
    glut_hooks.draw_2d();
    glDisable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glutSwapBuffers();
}

void tree(float bx, float by, float bz, float tx, float ty, float tz, int l) {
    float length;
    length = (tx - bx) / 2.0f;
    if (length < 0)
        length *= -1;
    if (!_cube_in_frustrum(bx + ((tx - bx) / 2), by + ((ty - by) / 2),
                           bz + ((tz - bz) / 2), length))
        return;
    if (l != 1) {
        float new_centre_x, new_centre_y, new_centre_z;
        new_centre_x = bx + (tx - bx) / 2.0f;
        new_centre_y = by + (ty - by) / 2.0f;
        new_centre_z = bz + (tz - bz) / 2.0f;
        l++;
        tree(bx, by, bz, new_centre_x, new_centre_y, new_centre_z, l);
        tree(new_centre_x, by, bz, tx, new_centre_y, new_centre_z, l);
        tree(bx, by, new_centre_z, new_centre_x, new_centre_y, tz, l);
        tree(new_centre_x, by, new_centre_z, tx, new_centre_y, tz, l);
        tree(bx, new_centre_y, bz, new_centre_x, ty, new_centre_z, l);
        tree(new_centre_x, new_centre_y, bz, tx, ty, new_centre_z, l);
        tree(bx, new_centre_y, new_centre_z, new_centre_x, ty, tz, l);
        tree(new_centre_x, new_centre_y, new_centre_z, tx, ty, tz, l);
        return;
    }
    for (int x = (int) bx; x < tx + 1; x++) {
        for (int y = (int) by; y < ty + 1; y++) {
            for (int z = (int) bz; z < tz + 1; z++) {
                if (x >= WORLD_XZ || y >= WORLD_Y || z >= WORLD_XZ)
                    continue;
                if (x <= -1 || y <= -1 || z <= -1 || world[x][y][z] == 0)
                    continue;
                if (!_cube_in_frustrum(x + 0.5f, y + 0.5f, z + 0.5f, 0.5))
                    continue;
                if (!((x > 0 && (x < WORLD_XZ - 1) && y > 0 &&
                       (y < WORLD_Y - 1) && z > 0 && (z < WORLD_XZ - 1) &&
                       (world[x + 1][y][z] == 0 || world[x - 1][y][z] == 0 ||
                        world[x][y + 1][z] == 0 || (world[x][y - 1][z] == 0) ||
                        world[x][y][z + 1] == 0 || world[x][y][z - 1] == 0)) ||
                      (x == 0 || x == WORLD_XZ - 1 || y == 0 ||
                       y == WORLD_Y - 1 || z == 0 || z == WORLD_XZ - 1)))
                    continue;
                display_list[view.count][0] = x;
                display_list[view.count][1] = y;
                display_list[view.count][2] = z;
                view.count++;
                assert_ok(
                    view.count <= MAX_CUBES,
                    "too many cubes to render"
                );
            }
        }
    }
}

void frustrum_extract() {
    float p[16];
    float m[16];
    float c[16];
    float t;
    glGetFloatv(GL_PROJECTION_MATRIX, p);
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    c[0] = m[0] * p[0] + m[1] * p[4] + m[2] * p[8] + m[3] * p[12];
    c[1] = m[0] * p[1] + m[1] * p[5] + m[2] * p[9] + m[3] * p[13];
    c[2] = m[0] * p[2] + m[1] * p[6] + m[2] * p[10] + m[3] * p[14];
    c[3] = m[0] * p[3] + m[1] * p[7] + m[2] * p[11] + m[3] * p[15];
    c[4] = m[4] * p[0] + m[5] * p[4] + m[6] * p[8] + m[7] * p[12];
    c[5] = m[4] * p[1] + m[5] * p[5] + m[6] * p[9] + m[7] * p[13];
    c[6] = m[4] * p[2] + m[5] * p[6] + m[6] * p[10] + m[7] * p[14];
    c[7] = m[4] * p[3] + m[5] * p[7] + m[6] * p[11] + m[7] * p[15];
    c[8] = m[8] * p[0] + m[9] * p[4] + m[10] * p[8] + m[11] * p[12];
    c[9] = m[8] * p[1] + m[9] * p[5] + m[10] * p[9] + m[11] * p[13];
    c[10] = m[8] * p[2] + m[9] * p[6] + m[10] * p[10] + m[11] * p[14];
    c[11] = m[8] * p[3] + m[9] * p[7] + m[10] * p[11] + m[11] * p[15];
    c[12] = m[12] * p[0] + m[13] * p[4] + m[14] * p[8] + m[15] * p[12];
    c[13] = m[12] * p[1] + m[13] * p[5] + m[14] * p[9] + m[15] * p[13];
    c[14] = m[12] * p[2] + m[13] * p[6] + m[14] * p[10] + m[15] * p[14];
    c[15] = m[12] * p[3] + m[13] * p[7] + m[14] * p[11] + m[15] * p[15];
    f[0][0] = c[3] - c[0];
    f[0][1] = c[7] - c[4];
    f[0][2] = c[11] - c[8];
    f[0][3] = c[15] - c[12];
    t = sqrtf(f[0][0] * f[0][0] + f[0][1] * f[0][1] + f[0][2] * f[0][2]);
    f[0][0] /= t;
    f[0][1] /= t;
    f[0][2] /= t;
    f[0][3] /= t;
    f[1][0] = c[3] + c[0];
    f[1][1] = c[7] + c[4];
    f[1][2] = c[11] + c[8];
    f[1][3] = c[15] + c[12];
    t = sqrtf(f[1][0] * f[1][0] + f[1][1] * f[1][1] + f[1][2] * f[1][2]);
    f[1][0] /= t;
    f[1][1] /= t;
    f[1][2] /= t;
    f[1][3] /= t;
    f[2][0] = c[3] + c[1];
    f[2][1] = c[7] + c[5];
    f[2][2] = c[11] + c[9];
    f[2][3] = c[15] + c[13];
    t = sqrtf(f[2][0] * f[2][0] + f[2][1] * f[2][1] + f[2][2] * f[2][2]);
    f[2][0] /= t;
    f[2][1] /= t;
    f[2][2] /= t;
    f[2][3] /= t;
    f[3][0] = c[3] - c[1];
    f[3][1] = c[7] - c[5];
    f[3][2] = c[11] - c[9];
    f[3][3] = c[15] - c[13];
    t = sqrtf(f[3][0] * f[3][0] + f[3][1] * f[3][1] + f[3][2] * f[3][2]);
    f[3][0] /= t;
    f[3][1] /= t;
    f[3][2] /= t;
    f[3][3] /= t;
    f[4][0] = c[3] - c[2];
    f[4][1] = c[7] - c[6];
    f[4][2] = c[11] - c[10];
    f[4][3] = c[15] - c[14];
    t = sqrtf(f[4][0] * f[4][0] + f[4][1] * f[4][1] + f[4][2] * f[4][2]);
    f[4][0] /= t;
    f[4][1] /= t;
    f[4][2] /= t;
    f[4][3] /= t;
    f[5][0] = c[3] + c[2];
    f[5][1] = c[7] + c[6];
    f[5][2] = c[11] + c[10];
    f[5][3] = c[15] + c[14];
    t = sqrtf(f[5][0] * f[5][0] + f[5][1] * f[5][1] + f[5][2] * f[5][2]);
    f[5][0] /= t;
    f[5][1] /= t;
    f[5][2] /= t;
    f[5][3] /= t;
}

void build_display_list() {
    frustrum_extract();
    tree(0, 0, 0, WORLD_XZ, WORLD_Y, WORLD_XZ, 0);
    glutPostRedisplay();
}

bool collision_detect(Position next) {
    int x, y, z;
    for (int x_offset = -1; x_offset < 2; x_offset++) {
        for (int y_offset = -1; y_offset < 2; y_offset++) {
            for (int z_offset = -1; z_offset < 2; z_offset++) {
                x = (int) (next.x * -1 + x_offset);
                y = (int) (next.y * -1 + y_offset);
                z = (int) (next.z * -1 + z_offset);
                // check if out of bounds
                if (x <= 0 || x >= WORLD_XZ) return true;
                if (z <= 0 || z >= WORLD_XZ) return true;
                if (y <= 0 || y >= WORLD_Y) return true;
                // check for cube
                if (world[x][y][z] != TRANSPARENT) return true;
            }
        }
    }
    return false;
}

void calc_player_move(Direction direction) {
    Position next_pos = player_pos;
    static float accel_x = 0.0f;
    static float accel_y = 0.0f;
    static float accel_z = 0.0f;
    if (direction != COAST) {
        float rot_x = (view.cam_x / 180.0f * PI);
        float rot_y = (view.cam_y / 180.0f * PI);
        // calculate coordinate
        switch (direction) {
            default:
                break;
            case FORWARDS:
                next_pos.x = player_pos.x - sinf(rot_y);
                if (!config.fly_control)
                    next_pos.y = player_pos.y + sinf(rot_x);
                next_pos.z = player_pos.z + cosf(rot_y);
                break;
            case BACKWARDS:
                next_pos.x = player_pos.x + sinf(rot_y);
                if (!config.fly_control)
                    next_pos.y = player_pos.y - sinf(rot_x);
                next_pos.z = player_pos.z - cosf(rot_y);
                break;
            case STRAFE_LEFT:
                next_pos.x = player_pos.x + cosf(rot_y);
                next_pos.z = player_pos.z + sinf(rot_y);
                break;
            case STRAFE_RIGHT:
                next_pos.x = player_pos.x - cosf(rot_y);
                next_pos.z = player_pos.z - sinf(rot_y);
                break;
        }
        // recalculate acceleration velocity
        accel_x += (next_pos.x - player_pos.x) / 4;
        accel_y += (next_pos.y - player_pos.y) / 4;
        accel_z += (next_pos.z - player_pos.z) / 4;
    }
    // apply acceleration velocity
    next_pos.x += accel_x;
    next_pos.y += accel_y;
    next_pos.z += accel_z;
    if (collision_detect(next_pos)) {
        // stop acceleration
        accel_x = 0;
        accel_y = 0;
        accel_z = 0;
        return;
    }
    // decay acceleration
    accel_x /= 1.0125;
    accel_y /= 1.0125;
    accel_z /= 1.0125;
    // idle_update position if changed
    player_pos = next_pos;
    glutPostRedisplay();
    player_pos = next_pos;
}

void calc_unit_move(Unit *unit) {
    Position next;
    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            if (unit->block[x][0][z] == TRANSPARENT) continue;
            next.x = -1 * unit->position.x + x - 1;
            next.y = -1 * unit->position.y - 1;
            next.z = -1 * unit->position.z + z - 1;
            if (collision_detect(next)) return;
        }
    }
    unit->position.y--;
}

void calc_line_of_sight() {
    float rot_x = (view.cam_x / 180.0f * PI);
    float rot_y = (view.cam_y / 180.0f * PI);
    // from
    laser.from.x = player_pos.x * -1;
    laser.from.y = player_pos.y * -1 - 1;
    laser.from.z = player_pos.z * -1;
    // to
    laser.to.x = (player_pos.x - sinf(rot_y) * LASER_DIST) * -1 - laser.from.x;
    laser.to.y = (player_pos.y + sinf(rot_x) * LASER_DIST) * -1 - laser.from.y;
    laser.to.z = (player_pos.z + cosf(rot_y) * LASER_DIST) * -1 - laser.from.z;
}

bool laser_collision_check() {
    float rot_x = (view.cam_x / 180.0f * PI);
    float rot_y = (view.cam_y / 180.0f * PI);
    for (int i = 0; i < WORLD_XZ*WORLD_XZ; i++) {
        Position pt = {
            .x=(player_pos.x - sinf(rot_y) * i) * -1,
            .y=(player_pos.y + sinf(rot_x) * i) * -1,
            .z= (player_pos.z + cosf(rot_y) * i) * -1
        };
        Unit *unit = unit_first();
        while (unit != NULL) {
            if (
                // a little bit generous with hit detection
                fabs(unit->position.x - pt.x) <= 2 &&
                fabs(unit->position.y - pt.y) <= 4 &&
                fabs(unit->position.z - pt.z) <= 2
            ) return true;
            unit = unit_next();
        }
    }
    return false;
}

void laser_draw() {
    double angle = 180.0f / PI * acos(laser.to.z / sqrt(
        laser.to.x * laser.to.x + laser.to.y * laser.to.y +
        laser.to.z * laser.to.z)
    );
    if (laser.to.z <= 0)
        angle *= -1;
    GLUquadricObj *quadric = gluNewQuadric();
    glPushMatrix();
    glTranslatef(laser.from.x, laser.from.y, laser.from.z);
    glRotated(angle, -laser.to.y * laser.to.z, laser.to.x * laser.to.z, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, *get_material(YELLOW));
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluCylinder(quadric, 0.25f, 0.25f, 100, 100, 1);
    glPopMatrix();

}

