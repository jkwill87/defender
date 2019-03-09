#include <cmath>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "debug.h"
#include "exec.h"
#include "units.hpp"

using namespace std;


// External Variable Declarations ----------------------------------------------

extern Laser laser;
extern Position player_pos;
extern View view;
extern World world_units;


// Static Variable Declarations ----------------------------------------------

class EventManager {

    vector<shared_ptr<Unit>> units;
    map<long, Human *> captivity;

    EventManager() {
        unit_purge();
        for (int i = 0; i < HUMAN_COUNT; i++) {
            Lander *lander = new Lander();
            // bind lander's get_captive function to an anonymous lamba which
            // associates its id key to the
            lander->get_captive = bind(
                [this](long id) { return captivity[id]; }, lander->id
            );
            units.emplace_back(lander);
        }
        for (int i = 0; i < LANDER_COUNT; i++) {
            units.emplace_back(new Human());
        }
        captivity[0]=new Human();
    }

    ~EventManager() {
        units.clear();
    }

    void schedule_render() {
        memset(world_units, 0, WORLD_XZ * WORLD_XZ * WORLD_Y);
        for (long i = units.size(); i > 0; i--) {
            units[i - 1]->render();
        }
    }

    void schedule_ai() {
        if (laser.active) {
            float rot_x = (view.cam_x / 180.0f * PI);
            float rot_y = (view.cam_y / 180.0f * PI);
            for (int i = 0; i < WORLD_XZ * WORLD_XZ; i++) {
                Position pt = {
                    (player_pos.x - sinf(rot_y) * i) * -1,
                    (player_pos.y + sinf(rot_x) * i) * -1,
                    (player_pos.z + cosf(rot_y) * i) * -1
                };
                for (long j = units.size(); j > 0; j--) {
                    auto unit = units[j - 1];
                    if (
                        fabs(unit->origin.x - pt.x) <= 2 &&
                        fabs(unit->origin.y - pt.y) <= 2 &&
                        fabs(unit->origin.z - pt.z) <= 2
                        )
                        unit->shoot();
                }
            }
        }
        for (long i = units.size(); i > 0; i--) {
            units[i - 1]->ai();
        }
    }


    friend void unit_init();
    friend void unit_cycle();
    friend void unit_purge();
};


// Function Definitions --------------------------------------------------------

static EventManager *event_manager;

void unit_init() {
    event_manager = new EventManager();
}

void unit_cycle() {
    ++Unit::cycle;
    event_manager->schedule_render();
    event_manager->schedule_render();
    event_manager->schedule_ai();
}

void unit_purge() {
    delete event_manager;
}
