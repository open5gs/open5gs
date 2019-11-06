# The app_redirect extension
PROJECT("Configurable Redirects server extension" C)

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
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/ard-host.h.in ${CMAKE_CURRENT_BINARY_DIR}/ard-host.h)

# Parser files
BISON_FILE(ard_conf.y)
FLEX_FILE(ard_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.ard_conf.c ard_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( APP_REDIR_SRC
	app_redir.c
	app_redir.h
	lex.ard_conf.c
	ard_conf.tab.c
	ard_conf.tab.h
	ard_rules.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(app_redirect ${APP_REDIR_SRC})


####
## INSTALL section ##

INSTALL(TARGETS app_redirect
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-app_redirect)
