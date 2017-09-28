# The dict_dcca extension
PROJECT("Diameter DCCA (Diameter Credit Control Application ) dictionary definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_dcca dict_dcca.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_dcca
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-rfc4006)
