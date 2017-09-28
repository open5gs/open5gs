# The app_sip extension
PROJECT("Diameter SIP Application (RFC4740)" C)

FIND_PACKAGE(MySQL REQUIRED)
INCLUDE_DIRECTORIES(${MySQL_INCLUDE_DIR}) 

# Parser files
BISON_FILE(app_sip.y)
FLEX_FILE(app_sip.l)
SET_SOURCE_FILES_PROPERTIES(lex.app_sip.c app_sip.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( DIAM_SIP_SRC
	lex.app_sip.c
	app_sip.tab.c
	app_sip.tab.h
	app_sip.c
	app_sip.h
	libapp_sip.c
	md5.c
	multimediaauth.c
	registrationtermination.c
	userauthorization.c
	pushprofile.c
	serverassignment.c
	locationinfo.c
	locationinfosl.c
)

# Compile as a module
FD_ADD_EXTENSION(app_sip ${DIAM_SIP_SRC})

TARGET_LINK_LIBRARIES(app_sip ${MySQL_LIBRARIES})

SUBDIRS(tools)

####
## INSTALL section ##

INSTALL(TARGETS app_sip
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-sip-server)

INSTALL(FILES README TODO
	DESTINATION /usr/share/doc/freeDiameter-sip-server
	CONFIGURATIONS DebianPackage 
	COMPONENT freeDiameter-sip-server)
