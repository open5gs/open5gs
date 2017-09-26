# The test_sip extension
PROJECT("Diameter-SIP client Test Application (RFC4740)" C)

FIND_PACKAGE(MySQL REQUIRED)
INCLUDE_DIRECTORIES(${MySQL_INCLUDE_DIR}) 

# List of source files
SET( TEST_SIP_SRC
	test_sip.c
	test_sip.h
	multimediaauth.c
	locationinfo.c
	locationinfosl.c
	registrationtermination.c
	userauthorization.c
	serverassignment.c
)

# Compile as a module
FD_ADD_EXTENSION(test_sip ${TEST_SIP_SRC})

TARGET_LINK_LIBRARIES(test_sip ${MySQL_LIBRARIES})


####
## INSTALL section ##

INSTALL(TARGETS test_sip
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)
