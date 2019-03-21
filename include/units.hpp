#pragma once

#include <array>
#include <map>
#include <string>
#include <vector>
#include "types.h"


// Unit Abstract Base Class Declaration ========================================

class Unit {
    // Type Declarations -------------------------------------------------------

    protected:
    typedef std::array<int, 3> PositionArray;
    typedef std::map<PositionArray, Colour> Layout;

    // Instance Variables ------------------------------------------------------

    protected:
    const long id;
    bool is_colliding_ground;
    bool is_colliding_unit;
    Coordinate target;
    Layout layout;

    public:
    Coordinate origin;
    static std::vector<Unit *> units;
    static uint8 cycle;
    const std::string as_str;

    // Constructor Declarations ------------------------------------------------

    public:
    Unit(int x, int y, int z, std::string name);
    virtual ~Unit();

    // Method Declarations  ----------------------------------------------------

    protected:
    static uint8 calc_min_y();
    static uint8 calc_min_y(int x, int z);
    static coordinate calc_random_coordinate(bool edge = false);
    int y_distance(const Unit *target);

    public:
    static Unit *find_unit(Coordinate coordinate);
    virtual void ai();
    virtual void render();
    void shoot();
    bool is_occupying(Coordinate &pos);
};


// Human Class Declaration =====================================================

class Human : public Unit {
    // Type Declarations -------------------------------------------------------

    private:
    typedef enum : uint8 {
        SETTLED = 0, FALLING, FLOATING, KILLED
    } State;

    // Instance Variables ------------------------------------------------------

    private:
    State state = SETTLED;
    uint8 fall_height = 0;
    uint8 terrain_height;

    public:
    bool available = true;

    // Constructor Declarations ------------------------------------------------

    public:
    Human(int x, int y, int z);
    explicit Human(Coordinate coordinate);
    explicit Human();
    ~Human() override;

    // Method Declarations  ----------------------------------------------------

    public:
    void ai() override;
    void render() override;
    void action_lift();
    void action_drop();
    void action_capture();
};


// Lander Class Declarations ===================================================

class Lander : public Unit {
    // Type Declarations -------------------------------------------------------

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

    // Instance Variables ------------------------------------------------------

    private:
    State state = SEARCHING;

    public:
    Human *captive = nullptr;

    // Constructor Declarations ------------------------------------------------

    public:
    Lander(int x, int y, int z);
    explicit Lander(Coordinate coordinate);
    explicit Lander();
    ~Lander() override;

    // Method Declarations  ----------------------------------------------------

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
    void abandon_captive();
    void ai() override;
    void render() override;
};
