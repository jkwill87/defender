Author
------

Name: Jessy Williams-Rancourt
Email: jwilli19@uoguelph.ca


Running
-------

From the project's top-level directory run 'make' to build and then './a1' to
run the program. I'm assuming that the testing machine uses the latest version
of MacOS Mojave, if not try uncommenting the second LIBS line in the makefile.


Design
------

Although it wasn't part of the requirements I put effort into further optimizing
the game engine. I was able to eliminate the cube "pop in" issue as well as
decrease frustrum computation time by parallelizing "tree()" and
"frustrum_extract()"" with OpenMP. On my system the game is no longer
bottlenecked by CPU and runs equally fast with or without -drawall flag.
