# The dict_dcca_starent extension
PROJECT("Diameter DCCA (Diameter Credit Control Application) Starent dictionary definitions " C)

# Compile as a module
FD_ADD_EXTENSION(dict_dcca_starent dict_dcca_starent.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_dcca_starent
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-rfc4006-extension-starent)

