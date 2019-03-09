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

    private:
    const long id;

    protected:
    Coordinate target;
    Layout layout;

    public:
    Coordinate origin;
    static std::vector<Unit *> units;
    static byte cycle;
    const std::string as_str;

    // Constructor Declarations ------------------------------------------------

    public:
    Unit(int x, int y, int z, std::string name);
    virtual ~Unit();

    // Method Declarations  ----------------------------------------------------

    protected:
    static byte calc_min_y();
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
    typedef enum {
        SETTLED = 0,
        FALLING,
        FLOATING,
        KILLED
    } State;

    // Instance Variables ------------------------------------------------------

    private:
    State state = SETTLED;
    byte fall_height = 0;
    byte terrain_height;

    public:
    bool available = true;

    // Constructor Declarations ------------------------------------------------

    public:
    Human(int x, int y, int z);
    explicit Human(Coordinate coordinate);
    explicit Human();
    ~Human();

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
    typedef enum {
        SEARCHING = 0,
        PURSUING,
        CAPTURING,
        ESCAPING,
        EXITED,
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
    ~Lander();

    // Method Declarations  ----------------------------------------------------

    private:
    void look();

    public:
    void ai() override;
    void render() override;
    void action_restart_search();
};
