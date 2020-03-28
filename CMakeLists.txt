# Project Configuration --------------------------------------------------------

CMAKE_MINIMUM_REQUIRED (VERSION 3.6)

PROJECT (defender)

SET (CMAKE_C_STANDARD 99)
SET (CMAKE_CXX_STANDARD 11)

SET (CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -pedantic -fsanitize=address -fno-omit-frame-pointer")
SET (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -pedantic -fsanitize=address -fno-omit-frame-pointer")

SET (CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")


# Graphics Library Configuration -----------------------------------------------

SET (OpenGL_GL_PREFERENCE GLVND)
FIND_PACKAGE (OpenGL REQUIRED)
FIND_PACKAGE (GLUT REQUIRED)
ADD_COMPILE_DEFINITIONS (GL_SILENCE_DEPRECATION)


# OpenML Library Configuration -------------------------------------------------

# FIND_PACKAGE (OpenMP)
# IF (OPENMP_FOUND)
#     SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#     SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# ENDIF ()


# Executables ------------------------------------------------------------------

INCLUDE_DIRECTORIES (include)
LINK_LIBRARIES (${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)

ADD_EXECUTABLE (defender
    include/debug.h
    include/definitions.h
    include/exec.h
    include/graphics.h
    include/types.h
    include/units.hpp
    src/exec/events.cpp
    src/exec/global.c
    src/exec/main.cpp
    src/graphics/engine.c
    src/graphics/hooks.c
    src/graphics/map.c
    src/graphics/materials.c
    src/graphics/pgm.c
    src/units/_unit.cpp
    src/units/human.cpp
    src/units/lander.cpp
    )
