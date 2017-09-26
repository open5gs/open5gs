# The dict_dcca_3gpp extension
PROJECT("Diameter DCCA (Diameter Credit Control Application) 3GPP dictionary definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_dcca_3gpp dict_dcca_3gpp.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_dcca_3gpp
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-rfc4006-extension-3gpp)
