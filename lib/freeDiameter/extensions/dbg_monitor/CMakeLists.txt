# Monitoring extension
PROJECT("Monitor extension" C)
FD_ADD_EXTENSION(dbg_monitor dbg_monitor.c)


####
## INSTALL section ##

INSTALL(TARGETS dbg_monitor
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)

