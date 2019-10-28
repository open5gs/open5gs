# Messages timing extension
PROJECT("Messages timing extension" C)
FD_ADD_EXTENSION(dbg_msg_timings dbg_msg_timings.c)


####
## INSTALL section ##

INSTALL(TARGETS dbg_msg_timings
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)

