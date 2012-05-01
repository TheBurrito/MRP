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

#Setup the doc target for make to run doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dab5267_mrp.doyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/dab5267_mrp.doxyfile
    @ONLY
  )
  
  add_custom_target(
    
    doc
    
      mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/doc
      COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/doc/*
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/dab5267_mrp.doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif(DOXYGEN_FOUND)

#INCLUDE (dab5267_mrp.cmake)
#PLAYER_ADD_PLAYERCPP_CLIENT (dummy SOURCES dummy.cpp)

include_directories(include /opt/playerstage/player-3.0.2/include/player-3.0)

add_subdirectory(core)
add_subdirectory(goto)
add_subdirectory(apf)
add_subdirectory(sensor)
add_subdirectory(mapping)
add_subdirectory(nav)

PLAYER_ADD_PLAYERCPP_CLIENT (mapper SOURCES mapper.cpp LINKFLAGS)
target_link_libraries(mapper ${STD_LIBS} ${DRAWING_LIBS})
target_link_libraries(mapper mrp_core mrp_sensor mrp_mapping)
