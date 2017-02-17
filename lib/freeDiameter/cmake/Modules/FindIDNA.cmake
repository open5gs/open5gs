# - Try to find GNU IDN library and headers
# Once done, this will define
#
#  IDNA_FOUND - system has IDNA
#  IDNA_INCLUDE_DIR - the IDNA include directories (<idna.h>)
#  IDNA_LIBRARIES - link these to use IDNA (idna_to_ascii_8z)

if (IDNA_INCLUDE_DIR AND IDNA_LIBRARIES)
  set(IDNA_FIND_QUIETLY TRUE)
endif (IDNA_INCLUDE_DIR AND IDNA_LIBRARIES)

# Include dir
find_path(IDNA_INCLUDE_DIR
  NAMES idna.h
)

# Library
find_library(IDNA_LIBRARY
  NAMES idn
)


# handle the QUIETLY and REQUIRED arguments and set IDNA_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IDNA DEFAULT_MSG IDNA_LIBRARY IDNA_INCLUDE_DIR)

# If we successfully found the idn library then add the library to the
# IDNA_LIBRARIES cmake variable otherwise set IDNA_LIBRARIES to nothing.
IF(IDNA_FOUND)
   SET( IDNA_LIBRARIES ${IDNA_LIBRARY} )
ELSE(IDNA_FOUND)
   SET( IDNA_LIBRARIES )
ENDIF(IDNA_FOUND)


# Lastly make it so that the IDNA_LIBRARY and IDNA_INCLUDE_DIR variables
# only show up under the advanced options in the gui cmake applications.
MARK_AS_ADVANCED( IDNA_LIBRARY IDNA_INCLUDE_DIR )

