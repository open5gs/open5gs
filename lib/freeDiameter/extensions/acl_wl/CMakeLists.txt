# The acl_wl extension
PROJECT("Access Control / White List" C)

# Parser files
BISON_FILE(aw_conf.y)
FLEX_FILE(aw_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.aw_conf.c aw_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( ACL_WL_SRC
	acl_wl.h
	acl_wl.c
	aw_tree.c
	lex.aw_conf.c
	aw_conf.tab.c
	aw_conf.tab.h
)

# Compile as a module
FD_ADD_EXTENSION(acl_wl ${ACL_WL_SRC})


####
## INSTALL section ##

# We install with the daemon component because it is a base feature.
INSTALL(TARGETS acl_wl
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
