# The rt_ereg extension
PROJECT("Regular expression matching based routing extension" C)

# Check if REG_STARTEND is provided on the host
SET(CHECK_REG_STARTEND_SOURCE_CODE "
	#include <unistd.h>
	#include <regex.h>
	int main() {
	   return regexec(NULL, NULL, 0, NULL, REG_STARTEND);
	}
	")
CHECK_C_SOURCE_COMPILES("${CHECK_REG_STARTEND_SOURCE_CODE}" HAVE_REG_STARTEND)
IF (HAVE_REG_STARTEND)
     ADD_DEFINITIONS(-DHAVE_REG_STARTEND)
ENDIF (HAVE_REG_STARTEND)


# Parser files
BISON_FILE(rtereg_conf.y)
FLEX_FILE(rtereg_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.rtereg_conf.c rtereg_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( RTEREG_SRC
	rtereg.c
	rtereg.h
	lex.rtereg_conf.c
	rtereg_conf.tab.c
	rtereg_conf.tab.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_ereg ${RTEREG_SRC})


####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_ereg
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
