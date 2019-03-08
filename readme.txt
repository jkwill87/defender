Author
------

Name: Jessy Williams
Email: jwilli19@uoguelph.ca


Running
-------

- Run "make" in the project top directory (where this file lives).
- Run ./a3 (or the symlinked a1) to start.


Design Notes
------------

- Chose to use C++11 for unit logic because their inherent hierarchy lends well
  to an object oriented design.
- Was able to eliminate the cube "pop in" issue as well as decrease frustrum
  computation time by parallelizing tree() and frustrum_extract() using OpenMP.
- On my system the game is no longer bottlenecked by CPU and runs equally fast
  with or without "-drawall" flag.
- Used cmake to build because trying to link C/C++ code with OpenGL, OpenMP,
  and GLUT for cross platform environments is bad news bears.
- Game speed, world dimensions, and lander visibility can be configured in
  definitions; feel free to tweak them to your liking.
