# The dict_legacy_xml extension
PROJECT("Legacy XML dictionary files support" C)

# Parser files
BISON_FILE(dict_lxml.y)
FLEX_FILE(dict_lxml.l)
SET_SOURCE_FILES_PROPERTIES(lex.dict_lxml.c dict_lxml.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# We use LibXml2 (from GNOME) to parse XML files
FIND_PACKAGE(LibXml2 REQUIRED)

# List of source files
SET( DICT_LXML_SRC
	dict_lxml.c
	dict_lxml_xml.c
	dict_lxml.h
	lex.dict_lxml.c
	dict_lxml.tab.c
	dict_lxml.tab.h
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(dict_legacy_xml ${DICT_LXML_SRC})

TARGET_LINK_LIBRARIES(dict_legacy_xml ${LIBXML2_LIBRARIES} )


####
## INSTALL section ##

INSTALL(TARGETS dict_legacy_xml
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-legacy)
