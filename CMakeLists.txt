cmake_minimum_required(VERSION 2.4)
PROJECT (dab5267_mrp)

#SET (CMAKE_MODULE_PATH "/usr/local/dcs/share/cmake/Modules")
#SET (CMAKE_MODULE_PATH "/usr/local/dcs/share/cmake/Modules:/usr/share/cmake-2.8/Modules")
INCLUDE (UsePlayerC++)

#Used for visualizing maps / data
find_package(GLUT)
find_package(OpenGL)

#Used for loading/saving map images
find_package(PNG)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
#SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

SET (STD_LIBS pthread rt)
SET (DRAWING_LIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY} X11 Xext)

#Setup the doc target for make to run doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dab5267_mrp.doxyfile.in
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

#archive containing common types/functionality for all mrp algorithms
add_subdirectory(core)

#Algorithms for robot-local space navigation
add_subdirectory(goto)

#Algorithms for robot-local obstacle avoidance with a map-local goal
add_subdirectory(apf)

#Classes for abstracting sensor input and sensor models from their source
add_subdirectory(sensor)

#Classes used for building/using maps
add_subdirectory(mapping)

#Classes/algorithms for path planning
add_subdirectory(nav)


#Tool for creating map files for a png file
add_executable(
  
  mapcreate 
  
    mapconvert.cpp
)
target_link_libraries(
  
  mapcreate 
  
    ${PNG_LIBRARY}
    
    mrp_core
    mrp_mapping
)

#Mapping assignment (#4) executable
PLAYER_ADD_PLAYERCPP_CLIENT(

  mapper 
  
  SOURCES
    mapper.cpp
  
  LINKFLAGS
)
target_link_libraries(

  mapper
    
    ${STD_LIBS}
    ${DRAWING_LIBS}
    ${PNG_LIBRARY}
    
    mrp_core
    mrp_sensor
    mrp_mapping
)

#Final project executable
PLAYER_ADD_PLAYERCPP_CLIENT(
  
  navigator
  
  SOURCES
    navigator.cpp
  
  LINKFLAGS
)
target_link_libraries(
  
  navigator 
  
    ${STD_LIBS} 
    ${DRAWING_LIBS} 
    ${PNG_LIBRARY}
    
    mrp_core
    mrp_sensor
    mrp_mapping
)
