#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>
#include "types.h"

class Unit {
    protected:
    typedef std::array<int, 3> PositionArray;
    typedef std::map<PositionArray, Colour> Layout;

    protected:
    const long id;
    bool is_colliding_ground;
    bool is_colliding_unit;
    Layout layout;

    public:
    Coordinate target;
    Coordinate origin;
    static std::vector<Unit *> units;
    static uint8 cycle;
    const std::string as_str;

    public:
    Unit(int x, int y, int z, std::string name);
    virtual ~Unit();

    protected:
    static uint8 calc_min_y();
    static uint8 calc_min_y(int x, int z);
    static coordinate calc_random_coordinate(
        bool edge = false,
        bool above_terrain = true
    );
    int y_distance(const Unit *target);

    public:
    static Unit *find_unit(Coordinate coordinate);
    virtual void ai();
    virtual void render();
    void shoot();
    bool is_occupying(Coordinate &pos);
};

class Human : public Unit {
    private:
    typedef enum : uint8 {
        SETTLED = 0,
        FALLING,
        FLOATING,
        KILLED
    } State;

    private:
    State state = SETTLED;
    uint8 fall_height = 0;
    uint8 terrain_height;

    public:
    bool available = true;

    public:
    Human(int x, int y, int z);
    explicit Human(Coordinate coordinate);
    explicit Human();
    ~Human() override;

    public:
    void ai() override;
    void render() override;
    void action_lift();
    void action_drop();
    void action_capture();
};

class Lander : public Unit {
    private:
    typedef enum : uint8 {
        SEARCHING = 0,
        PURSUING,
        CAPTURING,
        ESCAPING,
        EXITED,
        ATTACKING,
        KILLED
    } State;
    uint8 daze_counter=0;

    private:
    State state = SEARCHING;

    public:
    Human *captive = nullptr;

    public:
    Lander(int x, int y, int z);
    explicit Lander(Coordinate coordinate);
    explicit Lander();
    ~Lander() override;

    private:
    void set_captive(Human *);
    void new_search_path();
    void decide_next();

    // Actions
    void action_search();
    void action_bounce_ground();
    void action_bounce_unit();
    void action_pursue();
    void action_capture();
    void action_escape();
    void action_exit();
    void action_attack();
    void action_kill();

    // Deciders
    bool can_exit();
    bool can_escape();
    bool can_capture();
    bool can_pursue(Human **human);
    bool can_shoot_player();

    public:
    void abandon_captive(bool drop=false);
    void ai() override;
    void render() override;
};
