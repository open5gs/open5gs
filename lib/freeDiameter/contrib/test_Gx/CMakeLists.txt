# The dict_nasreq extension
PROJECT("Gx Interface for freediameter" C)

# Compile as a module
FD_ADD_EXTENSION(app_gx main_gx.c)


####
## INSTALL section ##

INSTALL(TARGETS app_gx 
       LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
       COMPONENT freeDiameter-rx-interface)
