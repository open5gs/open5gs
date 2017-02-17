# The dict_nas_mipv6 extension
PROJECT("MIPv6 NAS-to-HAAA Interaction Dictionary Definition" C)

# Compile as a module
FD_ADD_EXTENSION(dict_nas_mipv6 dict_nas_mipv6.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_nas_mipv6
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-mip6)
