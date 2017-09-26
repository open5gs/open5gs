# The rt_busypeer extension
PROJECT("Handling of TOO_BUSY messages and relay timeout capability routing extension" C)

# Parser files
BISON_FILE(rtbusy_conf.y)
FLEX_FILE(rtbusy_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.rtbusy_conf.c rtbusy_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( RTBUSY_SRC
	rtbusy.c
	rtbusy.h
	lex.rtbusy_conf.c
	rtbusy_conf.tab.c
	rtbusy_conf.tab.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_busypeers ${RTBUSY_SRC})


####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_busypeers
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
