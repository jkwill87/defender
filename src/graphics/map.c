#include <math.h>

#include "debug.h"
#include "graphics.h"


// External Variable Declarations ----------------------------------------------

extern Config config;
extern Laser laser;
extern Position player_pos;
extern World world;


// Static Variable Declarations ------------------------------------------------

static float alpha;
static float dim;
static float pt;
static Point pt_nw;
static Point pt_se;


// Static Function Definitions -------------------------------------------------

static void _set_2d_material(const Material *material) {
    glMaterialfv(GL_FRONT, GL_EMISSION, *material);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, *material);
}

static void _set_2d_colour(Colour colour, float alpha) {
    const Material *material = get_material_a(colour, alpha);
    _set_2d_material(material);
}


// Function Definitions --------------------------------------------------------

void map_pos_update() {
    dim = config.screen_width > config.screen_height
          ? config.screen_height
          : config.screen_width;
    switch (config.map_mode) {
        default:
        case HIDDEN:
            dim = pt_nw.x = pt_nw.y = pt_se.x = pt_se.y = 0;
            alpha = 0;
            break;
        case MINI:
            dim *= 0.25f;
            pt_nw.x = (int) (config.screen_width - dim - 25);
            pt_se.y = (int) (config.screen_height - dim - 25);
            pt_se.x = config.screen_width - 25;
            pt_nw.y = config.screen_height - 25;
            alpha = 0.5f;
            break;
        case FULL:
            dim *= 0.8f;
            dim = dim > WORLD_XZ ? dim : WORLD_XZ;
            pt_nw.x = (int) ((config.screen_width - dim) / 2);
            pt_se.y = (int) ((config.screen_height - dim) / 2);
            pt_se.x = (int) (config.screen_width -
                             (config.screen_width - dim) / 2);
            pt_nw.y = (int) (config.screen_height -
                             (config.screen_height - dim) / 2);
            alpha = 0.25f;
            break;
    }
    pt = dim / (float) WORLD_XZ;
}

void map_mode_toggle() {
    config.map_mode = (config.map_mode + 1) % 3;
    map_pos_update();
}

void map_outline() {
    if (dim <= 0) return;
    _set_2d_colour(WHITE, alpha);
    glLineWidth(pt / 2);
    glBegin(GL_LINE_LOOP);
    float gap = pt * 2;
    glVertex2f(pt_nw.x - gap, pt_nw.y + gap);
    glVertex2f(pt_nw.x - gap, pt_se.y - gap);
    glVertex2f(pt_se.x + gap, pt_se.y - gap);
    glVertex2f(pt_nw.x - gap, pt_se.y - gap);
    glVertex2f(pt_se.x + gap, pt_se.y - gap);
    glVertex2f(pt_se.x + gap, pt_nw.y + gap);
    glVertex2f(pt_nw.x - gap, pt_nw.y + gap);
    glVertex2f(pt_se.x + gap, pt_nw.y + gap);
    glEnd();
}

void map_terrain() {
    Position pos;
    glBegin(GL_QUADS);
    for (int z = 0; z < WORLD_XZ; z++) {
        for (int x = 0; x < WORLD_XZ; x++) {
            int y;
            for (y = WORLD_Y - 1; y >= 0; y--) {
                if (world[x][y][z] != TRANSPARENT) break;
            }
            pos.x = pt_nw.x + x * pt;
            pos.y = y / (WORLD_Y + 1.0f) * 255 / 100.0f;
            pos.z = pt_se.y + z * pt;
            _set_2d_material(
                &((Material) {pos.y, pos.y, pos.y, alpha / 2})
            );
            glVertex2f(pos.x, pos.z);
            glVertex2f(pos.x + pt, pos.z);
            glVertex2f(pos.x + pt, pos.z + pt);
            glVertex2f(pos.x, pos.z + pt);
        }
    }
    glEnd();
}

void map_player_pos() {
    float px_size = pt * 4;
    Position px = {
        .x=(pt_se.x + player_pos.x * pt),
        .y=(pt_se.y - player_pos.z * pt),
    };
    glBegin(GL_TRIANGLES);
    _set_2d_colour(RED, alpha * 1.5f);
    glVertex2f(px.x, px.y + px_size);
    glVertex2f(px.x + px_size, px.y - px_size);
    glVertex2f(px.x - px_size, px.y - px_size);
    glEnd();
}

void map_human_pos() {
    float px_size = pt * 2;
    Position px;
    Unit *unit = unit_first();
    while (unit != NULL) {
        if (unit->entity == HUMAN) {
            px.x = (pt_se.x - unit->position.x * pt);
            px.y = (pt_se.y + unit->position.z * pt);
            glBegin(GL_QUADS);
            _set_2d_colour(GREEN, alpha * 1.5f);
            glVertex2f(px.x - px_size, px.y - px_size);
            glVertex2f(px.x + px_size, px.y - px_size);
            glVertex2f(px.x + px_size, px.y + px_size);
            glVertex2f(px.x - px_size, px.y + px_size);
            glEnd();
        }
        unit = unit_next();
    }
}

void map_laser() {
    Position p1 = {
        .x=(pt_se.x - laser.from.x * pt),
        .y=(pt_se.y + laser.from.z * pt),
    };
    Position p2 = {
        .x=(p1.x - laser.to.x * pt),
        .y=(p1.y + laser.to.z * pt),
    };
    glBegin(GL_LINES);
    glLineWidth(pt);
    _set_2d_colour(YELLOW, alpha * 1.5f);
    glVertex2f(p1.x, p1.y);
    glVertex2f(
        (GLfloat) fmax(fmin(pt_se.x, p2.x), pt_nw.x),
        (GLfloat) fmax(fmin(pt_nw.y, p2.y), pt_se.y)
    );
    glEnd();
}
