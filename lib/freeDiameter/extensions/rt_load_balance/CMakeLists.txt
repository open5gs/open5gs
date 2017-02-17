# The rt_load_balance extension
PROJECT("Routing extension splits requests evenly over multiple hosts, using current load as routing indicator" C)

# List of source files
SET(RT_IGNORE_DH_SRC
	rt_load_balance.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_load_balance ${RT_IGNORE_DH_SRC})

####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_load_balance
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
