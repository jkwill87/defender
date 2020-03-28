# Defender

Written in C and C++. A class project for CIS4820 at the University of Guelph.

![](assets/screenshot.png)

# Features Implemented

1. Collision Between Pairs of Aliens
Aliens bounce off each other whenever they move to occupy the same world space. This is done by normalizing their trajectory vectors. Landers enter a "dazed" state when this occurs and will abandon their current activity for a period of time proportionate to the velocity of the impact. As a result they may drop humans they were carrying or lose sight of humans they were persuing. 

2. Collision Between Landers and the Ground
While searching or attacking landers bounce between X/Z coordinates on adjacent or opposing edges of the game world. The Y coordinate is chosen at random between 0 and the "WORLD_Y" constant defined in definition.h. Should one hit the ground they will bounce off the ground in the same manner described in (1).

3. Alien Opponents
When an alien successsfully exits with a human they change colour to red and begin to fire at the player. In this state their move patterns reverts to that of their searching behaviour. They will fire when within the "LANDER_ATTACK_RANGE" constant value as defined in definition.h.

# Gameplay Notes

To help facilitate grading the following keypresses may be helpful:

- Press 'r' to reset and restart game
- Press 'o' to toggle an oblique view to better observe collisions (player position is marked with a blue cube)
- Press 't' to toggle traction mode which reduces player drift
- Press 'p' to toggle pausing unit movement
- Press 'u' to toggle unlocking timer-based movement speed

Additionally see include/definition.h to tweak game parameters as desired.

# Running

- Run `cmake . && make` to build
- Run `./defender` to run	
- Tested on MacOS Catalina
