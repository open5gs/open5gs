# The app_radgw extension
PROJECT("RADIUS/Diameter extensible gateway application for freeDiameter" C)


########### Main app_radgw extension #############

# Parser files
BISON_FILE(rgw_conf.y)
FLEX_FILE(rgw_conf.l)
SET_SOURCE_FILES_PROPERTIES(lex.rgw_conf.c rgw_conf.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( RGW_DEFAULT_SRC
	radius.c
	md5.c
	rgw_msg_codes.c
	rgw_msg_attrtype.c
	rgw_main.c
	lex.rgw_conf.c
	rgw_conf.tab.c
	rgw_conf.tab.h
	rgw_clients.c
	rgw_plugins.c
	rgw_servers.c
	rgw_worker.c
)

SET( RG_COMMON_HEADER
	rgw_common.h
	radius.h
	md5.h
	hostap_compat.h
)

# Compile these files as a freeDiameter extension.
FD_ADD_EXTENSION(app_radgw ${RGW_DEFAULT_SRC} ${RG_COMMON_HEADER})


########### RADIUS/Diameter translation agent plugins (support for RADIUS protocol) ############
# Use the macro RGWX_ADD_PLUGIN(name files...) to create a plugin.
# It is equivalent to add_library with the appropriate parameters
# and naming conventions (.rgwx : Radius GateWay eXtension)
MACRO(RGWX_ADD_PLUGIN PLGNAME)
  ADD_LIBRARY(${PLGNAME} MODULE ${ARGN})
  SET_TARGET_PROPERTIES(${PLGNAME} PROPERTIES PREFIX "" )
  SET_TARGET_PROPERTIES(${PLGNAME} PROPERTIES SUFFIX ".rgwx" )
  INSTALL(TARGETS ${PLGNAME}
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-radius-gateway)
ENDMACRO(RGWX_ADD_PLUGIN)

# Ask unless ALL_EXTENSIONS is set:
MACRO(FD_OPTION_PLUGIN PLGVAR DESCR DEFLT)
   IF (NOT ALL_EXTENSIONS)
     OPTION(BUILD_${PLGVAR} ${DESCR} ${DEFLT})
   ENDIF (NOT ALL_EXTENSIONS)
   IF (BUILD_${PLGVAR} OR ALL_EXTENSIONS)
     SET(${PLGVAR} TRUE)
   ELSE (BUILD_${PLGVAR} OR ALL_EXTENSIONS)
     SET(${PLGVAR} FALSE)
   ENDIF (BUILD_${PLGVAR} OR ALL_EXTENSIONS)
ENDMACRO(FD_OPTION_PLUGIN PLGVAR DESCR DEFLT)


### Debug 
# Example of plugin:

FD_OPTION_PLUGIN(RGWX_SAMPLE "Build sample plugin? (for developers only)" OFF)
 	IF (RGWX_SAMPLE)
 	   RGWX_ADD_PLUGIN(sample ${RG_COMMON_HEADER} rgwx_sample.c)
 	ENDIF (RGWX_SAMPLE)

# A plugin for debug: dumps RADIUS and Diameter messages state at the time the plugin is called.
FD_OPTION_PLUGIN(RGWX_DEBUG "Build debug plugin? (display status of RADIUS and Diameter messages)" ON)
 	IF (RGWX_DEBUG)
 	   RGWX_ADD_PLUGIN(debug ${RG_COMMON_HEADER} rgwx_debug.c)
 	ENDIF (RGWX_DEBUG)


### Authentication, Authorization messages translation.
FD_OPTION_PLUGIN(RGWX_AUTH "Build Authentication & Authorization RADIUS translation plugin? (RFC2865, RFC3579)" ON)
	IF (RGWX_AUTH)
 	   RGWX_ADD_PLUGIN(auth ${RG_COMMON_HEADER} rgwx_auth.c)
	ENDIF (RGWX_AUTH)

### SIP Authentication, Authorization messages translation.
FD_OPTION_PLUGIN(RGWX_SIP "Build SIP RADIUS translation plugin? (RFC4740 or RFC5090)" OFF)
        IF (RGWX_SIP)
           RGWX_ADD_PLUGIN(sip ${RG_COMMON_HEADER} rgwx_sip.c)
        ENDIF (RGWX_SIP)



### Accounting messages translation.
FD_OPTION_PLUGIN(RGWX_ACCT "Build Accounting RADIUS translation plugin? (RFC2866)" ON)
	IF (RGWX_ACCT)
 	   RGWX_ADD_PLUGIN(acct ${RG_COMMON_HEADER} rgwx_acct.c)
	ENDIF (RGWX_ACCT)


### Generic plugin to handle some attributes (either delete them or simply echo them in the answer) 
FD_OPTION_PLUGIN(RGWX_ECHODROP "Build 'echo/drop' plugin? (drop specific RADIUS attributes or echo them in RADIUS answer)" ON)
  	IF (RGWX_ECHODROP)
  	   BISON_FILE(rgwx_echodrop.y)
  	   FLEX_FILE(rgwx_echodrop.l)
  	   SET_SOURCE_FILES_PROPERTIES(lex.rgwx_echodrop.c rgwx_echodrop.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")
 	   RGWX_ADD_PLUGIN(echodrop ${RG_COMMON_HEADER} rgwx_echodrop.h rgwx_echodrop.c lex.rgwx_echodrop.c rgwx_echodrop.tab.c rgwx_echodrop.tab.h )
  	ENDIF (RGWX_ECHODROP)
	
	
####
## INSTALL section ##

INSTALL(TARGETS app_radgw
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-radius-gateway)
	
# Note that all compiled plugins are added by the RGWX_ADD_PLUGIN macro in the component freeDiameter-radius-gateway as well.
