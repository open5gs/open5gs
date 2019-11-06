# - Try to find SCTP library and headers
# Once done, this will define
#
#  SCTP_FOUND - system has SCTP
#  SCTP_INCLUDE_DIR - the SCTP include directories
#  SCTP_LIBRARIES - link these to use SCTP

if (SCTP_INCLUDE_DIR AND SCTP_LIBRARIES)
  set(SCTP_FIND_QUIETLY TRUE)
endif (SCTP_INCLUDE_DIR AND SCTP_LIBRARIES)

# Include dir
find_path(SCTP_INCLUDE_DIR
  NAMES netinet/sctp.h
)

# Library
find_library(SCTP_LIBRARY
  NAMES sctp
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
#set(SCTP_PROCESS_INCLUDES SCTP_INCLUDE_DIR)
#set(SCTP_PROCESS_LIBS SCTP_LIBRARY)
#libfind_process(SCTP)


# handle the QUIETLY and REQUIRED arguments and set SCTP_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SCTP DEFAULT_MSG SCTP_LIBRARY SCTP_INCLUDE_DIR)

# If we successfully found the sctp library then add the library to the
# SCTP_LIBRARIES cmake variable otherwise set SCTP_LIBRARIES to nothing.
IF(SCTP_FOUND)
   SET( SCTP_LIBRARIES ${SCTP_LIBRARY} )
ELSE(SCTP_FOUND)
   SET( SCTP_LIBRARIES )
ENDIF(SCTP_FOUND)


# Lastly make it so that the SCTP_LIBRARY and SCTP_INCLUDE_DIR variables
# only show up under the advanced options in the gui cmake applications.
MARK_AS_ADVANCED( SCTP_LIBRARY SCTP_INCLUDE_DIR )

