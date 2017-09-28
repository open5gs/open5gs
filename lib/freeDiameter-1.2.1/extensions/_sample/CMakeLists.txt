# The sample extension
PROJECT("Sample extension")

# Compile as a module
FD_ADD_EXTENSION(dbg_sample sample.c hello.cpp fini.c)


####
## INSTALL section ##

# Uncomment the following lines to have the extension installed
# INSTALL(TARGETS dbg_sample
# 	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
# 	COMPONENT freeDiameter-debug-tools)
