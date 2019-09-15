# Utilities for app_sip
PROJECT("Diameter SIP Application (RFC4740) tools" C)

ADD_EXECUTABLE(freediameter-sip-rtr app_sip_rtr.c)
ADD_EXECUTABLE(freediameter-sip-ppr app_sip_ppr.c)

####
## INSTALL section ##

INSTALL(TARGETS freediameter-sip-rtr freediameter-sip-ppr
	RUNTIME DESTINATION ${INSTALL_DAEMON_SUFFIX}
	COMPONENT freeDiameter-sip-server)
