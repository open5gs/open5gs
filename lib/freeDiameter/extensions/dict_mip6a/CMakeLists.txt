# The dict_mip6a extension
PROJECT("Diameter Mobile IPv6 Auth (MIP6A) Dictionary Definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_mip6a dict_mip6a.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_mip6a
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-mip6)
