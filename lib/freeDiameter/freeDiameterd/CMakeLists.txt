# The subproject name
Project("freeDiameter simple daemon" C)

# Since it uses the libfdcore.h, it needs the include dependencies
INCLUDE_DIRECTORIES(${LFDCORE_INCLUDES})

# Build the executable
ADD_EXECUTABLE(freeDiameterd main.c)

# The version
SET_TARGET_PROPERTIES(freeDiameterd PROPERTIES 
	VERSION ${FD_PROJECT_VERSION_MAJOR}.${FD_PROJECT_VERSION_MINOR}.${FD_PROJECT_VERSION_REV})

# The link command
TARGET_LINK_LIBRARIES(freeDiameterd libfdproto libfdcore ${GNUTLS_LIBRARIES})

####
## INSTALL section ##

INSTALL(TARGETS freeDiameterd
	RUNTIME DESTINATION ${INSTALL_DAEMON_SUFFIX}
	COMPONENT freeDiameter-daemon)
