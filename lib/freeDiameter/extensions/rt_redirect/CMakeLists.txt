# The rt_redirect extension
PROJECT("Diameter Redirect messages support" C)

SET(RT_REDIR_SRC
	rt_redir.h
	rt_redir.c
	redir_entries.c
	redir_expiry.c
	redir_fwd.c
	redir_out.c
	)

# Compile as a module
FD_ADD_EXTENSION(rt_redirect ${RT_REDIR_SRC})


####
## INSTALL section ##

INSTALL(TARGETS rt_redirect
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-daemon)
