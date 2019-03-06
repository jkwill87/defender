#include <math.h>

#include "debug.h"
#include "graphics.h"


// External Variable Declarations ----------------------------------------------

extern Position player_pos;
extern View view;
extern Config config;
extern World world_terrain;

static Coordinate pos_to_coord(Position pos) {
    return (Coordinate) {(int) pos.x, (int) pos.y, (int) pos.z};
}


// Function Definitions --------------------------------------------------------


void calc_player_move(Direction direction) {
    Position player_pos_next = player_pos;
    static float accel_x = 0.0f;
    static float accel_y = 0.0f;
    static float accel_z = 0.0f;
    if (direction != DIRECTION_COAST) {
        float rot_x = (view.cam_x / 180.0f * PI);
        float rot_y = (view.cam_y / 180.0f * PI);
        // calculate coordinate
        switch (direction) {
            default:
                break;
            case DIRECTION_FORWARD:
                player_pos_next.x = player_pos.x - sinf(rot_y);
                if (!config.fly_control)
                    player_pos_next.y = player_pos.y + sinf(rot_x);
                player_pos_next.z = player_pos.z + cosf(rot_y);
                break;
            case DIRECTION_BACK:
                player_pos_next.x = player_pos.x + sinf(rot_y);
                if (!config.fly_control)
                    player_pos_next.y = player_pos.y - sinf(rot_x);
                player_pos_next.z = player_pos.z - cosf(rot_y);
                break;
            case DIRECTION_LEFT:
                player_pos_next.x = player_pos.x + cosf(rot_y);
                player_pos_next.z = player_pos.z + sinf(rot_y);
                break;
            case DIRECTION_RIGHT:
                player_pos_next.x = player_pos.x - cosf(rot_y);
                player_pos_next.z = player_pos.z - sinf(rot_y);
                break;
        }
        // recalculate acceleration velocity
        accel_x += (player_pos_next.x - player_pos.x) / 4;
        accel_y += (player_pos_next.y - player_pos.y) / 4;
        accel_z += (player_pos_next.z - player_pos.z) / 4;
    }
    // apply acceleration velocity
    player_pos_next.x += accel_x;
    player_pos_next.y += accel_y;
    player_pos_next.z += accel_z;
    if (has_collided(pos_to_coord(player_pos_next))) {
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
    player_pos = player_pos_next;
}

bool has_collided(Coordinate coord) {
    int x, y, z;
    for (int x_offset = -1; x_offset < 2; x_offset++) {
        for (int y_offset = -1; y_offset < 2; y_offset++) {
            for (int z_offset = -1; z_offset < 2; z_offset++) {
                x = (coord.x * -1 + x_offset);
                y = (coord.y * -1 + y_offset);
                z = (coord.z * -1 + z_offset);
                // check if out of bounds
                if (x <= 0 || x >= WORLD_XZ) return true;
                if (z <= 0 || z >= WORLD_XZ) return true;
                if (y <= 0 || y >= WORLD_Y) return true;
                // check for cube
                if (world_terrain[x][y][z] != COLOUR_NONE) return true;
            }
        }
    }
    return false;
}

bool has_laser_collided() {
    //float rot_x = (view.cam_x / 180.0f * PI);
    //float rot_y = (view.cam_y / 180.0f * PI);
    //for (int i = 0; i < WORLD_XZ * WORLD_XZ; i++) {
    //    Coordinate pt = {
    //        .x=(int) ((player_pos.x - sinf(rot_y) * i) * -1),
    //        .y=(int) ((player_pos.y + sinf(rot_x) * i) * -1),
    //        .z=(int) ((player_pos.z + cosf(rot_y) * i) * -1)
    //    };
    //    Npc *unit = npc_first();
    //    while (unit != NULL) {
    //        if (
    //            // a little bit generous with hit detection
    //            fabs(unit->pos.x - pt.x) <= 2 &&
    //            fabs(unit->pos.y - pt.y) <= 4 &&
    //            fabs(unit->pos.z - pt.z) <= 2
    //            )
    //            return true;
    //        unit = npc_next();
    //    }
    //}
    return false;
}
