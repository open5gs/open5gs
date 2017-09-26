# The dict_nasreq extension
PROJECT("Diameter NASREQ (RFC4005) dictionary definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_nasreq dict_nasreq.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_nasreq
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-RFC4005)
