# The subproject name
Project("freeDiameter core library" C)

# Configuration parser
BISON_FILE(fdd.y)
FLEX_FILE(fdd.l)
SET_SOURCE_FILES_PROPERTIES(lex.fdd.c fdd.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET(FDCORE_SRC
	fdcore-internal.h
	apps.c
	cnxctx.h
	config.c
	core.c
	cnxctx.c
	endpoints.c
	events.c
	extensions.c
	fifo_stats.c
	hooks.c
	dict_base_proto.c
	messages.c
	queues.c
	peers.c
	p_ce.c
	p_cnx.c
	p_dw.c
	p_dp.c
	p_expiry.c
	p_out.c
	p_psm.c
	p_sr.c
	routing_dispatch.c
	server.c
	tcp.c
	version.c
	)

IF(NOT DISABLE_SCTP)
	SET(FDCORE_SRC ${FDCORE_SRC} sctp.c sctp3436.c)
ENDIF(NOT DISABLE_SCTP)

SET(FDCORE_GEN_SRC
		lex.fdd.c
		fdd.tab.c
		fdd.tab.h
	)
	
# Save the list of files for the tests 
SET(FDCORE_SRC ${FDCORE_SRC} PARENT_SCOPE)
SET(FDCORE_GEN_SRC ${FDCORE_GEN_SRC} PARENT_SCOPE)

# Include path
INCLUDE_DIRECTORIES(${LFDCORE_INCLUDES})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Build the executable
ADD_LIBRARY(libfdcore SHARED ${FDCORE_SRC} ${FDCORE_GEN_SRC})
ADD_DEPENDENCIES(libfdcore version_information)

# Avoid the liblib name, and set the version
SET_TARGET_PROPERTIES(libfdcore PROPERTIES 
	OUTPUT_NAME "fdcore"
	SOVERSION ${FD_PROJECT_VERSION_API}
	VERSION ${FD_PROJECT_VERSION_MAJOR}.${FD_PROJECT_VERSION_MINOR}.${FD_PROJECT_VERSION_REV}
	LINK_INTERFACE_LIBRARIES "${LFDCORE_LINK_INTERFACES}")

# The library itself needs other libraries 
LINK_DIRECTORIES(${CURRENT_BINARY_DIR}/../libfdproto)
TARGET_LINK_LIBRARIES(libfdcore libfdproto ${LFDCORE_LIBS})


####
## INSTALL section ##

INSTALL(TARGETS libfdcore
	LIBRARY DESTINATION ${INSTALL_LIBRARY_SUFFIX}
	COMPONENT freeDiameter-common)
