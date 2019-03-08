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
    static long id_counter;

    protected:
    Coordinate target;
    Layout layout;
    bool to_remove=false;

    public:
    Coordinate origin;
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
    virtual void ai();
    virtual void render();
    virtual void shoot();
    bool is_occupying(Coordinate &pos);


    // Friendship Declarations  ------------------------------------------------

    friend void unit_cycle();
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

    // Method Declarations  ----------------------------------------------------

    public:
    void ai() override;
    void render() override;
    void shoot() override;
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
    Human *captive = nullptr;

    // Constructor Declarations ------------------------------------------------

    public:
    Lander(int x, int y, int z);
    explicit Lander(Coordinate coordinate);
    explicit Lander();

    // Method Declarations  ----------------------------------------------------

    private:
    void look();

    public:
    void ai() override;
    void render() override;
    void shoot() override;
};
