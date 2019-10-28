# - Find gnutls
# Find the native GNUTLS includes and library
#
#  GNUTLS_FOUND - True if gnutls found.
#  GNUTLS_INCLUDE_DIR - where to find gnutls.h, etc.
#  GNUTLS_LIBRARIES - List of libraries when using gnutls.
#  GNUTLS_VERSION_210 - true if GnuTLS version is >= 2.10.0 (does not require additional separate gcrypt initialization)
#  GNUTLS_VERSION_212 - true if GnuTLS version is >= 2.12.0 (supports gnutls_transport_set_vec_push_function)
#  GNUTLS_VERSION_300 - true if GnuTLS version is >= 3.00.0 (x509 verification functions changed)
#  GNUTLS_VERSION_310 - true if GnuTLS version is >= 3.01.0 (stabilization branch with new APIs)

if (GNUTLS_INCLUDE_DIR AND GNUTLS_LIBRARIES)
  set(GNUTLS_FIND_QUIETLY TRUE)
endif (GNUTLS_INCLUDE_DIR AND GNUTLS_LIBRARIES)

# Include dir
find_path(GNUTLS_INCLUDE_DIR
	NAMES
	  gnutls.h
	  gnutls/gnutls.h
)

# Library
find_library(GNUTLS_LIBRARY 
  NAMES gnutls
)

# handle the QUIETLY and REQUIRED arguments and set GNUTLS_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GNUTLS DEFAULT_MSG GNUTLS_LIBRARY GNUTLS_INCLUDE_DIR)

IF(GNUTLS_FOUND)
  SET( GNUTLS_LIBRARIES ${GNUTLS_LIBRARY} )
ELSE(GNUTLS_FOUND)
  SET( GNUTLS_LIBRARIES )
ENDIF(GNUTLS_FOUND)

# Lastly make it so that the GNUTLS_LIBRARY and GNUTLS_INCLUDE_DIR variables
# only show up under the advanced options in the gui cmake applications.
MARK_AS_ADVANCED( GNUTLS_LIBRARY GNUTLS_INCLUDE_DIR )

# Now check if the library is recent. gnutls_hash was added in 2.10.0.
# Also test library is even more recent. gnutls_x509_trust_list_verify_crt was added in 3.00.0.
IF(GNUTLS_FOUND)
  IF( NOT( "${GNUTLS_VERSION_TEST_FOR}" STREQUAL "${GNUTLS_LIBRARY}" ))
    INCLUDE (CheckLibraryExists) 
    MESSAGE(STATUS "Checking GNUTLS version")
    UNSET(GNUTLS_VERSION_210)
    UNSET(GNUTLS_VERSION_210 CACHE)
    UNSET(GNUTLS_VERSION_212)
    UNSET(GNUTLS_VERSION_212 CACHE)
    UNSET(GNUTLS_VERSION_300)
    UNSET(GNUTLS_VERSION_300 CACHE)
    UNSET(GNUTLS_VERSION_310)
    UNSET(GNUTLS_VERSION_310 CACHE)
    GET_FILENAME_COMPONENT(GNUTLS_PATH ${GNUTLS_LIBRARY} PATH)
    CHECK_LIBRARY_EXISTS(gnutls gnutls_hash ${GNUTLS_PATH} GNUTLS_VERSION_210) 
    CHECK_LIBRARY_EXISTS(gnutls gnutls_transport_set_vec_push_function ${GNUTLS_PATH} GNUTLS_VERSION_212) 
    CHECK_LIBRARY_EXISTS(gnutls gnutls_x509_trust_list_verify_crt ${GNUTLS_PATH} GNUTLS_VERSION_300) 
    CHECK_LIBRARY_EXISTS(gnutls gnutls_handshake_set_timeout ${GNUTLS_PATH} GNUTLS_VERSION_310) 
    SET( GNUTLS_VERSION_TEST_FOR ${GNUTLS_LIBRARY} CACHE INTERNAL "Version the test was made against" )
  ENDIF (NOT( "${GNUTLS_VERSION_TEST_FOR}" STREQUAL "${GNUTLS_LIBRARY}" ))
ENDIF(GNUTLS_FOUND)
