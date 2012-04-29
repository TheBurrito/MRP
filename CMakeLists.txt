cmake_minimum_required(VERSION 2.4)
PROJECT (dab5267_mapping)

#SET (CMAKE_MODULE_PATH "/usr/local/dcs/share/cmake/Modules")
#SET (CMAKE_MODULE_PATH "/usr/local/dcs/share/cmake/Modules:/usr/share/cmake-2.8/Modules")
INCLUDE (UsePlayerC++)
find_package(GLUT)
find_package(OpenGL)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
#SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

SET (STD_LIBS pthread rt)
SET (DRAWING_LIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY} X11 Xext)

INCLUDE (dab5267_mrp.cmake)

PLAYER_ADD_PLAYERCPP_CLIENT (dummy SOURCES dummy.cpp)

PLAYER_ADD_PLAYERCPP_CLIENT (mapper SOURCES mapper.cpp LINKFLAGS)
target_link_libraries(mapper ${STD_LIBS} ${DRAWING_LIBS})
target_link_libraries(mapper mrp_core mrp_sensor mrp_mapping)
