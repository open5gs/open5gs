# The rt_ignore_dh extension
PROJECT("Routing extension that removes Destination-Host from messages and restores from Proxy-Info for answers" C)

# List of source files
SET(RT_IGNORE_DH_SRC
	rt_ignore_dh.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_ignore_dh ${RT_IGNORE_DH_SRC})

####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_ignore_dh
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
