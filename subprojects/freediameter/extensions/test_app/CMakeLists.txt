# The test_app extension
PROJECT("Test Diameter Application" C)

# Parser files
BISON_FILE(ta_conf.y)
FLEX_FILE(ta_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.ta_conf.c ta_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( APP_TEST_SRC
	test_app.h
	test_app.c
	lex.ta_conf.c
	ta_conf.tab.c
	ta_conf.tab.h
	ta_dict.c
	ta_cli.c
	ta_bench.c
	ta_serv.c
)

# Compile as a module
FD_ADD_EXTENSION(test_app ${APP_TEST_SRC})


####
## INSTALL section ##

INSTALL(TARGETS test_app
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)
