/**
 * map.c
 *
 * Logic for drawing 2d game map overlay.
 */

#include <math.h>
#include "debug.h"
#include "graphics.h"


// External Variable Declarations ----------------------------------------------

extern Config config;
extern Position player_pos;
extern Laser laser;
extern World world_terrain;
extern World world_units;


// Static Variable Declarations ------------------------------------------------

static float alpha;
static float dim;
static float pt;
static int pt_nw_x, pt_nw_y;
static int pt_se_x, pt_se_y;


// Static Function Definitions -------------------------------------------------

static void _set_2d_material(Material *material) {
    glMaterialfv(GL_FRONT, GL_EMISSION, *material);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, *material);
}

static void _set_2d_colour(Colour colour, float alpha) {
    Material *material = get_material_a(colour, alpha);
    _set_2d_material(material);
}


// Function Definitions --------------------------------------------------------

void map_pos_update() {
    dim = config.screen_width > config.screen_height ? config.screen_height
          : config.screen_width;
    switch (config.map_mode) {
        default:
        case MAP_HIDDEN:
            dim = pt_nw_x = pt_nw_y = pt_se_x = pt_se_y = 0;
            alpha = 0;
            break;
        case MAP_MINI:
            dim *= 0.25f;
            pt_nw_x = (int)(config.screen_width - dim - 25);
            pt_se_y = (int)(config.screen_height - dim - 25);
            pt_se_x = config.screen_width - 25;
            pt_nw_y = config.screen_height - 25;
            alpha = 0.5f;
            break;
        case MAP_FULL:
            dim *= 0.8f;
            dim = dim > WORLD_XZ ? dim : WORLD_XZ;
            pt_nw_x = (int)((config.screen_width - dim) / 2);
            pt_se_y = (int)((config.screen_height - dim) / 2);
            pt_se_x =
                (int)(config.screen_width - (config.screen_width - dim) / 2);
            pt_nw_y =
                (int)(config.screen_height - (config.screen_height - dim) / 2);
            alpha = 0.25f;
            break;
    }
    pt = dim / (float)WORLD_XZ;
}

void map_mode_toggle() {
    config.map_mode = (config.map_mode + 1) % 3;
    map_pos_update();
}

void map_outline_layer() {
    if (dim <= 0) return;
    _set_2d_colour(COLOUR_WHITE, alpha);
    glLineWidth(pt / 2);
    glBegin(GL_LINE_LOOP);
    float gap = pt * 2;
    glVertex2f(pt_nw_x - gap, pt_nw_y + gap);
    glVertex2f(pt_nw_x - gap, pt_se_y - gap);
    glVertex2f(pt_se_x + gap, pt_se_y - gap);
    glVertex2f(pt_nw_x - gap, pt_se_y - gap);
    glVertex2f(pt_se_x + gap, pt_se_y - gap);
    glVertex2f(pt_se_x + gap, pt_nw_y + gap);
    glVertex2f(pt_nw_x - gap, pt_nw_y + gap);
    glVertex2f(pt_se_x + gap, pt_nw_y + gap);
    glEnd();
}

void map_terrain_layer() {
    glBegin(GL_QUADS);
    float px_x, px_y;
    float map_height;
    for (int z = 0; z < WORLD_XZ; z++) {
        for (int x = 0; x < WORLD_XZ; x++) {
            int y;
            for (y = WORLD_Y - 1; y >= 0; y--) {
                if (world_terrain[x][y][z] != COLOUR_NONE) break;
            }
            px_x = pt_nw_x + x * pt;
            map_height = y / (WORLD_Y + 1.0f) * 255 / 100.0f;
            px_y = pt_se_y + z * pt;
            _set_2d_material(
            &((Material) {
                map_height, map_height, map_height, alpha / 2
            }));
            glVertex2f(px_x, px_y);
            glVertex2f(px_x + pt, px_y);
            glVertex2f(px_x + pt, px_y + pt);
            glVertex2f(px_x, px_y + pt);
        }
    }
    glEnd();
}

void map_player_layer() {
    float px_size = pt * 4;
    float px_x = pt_se_x + player_pos.x * pt;
    float px_y = pt_se_y - player_pos.z * pt;
    glBegin(GL_TRIANGLES);
    _set_2d_colour(COLOUR_RED, alpha * 1.5f);
    glVertex2f(px_x, px_y + px_size);
    glVertex2f(px_x + px_size, px_y - px_size);
    glVertex2f(px_x - px_size, px_y - px_size);
    glEnd();
}

void map_npc_layer() {
    float px_size = pt * 2;
    float px_x, px_y;
    for (int z = 0; z < WORLD_XZ; z++) {
        for (int x = 0; x < WORLD_XZ; x++) {
            int y;
            for (y = WORLD_Y - 1; y >= 0; y--) {
                if (world_units[x][y][z] == COLOUR_NONE) continue;
                px_x = pt_se_x - x * pt;
                px_y = pt_nw_y - z * pt;
                glBegin(GL_QUADS);
                _set_2d_colour(COLOUR_GREEN, alpha * 1.5f);
                glVertex2f(px_x - px_size, px_y - px_size);
                glVertex2f(px_x + px_size, px_y - px_size);
                glVertex2f(px_x + px_size, px_y + px_size);
                glVertex2f(px_x - px_size, px_y + px_size);
                glEnd();
                break;
            }
        }
    }
}

void map_laser_layer() {
    float p1_x = pt_se_x - laser.from.x * pt;
    float p1_y = pt_se_y + laser.from.z * pt;
    float p2_x = p1_x - laser.to.x * pt;
    float p2_y = p1_y + laser.to.z * pt;
    glBegin(GL_LINES);
    glLineWidth(pt);
    _set_2d_colour(COLOUR_YELLOW, alpha * 1.5f);
    glVertex2f(p1_x, p1_y);
    glVertex2f((GLfloat)fmax(fmin(pt_se_x, p2_x), pt_nw_x),
               (GLfloat)fmax(fmin(pt_nw_y, p2_y), pt_se_y));
    glEnd();
}
