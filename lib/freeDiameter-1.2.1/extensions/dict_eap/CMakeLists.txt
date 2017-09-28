# The dict_eap extension
PROJECT("Diameter EAP (RFC4072) dictionary definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_eap dict_eap.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_eap
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-RFC4072)
