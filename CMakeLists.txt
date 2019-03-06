# Project Configuration --------------------------------------------------------

CMAKE_MINIMUM_REQUIRED (VERSION 3.6)
PROJECT (A2)
SET (CMAKE_C_FLAGS "-std=c99")
#SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
SET (CMAKE_BUILD_TYPE Debug)


# Graphics Library Configuration -----------------------------------------------

SET (OpenGL_GL_PREFERENCE GLVND)
FIND_PACKAGE (OpenGL REQUIRED)
FIND_PACKAGE (GLUT REQUIRED)
ADD_COMPILE_DEFINITIONS (GL_SILENCE_DEPRECATION)


# Executables ------------------------------------------------------------------

INCLUDE_DIRECTORIES (include)
LINK_LIBRARIES (${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)

ADD_EXECUTABLE (a2
    include/definitions.h
    include/debug.h
    include/graphics.h
    include/types.h
    include/world.h
    src/exec/init.c
    src/exec/main.c
    src/graphics/engine.c
    src/graphics/map.c
    src/graphics/materials.c
    src/world/pgm.c
    src/world/units.c
    src/graphics/hooks.c
    )

ADD_EXECUTABLE (test_pgm
    include/definitions.h
    include/debug.h
    include/graphics.h
    include/types.h
    include/world.h
    src/exec/init.c
    src/graphics/engine.c
    src/graphics/materials.c
    src/graphics/map.c
    src/world/pgm.c
    src/world/units.c
    tests/test_pgm.c
    src/graphics/hooks.c
    )

ADD_EXECUTABLE (test_terrain
    include/definitions.h
    include/debug.h
    include/graphics.h
    include/types.h
    include/world.h
    src/exec/init.c
    src/graphics/engine.c
    src/graphics/map.c
    src/graphics/materials.c
    src/world/pgm.c
    src/world/units.c
    tests/test_terrain.c
    src/graphics/hooks.c
    )
