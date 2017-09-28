# The dict_mip6i extension
PROJECT("Diameter Mobile IPv6 IKE (MIP6I) Dictionary Definition" C)

# Compile as a module
FD_ADD_EXTENSION(dict_mip6i dict_mip6i.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_mip6i
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-mip6)
