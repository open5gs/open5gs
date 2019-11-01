# The rt_randomize extension
PROJECT("Routing extension randomly increases the routing count for one of the highest-rated hosts, if there are multiple ones" C)

# List of source files
SET(RT_RANDOMIZE_SRC
	rt_randomize.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_randomize ${RT_RANDOMIZE_SRC})

####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_randomize
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
