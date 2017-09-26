# The rt_default extension
PROJECT("Configurable routing extension" C)

# Check if REG_STARTEND is provided on the host
SET(CHECK_REG_STARTEND_SOURCE_CODE "
	#include <unistd.h>
	#include <regex.h>
	int main() {
	   return regexec(NULL, NULL, 0, NULL, REG_STARTEND);
	}
	")
CHECK_C_SOURCE_COMPILES("${CHECK_REG_STARTEND_SOURCE_CODE}" HAVE_REG_STARTEND)
# Generate the host.h file
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/rt_default-host.h.in ${CMAKE_CURRENT_BINARY_DIR}/rt_default-host.h)

# Parser files
BISON_FILE(rtd_conf.y)
FLEX_FILE(rtd_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.rtd_conf.c rtd_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( RT_DEFAULT_SRC
	rt_default.c
	rt_default.h
	lex.rtd_conf.c
	rtd_conf.tab.c
	rtd_conf.tab.h
	rtd_rules.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(rt_default ${RT_DEFAULT_SRC})


####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS rt_default
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
