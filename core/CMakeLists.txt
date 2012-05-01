#Core lower level items that don't rely on any higher level constructs, or
# are used by mutliple higher level libraries.
add_library(

	mrp_core
	
		pid.cpp
		common.cpp
)
