# The test_netemul extension
PROJECT("Simple Diameter network emulator proxy extension (latency, PDV, duplicates)" C)

# Parser files
BISON_FILE(test_netemul.y)
FLEX_FILE(test_netemul.l)
SET_SOURCE_FILES_PROPERTIES(lex.test_netemul.c test_netemul.tab.c PROPERTIES COMPILE_FLAGS "-I ${CMAKE_CURRENT_SOURCE_DIR}")

# List of source files
SET( TNEMUL_SRC
	test_netemul.c
	test_netemul.h
	lex.test_netemul.c
	test_netemul.tab.c
	test_netemul.tab.h
	tne_process.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Compile these files as a freeDiameter extension
FD_ADD_EXTENSION(test_netemul ${TNEMUL_SRC})


# math functions
CHECK_FUNCTION_EXISTS (sqrt HAVE_SQRT)
IF (HAVE_SQRT)
   SET(MATH_LIBS "")
ELSE (HAVE_SQRT)
   CHECK_LIBRARY_EXISTS (m sqrt "" HAVE_LIBM)
   IF (HAVE_LIBM)
     SET(MATH_LIBS "-lm")
   ENDIF (HAVE_LIBM)
ENDIF (HAVE_SQRT)

TARGET_LINK_LIBRARIES(test_netemul ${MATH_LIBS})

####
## INSTALL section ##

INSTALL(TARGETS test_netemul
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)
