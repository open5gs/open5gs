# The dict_rfc5777 extension
PROJECT("Traffic Classification and Quality of Service (QoS) Attributes for Diameter (RFC 5777)" C)

# Compile as a module
FD_ADD_EXTENSION(dict_rfc5777 dict_rfc5777.c)


####
## INSTALL section ##

INSTALL(TARGETS dict_rfc5777
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-RFC5777)
