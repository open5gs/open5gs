# The rt_change_dh extension
PROJECT("Routing extension that modifies the Destination-Host on certain requests messages and restores from Proxy-Info for responses" C)

# List of source files
SET(RT_CHANGE_DH_SRC
	rt_change_dh.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_change_dh ${RT_CHANGE_DH_SRC})

####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_change_dh
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
