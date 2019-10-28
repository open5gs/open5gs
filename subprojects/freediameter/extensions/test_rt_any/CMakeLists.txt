# The test_rt_any extension
PROJECT("Random routing extension (for tests)" C)

# Compile as a module
FD_ADD_EXTENSION(test_rt_any test_rt_any.c)


####
## INSTALL section ##

INSTALL(TARGETS test_rt_any
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)
