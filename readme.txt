Author
------

Name: Jessy Williams-Rancourt
Email: jwilli19@uoguelph.ca


Running
-------

(1) Run "make" in the project top directory (where this file lives).

(2) Run ./a3 (or the symlinked ./a1) to start.


Development Notes
-----------------

- Objectives 1-4 implemented.

- Chose to use C++11 for unit logic because their inherent hierarchy lends well
  to an object oriented design. Linking with C was a bit of a challenge but it
  should compile with the provided makefile which was tested on the Thorn iMacs.

- Further optimized game engine to eliminate the cube "pop in" issue.

- Was able to decrease frustrum computation time by parallelizing tree() and
  frustrum_extract() but this requires OpenMP which is not available on the Mac
  Lab computers. Bummer. Either way it compiles without them.

- Game speed, world dimensions, lander visibility, and human fall survival
  height can be configured in include/definitions.h; feel free to tweak them if
  it helps with grading.
