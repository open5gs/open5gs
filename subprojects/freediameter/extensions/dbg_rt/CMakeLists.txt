# The rt_debug extension
PROJECT("Routing module debug extension" C)

# Compile as a module
FD_ADD_EXTENSION(dbg_rt dbg_rt.c)


####
## INSTALL section ##

INSTALL(TARGETS dbg_rt
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)
