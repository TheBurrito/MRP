# Sets up the various "libraries" of code made available by the ridiculous
# number of source files I have setup. The idea is to break up the code into
# coherant segments that can be subdivided into sub folders in the future.

#Header files are included in the targets for the sake of sanity in the Eclipse project
#generation.

#some common library sets based on task
SET (MRP_MAPPING_LIBS mrp_core mrp_sensor mrp_mapping)
SET (MRP_NAV_LIBS ${MRP_MAPPING_LIBS} mrp_nav)

#Setup the doc target for make to run doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dab5267_mrp.in
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

#Core lower level items that don't rely on any higher level constructs, or
# are used by mutliple higher level libraries.
add_library(

	mrp_core
	
	  types.h types.cpp
		pid.h pid.cpp
		path.h path.cpp
		common.h common.cpp
		dynamicgrid.h #dynamicgrid.cpp
)

#Collection of APF algorithms and objects
add_library(

	mrp_apf
	
		apf.h apf.cpp forceinvdist.cpp
		mindistcalc.h mindistcalc.cpp
)

#Collection of Goto algorithms and objects
add_library(

	mrp_goto
	
		goto.h goto.cpp
		simplegoto.h simplegoto.cpp
		cosgoto.h cosgoto.cpp
)

#Various sensors available to other portions of code
add_library(

	mrp_sensor
	
		sensor-ranging.h
		sensor-laser.h sensor-laser.cpp
		sensor-sonar.h sensor-sonar.cpp
)

#The library responsible for building maps out of sensor data or from files
add_library(

	mrp_mapping
	
		map.h map.cpp
		dynamicmap.h dynamicmap.cpp
		sonarmodel.h sonarmodel.cpp
)

#Higher level path planning algorithms. Will include algorithms such as A*
add_library(

  mrp_nav
  
    gridvoronoi.h gridvoronoi.cpp
)
