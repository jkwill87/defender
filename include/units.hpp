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
    Unit(Coordinate coordinate, std::string name);
    virtual ~Unit();

    // Method Declarations  ----------------------------------------------------

    protected:
    int y_distance(const Unit *target);
    void move_to_target();
    bool is_at_target();
    void remove();

    public:
    virtual void action_hit() = 0;
    bool is_occupying(Coordinate &pos);
    virtual void render();
    virtual void ai();
    static Unit *find_at_coordinate(Coordinate coordinate);
};


// Human Class Declaration =====================================================

class Human : public Unit {

    // Type Declarations -------------------------------------------------------

    private:
    typedef enum {
        SETTLED=0,
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
    void ai() override;
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
        SEARCHING=0,
        PURSUING,
        CAPTURING,
        ESCAPING,
        KILLED
    } State;

    typedef enum {
        UNDETECTED=0,
        DETECTED,
        LOCATED,
        LOCKED_ON
    } Awareness;

    // Instance Variables ------------------------------------------------------

    private:
    State state = SEARCHING;
    Awareness awareness = UNDETECTED;
    Human *captive = nullptr;

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
    void look();

    public:
    void ai() override;
    void render() override;
    void action_hit() override;
};
