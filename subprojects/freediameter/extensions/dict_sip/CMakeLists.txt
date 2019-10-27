# The dict_sip extension
PROJECT("Diameter SIP (RFC4740) dictionary definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_sip dict_sip.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_sip
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-RFC4740)
