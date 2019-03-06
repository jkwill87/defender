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
    Layout layout;

    public:
    Coordinate origin;
    static std::vector<Unit *> units;
    const std::string as_str;

    // Constructor Declarations ------------------------------------------------

    public:
    Unit(int x, int y, int z, std::string name);
    Unit(Coordinate coordinate, std::string name);
    virtual ~Unit();

    // Method Declarations  ----------------------------------------------------

    protected:
    int y_distance(const Unit *target);
    void move_towards(const Unit *target);
    void remove();

    public:
    virtual void action_hit() = 0;
    bool is_occupying(Coordinate &pos);
    virtual void render();
};


// Human Class Declaration =====================================================

class Human : public Unit {

    // Type Declarations -------------------------------------------------------

    private:
    typedef enum {
        SETTLED,
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

    // Method Declarations  ----------------------------------------------------

    private:
    void ai_settled();
    void ai_floating();
    void ai_falling();

    public:
    void render() override;
    void action_drop();
    void action_lift();
    void action_capture();
    void action_hit() override;
};


// Lander Class Declarations ===================================================

class Lander : public Unit {

    // Type Declarations -------------------------------------------------------

    private:
    typedef enum {
        SEARCHING,
        PURSUING,
        CAPTURING,
        ESCAPING,
        KILLED
    } State;

    // Instance Variables ------------------------------------------------------

    private:
    State state = SEARCHING;
    Human *target = nullptr;
    bool frame_a = false;

    // Constructor Declarations ------------------------------------------------

    public:
    Lander(int x, int y, int z);
    explicit Lander(Coordinate coordinate);

    // Method Declarations  ----------------------------------------------------

    private:
    void ai_search();
    void ai_pursue();
    void ai_capture();
    void ai_escape();

    public:
    void render() override;
    void action_hit() override;
};
