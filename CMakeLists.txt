# Project Configuration --------------------------------------------------------

CMAKE_MINIMUM_REQUIRED (VERSION 3.6)

PROJECT (a3)
SET (CMAKE_C_STANDARD 99)
SET (CMAKE_CXX_STANDARD 11)

SET (CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -pedantic -fsanitize=address -fno-omit-frame-pointer")
SET (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -pedantic -fsanitize=address -fno-omit-frame-pointer")

SET (CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# Graphics Library Configuration -----------------------------------------------

SET (OpenGL_GL_PREFERENCE GLVND)
FIND_PACKAGE (OpenGL REQUIRED)
FIND_PACKAGE (GLUT REQUIRED)
ADD_COMPILE_DEFINITIONS (GL_SILENCE_DEPRECATION)

# OpenML Library Configuration -------------------------------------------------
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


# Executables ------------------------------------------------------------------

INCLUDE_DIRECTORIES (include)
LINK_LIBRARIES (${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)

ADD_EXECUTABLE (a3
    include/debug.h
    include/definitions.h
    include/exec.hpp
    include/graphics.h
    include/types.h
    include/units.hpp
    src/exec/events.cpp
    src/exec/global.c
    src/exec/main.cpp
    src/exec/random.cpp
    src/graphics/engine.c
    src/graphics/hooks.c
    src/graphics/map.c
    src/graphics/materials.c
    src/graphics/movement.c
    src/graphics/pgm.c
    src/units/_unit.cpp
    src/units/human.cpp
    src/units/lander.cpp
    )
