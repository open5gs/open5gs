# The dbg_interactive extension
PROJECT("Interactive debug facility based on SWIG" C)


# This module is using Python
SET(Python_ADDITIONAL_VERSIONS 2.7 2.6 2.5 2.4 2.3 2.2 2.1 2.0)
FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

##### 
# Wrapper to fD in python
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
SET(CMAKE_SWIG_FLAGS -castmode -threads)

# Add the dependencies for re-swig-ing the file
SET(SWIG_MODULE_fDpy_EXTRA_DEPS 
	${CMAKE_BINARY_DIR}/include/freeDiameter/freeDiameter-host.h 
	${CMAKE_SOURCE_DIR}/include/freeDiameter/libfdcore.h 
	${CMAKE_SOURCE_DIR}/include/freeDiameter/libfdproto.h 
	lists.i
	dictionary.i
	sessions.i
	routing.i
	messages.i
	dispatch.i
	queues.i
	peers.i
	events.i
	endpoints.i
	posix.i
	hooks.i
	)
SET_SOURCE_FILES_PROPERTIES(dbg_interactive.i PROPERTIES SWIG_MODULE_NAME fDpy)

# The following code is inspired from SWIG_ADD_MODULE, but we do only what we need
SWIG_MODULE_INITIALIZE(fDpy python)
SWIG_ADD_SOURCE_TO_MODULE(fDpy swig_generated_sources "dbg_interactive.i")
	
# In order to avoid shipping the python file that contains the shadow class definitions, 
# we transform this file in a C-style string and compile it within our software.
# We use for this purpose xxd tool provided with vim package.
FIND_PROGRAM(XXD_EXECUTABLE xxd)
# To avoid the dependency, simply compile it if not provided
if (NOT XXD_EXECUTABLE) 
   SET_SOURCE_FILES_PROPERTIES(helper/xxd.c PROPERTIES COMPILE_DEFINITIONS UNIX)
   ADD_EXECUTABLE(xxd helper/xxd.c)
   SET(XXD_EXECUTABLE xxd)
endif (NOT XXD_EXECUTABLE) 
# And now the magic command
ADD_CUSTOM_COMMAND(
    OUTPUT "fDpy-inc.c"
    COMMAND "${XXD_EXECUTABLE}"
    ARGS "-i"
    "fDpy.py"
    "fDpy-inc.c"
    MAIN_DEPENDENCY "${swig_extra_generated_files}"
    COMMENT "Shadow definitions") 

# Ensure that the generated source files are removed
GET_DIRECTORY_PROPERTY(swig_extra_clean_files ADDITIONAL_MAKE_CLEAN_FILES)
SET_DIRECTORY_PROPERTIES(PROPERTIES
        ADDITIONAL_MAKE_CLEAN_FILES "${swig_extra_clean_files};${swig_generated_sources};fDpy-inc.c")


#####
# Extension that embeds the python interpreter
FD_ADD_EXTENSION(dbg_interactive dbg_interactive.c ${swig_generated_sources} fDpy-inc.c)
TARGET_LINK_LIBRARIES(dbg_interactive ${PYTHON_LIBRARIES})


####
## INSTALL section ##

INSTALL(TARGETS dbg_interactive
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-debug-tools)
