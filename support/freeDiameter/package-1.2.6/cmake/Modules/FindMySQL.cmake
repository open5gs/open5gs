# - Find mysqlclient
#
# -*- cmake -*-
#
# Find the native MySQL includes and library
#
#  MySQL_INCLUDE_DIR - where to find mysql.h, etc.
#  MySQL_LIBRARIES   - List of libraries when using MySQL.
#  MySQL_FOUND       - True if MySQL found.

IF (MySQL_INCLUDE_DIR AND MySQL_LIBRARY)
  # Already in cache, be silent
  SET(MySQL_FIND_QUIETLY TRUE)
ENDIF (MySQL_INCLUDE_DIR AND MySQL_LIBRARY)

# Include dir
FIND_PATH(MySQL_INCLUDE_DIR 
  NAMES mysql.h
  PATH_SUFFIXES mysql
)

# Library
#SET(MySQL_NAMES mysqlclient mysqlclient_r)
#SET(MySQL_NAMES mysqlclient_r)
SET(MySQL_NAMES mysqlclient_r mysqlclient)
FIND_LIBRARY(MySQL_LIBRARY
  NAMES ${MySQL_NAMES}
  PATHS /usr/lib /usr/local/lib
  PATH_SUFFIXES mysql
)

IF (MySQL_INCLUDE_DIR AND MySQL_LIBRARY)
  SET(MySQL_FOUND TRUE)
  SET( MySQL_LIBRARIES ${MySQL_LIBRARY} )
ELSE (MySQL_INCLUDE_DIR AND MySQL_LIBRARY)
  SET(MySQL_FOUND FALSE)
  SET( MySQL_LIBRARIES )
ENDIF (MySQL_INCLUDE_DIR AND MySQL_LIBRARY)


IF (MySQL_FOUND)
  IF (NOT MySQL_FIND_QUIETLY)
    MESSAGE(STATUS "Found MySQL: ${MySQL_LIBRARY}")
  ENDIF (NOT MySQL_FIND_QUIETLY)
ELSE (MySQL_FOUND)
  IF (MySQL_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for MySQL libraries named ${MySQL_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find MySQL library")
  ENDIF (MySQL_FIND_REQUIRED)
ENDIF (MySQL_FOUND)

MARK_AS_ADVANCED(
  MySQL_LIBRARY
  MySQL_INCLUDE_DIR
  )

