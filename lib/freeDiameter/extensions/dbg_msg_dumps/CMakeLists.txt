PROJECT("Messages dump extension" C)
FD_ADD_EXTENSION(dbg_msg_dumps dbg_msg_dumps.c)


####
## INSTALL section ##

INSTALL(TARGETS dbg_msg_dumps
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)

