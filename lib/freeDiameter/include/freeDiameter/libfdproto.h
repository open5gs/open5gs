/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2013, WIDE Project and NICT								 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY S_OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

/* This file contains the definitions of functions and types used by the libfreeDiameter library.
 *
 * This library is meant to be used by both the freeDiameter daemon and its extensions.
 * It provides the tools to manipulate Diameter messages and related data.
 * This file should always be included as #include <freeDiameter/libfreeDiameter.h>
 *
 * If any change is made to this file, you must increment the FD_PROJECT_VERSION_API version.
 *
 * The file contains the following parts:
 *	DEBUG
 *	MACROS
 *      OCTET STRINGS
 *	THREADS
 *	LISTS
 *	DICTIONARY
 *	SESSIONS
 *	MESSAGES
 *	DISPATCH
 *	QUEUES
 */

#ifndef _LIBFDPROTO_H
#define _LIBFDPROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FD_IS_CONFIG
#error "You must include 'freeDiameter-host.h' before this file."
#endif /* FD_IS_CONFIG */

#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include <libgen.h>	/* for basename */

#ifdef SWIG
#define _ATTRIBUTE_PRINTFLIKE_(_f,_v)
#else
#define _ATTRIBUTE_PRINTFLIKE_(_f,_v) __attribute__ ((format (printf, _f, _v)))
#endif /* SWIG */

/* Remove some deprecated warnings from some gnutls versions, when possible */
#if defined(__GNUC__)
# define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
# define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
# if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406		/* 4.6.x */
#  define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(push) \
     GCC_DIAG_PRAGMA(ignored x)
#  define GCC_DIAG_ON(x) GCC_DIAG_PRAGMA(pop)
# else							/* older */
#  define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(ignored x)
#  define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(warning x)
# endif
#else
# define GCC_DIAG_OFF(x)
# define GCC_DIAG_ON(x)
#endif

/*============================================================*/
/*                       CONSTANTS                            */
/*============================================================*/

#define DIAMETER_PORT		3868
#define DIAMETER_SECURE_PORT	5658


/*============================================================*/
/*                          INIT                              */
/*============================================================*/

/* This function must be called first, before any call to another library function */
int fd_libproto_init(void); /* note if you are using libfdcore, it handles this already */

/* Call this one when the application terminates, to destroy internal threads */
void fd_libproto_fini(void);

/* Retrieve the version of the binary */
extern const char fd_libproto_version[];

/*============================================================*/
/*                          DEBUG                             */
/*============================================================*/


/*
 * FUNCTION:	fd_log
 *
 * PARAMETERS:
 *  loglevel	: Integer, how important the message is. Valid values are macros FD_LOG_*
 *  format 	: Same format string as in the printf function
 *  ...		: Same list as printf
 *
 * DESCRIPTION: 
 * Write information to log.
 * The format and arguments may contain UTF-8 encoded data. The
 * output medium is expected to support this encoding.
 *
 * RETURN VALUE:
 *  None.
 */
void fd_log ( int, const char *, ... ) _ATTRIBUTE_PRINTFLIKE_(2,3);
#ifndef SWIG
void fd_log_va( int, const char *, va_list);
#endif /* SWIG */

/* these are internal objects of the debug facility, 
might be useful to control the behavior from outside */
extern pthread_mutex_t	fd_log_lock;
extern char * fd_debug_one_function;
extern char * fd_debug_one_file;

/*
 * FUNCTION:	fd_log_threadname
 *
 * PARAMETERS:
 *  name 	: \0-terminated string containing a name to identify the current thread.
 *
 * DESCRIPTION: 
 *  Name the current thread, useful for debugging multi-threaded problems.
 *
 * This function assumes that a global thread-specific key called "fd_log_thname" exists
 * in the address space of the current process.
 *
 * RETURN VALUE:
 *  None.
 */
void fd_log_threadname ( const char * name );
extern pthread_key_t	fd_log_thname;

/*
 * FUNCTION:	fd_log_time
 *
 * PARAMETERS:
 *  ts	 	: The timestamp to log, or NULL for "now"
 *  buf 	: An array where the time must be stored
 *  len		: size of the buffer
 *  incl_date   : The day of year is included in the output
 *  incl_ms     : millisecond value is included in the output
 *
 * DESCRIPTION: 
 *  Writes the timestamp (in human readable format) in a buffer. 
 *
 * RETURN VALUE:
 *  pointer to buf.
 */
char * fd_log_time ( struct timespec * ts, char * buf, size_t len, int incl_date, int incl_ms );

/*
 * FUNCTION:    fd_log_handler_register
 * MACRO:
 *
 * PARAMETERS:
 *  loglevel    : priority of the message
 *  format      : Same format string as in the printf function
 *  va_list     : Argument list
 *
 * DESCRIPTION:
 * Register an external method for logging purposes.
 *
 * RETURN VALUE:
 * int          : Success or failure
 */
int fd_log_handler_register ( void (*logger)(int loglevel, const char * format, va_list args) );

/*
 * FUNCTION:    fd_log_handler_unregister
 * MACRO:
 *
 * PARAMETERS:
 *
 * DESCRIPTION:
 * Unregister the external logging function.
 *
 * RETURN VALUE:
 * int          : Success or failure
 */
int fd_log_handler_unregister ( void );


/* All dump functions follow this same prototype:
 * PARAMETERS:
 *   buf   : *buf can be NULL on entry, it will be malloc'd. Otherwise it is realloc'd if needed.
 *   len   : the current size of the buffer (in/out)
 *   offset: (optional) if provided, starts writing dump at offset in the buffer, and updated upon exit. if NULL, starts at offset O.
 *
 * RETURN VALUE:
 *   *buf upon success, NULL upon failure.
 *
 * REMARKS:
 *  - After the buffer has been used, it should be freed.
 *  - Depending on the function, the created string may be multi-line. However, it should never be terminated with a '\n'.
 */
#define DECLARE_FD_DUMP_PROTOTYPE( function_name, args... )	\
	char * function_name(char ** buf, size_t *len, size_t *offset, ##args)

#ifdef SWIG
#define DECLARE_FD_DUMP_PROTOTYPE_simple( function_name )	\
	char * function_name(char ** buf, size_t *len, size_t *offset)
#endif /* SWIG */
	

/* Helper functions for the *dump functions that add into a buffer */
DECLARE_FD_DUMP_PROTOTYPE( fd_dump_extend, const char * format, ... ) _ATTRIBUTE_PRINTFLIKE_(4,5);
DECLARE_FD_DUMP_PROTOTYPE( fd_dump_extend_hexdump, uint8_t *data, size_t datalen, size_t trunc, size_t wrap );


/* Some helpers macro for writing such *_dump routine */
#define FD_DUMP_STD_PARAMS  buf, len, offset
#define FD_DUMP_HANDLE_OFFSET()  size_t o = 0; if (!offset) offset = &o; if (buf && (*buf) && !(*offset)) **buf='\0'
#define FD_DUMP_HANDLE_TRAIL()	while ((*buf) && (*offset > 0) && ((*buf)[*offset - 1] == '\n')) { *offset -= 1; (*buf)[*offset] = '\0'; }



/*============================================================*/
/*                    DEBUG MACROS                            */
/*============================================================*/

#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif /* ASSERT */

/* log levels definitions, that are passed to the logger */
#define FD_LOG_ANNOYING  0  /* very verbose loops and such "overkill" traces. Only active when the framework is compiled in DEBUG mode. */
#define FD_LOG_DEBUG     1  /* Get a detailed sense of what is going on in the framework. Use this level for normal debug */
#define FD_LOG_NOTICE    3  /* Normal execution states worth noting */
#define FD_LOG_ERROR     5  /* Recoverable or expected error conditions */
#define FD_LOG_FATAL     6  /* Unrecoverable error, e.g. malloc fail, etc. that requires the framework to shutdown */

/* The level used by the default logger, can be changed by command-line arguments. Ignored for other loggers. */
extern int fd_g_debug_lvl;

/* Some portability code to get nice function name in __PRETTY_FUNCTION__ */
#if (!defined( __func__)) && (__STDC_VERSION__ < 199901L)
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else /* __GNUC__ >= 2 */
#  define __func__ "<unknown>"
# endif /* __GNUC__ >= 2 */
#endif /*(!defined( __func__)) && (__STDC_VERSION__ < 199901L) */
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __func__
#endif /* __PRETTY_FUNCTION__ */

/* A version of __FILE__ without the full path */
static char * file_bname = NULL;
static char * file_bname_init(char * full) { file_bname = basename(full); return file_bname; }
#define __STRIPPED_FILE__	(file_bname ?: file_bname_init((char *)__FILE__))



/* In DEBUG mode, we add meta-information along each trace. This makes multi-threading problems easier to debug. */
#if (defined(DEBUG) && !defined(DEBUG_WITHOUT_META))
# define STD_TRACE_FMT_STRING "pid:%s in %s@%s:%d: "
# define STD_TRACE_FMT_ARGS   , ((char *)pthread_getspecific(fd_log_thname) ?: "unnamed"), __PRETTY_FUNCTION__, __STRIPPED_FILE__, __LINE__
#else /* DEBUG && !DEBUG_WITHOUT_META */
# define STD_TRACE_FMT_STRING ""
# define STD_TRACE_FMT_ARGS
#endif /* DEBUG && !DEBUG_WITHOUT_META */

/*************************
  The general debug macro
 *************************/
#define LOG(printlevel,format,args... ) \
	fd_log((printlevel), STD_TRACE_FMT_STRING format STD_TRACE_FMT_ARGS, ## args)

/*
 * Use the following macros in the code to get traces with location & pid in debug mode: 
 */
#ifdef DEBUG
# define LOG_A(format,args... ) \
		do { if ((fd_debug_one_function && !strcmp(fd_debug_one_function, __PRETTY_FUNCTION__)) \
		 || (fd_debug_one_file && !strcmp(fd_debug_one_file, __STRIPPED_FILE__) ) ) {		\
		 	LOG(FD_LOG_DEBUG,"[DBG_MATCH] " format,##args);					\
		} else {										\
			LOG(FD_LOG_ANNOYING,format,##args);						\
		} } while (0)
#else /* DEBUG */
# define LOG_A(format,args... ) /* not defined in release */
#endif /* DEBUG */

/* Debug information useful to follow in detail what is going on */
#define LOG_D(format,args... ) \
		LOG(FD_LOG_DEBUG, format, ##args)

/* Report a normal message that is useful for normal admin monitoring */
#define LOG_N(format,args... ) \
		LOG(FD_LOG_NOTICE, format,##args)

/* Report an error */
#define LOG_E(format,args... ) \
		LOG(FD_LOG_ERROR, format, ##args)

/* Report a fatal error */
#define LOG_F(format,args... ) \
		LOG(FD_LOG_FATAL, format, ##args)


/*************
 Derivatives
 ************/
/* Trace a binary buffer content */
#define LOG_BUFFER(printlevel, prefix, buf, bufsz, suffix ) {								\
	int __i;													\
	size_t __sz = (size_t)(bufsz);											\
	uint8_t * __buf = (uint8_t *)(buf);										\
	char __strbuf[1024+1];												\
	for (__i = 0; (__i < __sz) && (__i<(sizeof(__strbuf)/2)); __i++) {						\
		sprintf(__strbuf + (2 * __i), "%02hhx", __buf[__i]);     						\
	}														\
        fd_log(printlevel, STD_TRACE_FMT_STRING "%s%s%s" STD_TRACE_FMT_ARGS,  						\
               (prefix), __strbuf, (suffix));										\
}

/* Split a multi-line buffer into separate calls to the LOG function. */
#define LOG_SPLIT(printlevel, per_line_prefix, mlbuf, per_line_suffix ) {						\
	char * __line = (mlbuf), *__next;										\
	char * __p = (per_line_prefix), *__s = (per_line_suffix);							\
	while ((__next = strchr(__line, '\n')) != NULL) {								\
		LOG(printlevel, "%s%.*s%s", __p ?:"", (int)(__next - __line), __line, __s ?:"");			\
		__line = __next + 1;											\
	}														\
	LOG(printlevel, "%s%s%s", __p ?:"", __line, __s ?:"");								\
}

/* Helper for function entry -- for very detailed trace of the execution */
#define TRACE_ENTRY(_format,_args... ) \
		LOG_A("[enter] %s(" _format ") {" #_args "}", __PRETTY_FUNCTION__, ##_args );

/* Helper for debugging by adding traces -- for debuging a specific location of the code */
#define TRACE_HERE()	\
		LOG_F(" -- debug checkpoint %d -- ", fd_breakhere());
int fd_breakhere(void);

/* Helper for tracing the CHECK_* macros below -- very very verbose code execution! */
#define TRACE_CALL( str... ) 	\
	 	LOG_A( str )

/* For development only, to keep track of TODO locations in the code */
#ifndef ERRORS_ON_TODO
# define TODO( _msg, _args... ) \
		LOG_F( "TODO: " _msg , ##_args);
#else /* ERRORS_ON_TODO */
# define TODO( _msg, _args... ) \
		"TODO" = _msg ## _args; /* just a stupid compilation error to spot the todo */
#endif /* ERRORS_ON_TODO */


/*============================================================*/
/*                  ERROR CHECKING MACRO                      */
/*============================================================*/

/* Macros to check a return value and branch out in case of error.
 * These macro additionally provide the logging information.
 *
 * The name "__ret__" is always available in the __fallback__ parameter and contains the error code.
 */
 
#define CHECK_PRELUDE(__call__) 			\
		int __ret__; 				\
		TRACE_CALL("Check: %s", #__call__ );	\
		__ret__ = (__call__)
	
#define DEFAULT_FB	return __ret__;

/* System check: error case if < 0, error value in errno */
#define CHECK_SYS_GEN( faillevel, __call__, __fallback__  ) { 						\
		CHECK_PRELUDE(__call__);								\
		if (__ret__ < 0) {									\
			__ret__ = errno;								\
			LOG(faillevel, "ERROR: in '%s' :\t%s",  #__call__ , strerror(__ret__));    	\
			__fallback__;									\
		}											\
}


/* Check the return value of a function and execute fallback in case of error or special value */
#define CHECK_FCT_GEN2( faillevel, __call__, __speval__, __fallback1__, __fallback2__ ) {		\
		CHECK_PRELUDE(__call__);								\
		if (__ret__ != 0) {									\
			if (__ret__ == (__speval__)) {							\
				__fallback1__;								\
			} else {									\
				LOG(faillevel, "ERROR: in '%s' :\t%s", #__call__ , strerror(__ret__));	\
				__fallback2__;								\
			}										\
		}											\
}

/* Check the return value of a function and execute fallback in case of error (return value different from 0) */
#define CHECK_FCT_GEN( faillevel, __call__, __fallback__) \
	       CHECK_FCT_GEN2( faillevel, (__call__), 0, , (__fallback__) )

/* Check that a memory allocator did not return NULL, otherwise log an error and execute fallback */
#define CHECK_MALLOC_GEN( faillevel, __call__, __fallback__ ) { 				       \
	       void *  __ptr__; 								       \
	       TRACE_CALL("Check: %s", #__call__ );						       \
	       __ptr__ = (void *)(__call__);							       \
	       if (__ptr__ == NULL) {								       \
		       int __ret__ = errno;							       \
		       LOG(faillevel, "ERROR: in '%s' :\t%s",  #__call__ , strerror(__ret__));         \
		       __fallback__;								       \
	       }										       \
}

/* Check parameters at function entry, execute fallback on error */
#define CHECK_PARAMS_GEN( faillevel, __bool__, __fallback__ ) {					       \
	       TRACE_CALL("Check: %s", #__bool__ );						       \
	       if ( ! (__bool__) ) {								       \
		       int __ret__ = EINVAL;							       \
		       LOG(faillevel, "ERROR: invalid parameter '%s'",  #__bool__ );  	       	       \
		       __fallback__;								       \
	       }										       \
}


/*============================================================*/
/*          COMPATIBILITY MACROS, TO BE REMOVED		      */
/*============================================================*/
/* Redefine the old macros for transition of the code */
#ifndef EXCLUDE_DEPRECATED

#define MARK_DEPRECATED	/* __attribute__ ((deprecated)) */

enum old_levels {
	NONE = 0,
	INFO = 1,
	FULL = 2,
	ANNOYING = 4,
	FCTS = 6,
	CALL = 9
} MARK_DEPRECATED;

static __inline__ int old_TRACE_BOOL( enum old_levels level, const char * file, const char * func ) MARK_DEPRECATED
{
	if ((fd_debug_one_function && !strcmp(fd_debug_one_function, func))
		|| (fd_debug_one_file && !strcmp(fd_debug_one_file, file) ))
		return 2; /* Level override */
	if (level <= fd_g_debug_lvl)
		return 1; /* Normal level */
	return 0;  /* No trace */
}
#define TRACE_BOOL(level)  old_TRACE_BOOL((level), __STRIPPED_FILE__, __PRETTY_FUNCTION__)

#ifndef SWIG
static __inline__ void fd_log_deprecated( int level, const char *format, ... ) MARK_DEPRECATED
{ 
	va_list ap;
	va_start(ap, format);
	fd_log_va(level, format, ap);
	va_end(ap);
}
#else /* SWIG */
void fd_log_deprecated( int level, const char *format, ... );
#endif /* SWIG */
static __inline__ void replace_me() MARK_DEPRECATED { }

#define TRACE_BUFFER(...) replace_me();
#define TRACE_NOTICE(...) replace_me();


/* Use the LOG_* instead, or use the new *_dump functions when dumping an object */
#define fd_log_debug(format,args...)  fd_log_deprecated(FD_LOG_DEBUG, format, ## args)
#define fd_log_notice(format,args...) fd_log_deprecated(FD_LOG_NOTICE, format, ## args)
#define fd_log_error(format,args...)  fd_log_deprecated(FD_LOG_ERROR, format, ## args)

/* old macro for traces. To be replaced by appropriate LOG_* macros. */
# define TRACE_DEBUG(oldlevel, format,args... ) {					\
		int __l__;								\
		if ((__l__ = TRACE_BOOL(oldlevel))) {					\
			if      (oldlevel <= NONE) { LOG_E(format,##args); }		\
			else if (oldlevel <= INFO) { LOG_N(format,##args); }		\
			else if (__l__ == 2)       { LOG_N(format,##args); }		\
			else if (oldlevel <= FULL) { LOG_D(format,##args); }		\
			else                       { LOG_A(format,##args); }		\
}		}

/* the following macro must be replaced with LOG_E or LOG_F */
# define TRACE_ERROR	LOG_E


/* The following macros are missing the faillevel information, which indicates at what log level the error case should be displayed. */
# define CHECK_SYS_DO( __call__, __fallback__  ) { 							\
		CHECK_PRELUDE(__call__);								\
		if (__ret__ < 0) {									\
			__ret__ = errno;								\
			TRACE_ERROR("ERROR: in '%s' :\t%s",  #__call__ , strerror(__ret__));    	\
			__fallback__;									\
		}											\
}

# define CHECK_SYS( __call__  ) \
		CHECK_SYS_DO( (__call__), return __ret__  )


# define CHECK_POSIX_DO2( __call__, __speval__, __fallback1__, __fallback2__ ) {			\
		CHECK_PRELUDE(__call__);								\
		if (__ret__ != 0) {									\
			if (__ret__ == (__speval__)) {							\
				__fallback1__;								\
			} else {									\
				TRACE_ERROR("ERROR: in '%s' :\t%s", #__call__ , strerror(__ret__));	\
				__fallback2__;								\
			}										\
		}											\
}

# define CHECK_POSIX_DO( __call__, __fallback__ )	\
		CHECK_POSIX_DO2( (__call__), 0, , __fallback__ )

# define CHECK_POSIX( __call__ )	\
		CHECK_POSIX_DO( (__call__), return __ret__ )
		
# define CHECK_MALLOC_DO( __call__, __fallback__ ) { 				   		       \
	       void *  __ptr__; 								       \
	       TRACE_CALL("Check: %s", #__call__ );						       \
	       __ptr__ = (void *)(__call__);							       \
	       if (__ptr__ == NULL) {								       \
		       int __ret__ = errno;							       \
		      TRACE_ERROR("ERROR: in '%s' :\t%s",  #__call__ , strerror(__ret__));             \
		       __fallback__;								       \
	       }										       \
}

# define CHECK_MALLOC( __call__ )	\
		CHECK_MALLOC_DO( (__call__), return __ret__ )
	
# define CHECK_PARAMS_DO( __bool__, __fallback__ ) {					       	       \
	       TRACE_CALL("Check: %s", #__bool__ );						       \
	       if ( ! (__bool__) ) {								       \
		       int __ret__ = EINVAL;							       \
		       TRACE_ERROR("ERROR: Invalid parameter '%s', %d",  #__bool__, __ret__ );         \
		       __fallback__;								       \
	       }										       \
}

# define CHECK_PARAMS( __bool__ )	\
		CHECK_PARAMS_DO( (__bool__), return __ret__ )

# define CHECK_FCT_DO	CHECK_POSIX_DO
# define CHECK_FCT	CHECK_POSIX

#endif /* EXCLUDE_DEPRECATED */
	       

/*============================================================*/
/*	Optimized code: remove all debugging code	      */
/*============================================================*/
#ifdef STRIP_DEBUG_CODE
#undef LOG_D
#undef LOG_N
#undef LOG_E
#undef LOG_F
#undef LOG_BUFFER

#define LOG_D(format,args... ) /* noop */
#define LOG_N(format,args...) fd_log(FD_LOG_NOTICE, format, ## args)
#define LOG_E(format,args...) fd_log(FD_LOG_ERROR, format, ## args)
#define LOG_F(format,args...) fd_log(FD_LOG_FATAL, format, ## args)
#define LOG_BUFFER(printlevel, level, prefix, buf, bufsz, suffix ) {								\
	if (printlevel > FD_LOG_DEBUG) {											\
		int __i;													\
		size_t __sz = (size_t)(bufsz);											\
		uint8_t * __buf = (uint8_t *)(buf);										\
		char * __strbuf[1024+1];											\
		for (__i = 0; (__i < __sz) && (__i<(sizeof(__strbuf)/2); __i++) {						\
			sprintf(__strbuf + (2 * __i), "%02.2hhx", __buf[__i]);     						\
		}														\
                fd_log(printlevel, prefix"%s"suffix, __strbuf);									\
	}
#endif /* STRIP_DEBUG_CODE */

/*============================================================*/
/*		    OTHER MACROS			      */
/*============================================================*/
/* helper macros (pre-processor hacks to allow macro arguments) */
#define __tostr( arg )  #arg
#define _stringize( arg ) __tostr( arg )
#define __agr( arg1, arg2 ) arg1 ## arg2
#define _aggregate( arg1, arg2 ) __agr( arg1, arg2 )

/* Some aliases to socket addresses structures */
#define sSS	struct sockaddr_storage
#define sSA	struct sockaddr
#define sSA4	struct sockaddr_in
#define sSA6	struct sockaddr_in6

/* The sockaddr length of a sSS structure */
#define sSAlen( _sa_ )	\
	( (socklen_t) ( (((sSA *)_sa_)->sa_family == AF_INET) ? (sizeof(sSA4)) :		\
				((((sSA *)_sa_)->sa_family == AF_INET6) ? (sizeof(sSA6)) :	\
					0 ) ) )
#define sSAport( _sa_ )	\
	( (socklen_t) ( (((sSA *)_sa_)->sa_family == AF_INET) ? (((sSA4 *)(_sa_))->sin_port) :		\
				((((sSA *)_sa_)->sa_family == AF_INET6) ? (((sSA6 *)(_sa_))->sin6_port) :	\
					0 ) ) )

DECLARE_FD_DUMP_PROTOTYPE(fd_sa_dump, sSA * sa, int flags);
#define sSA_DUMP_STRLEN	(INET6_ADDRSTRLEN + 1 + 32 + 2)
void fd_sa_sdump_numeric(char * buf /* must be at least sSA_DUMP_STRLEN */, sSA * sa);


/* A l4 protocol name (TCP / SCTP) */
#ifdef DISABLE_SCTP
#define IPPROTO_NAME( _proto )					\
	(((_proto) == IPPROTO_TCP) ? "TCP" :			\
			"Unknown")
#else /* DISABLE_SCTP */
#define IPPROTO_NAME( _proto )					\
	( ((_proto) == IPPROTO_TCP) ? "TCP" :			\
		(((_proto) == IPPROTO_SCTP) ? "SCTP" :		\
			"Unknown"))
#endif /* DISABLE_SCTP */

/* Define the value of IP loopback address */
#ifndef INADDR_LOOPBACK 
#define INADDR_LOOPBACK	inet_addr("127.0.0.1")
#endif /* INADDR_LOOPBACK */

#ifndef INADDR_BROADCAST
#define	INADDR_BROADCAST	((in_addr_t) 0xffffffff)
#endif /* INADDR_BROADCAST */

/* An IP equivalent to IN6_IS_ADDR_LOOPBACK */
#ifndef IN_IS_ADDR_LOOPBACK
#define IN_IS_ADDR_LOOPBACK(a) \
  ((((long int) (a)->s_addr) & ntohl(0xff000000)) == ntohl(0x7f000000))
#endif /* IN_IS_ADDR_LOOPBACK */

/* An IP equivalent to IN6_IS_ADDR_UNSPECIFIED */
#ifndef IN_IS_ADDR_UNSPECIFIED
#define IN_IS_ADDR_UNSPECIFIED(a) \
  (((long int) (a)->s_addr) == 0x00000000)
#endif /* IN_IS_ADDR_UNSPECIFIED */

/* create a V4MAPPED address */
#define IN6_ADDR_V4MAP( a6, a4 ) {			\
	((uint32_t *)(a6))[0] = 0;			\
	((uint32_t *)(a6))[1] = 0;			\
	((uint32_t *)(a6))[2] = htonl(0xffff);		\
	((uint32_t *)(a6))[3] = (uint32_t)(a4);		\
}

/* Retrieve a v4 value from V4MAPPED address ( takes a s6_addr as param) */
#define IN6_ADDR_V4UNMAP( a6 ) 				\
	(((in_addr_t *)(a6))[3])


/* We provide macros to convert 64 bit values to and from network byte-order, on systems where it is not already provided. */
#ifndef HAVE_NTOHLL	/* Defined by the cmake step, if the ntohll symbol is defined on the system */
# if HOST_BIG_ENDIAN
    /* In big-endian systems, we don't have to change the values, since the order is the same as network */
#   define ntohll(x) (x)
#   define htonll(x) (x)
# else /* HOST_BIG_ENDIAN */
    /* For these systems, we must reverse the bytes. Use ntohl and htonl on sub-32 blocs, and inverse these blocs. */
#   define ntohll(x) (typeof (x))( (((uint64_t)ntohl( (uint32_t)(x))) << 32 ) | ((uint64_t) ntohl( ((uint64_t)(x)) >> 32 ))) 
#   define htonll(x) (typeof (x))( (((uint64_t)htonl( (uint32_t)(x))) << 32 ) | ((uint64_t) htonl( ((uint64_t)(x)) >> 32 ))) 
# endif /* HOST_BIG_ENDIAN */
#endif /* HAVE_NTOHLL */

/* This macro will give the next multiple of 4 for an integer (used for padding sizes of AVP). */
#define PAD4(_x) ((_x) + ( (4 - (_x)) & 3 ) )

/* Useful to display any value as (safe) ASCII (will garbage UTF-8 output...) */
#define ASCII(_c) ( ((_c < 32) || (_c > 127)) ? ( _c ? '?' : ' ' ) : _c )

/* Compare timespec structures */
#define TS_IS_INFERIOR( ts1, ts2 ) 		\
	(    ((ts1)->tv_sec  < (ts2)->tv_sec ) 	\
	  || (((ts1)->tv_sec  == (ts2)->tv_sec ) && ((ts1)->tv_nsec < (ts2)->tv_nsec) ))

/* Compute diff between two timespecs (pointers) */
#define TS_DIFFERENCE( tsdiff, tsstart, tsend )	{					\
	if ((tsend)->tv_nsec < (tsstart)->tv_nsec ) {					\
		(tsdiff)->tv_sec = (tsend)->tv_sec - (tsstart)->tv_sec - 1;		\
		(tsdiff)->tv_nsec = (tsend)->tv_nsec + 1000000000 - (tsstart)->tv_nsec;	\
	} else {									\
		(tsdiff)->tv_sec  = (tsend)->tv_sec  - (tsstart)->tv_sec;		\
		(tsdiff)->tv_nsec = (tsend)->tv_nsec - (tsstart)->tv_nsec;		\
	}}
		

/* This gives a good size for buffered reads */
#ifndef BUFSIZ
#define BUFSIZ 96
#endif /* BUFSIZ */

/* This gives the length of a const string */
#define CONSTSTRLEN( str ) (sizeof(str) - 1)


/*============================================================*/
/*                         PORTABILITY                        */
/*============================================================*/
#ifndef HAVE_CLOCK_GETTIME
  #define CLOCK_REALTIME  0
  #include <sys/time.h>
  int clock_gettime(int clk_id, struct timespec* ts);
#endif /* HAVE_CLOCK_GETTIME */

#ifndef HAVE_STRNDUP
char * strndup (char *str, size_t len);
#endif /* HAVE_STRNDUP */


/*============================================================*/
/*                         BINARY STRINGS                     */
/*============================================================*/

/* Compute a hash value of a binary string. 
The hash must remain local to this machine, there is no guarantee that same input
will give same output on a different system (endianness) */
uint32_t fd_os_hash ( uint8_t * string, size_t len );

/* This type used for binary strings that contain no \0 except as their last character. 
It means some string operations can be used on it. */
typedef uint8_t * os0_t;

/* Same as strdup but for os0_t strings */
os0_t os0dup_int(os0_t s, size_t l);
#define os0dup( _s, _l)  (void *)os0dup_int((os0_t)(_s), _l)

/* Check that an octet string value can be used as os0_t */
static __inline__ int fd_os_is_valid_os0(uint8_t * os, size_t oslen) {
	/* The only situation where it is not valid is when it contains a \0 inside the octet string */
	return (memchr(os, '\0', oslen) == NULL);
}

/* The following type denotes a verified DiameterIdentity value (that contains only pure letters, digits, hyphen, dot) */
typedef char * DiamId_t;

/* Maximum length of a hostname we accept */
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 512
#endif /* HOST_NAME_MAX */

/* Check if a binary string contains a valid Diameter Identity value.
  rfc3588 states explicitely that such a Diameter Identity consists only of ASCII characters. */
int fd_os_is_valid_DiameterIdentity(uint8_t * os, size_t ossz);

/* The following function validates a string as a Diameter Identity or applies the IDNA transformation on it 
 if *inoutsz is != 0 on entry, *id may not be \0-terminated.
 memory has the following meaning: 0: *id can be realloc'd. 1: *id must be malloc'd on output (was static)
*/
int fd_os_validate_DiameterIdentity(char ** id, size_t * inoutsz, int memory);

/* Create an order relationship for binary strings (not needed to be \0 terminated). 
   It does NOT mimic strings relationships so that it is more efficient. It is case sensitive.
   (the strings are actually first ordered by their lengh, then by their bytes contents)
   returns: -1 if os1 < os2;  +1 if os1 > os2;  0 if they are equal */
int fd_os_cmp_int(os0_t os1, size_t os1sz, os0_t os2, size_t os2sz);
#define fd_os_cmp(_o1, _l1, _o2, _l2)  fd_os_cmp_int((os0_t)(_o1), _l1, (os0_t)(_o2), _l2)

/* A roughly case-insensitive variant, which actually only compares ASCII chars (0-127) in a case-insentitive maneer 
  -- it does not support locales where a lowercase letter uses more space than upper case, such as ß -> ss
 It is slower than fd_os_cmp.
 Note that the result is NOT the same as strcasecmp !!!
 
 This function gives the same order as fd_os_cmp, except when it finds 2 strings to be equal.
 However this is not always sufficient:
 	for example fd_os_cmp gives: "Ac" < "aB" < "aa"
	if you attempt to fd_os_almostcasesrch "Aa" you will actually have to go past "aB" which is > "Aa".
	Therefore you can use the maybefurther parameter.
	This parameter is 1 on return if os1 may have been stored further that os2 (assuming os2 values are ordered by fd_os_cmp)
	and 0 if we are sure that it is not the case.
	When looping through a list of fd_os_cmp classified values, this parameter must be used to stop looping, in addition to the comp result.
 */
int fd_os_almostcasesrch_int(uint8_t * os1, size_t os1sz, uint8_t * os2, size_t os2sz, int * maybefurther);
#define fd_os_almostcasesrch(_o1, _l1, _o2, _l2, _mb)  fd_os_almostcasesrch_int((os0_t)(_o1), _l1, (os0_t)(_o2), _l2, _mb)

/* Analyze a DiameterURI and return its components. 
  Return EINVAL if the URI is not valid. 
  *diamid is malloc'd on function return and must be freed (it is processed by fd_os_validate_DiameterIdentity).
  *secure is 0 (no security) or 1 (security enabled) on return.
  *port is 0 (default) or a value in host byte order on return.
  *transport is 0 (default) or IPPROTO_* on return.
  *proto is 0 (default) or 'd' (diameter), 'r' (radius), or 't' (tacacs+) on return.
  */
int fd_os_parse_DiameterURI(uint8_t * uri, size_t urisz, DiamId_t * diamid, size_t * diamidlen, int * secure, uint16_t * port, int * transport, char *proto);

/*============================================================*/
/*                          THREADS                           */
/*============================================================*/

/* Terminate a thread */
static __inline__ int fd_thr_term(pthread_t * th)
{
	void * th_ret = NULL;
	
	CHECK_PARAMS(th);
	
	/* Test if it was already terminated */
	if (*th == (pthread_t)NULL)
		return 0;
	
	/* Cancel the thread if it is still running - ignore error if it was already terminated */
	(void) pthread_cancel(*th);
	
	/* Then join the thread */
	CHECK_POSIX( pthread_join(*th, &th_ret) );
	
	if (th_ret == PTHREAD_CANCELED) {
		TRACE_DEBUG(ANNOYING, "The thread %p was canceled", (void *)*th);
	} else {
		TRACE_DEBUG(CALL, "The thread %p returned %p", (void *)*th, th_ret);
	}
	
	/* Clean the location */
	*th = (pthread_t)NULL;
	
	return 0;
}


/*************
 Cancelation cleanup handlers for common objects 
 *************/
static __inline__ void fd_cleanup_mutex( void * mutex )
{
	CHECK_POSIX_DO( pthread_mutex_unlock((pthread_mutex_t *)mutex), /* */);
}
		
static __inline__ void fd_cleanup_rwlock( void * rwlock )
{
	CHECK_POSIX_DO( pthread_rwlock_unlock((pthread_rwlock_t *)rwlock), /* */);
}

static __inline__ void fd_cleanup_buffer( void * buffer )
{
	free(buffer);
}
static __inline__ void fd_cleanup_socket(void * sockptr)
{
	if (sockptr && (*(int *)sockptr > 0)) {
		CHECK_SYS_DO( close(*(int *)sockptr), /* ignore */ );
		*(int *)sockptr = -1;
	}
}


/*============================================================*/
/*                          LISTS                             */
/*============================================================*/

/* The following structure represents a chained list element  */
struct fd_list {
	struct fd_list 	*next; /* next element in the list */
	struct fd_list 	*prev; /* previous element in the list */
	struct fd_list 	*head; /* head of the list */
	void		*o;    /* additional pointer, used for any purpose (ex: start of the parent object) */
};

/* Initialize a list element */
#define FD_LIST_INITIALIZER( _list_name ) \
	{ .next = & _list_name, .prev = & _list_name, .head = & _list_name, .o = NULL }
#define FD_LIST_INITIALIZER_O( _list_name, _obj ) \
	{ .next = & _list_name, .prev = & _list_name, .head = & _list_name, .o = _obj }
void fd_list_init ( struct fd_list * list, void * obj );

/* Return boolean, true if the list is empty */
#define FD_IS_LIST_EMPTY( _list ) ((((struct fd_list *)(_list))->head == (_list)) && (((struct fd_list *)(_list))->next == (_list)))

/* Insert an item in a list at known position */
void fd_list_insert_after  ( struct fd_list * ref, struct fd_list * item );
void fd_list_insert_before ( struct fd_list * ref, struct fd_list * item );

/* Move all elements from a list at the end of another */
void fd_list_move_end(struct fd_list * ref, struct fd_list * senti);

/* Insert an item in an ordered list -- ordering function must be provided. If duplicate object found, EEXIST and it is returned in ref_duplicate */
int fd_list_insert_ordered( struct fd_list * head, struct fd_list * item, int (*cmp_fct)(void *, void *), void ** ref_duplicate);

/* Unlink an item from a list */
void fd_list_unlink ( struct fd_list * item );




/*============================================================*/
/*                        DICTIONARY                          */
/*============================================================*/

/* Structure that contains the complete dictionary definitions */
struct dictionary;

/* Structure that contains a dictionary object */
struct dict_object;

/* Types of object in the dictionary. */
enum dict_object_type {
	DICT_VENDOR	= 1,	/* Vendor */
	DICT_APPLICATION,	/* Diameter Application */
	DICT_TYPE,		/* AVP data type */
	DICT_ENUMVAL,		/* Named constant (value of an enumerated AVP type) */
	DICT_AVP,		/* AVP */
	DICT_COMMAND,		/* Diameter Command */
	DICT_RULE		/* a Rule for AVP in command or grouped AVP */
#define DICT_TYPE_MAX	DICT_RULE
};
	
/* Initialize a dictionary */
int fd_dict_init(struct dictionary ** dict);
/* Destroy a dictionary */
int fd_dict_fini(struct dictionary ** dict);

/*
 * FUNCTION:	fd_dict_new
 *
 * PARAMETERS:
 *  dict	: Pointer to the dictionnary where the object is created
 *  type 	: What kind of object must be created
 *  data 	: pointer to the data for the object. 
 *          	 type parameter is used to determine the type of data (see below for detail).
 *  parent 	: a reference to a parent object, if needed.
 *  ref 	: upon successful creation, reference to new object is stored here if !null.
 *
 * DESCRIPTION: 
 *  Create a new object in the dictionary. 
 *  See following object sections in this header file for more information on data and parent parameters format.
 *
 * RETURN VALUE:
 *  0      	: The object is created in the dictionary.
 *  EINVAL 	: A parameter is invalid.
 *  EEXIST 	: This object is already defined in the dictionary (with conflicting data). 
 *                If "ref" is not NULL, it points to the existing element on return.
 *  (other standard errors may be returned, too, with their standard meaning. Example:
 *    ENOMEM 	: Memory allocation for the new object element failed.)
 */
int fd_dict_new ( struct dictionary * dict, enum dict_object_type type, void * data, struct dict_object * parent, struct dict_object ** ref );

/*
 * FUNCTION: 	fd_dict_search
 *
 * PARAMETERS:
 *  dict	: Pointer to the dictionnary where the object is searched
 *  type 	: type of object that is being searched
 *  criteria 	: how the object must be searched. See object-related sections below for more information.
 *  what 	: depending on criteria, the data that must be searched.
 *  result 	: On successful return, pointer to the object is stored here.
 *  retval	: this value is returned if the object is not found and result is not NULL.
 *
 * DESCRIPTION: 
 *   Perform a search in the dictionary. 
 *   See the object-specific sections below to find how to look for each objects.
 *   If the "result" parameter is NULL, the function is used to check if an object is in the dictionary.
 *   Otherwise, a reference to the object is stored in result if found.
 *   If result is not NULL and the object is not found, retval is returned (should be 0 or ENOENT usually)
 *
 * RETURN VALUE:
 *  0      	: The object has been found in the dictionary, or *result is NULL.
 *  EINVAL 	: A parameter is invalid.
 *  ENOENT	: No matching object has been found, and result was NULL.
 */
int fd_dict_search ( struct dictionary * dict, enum dict_object_type type, int criteria, const void * what, struct dict_object ** result, int retval );

/* Special case: get the generic error command object */
int fd_dict_get_error_cmd(struct dictionary * dict, struct dict_object ** obj);

/*
 * FUNCTION:	fd_dict_getval
 *
 * PARAMETERS:
 *  object 	: Pointer to a dictionary object.
 *  data 	: pointer to a structure to hold the data for the object.
 *          	  The type is the same as "data" parameter in fd_dict_new function.
 *
 * DESCRIPTION: 
 *  Retrieve content of a dictionary object.
 *  See following object sections in this header file for more information on data and parent parameters format.
 *
 * RETURN VALUE:
 *  0      	: The content of the object has been retrieved.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_dict_getval ( struct dict_object * object, void * val);
int fd_dict_gettype ( struct dict_object * object, enum dict_object_type * type);
int fd_dict_getdict ( struct dict_object * object, struct dictionary ** dict);

/* Debug functions */
DECLARE_FD_DUMP_PROTOTYPE(fd_dict_dump_object, struct dict_object * obj);
DECLARE_FD_DUMP_PROTOTYPE(fd_dict_dump, struct dictionary * dict);

/* Function to access full contents of the dictionary, see doc in dictionary.c */
int fd_dict_getlistof(int criteria, void * parent, struct fd_list ** sentinel);

/* Function to remove an entry from the dictionary.
  This cannot be used if the object has children (for example a vendor with vendor-specific AVPs).
  In such case, the children must be removed first. */
int fd_dict_delete(struct dict_object * obj);

/*
 ***************************************************************************
 *
 * Vendor object 
 *
 * These types are used to manage vendors in the dictionary
 *
 ***************************************************************************
 */

/* Type to hold a Vendor ID: "SMI Network Management Private Enterprise Codes" (RFC3232) */
typedef uint32_t	vendor_id_t;

/* Type to hold data associated to a vendor */
struct dict_vendor_data {
	vendor_id_t	 vendor_id;	/* ID of a vendor */
	char *	 	 vendor_name;	/* The name of this vendor */
};

/* The criteria for searching a vendor object in the dictionary */
enum {
	VENDOR_BY_ID = 10,	/* "what" points to a vendor_id_t */
	VENDOR_BY_NAME,		/* "what" points to a char * */
	VENDOR_OF_APPLICATION,	/* "what" points to a struct dict_object containing an application (see below) */
	VENDOR_OF_AVP,		/* "what" points to a struct dict_object containing an avp (see below) */
};

/***
 *  API usage :

Note: the value of "vendor_name" is copied when the object is created, and the string may be disposed afterwards.
On the other side, when value is retrieved with dict_getval, the string is not copied and MUST NOT be freed. It will
be freed automatically along with the object itself with call to dict_fini later.
 
- fd_dict_new:
 The "parent" parameter is not used for vendors. 
 Sample code to create a vendor:
 {
	 int ret;
	 struct dict_object * myvendor;
	 struct dict_vendor_data myvendordata = { 23455, "my vendor name" };  -- just an example...
	 ret = fd_dict_new ( dict, DICT_VENDOR, &myvendordata, NULL, &myvendor );
 }

- fd_dict_search:
 Sample codes to look for a vendor object, by its id or name:
 {
	 int ret;
	 struct dict_object * vendor_found;
	 vendor_id_t vendorid = 23455;
	 ret = fd_dict_search ( dict, DICT_VENDOR, VENDOR_BY_ID, &vendorid, &vendor_found, ENOENT);
	 - or -
	 ret = fd_dict_search ( dict, DICT_VENDOR, VENDOR_BY_NAME, "my vendor name", &vendor_found, ENOENT);
 }
 
 - fd_dict_getval:
 Sample code to retrieve the data from a vendor object:
 {
	 int ret;
	 struct dict_object * myvendor;
	 struct dict_vendor_data myvendordata;
	 ret = fd_dict_search ( dict, DICT_VENDOR, VENDOR_BY_NAME, "my vendor name", &myvendor, ENOENT);
	 ret = fd_dict_getval ( myvendor, &myvendordata );
	 printf("my vendor id: %d\n", myvendordata.vendor_id );
 }
		 
*/
		
/* Special function: */
uint32_t * fd_dict_get_vendorid_list(struct dictionary * dict);
	 
/*
 ***************************************************************************
 *
 * Application object 
 *
 * These types are used to manage Diameter applications in the dictionary
 *
 ***************************************************************************
 */

/* Type to hold a Diameter application ID: IANA assigned value for this application. */
typedef uint32_t	application_id_t;

/* Type to hold data associated to an application */
struct dict_application_data {
	application_id_t	 application_id;	/* ID of the application */
	char *	 		 application_name;	/* The name of this application */
};

/* The criteria for searching an application object in the dictionary */
enum {
	APPLICATION_BY_ID = 20,		/* "what" points to a application_id_t */
	APPLICATION_BY_NAME,		/* "what" points to a char * */
	APPLICATION_OF_TYPE,		/* "what" points to a struct dict_object containing a type object (see below) */
	APPLICATION_OF_COMMAND		/* "what" points to a struct dict_object containing a command (see below) */
};

/***
 *  API usage :

The "parent" parameter of dict_new may point to a vendor object to inform of what vendor defines the application.
for standard-track applications, the "parent" parameter should be NULL.
The vendor associated to an application is retrieved with VENDOR_OF_APPLICATION search criteria on vendors.

- fd_dict_new:
 Sample code for application creation:
 {
	 int ret;
	 struct dict_object * vendor;
	 struct dict_object * appl;
	 struct dict_vendor_data vendor_data = {
		 23455,
		 "my vendor name"
	 };
	 struct dict_application_data app_data = {
		 9789,
		 "my vendor's application"
	 };
	
	 ret = fd_dict_new ( dict, DICT_VENDOR, &vendor_data, NULL, &vendor );
	 ret = fd_dict_new ( dict, DICT_APPLICATION, &app_data, vendor, &appl );
 }

- fd_dict_search:
 Sample code to retrieve the vendor of an application
 {
	 int ret;
	 struct dict_object * vendor, * appli;
	 
	 ret = fd_dict_search ( dict, DICT_APPLICATION, APPLICATION_BY_NAME, "my vendor's application", &appli, ENOENT);
	 ret = fd_dict_search ( dict, DICT_VENDOR, VENDOR_OF_APPLICATION, appli, &vendor, ENOENT);
 }
 
 - fd_dict_getval:
 Sample code to retrieve the data from an application object:
 {
	 int ret;
	 struct dict_object * appli;
	 struct dict_application_data appl_data;
	 ret = fd_dict_search ( dict, DICT_APPLICATION, APPLICATION_BY_NAME, "my vendor's application", &appli, ENOENT);
	 ret = fd_dict_getval ( appli, &appl_data );
	 printf("my application id: %s\n", appl_data.application_id );
 }

*/

/*
 ***************************************************************************
 *
 * Type object 
 *
 * These types are used to manage AVP data types in the dictionary
 *
 ***************************************************************************
 */

/* Type to store any AVP value */ 
union avp_value {
	struct {
		uint8_t *data;	/* bytes buffer */
		size_t   len;	/* length of the data buffer */
	}           os;		/* Storage for an octet string */
	int32_t     i32;	/* integer 32 */
	int64_t     i64;	/* integer 64 */
	uint32_t    u32;	/* unsigned 32 */
	uint64_t    u64;	/* unsigned 64 */
	float       f32;	/* float 32 */
	double 	    f64;	/* float 64 */
};

/* These are the basic AVP types defined in RFC3588bis */
enum dict_avp_basetype {
	AVP_TYPE_GROUPED,
	AVP_TYPE_OCTETSTRING,
	AVP_TYPE_INTEGER32,
	AVP_TYPE_INTEGER64,
	AVP_TYPE_UNSIGNED32,
	AVP_TYPE_UNSIGNED64,
	AVP_TYPE_FLOAT32,
	AVP_TYPE_FLOAT64
#define AVP_TYPE_MAX AVP_TYPE_FLOAT64
};

/* Callbacks that can be associated with a derived type to easily interpret the AVP value. */
/*
 * CALLBACK:	dict_avpdata_interpret
 *
 * PARAMETERS:
 *   val         : Pointer to the AVP value that must be interpreted.
 *   interpreted : The result of interpretation is stored here. The format and meaning depends on each type.
 *
 * DESCRIPTION: 
 *   This callback can be provided with a derived type in order to facilitate the interpretation of formated data.
 *  For example, when an AVP of type "Address" is received, it can be used to convert the octetstring into a struct sockaddr.
 *  This callback is not called directly, but through the message's API msg_avp_value_interpret function.
 *
 * RETURN VALUE:
 *  0      	: Operation complete.
 *  !0 		: An error occurred, the error code is returned.
 */
typedef int (*dict_avpdata_interpret) (union avp_value * value, void * interpreted);
/*
 * CALLBACK:	dict_avpdata_encode
 *
 * PARAMETERS:
 *   data	: The formated data that must be stored in the AVP value.
 *   val	: Pointer to the AVP value storage area where the data must be stored.
 *
 * DESCRIPTION: 
 *   This callback can be provided with a derived type in order to facilitate the encoding of formated data.
 *  For example, it can be used to convert a struct sockaddr in an AVP value of type Address.
 *  This callback is not called directly, but through the message's API msg_avp_value_encode function.
 *  If the callback is defined for an OctetString based type, the created string must be malloc'd. free will be called 
 *  automatically later.
 *
 * RETURN VALUE:
 *  0      	: Operation complete.
 *  !0 		: An error occurred, the error code is returned.
 */
typedef int (*dict_avpdata_encode) (void * data, union avp_value * val);


/* Type to hold data associated to a derived AVP data type */
struct dict_type_data {
	enum dict_avp_basetype	 type_base;	/* How the data of such AVP must be interpreted */
	char *	 		 type_name;	/* The name of this type */
	dict_avpdata_interpret	 type_interpret;/* cb to convert the AVP value in more comprehensive format (or NULL) */
	dict_avpdata_encode	 type_encode;	/* cb to convert formatted data into an AVP value (or NULL) */
	DECLARE_FD_DUMP_PROTOTYPE((*type_dump), union avp_value * val); /* cb called by fd_msg_dump_* for this type of data (if != NULL). Returned string must be freed.  */
};

/* The criteria for searching a type object in the dictionary */
enum {
	TYPE_BY_NAME = 30,		/* "what" points to a char * */
	TYPE_OF_ENUMVAL,		/* "what" points to a struct dict_object containing an enumerated constant (DICT_ENUMVAL, see below). */
	TYPE_OF_AVP			/* "what" points to a struct dict_object containing an AVP object. */
};

/****
 Callbacks defined in libfdproto/dictionary_functions.c file -- see that file for usage. 
 */

/* Convert an Address type AVP into a struct sockaddr_storage */
int fd_dictfct_Address_encode(void * data, union avp_value * avp_value);
int fd_dictfct_Address_interpret(union avp_value * avp_value, void * interpreted);
DECLARE_FD_DUMP_PROTOTYPE(fd_dictfct_Address_dump, union avp_value * avp_value);

/* Display the content of an AVP of type UTF8String in the log file */
DECLARE_FD_DUMP_PROTOTYPE(fd_dictfct_UTF8String_dump, union avp_value * avp_value);

/* For Time AVPs, map with time_t value directly */
int fd_dictfct_Time_encode(void * data, union avp_value * avp_value);
int fd_dictfct_Time_interpret(union avp_value * avp_value, void * interpreted);
DECLARE_FD_DUMP_PROTOTYPE(fd_dictfct_Time_dump, union avp_value * avp_value);



/****/

/***
 *  API usage :

- fd_dict_new:
 The "parent" parameter may point to an application object, when a type is defined by a Diameter application. 
 
 Sample code:
 {
	 int ret;
	 struct dict_object * mytype;
	 struct dict_type_data mytypedata = 
		{ 
		 AVP_TYPE_OCTETSTRING,
		 "Address",
		 NULL,
		 NULL
		};
	 ret = fd_dict_new ( dict, DICT_TYPE, &mytypedata, NULL, &mytype );
 }

- fd_dict_search:
 Sample code:
 {
	 int ret;
	 struct dict_object * address_type;
	 ret = fd_dict_search ( dict, DICT_TYPE, TYPE_BY_NAME, "Address", &address_type, ENOENT);
 }
 
*/

/*
 ***************************************************************************
 *
 * Enumerated values object 
 *
 * These types are used to manage named constants of some AVP,
 * for enumerated types. freeDiameter allows constants for types others than Unsigned32
 *
 ***************************************************************************
 */

/* Type to hold data of named constants for AVP */
struct dict_enumval_data {
	char *	 	 enum_name;	/* The name of this constant */
	union avp_value  enum_value;	/* Value of the constant. Union term depends on parent type's base type. */
};

/* The criteria for searching a constant in the dictionary */
enum {
	ENUMVAL_BY_STRUCT = 40,	/* "what" points to a struct dict_enumval_request as defined below */
	ENUMVAL_BY_NAME,	/* This cannot be used for researches */
	ENUMVAL_BY_VALUE	/* This cannot be used for researches */
};

struct dict_enumval_request {
	/* Identifier of the parent type, one of the following must not be NULL */
	struct dict_object	*type_obj;
	char *			 type_name;
	
	/* Search criteria for the constant */
	struct dict_enumval_data search; /* search.enum_value is used only if search.enum_name == NULL */
};

/***
 *  API usage :

- fd_dict_new:
 The "parent" parameter must point to a derived type object. 
 Sample code to create a type "Boolean" with two constants "True" and "False":
 {
	 int ret;
	 struct dict_object * type_boolean;
	 struct dict_type_data type_boolean_data = 
		{ 
		 AVP_TYPE_INTEGER32,
		 "Boolean",
		 NULL,
		 NULL
		};
	 struct dict_enumval_data boolean_false =
	 	{
		 .enum_name="False",
		 .enum_value.i32 = 0
	 	};
	 struct dict_enumval_data boolean_true =
	 	{
		 .enum_name="True",
		 .enum_value.i32 = -1
	 	};
	 ret = fd_dict_new ( dict, DICT_TYPE, &type_boolean_data, NULL, &type_boolean );
	 ret = fd_dict_new ( dict, DICT_ENUMVAL, &boolean_false, type_boolean, NULL );
	 ret = fd_dict_new ( dict, DICT_ENUMVAL, &boolean_true , type_boolean, NULL );
	 
 }

- fd_dict_search:
 Sample code to look for a constant name, by its value:
 {
	 int ret;
	 struct dict_object * value_found;
	 struct dict_enumval_request boolean_by_value =
	 	{
		 .type_name = "Boolean",
		 .search.enum_name=NULL,
		 .search.enum_value.i32 = -1
	 	};
	 
	 ret = fd_dict_search ( dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &boolean_by_value, &value_found, ENOENT);
 }
 
 - fd_dict_getval:
 Sample code to retrieve the data from a constant object:
 {
	 int ret;
	 struct dict_object * value_found;
	 struct dict_enumval_data boolean_data = NULL;
	 struct dict_enumval_request boolean_by_value =
	 	{
		 .type_name = "Boolean",
		 .search.enum_name=NULL,
		 .search.enum_value.i32 = 0
	 	};
	 
	 ret = fd_dict_search ( dict, DICT_ENUMVAL, ENUMVAL_BY_STRUCT, &boolean_by_value, &value_found, ENOENT);
	 ret = fd_dict_getval ( value_found, &boolean_data );
	 printf(" Boolean with value 0: %s", boolean_data.enum_name );
 }
*/
	 
/*
 ***************************************************************************
 *
 * AVP object 
 *
 * These objects are used to manage AVP definitions in the dictionary
 *
 ***************************************************************************
 */

/* Type to hold an AVP code. For vendor 0, these codes are assigned by IANA. Otherwise, it is managed by the vendor */
typedef uint32_t	avp_code_t;

/* Values of AVP flags */
#define	AVP_FLAG_VENDOR	  	0x80
#define	AVP_FLAG_MANDATORY	0x40
#define	AVP_FLAG_RESERVED3	0x20
#define	AVP_FLAG_RESERVED4	0x10
#define	AVP_FLAG_RESERVED5	0x08
#define	AVP_FLAG_RESERVED6	0x04
#define	AVP_FLAG_RESERVED7	0x02
#define	AVP_FLAG_RESERVED8	0x01

/* For dumping flags and values */
#define DUMP_AVPFL_str	"%c%c%s%s%s%s%s%s"
#define DUMP_AVPFL_val(_val) (_val & AVP_FLAG_VENDOR)?'V':'-' , (_val & AVP_FLAG_MANDATORY)?'M':'-',	\
				(_val & AVP_FLAG_RESERVED3)?"3":"", (_val & AVP_FLAG_RESERVED4)?"4":"", \
				(_val & AVP_FLAG_RESERVED5)?"5":"", (_val & AVP_FLAG_RESERVED6)?"6":"", (_val & AVP_FLAG_RESERVED7)?"7":"", (_val & AVP_FLAG_RESERVED8)?"8":""

/* Type to hold data associated to an avp */
struct dict_avp_data {
	avp_code_t	 	 avp_code;	/* Code of the avp */
	vendor_id_t	 	 avp_vendor;	/* Vendor of the AVP, or 0 */
	char *			 avp_name;	/* Name of this AVP */
	uint8_t		 	 avp_flag_mask;	/* Mask of fixed AVP flags */
	uint8_t		 	 avp_flag_val;	/* Values of the fixed flags */
	enum dict_avp_basetype 	 avp_basetype;	/* Basic type of data found in the AVP */
};

/* The criteria for searching an avp object in the dictionary */
enum {
	AVP_BY_CODE = 50,	/* "what" points to an avp_code_t, vendor is always 0 */
	AVP_BY_NAME,		/* "what" points to a char *, vendor is always 0 */
	AVP_BY_NAME_ALL_VENDORS,/* "what" points to a string. Might be quite slow... */
	AVP_BY_STRUCT,		/* "what" points to a struct dict_avp_request_ex (see below) */
			
	/* kept for backward compatibility, better use AVP_BY_STRUCT above instead */
	AVP_BY_CODE_AND_VENDOR,	/* "what" points to a struct dict_avp_request (see below), where avp_vendor and avp_code are set */
	AVP_BY_NAME_AND_VENDOR	/* "what" points to a struct dict_avp_request (see below), where avp_vendor and avp_name are set */
};

/* Struct used for some researchs */
struct dict_avp_request_ex {
	struct {
		/* Only one of the following fields must be set. */
		struct dict_object * 	vendor;		/* most efficient if already known, set to NULL to ignore */
		vendor_id_t	 	vendor_id; 	/* set to 0 to ignore -- prefer AVP_BY_CODE or AVP_BY_NAME for vendor 0 */
		const char *			vendor_name;	/* set to NULL to ignore */
	} avp_vendor;
	
	struct {
		/* Only one of the following fields must be set */
		avp_code_t	 avp_code; /* set to 0 to ignore */
		const char *		 avp_name; /* set to NULL to ignore */
	} avp_data;
};

struct dict_avp_request {
	vendor_id_t	 avp_vendor;
	avp_code_t	 avp_code;
	char *		 avp_name;
};



/***
 *  API usage :

If "parent" parameter is not NULL during AVP creation, it must point to a DICT_TYPE object. 
The extended type is then attached to the AVP. In case where it is an enumerated type, the value of 
AVP is automatically interpreted in debug messages, and in message checks.
The derived type of an AVP can be retrieved with: dict_search ( DICT_TYPE, TYPE_OF_AVP, avp, ... )

To create the rules (ABNF) for children of Grouped AVP, see the DICT_RULE related part.

- fd_dict_new:
 Sample code for AVP creation:
 {
	 int ret;
	 struct dict_object * user_name_avp;
	 struct dict_object * boolean_type;
	 struct dict_object * sample_boolean_avp;
	 struct dict_avp_data user_name_data = {
		 1,					// code
		 0,					// vendor
		 "User-Name",				// name
		 AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,	// fixed mask: V and M values must always be defined as follow. other flags can be set or cleared
		 AVP_FLAG_MANDATORY,			// the V flag must be cleared, the M flag must be set.
		 AVP_TYPE_OCTETSTRING			// User-Name AVP contains OctetString data (further precision such as UTF8String can be given with a parent derived type)
	 };
	 struct dict_avp_data sample_boolean_data = {
		 31337,
		 23455,
		 "Sample-Boolean",
		 AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,
		 AVP_FLAG_VENDOR,
		 AVP_TYPE_INTEGER32			// This MUST be the same as parent type's
	 };
	
 	 -- Create an AVP with a base type --
	 ret = fd_dict_new ( dict, DICT_AVP, &user_name_data, NULL, &user_name_avp );
	 
	 -- Create an AVP with a derived type --
	 ret = fd_dict_search ( dict, DICT_TYPE, TYPE_BY_NAME, "Boolean", &boolean_type, ENOENT);
	 ret = fd_dict_new ( dict, DICT_AVP, &sample_boolean_data , boolean_type, &sample_boolean_avp );
	 
 }

- fd_dict_search:
 Sample code to look for an AVP
 {
	 int ret;
	 struct dict_object * avp_username;
	 struct dict_object * avp_sampleboolean;
	 struct dict_avp_request avpvendorboolean =
	 	{
		 .avp_vendor = 23455,
		 .avp_name   = "Sample-Boolean"
	 	};
	 
	 ret = fd_dict_search ( dict, DICT_AVP, AVP_BY_NAME, "User-Name", &avp_username, ENOENT);
	 
	 ret = fd_dict_search ( dict, DICT_AVP, AVP_BY_NAME_AND_VENDOR, &avpvendorboolean, &avp_sampleboolean, ENOENT);

	 -- this would also work, but be slower, because it has to search all vendor dictionaries --
	 ret = fd_dict_search ( dict, DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Sample-Boolean", &avp_sampleboolean, ENOENT);
	 
 }
 
 - fd_dict_getval:
 Sample code to retrieve the data from an AVP object:
 {
	 int ret;
	 struct dict_object * avp_username;
	 struct dict_avp_data user_name_data;
	 ret = fd_dict_search ( dict, DICT_AVP, AVP_BY_NAME, "User-Name", &avp_username, ENOENT);
	 ret = fd_dict_getval ( avp_username, &user_name_data );
	 printf("User-Name code: %d\n", user_name_data.avp_code );
 }

*/

/*
 ***************************************************************************
 *
 * Command object 
 *
 * These types are used to manage commands objects in the dictionary
 *
 ***************************************************************************
 */

/* Type to hold a Diameter command code: IANA assigned values. 0x0-0x7fffff=standard, 0x800000-0xfffffd=vendors, 0xfffffe-0xffffff=experimental */
typedef uint32_t	command_code_t;

/* Values of command flags */
#define CMD_FLAG_REQUEST	0x80
#define CMD_FLAG_PROXIABLE	0x40
#define CMD_FLAG_ERROR		0x20
#define CMD_FLAG_RETRANSMIT	0x10
#define CMD_FLAG_RESERVED5	0x08
#define CMD_FLAG_RESERVED6	0x04
#define CMD_FLAG_RESERVED7	0x02
#define CMD_FLAG_RESERVED8	0x01

/* For dumping flags and values */
#define DUMP_CMDFL_str	"%c%c%c%c%s%s%s%s"
#define DUMP_CMDFL_val(_val) (_val & CMD_FLAG_REQUEST)?'R':'-' , (_val & CMD_FLAG_PROXIABLE)?'P':'-' , (_val & CMD_FLAG_ERROR)?'E':'-' , (_val & CMD_FLAG_RETRANSMIT)?'T':'-', \
				(_val & CMD_FLAG_RESERVED5)?"5":"", (_val & CMD_FLAG_RESERVED6)?"6":"", (_val & CMD_FLAG_RESERVED7)?"7":"", (_val & CMD_FLAG_RESERVED8)?"8":""

/* Type to hold data associated to a command */
struct dict_cmd_data {
	command_code_t	 cmd_code;	/* code of the command */
	char *		 cmd_name;	/* Name of the command */
	uint8_t		 cmd_flag_mask;	/* Mask of fixed-value flags */
	uint8_t		 cmd_flag_val;	/* values of the fixed flags */
};

/* The criteria for searching an avp object in the dictionary */
enum {
	CMD_BY_NAME = 60,	/* "what" points to a char * */
	CMD_BY_CODE_R,		/* "what" points to a command_code_t. The "Request" command is returned. */
	CMD_BY_CODE_A,		/* "what" points to a command_code_t. The "Answer" command is returned. */
	CMD_ANSWER		/* "what" points to a struct dict_object of a request command. The corresponding "Answer" command is returned. */
};


/***
 *  API usage :

The "parent" parameter of dict_new may point to an application object to inform of what application defines the command.
The application associated to a command is retrieved with APPLICATION_OF_COMMAND search criteria on applications.

To create the rules for children of commands, see the DICT_RULE related part.

Note that the "Request" and "Answer" commands are two independant objects. This allows to have different rules for each.

- fd_dict_new:
 Sample code for command creation:
 {
	 int ret;
	 struct dict_object * cer;
	 struct dict_object * cea;
	 struct dict_cmd_data ce_data = {
		 257,					// code
		 "Capabilities-Exchange-Request",	// name
		 CMD_FLAG_REQUEST,			// mask
		 CMD_FLAG_REQUEST			// value. Only the "R" flag is constrained here, set.
	 };
	
	 ret = fd_dict_new (dict,  DICT_COMMAND, &ce_data, NULL, &cer );
	 
	 ce_data.cmd_name = "Capabilities-Exchange-Answer";
	 ce_data.cmd_flag_val = 0;			// Same constraint on "R" flag, but this time it must be cleared.

	 ret = fd_dict_new ( dict, DICT_COMMAND, &ce_data, NULL, &cea );
 }

- fd_dict_search:
 Sample code to look for a command
 {
	 int ret;
	 struct dict_object * cer, * cea;
	 command_code_t	code = 257;
	 ret = fd_dict_search ( dict, DICT_COMMAND, CMD_BY_NAME, "Capabilities-Exchange-Request", &cer, ENOENT);
	 ret = fd_dict_search ( dict, DICT_COMMAND, CMD_BY_CODE_R, &code, &cer, ENOENT);
 }
 
 - fd_dict_getval:
 Sample code to retrieve the data from a command object:
 {
	 int ret;
	 struct dict_object * cer;
	 struct dict_object * cea;
	 struct dict_cmd_data cea_data;
	 ret = fd_dict_search ( dict, DICT_COMMAND, CMD_BY_NAME, "Capabilities-Exchange-Request", &cer, ENOENT);
	 ret = fd_dict_search ( dict, DICT_COMMAND, CMD_ANSWER, cer, &cea, ENOENT);
	 ret = fd_dict_getval ( cea, &cea_data );
	 printf("Answer to CER: %s\n", cea_data.cmd_name );
 }

*/

/*
 ***************************************************************************
 *
 * Rule object
 *
 * These objects are used to manage rules in the dictionary (ABNF implementation)
 * This is used for checking messages validity (more powerful than a DTD)
 *
 ***************************************************************************
 */

/* This defines the kind of rule that is defined */
enum rule_position {
	RULE_FIXED_HEAD = 1,	/* The AVP must be at the head of the group. The rule_order field is used to specify the position. */
	RULE_REQUIRED,		/* The AVP must be present in the parent, but its position is not defined. */
	RULE_OPTIONAL,		/* The AVP may be present in the message. Used to specify a max number of occurences for example */
	RULE_FIXED_TAIL		/* The AVP must be at the end of the group. The rule_order field is used to specify the position. */
};

/* Content of a RULE object data */
struct dict_rule_data {
	struct dict_object	*rule_avp;	/* Pointer to the AVP object that is concerned by this rule */
	enum rule_position	 rule_position;	/* The position in which the rule_avp must appear in the parent */
	unsigned		 rule_order;	/* for RULE_FIXED_* rules, the place. 1,2,3.. for HEAD rules; ...,3,2,1 for TAIL rules. */
	int	 		 rule_min;	/* Minimum number of occurences. -1 means "default": 0 for optional rules, 1 for other rules */
	int			 rule_max;	/* Maximum number of occurences. -1 means no maximum. 0 means the AVP is forbidden. */
};

/* The criteria for searching a rule in the dictionary */
enum {
	RULE_BY_AVP_AND_PARENT = 70	/* "what" points to a struct dict_rule_request -- see below. This is used to query "what is the rule for this AVP in this group?" */
};

/* Structure for querying the dictionary about a rule */
struct dict_rule_request {
	struct dict_object	*rule_parent;	/* The grouped avp or command to which the rule apply */
	struct dict_object	*rule_avp;	/* The AVP concerned by this rule */
};


/***
 *  API usage :

The "parent" parameter can not be NULL. It points to the object (grouped avp or command) to which this rule apply (i.e. for which the ABNF is defined).

- fd_dict_new:
 Sample code for rule creation. Let's create the Proxy-Info grouped AVP for example.
 {
	int ret;
	struct dict_object * proxy_info_avp;
	struct dict_object * proxy_host_avp;
	struct dict_object * proxy_state_avp;
	struct dict_object * diameteridentity_type;
	struct dict_rule_data rule_data;
	struct dict_type_data di_type_data = { AVP_TYPE_OCTETSTRING, "DiameterIdentity", NULL, NULL };
	struct dict_avp_data proxy_info_data = { 284, 0, "Proxy-Info", AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, AVP_FLAG_MANDATORY, AVP_TYPE_GROUPED };
	struct dict_avp_data proxy_host_data = { 280, 0, "Proxy-Host", AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, AVP_FLAG_MANDATORY, AVP_TYPE_OCTETSTRING };
	struct dict_avp_data proxy_state_data = { 33, 0, "Proxy-State",AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY, AVP_FLAG_MANDATORY, AVP_TYPE_OCTETSTRING };
	
	-- Create the parent AVP
	ret = fd_dict_new ( dict, DICT_AVP, &proxy_info_data, NULL, &proxy_info_avp );
	
	-- Create the first child AVP.
	ret = fd_dict_new ( dict, DICT_TYPE, &di_type_data, NULL, &diameteridentity_type );
	ret = fd_dict_new ( dict, DICT_AVP, &proxy_host_data, diameteridentity_type, &proxy_host_avp );
	
	-- Create the other child AVP
	ret = fd_dict_new ( dict, DICT_AVP, &proxy_state_data, NULL, &proxy_state_avp );
	
	-- Now we can create the rules. Both children AVP are mandatory.
	rule_data.rule_position = RULE_REQUIRED;
	rule_data.rule_min = -1;
	rule_data.rule_max = -1;
	
	rule_data.rule_avp = proxy_host_avp;
	ret = fd_dict_new ( dict, DICT_RULE, &rule_data, proxy_info_avp, NULL );
	
	rule_data.rule_avp = proxy_state_avp;
	ret = fd_dict_new ( dict, DICT_RULE, &rule_data, proxy_info_avp, NULL );
}

- fd_dict_search and fd_dict_getval are similar to previous examples.

*/
		
/* Define some hard-coded values */
/* Application */
#define AI_RELAY			0xffffffff

/* Commands Codes */
#define CC_CAPABILITIES_EXCHANGE	257
#define CC_RE_AUTH			258
#define CC_ACCOUNTING			271
#define CC_ABORT_SESSION		274
#define CC_SESSION_TERMINATION		275
#define CC_DEVICE_WATCHDOG		280
#define CC_DISCONNECT_PEER		282

/* AVPs (Vendor 0) */
#define AC_USER_NAME			1
#define AC_PROXY_STATE			33
#define AC_HOST_IP_ADDRESS		257
#define AC_AUTH_APPLICATION_ID		258
#define AC_ACCT_APPLICATION_ID		259
#define AC_VENDOR_SPECIFIC_APPLICATION_ID 260
#define AC_REDIRECT_HOST_USAGE		261
#define AC_REDIRECT_MAX_CACHE_TIME	262
#define AC_SESSION_ID 			263
#define AC_ORIGIN_HOST			264
#define AC_SUPPORTED_VENDOR_ID		265
#define AC_VENDOR_ID			266
#define AC_FIRMWARE_REVISION		267
#define AC_RESULT_CODE			268
#define AC_PRODUCT_NAME			269
#define AC_DISCONNECT_CAUSE		273
#define ACV_DC_REBOOTING			0
#define ACV_DC_BUSY				1
#define ACV_DC_NOT_FRIEND			2
#define AC_ORIGIN_STATE_ID		278
#define AC_FAILED_AVP			279
#define AC_PROXY_HOST			280
#define AC_ERROR_MESSAGE		281
#define AC_ROUTE_RECORD			282
#define AC_DESTINATION_REALM		283
#define AC_PROXY_INFO			284
#define AC_REDIRECT_HOST		292
#define AC_DESTINATION_HOST		293
#define AC_ERROR_REPORTING_HOST		294
#define AC_ORIGIN_REALM			296
#define AC_INBAND_SECURITY_ID		299
#define ACV_ISI_NO_INBAND_SECURITY		0
#define ACV_ISI_TLS				1

/* Error codes from Base protocol
(reference: http://www.iana.org/assignments/aaa-parameters/aaa-parameters.xml#aaa-parameters-4)
Note that currently, rfc3588bis-26 has some different values for some of these 
*/
#define ER_DIAMETER_MULTI_ROUND_AUTH			1001

#define ER_DIAMETER_SUCCESS				2001
#define ER_DIAMETER_LIMITED_SUCCESS			2002

#define ER_DIAMETER_COMMAND_UNSUPPORTED			3001 /* 5019 ? */
#define ER_DIAMETER_UNABLE_TO_DELIVER			3002
#define ER_DIAMETER_REALM_NOT_SERVED			3003
#define ER_DIAMETER_TOO_BUSY				3004
#define ER_DIAMETER_LOOP_DETECTED			3005
#define ER_DIAMETER_REDIRECT_INDICATION			3006
#define ER_DIAMETER_APPLICATION_UNSUPPORTED		3007
#define ER_DIAMETER_INVALID_HDR_BITS			3008 /* 5020 ? */
#define ER_DIAMETER_INVALID_AVP_BITS			3009 /* 5021 ? */
#define ER_DIAMETER_UNKNOWN_PEER			3010 /* 5018 ? */

#define ER_DIAMETER_AUTHENTICATION_REJECTED		4001
#define ER_DIAMETER_OUT_OF_SPACE			4002
#define ER_ELECTION_LOST				4003

#define ER_DIAMETER_AVP_UNSUPPORTED			5001
#define ER_DIAMETER_UNKNOWN_SESSION_ID			5002
#define ER_DIAMETER_AUTHORIZATION_REJECTED		5003
#define ER_DIAMETER_INVALID_AVP_VALUE			5004
#define ER_DIAMETER_MISSING_AVP				5005
#define ER_DIAMETER_RESOURCES_EXCEEDED			5006
#define ER_DIAMETER_CONTRADICTING_AVPS			5007
#define ER_DIAMETER_AVP_NOT_ALLOWED			5008
#define ER_DIAMETER_AVP_OCCURS_TOO_MANY_TIMES		5009
#define ER_DIAMETER_NO_COMMON_APPLICATION		5010
#define ER_DIAMETER_UNSUPPORTED_VERSION			5011
#define ER_DIAMETER_UNABLE_TO_COMPLY			5012
#define ER_DIAMETER_INVALID_BIT_IN_HEADER		5013 /* 3011 ? */
#define ER_DIAMETER_INVALID_AVP_LENGTH			5014
#define ER_DIAMETER_INVALID_MESSAGE_LENGTH		5015 /* 3012 ? */
#define ER_DIAMETER_INVALID_AVP_BIT_COMBO		5016 /* deprecated? */
#define ER_DIAMETER_NO_COMMON_SECURITY			5017


/*============================================================*/
/*                         SESSIONS                           */
/*============================================================*/

/* Modules that want to associate a state with a Session-Id must first register a handler of this type */
struct session_handler;

/* This opaque structure represents a session associated with a Session-Id */
struct session;

/* The state information that a module associate with a session -- each module defines its own data format */
struct sess_state; /* declare this in your own extension */

typedef DECLARE_FD_DUMP_PROTOTYPE((*session_state_dump), struct sess_state * st);

/* The following function must be called to activate the session expiry mechanism */
int fd_sess_start(void);

/*
 * FUNCTION:	fd_sess_handler_create
 *
 * PARAMETERS:
 *  handler	: location where the new handler must be stored.
 *  cleanup	: a callback function that must be called when the session with associated data is destroyed.
 *  dumper      : if not NULL, will be called during fd_sess_dump to display the data associated with a session. NULL otherwise.
 *  opaque      : A pointer that is passed to the cleanup callback -- the content is never examined by the framework.
 *
 * DESCRIPTION: 
 *  Create a new session handler. This is needed by a module to associate a state with a session object.
 * The cleanup handler is called when the session timeout expires, or fd_sess_destroy is called. It must free
 * the state associated with the session, and eventually trig other actions (send a STR, ...).
 *
 * RETURN VALUE:
 *  0      	: The new handler has been created.
 *  EINVAL 	: A parameter is invalid.
 *  ENOMEM	: Not enough memory to complete the operation
 */
int fd_sess_handler_create ( struct session_handler ** handler, void (*cleanup)(struct sess_state * state, os0_t sid, void * opaque), session_state_dump dumper, void * opaque );

	
/*
 * FUNCTION:	fd_sess_handler_destroy
 *
 * PARAMETERS:
 *  handler	: location of an handler created by fd_sess_handler_create.
 *  opaque      : the opaque pointer registered with the callback is restored here (if ! NULL).
 *
 * DESCRIPTION: 
 *  This destroys a session handler (typically called when an application is shutting down).
 * If sessions states are registered with this handler, the cleanup callback is called on them.
 *
 * RETURN VALUE:
 *  0      	: The handler was destroyed.
 *  EINVAL 	: A parameter is invalid.
 *  ENOMEM	: Not enough memory to complete the operation
 */
int fd_sess_handler_destroy ( struct session_handler ** handler, void **opaque );



/*
 * FUNCTION:	fd_sess_new
 *
 * PARAMETERS:
 *  session	  : The location where the session object will be created upon success.
 *  diamid	  : a Diameter Identity, or NULL.
 *  diamidlen	  : if diamid is \0-terminated, this can be 0. Otherwise, the length of diamid.
 *  opt           : Additional string, or NULL. Usage is described below.
 *  optlen	  : if opt is \0-terminated, this can be 0. Otherwise, the length of opt.
 *
 * DESCRIPTION: 
 *   Create a new session object. The Session-Id string associated with this session is generated as follow:
 *  If diamId parameter is provided, the string is created according to the RFC: <diamId>;<high32>;<low32>[;opt] where
 *    diamId is a Diameter Identity.
 *    high32 and low32 are the parts of a monotonic 64 bits counter initialized to (time, 0) at startup.
 *    opt is an optional string that can be concatenated to the identifier.
 *  If diamId is NULL, the string is exactly the content of opt.
 *
 * RETURN VALUE:
 *  0      	: The session is created, the initial msg refcount is 1.
 *  EINVAL 	: A parameter is invalid.
 *  EALREADY	: A session with the same name already exists (returned in *session), the msg refcount is increased.
 *  ENOMEM	: Not enough memory to complete the operation
 */
int fd_sess_new ( struct session ** session, DiamId_t diamid, size_t diamidlen, uint8_t * opt, size_t optlen );

/*
 * FUNCTION:	fd_sess_fromsid
 *
 * PARAMETERS:
 *  sid	  	: pointer to a string containing a Session-Id (should be UTF-8).
 *  len		: length of the sid string (which does not need to be '\0'-terminated)
 *  session	: On success, pointer to the session object created / retrieved.
 *  isnew	: if not NULL, set to 1 on return if the session object has been created, 0 if it was simply retrieved.
 *
 * DESCRIPTION: 
 *   Retrieve a session object from a Session-Id string. In case no session object was previously existing with this 
 *  id, a new object is silently created (equivalent to fd_sess_new with flag SESSION_NEW_FULL).
 *
 * RETURN VALUE:
 *  0      	: The session parameter has been updated.
 *  EINVAL 	: A parameter is invalid.
 *  ENOMEM	: Not enough memory to complete the operation
 */
int fd_sess_fromsid ( uint8_t * sid, size_t len, struct session ** session, int * isnew);

/* only use the following in specific situations, e.g. app_radgw extension. They are normally handled by the framework only */
int fd_sess_fromsid_msg ( uint8_t * sid, size_t len, struct session ** session, int * isnew); 
int fd_sess_ref_msg ( struct session * session );

/*
 * FUNCTION:	fd_sess_getsid
 *
 * PARAMETERS:
 *  session	: Pointer to a session object.
 *  sid	  	: On success, the location of the sid is stored here.
 *
 * DESCRIPTION: 
 *   Retrieve the session identifier (Session-Id) corresponding to a session object.
 *  The returned sid is a \0-terminated binary string which might be UTF-8 (but there is no guarantee in the framework).
 *  It may be used for example to set the value of an AVP.
 *  Note that the sid string is not copied, just its reference... do not free it!
 *
 * RETURN VALUE:
 *  0      	: The sid & len parameters have been updated.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_sess_getsid ( struct session * session, os0_t * sid, size_t * sidlen );

/*
 * FUNCTION:	fd_sess_settimeout
 *
 * PARAMETERS:
 *  session	: The session for which to set the timeout.
 *  timeout	: The date when the session times out.
 *
 * DESCRIPTION: 
 *   Set the lifetime for a given session object. This function may be 
 * called several times on the same object to update the timeout value.
 *   When the timeout date is reached, the cleanup handler of each 
 * module that registered data with this session is called, then the 
 * session is cleared.
 *
 *   There is a possible race condition between cleanup of the session
 * and use of its data; applications should ensure that they are not 
 * using data from a session that is about to expire / expired.
 *
 * RETURN VALUE:
 *  0      	: The session timeout has been updated.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_sess_settimeout( struct session * session, const struct timespec * timeout );

/*
 * FUNCTION:	fd_sess_destroy
 *
 * PARAMETERS:
 *  session	: Pointer to a session object.
 *
 * DESCRIPTION: 
 *   Destroys all associated states of a session, if any.
 * Equivalent to a session timeout expired, but the effect is immediate.
 * The session itself is marked as deleted, and will be freed when it is not referenced 
 * by any message anymore.
 *
 * RETURN VALUE:
 *  0      	: The session no longer exists.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_sess_destroy ( struct session ** session );

/*
 * FUNCTION:	fd_sess_reclaim
 *
 * PARAMETERS:
 *  session	: Pointer to a session object.
 *
 * DESCRIPTION: 
 *   Equivalent to fd_sess_destroy, only if no session_state is associated with the session.
 *  Otherwise, this function has no effect (except that it sets *session to NULL).
 *
 * RETURN VALUE:
 *  0      	: The session was reclaimed.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_sess_reclaim ( struct session ** session );




/*
 * FUNCTION:	fd_sess_state_store
 *
 * PARAMETERS:
 *  handler	: The handler with which the state is registered.
 *  session	: The session object with which the state is registered.
 *  state	: An application state (opaque data) to store with the session.
 *
 * DESCRIPTION: 
 *  Stores an application state with a session. This state can later be retrieved
 * with fd_sess_state_retrieve, or implicitly in the cleanup handler when the session
 * is destroyed.
 *
 * RETURN VALUE:
 *  0      	: The state has been stored.
 *  EINVAL 	: A parameter is invalid.
 *  EALREADY	: Data was already associated with this session and client.
 *  ENOMEM	: Not enough memory to complete the operation
 */
int fd_sess_state_store ( struct session_handler * handler, struct session * session, struct sess_state ** state );

/*
 * FUNCTION:	fd_sess_state_retrieve
 *
 * PARAMETERS:
 *  handler	: The handler with which the state was registered.
 *  session	: The session object with which the state was registered.
 *  state	: Location where the state must be saved if it is found.
 *
 * DESCRIPTION: 
 *  Retrieves a state saved by fd_sess_state_store.
 * After this function has been called, the state is no longer associated with 
 * the session. A new call to fd_sess_state_store must be performed in order to
 * store again the data with the session.
 *
 * RETURN VALUE:
 *  0      	: *state is updated (NULL or points to the state if it was found).
 *  EINVAL 	: A parameter is invalid.
 */
int fd_sess_state_retrieve ( struct session_handler * handler, struct session * session, struct sess_state ** state ); 


/* For debug */
DECLARE_FD_DUMP_PROTOTYPE(fd_sess_dump, struct session * session, int with_states);
DECLARE_FD_DUMP_PROTOTYPE(fd_sess_dump_hdl, struct session_handler * handler);

/* For statistics / monitoring: get the number of struct session in memory */
int fd_sess_getcount(uint32_t *cnt);

/*============================================================*/
/*                         ROUTING                            */
/*============================================================*/

/* The following functions are helpers for the routing module. 
  The routing data is stored in the message itself. */

/* Structure that contains the routing data for a message */
struct rt_data;

/* Following functions are helpers to create the routing data of a message */
int  fd_rtd_init(struct rt_data ** rtd);
void fd_rtd_free(struct rt_data ** rtd);

/* Add a peer to the candidates list. */
int  fd_rtd_candidate_add(struct rt_data * rtd, DiamId_t cfg_peerid, size_t cfg_peeridlen, DiamId_t peerid, size_t peeridlen, DiamId_t realm, size_t realmlen);

/* Remove a peer from the candidates (if it is found). The search is case-insensitive. */
void fd_rtd_candidate_del(struct rt_data * rtd, uint8_t * id, size_t idsz);

/* Extract the list of valid candidates, and initialize their scores to 0 */
void fd_rtd_candidate_extract(struct rt_data * rtd, struct fd_list ** candidates, int ini_score);

/* If a peer returned a protocol error for this message, save it so that we don't try to send it there again. Optionally retrieve the current list of candidates. */
int  fd_rtd_error_add(struct rt_data * rtd, DiamId_t sentto, size_t senttolen, uint8_t * origin, size_t originsz, uint32_t rcode, struct fd_list ** candidates, int * sendingattemtps);

/* Only retrieve the number of times this message has been processed by the routing-out mechanism (i.e. number of times it was failed over) */
int  fd_rtd_get_nb_attempts(struct rt_data * rtd, int * sendingattemtps);

/* The extracted list items have the following structure: */
struct rtd_candidate {
	struct fd_list	chain;	/* link in the list returned by the previous fcts */
	DiamId_t	cfg_diamid;	/* the configured diameter Id of the peer */
	size_t		cfg_diamidlen; /* cached size of the cfg_diamid string */
	DiamId_t	diamid;	/* the learned diameter Id of the peer */
	size_t		diamidlen; /* cached size of the diamid string */
	DiamId_t	realm;	/* the diameter realm of the peer */
	size_t		realmlen; /* cached size of realm */
	int		score;	/* the current routing score for this peer, see fd_rt_out_register definition for details */
};

/* Reorder the list of peers by score */
int  fd_rtd_candidate_reorder(struct fd_list * candidates);

/* Note : it is fine for a callback to add a new entry in the candidates list after the list has been extracted. The diamid must then be malloc'd. */
/* Beware that this could lead to routing loops */

/*============================================================*/
/*                         MESSAGES                           */
/*============================================================*/

/* The following types are opaque */
struct	msg;	/* A message: command with children AVPs (possibly grand children) */
struct	avp;	/* AVP object */

/* Some details about chaining:
 *
 *  A message is made of a header ( msg ) and 0 or more AVPs ( avp ).
 * The structure is a kind of tree, where some AVPs (grouped AVPs) can contain other AVPs.
 * Example:
 * msg
 *  |-avp
 *  |-gavp
 *  |   |-avp
 *  |   |-avp
 *  |   \-avp
 *  |-avp
 *  \-avp
 *
 */

/* The following type is used to point to either a msg or an AVP */
typedef void msg_or_avp;

/* The Diameter protocol version */
#define DIAMETER_VERSION	1

/* In the two following types, some fields are marked (READONLY). 
 * This means that the content of these fields will be overwritten by the daemon so modifying it is useless.
 */

/* The following structure represents the header of a message. All data is in host byte order. */
struct msg_hdr {
	uint8_t		 msg_version;		/* (READONLY) Version of Diameter: must be DIAMETER_VERSION. */
	uint32_t	 msg_length;		/* (READONLY)(3 bytes) indicates the length of the message */
	uint8_t		 msg_flags;		/* Message flags: CMD_FLAG_* */
	command_code_t	 msg_code;		/* (3 bytes) the command-code. See dictionary-api.h for more detail */
	application_id_t msg_appl;		/* The application issuing this message */
	uint32_t	 msg_hbhid;		/* The Hop-by-Hop identifier of the message */
	uint32_t	 msg_eteid;		/* The End-to-End identifier of the message */
};

/* The following structure represents the visible content of an AVP. All data is in host byte order. */
struct avp_hdr {
	avp_code_t	 avp_code;		/* the AVP Code */
	uint8_t		 avp_flags;		/* AVP_FLAG_* flags */
	uint32_t	 avp_len;		/* (READONLY)(Only 3 bytes are used) the length of the AVP as described in the RFC */
	vendor_id_t	 avp_vendor;		/* Only used if AVP_FLAG_VENDOR is present */
	union avp_value *avp_value;		/* pointer to the value of the AVP. NULL means that the value is not set / not understood.
						   One should not directly change this value. Use the msg_avp_setvalue function instead.
						   The content of the pointed structure can be changed directly, with this restriction:
						     if the AVP is an OctetString, and you change the value of the pointer avp_value->os.data, then
						     you must call free() on the previous value, and the new one must be free()-able.
						 */
};

/* The following enum is used to browse inside message hierarchy (msg, gavp, avp) */
enum msg_brw_dir {
	MSG_BRW_NEXT = 1,	/* Get the next element at the same level, or NULL if this is the last element. */
	MSG_BRW_PREV,		/* Get the previous element at the same level, or NULL if this is the first element. */
	MSG_BRW_FIRST_CHILD,	/* Get the first child AVP of this element, if any. */
	MSG_BRW_LAST_CHILD,	/* Get the last child AVP of this element, if any. */
	MSG_BRW_PARENT,		/* Get the parent element of this element, if any. Only the msg_t object has no parent. */
	MSG_BRW_WALK		/* This is equivalent to FIRST_CHILD or NEXT or PARENT->next, first that is not NULL. Use this to walk inside all AVPs. */
};

/* Some flags used in the functions below */
#define AVPFL_SET_BLANK_VALUE	   0x01	/* When creating an AVP, initialize its value to a blank area */
#define AVPFL_SET_RAWDATA_FROM_AVP 0x02 /* When creating an AVP, initialize its rawdata area from an existing AVP -- it is only blank padding (for error reporting) */
#define AVPFL_MAX		   AVPFL_SET_RAWDATA_FROM_AVP	/* The biggest valid flag value */
	
#define MSGFL_ALLOC_ETEID	0x01	/* When creating a message, a new end-to-end ID is allocated and set in the message */
#define MSGFL_ANSW_ERROR	0x02	/* When creating an answer message, set the 'E' bit and use the generic error ABNF instead of command-specific ABNF */
#define MSGFL_ANSW_NOSID	0x04	/* When creating an answer message, do not add the Session-Id even if present in request */
#define MSGFL_ANSW_NOPROXYINFO	0x08	/* When creating an answer message, do not add the Proxy-Info AVPs presents in request */
#define MSGFL_MAX		MSGFL_ANSW_NOPROXYINFO	/* The biggest valid flag value */

/**************************************************/
/*   Message creation, manipulation, disposal     */
/**************************************************/
/*
 * FUNCTION:	fd_msg_avp_new
 *
 * PARAMETERS:
 *  model 	: Pointer to a DICT_AVP dictionary object describing the avp to create, or NULL if flags are used.
 *  flags	: Flags to use in creation (AVPFL_*, see above).
 *  avp 	: Upon success, pointer to the new avp is stored here. It points to reference AVP upon function call when flags are used.
 *
 * DESCRIPTION: 
 *   Create a new AVP instance.
 *
 * RETURN VALUE:
 *  0      	: The AVP is created.
 *  EINVAL 	: A parameter is invalid.
 *  (other standard errors may be returned, too, with their standard meaning. Example:
 *    ENOMEM 	: Memory allocation for the new avp failed.)
 */
int fd_msg_avp_new ( struct dict_object * model, int flags, struct avp ** avp );

/*
 * FUNCTION:	fd_msg_new
 *
 * PARAMETERS:
 *  model 	: Pointer to a DICT_COMMAND dictionary object describing the message to create, or NULL.
 *  dictappl 	: Pointer to a DICT_APPLICATION dictionary used to overload the application learned from the DICT_COMMAND object.
 *  flags	: combination of MSGFL_* flags.
 *  msg 	: Upon success, pointer to the new message is stored here.
 *
 * DESCRIPTION: 
 *   Create a new empty Diameter message. 
 *
 * RETURN VALUE:
 *  0      	: The message is created.
 *  EINVAL 	: A parameter is invalid.
 *  (other standard errors may be returned, too, with their standard meaning. Example:
 *    ENOMEM 	: Memory allocation for the new message failed.)
 */
int fd_msg_new ( struct dict_object * model, int flags, struct msg ** msg );
int fd_msg_new_with_appl ( struct dict_object * model, struct dict_object * dictappl, int flags, struct msg ** msg );

/*
 * FUNCTION:	msg_new_answer_from_req
 *
 * PARAMETERS:
 *  dict	: Pointer to the dictionary containing the model of the query.
 *  msg		: The location of the query on function call. Updated by the location of answer message on return.
 *  flag        : Pass MSGFL_ANSW_ERROR to indicate if the answer is an error message (will set the 'E' bit)
 *              : See other MSGFL_ANSW_* definition above for other flags.
 *
 * DESCRIPTION: 
 *   This function creates the empty answer message corresponding to a request.
 *  The header is set properly (R flag, ccode, appid, hbhid, eteid)
 *  The Session-Id AVP is copied if present.
 *  The calling code should usually call fd_msg_rescode_set function on the answer.
 *  Upon return, the original query may be retrieved by calling fd_msg_answ_getq on the message.
 *
 * RETURN VALUE:
 *  0      	: Operation complete.
 *  !0      	: an error occurred.
 */
int fd_msg_new_answer_from_req ( struct dictionary * dict, struct msg ** msg, int flag );

/*
 * FUNCTION:	fd_msg_browse
 *
 * PARAMETERS:
 *  reference 	: Pointer to a struct msg or struct avp.
 *  dir         : Direction for browsing
 *  found       : If not NULL, updated with the element that has been found, if any, or NULL if no element was found / an error occurred.
 *  depth	: If not NULL, points to an integer representing the "depth" of this object in the tree. This is a relative value, updated on return.
 *
 * DESCRIPTION: 
 *   Explore the content of a message object (hierarchy). If "found" is null, only error checking is performed.
 *  If "depth" is provided, it is updated as follow on successful function return:
 *   - not modified for MSG_BRW_NEXT and MSG_BRW_PREV.
 *   - *depth = *depth + 1 for MSG_BRW_FIRST_CHILD and MSG_BRW_LAST_CHILD.
 *   - *depth = *depth - 1 for MSG_BRW_PARENT.
 *   - *depth = *depth + X for MSG_BRW_WALK, with X between 1 (returned the 1st child) and -N (returned the Nth parent's next).
 *
 * RETURN VALUE:
 *  0      	: found has been updated (if non NULL).
 *  EINVAL 	: A parameter is invalid.
 *  ENOENT	: No element has been found where requested, and "found" was NULL (otherwise, *found is set to NULL and 0 is returned). 
 */
int fd_msg_browse_internal ( msg_or_avp * reference, enum msg_brw_dir dir, msg_or_avp ** found, int * depth );
/* Macro to avoid having to cast the third parameter everywhere */
#define fd_msg_browse( ref, dir, found, depth )	\
	fd_msg_browse_internal( (ref), (dir), (void *)(found), (depth) )


/*
 * FUNCTION:	fd_msg_avp_add
 *
 * PARAMETERS:
 *  reference 	: Pointer to a valid msg or avp.
 *  dir         : location where the new AVP should be inserted, relative to the reference. MSG_BRW_PARENT and MSG_BRW_WALK are not valid.
 *  avp         : pointer to the AVP object that must be inserted.
 *
 * DESCRIPTION: 
 *   Adds an AVP into an object that can contain it: grouped AVP or message.
 * Note that the added AVP will be freed at the same time as the object it is added to, 
 * so it should not be freed after the call to this function.
 *
 * RETURN VALUE:
 *  0      	: The AVP has been added.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_avp_add ( msg_or_avp * reference, enum msg_brw_dir dir, struct avp *avp);

/*
 * FUNCTION:	fd_msg_search_avp
 *
 * PARAMETERS:
 *  msg 	: The message structure in which to search the AVP.
 *  what 	: The dictionary model of the AVP to search.
 *  avp		: location where the AVP reference is stored if found.
 *
 * DESCRIPTION: 
 *   Search the first top-level AVP of a given model inside a message.
 * Note: only the first instance of the AVP is returned by this function.
 * Note: only top-level AVPs are searched, not inside grouped AVPs.
 * Use msg_browse if you need more advanced research features.
 *
 * RETURN VALUE:
 *  0      	: The AVP has been found.
 *  EINVAL 	: A parameter is invalid.
 *  ENOENT	: No AVP has been found, and "avp" was NULL (otherwise, *avp is set to NULL and 0 returned).
 */
int fd_msg_search_avp ( struct msg * msg, struct dict_object * what, struct avp ** avp );

/*
 * FUNCTION:	fd_msg_free
 *
 * PARAMETERS:
 *  object      : pointer to the message or AVP object that must be unlinked and freed.
 *
 * DESCRIPTION: 
 *   Unlink and free a message or AVP object and its children.
 *  If the object is an AVP linked into a message, the AVP is removed before being freed.
 *
 * RETURN VALUE:
 *  0      	: The message has been freed.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_free ( msg_or_avp * object );

/***************************************/
/*   Dump functions                    */
/***************************************/
/*
 * FUNCTION:	fd_msg_dump_*
 *
 * PARAMETERS:
 *  see definition of DECLARE_FD_DUMP_PROTOTYPE,
 *  obj		 : A msg or avp object to dump.
 *  dict         : the dictionary to use if parsing is requested (optional)
 *  force_parsing: by default these functions do not parse the object but dump hexa values in that case.
 *                 use !0 to force parsing. If parsing fails, the hexa dump is still provided.
 *  recurse      : allow the function to go through the children objects if any to dump more information. might require parsing.
 *
 * DESCRIPTION: 
 *   These functions dump the content of a message or avp into a buffer
 * either recursively or only the object itself.
 *
 * RETURN VALUE:
 *   - see DECLARE_FD_DUMP_PROTOTYPE,
 */
/* one-line dump with only short information */
DECLARE_FD_DUMP_PROTOTYPE( fd_msg_dump_summary, msg_or_avp *obj, struct dictionary *dict, int force_parsing, int recurse );
/* one-line dump with all the contents of the message */
DECLARE_FD_DUMP_PROTOTYPE( fd_msg_dump_full, msg_or_avp *obj, struct dictionary *dict, int force_parsing, int recurse );
/* multi-line human-readable dump similar to wireshark output */
DECLARE_FD_DUMP_PROTOTYPE( fd_msg_dump_treeview, msg_or_avp *obj, struct dictionary *dict, int force_parsing, int recurse );


/*********************************************/
/*   Message metadata management functions   */
/*********************************************/
/*
 * FUNCTION:	fd_msg_model
 *
 * PARAMETERS:
 *  reference 	: Pointer to a valid msg or avp.
 *  model       : on success, pointer to the dictionary model of this command or AVP. NULL if the model is unknown.
 *
 * DESCRIPTION: 
 *   Retrieve the dictionary object describing this message or avp. If the object is unknown or the fd_msg_parse_dict has not been called,
 *  *model is set to NULL.
 *
 * RETURN VALUE:
 *  0      	: The model has been set.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_model ( msg_or_avp * reference, struct dict_object ** model );

/*
 * FUNCTION:	fd_msg_hdr
 *
 * PARAMETERS:
 *  msg 	: Pointer to a valid message object.
 *  pdata 	: Upon success, pointer to the msg_hdr structure of this message. The fields may be modified.
 *
 * DESCRIPTION: 
 *   Retrieve location of modifiable section of a message. 
 *
 * RETURN VALUE:
 *  0      	: The location has been written.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_hdr ( struct msg *msg, struct msg_hdr ** pdata );

/*
 * FUNCTION:	fd_msg_avp_hdr
 *
 * PARAMETERS:
 *  avp 	: Pointer to a valid avp object.
 *  pdata 	: Upon success, pointer to the avp_hdr structure of this avp. The fields may be modified.
 *
 * DESCRIPTION: 
 *   Retrieve location of modifiable data of an avp. 
 *
 * RETURN VALUE:
 *  0      	: The location has been written.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_avp_hdr ( struct avp *avp, struct avp_hdr ** pdata );

/*
 * FUNCTION:	fd_msg_answ_associate, fd_msg_answ_getq, fd_msg_answ_detach
 *
 * PARAMETERS:
 *  answer	: the received answer message
 *  query	: the corresponding query that had been sent
 *
 * DESCRIPTION:
 *  fd_msg_answ_associate associates a query msg with the received answer. 
 * Query is retrieved with fd_msg_answ_getq.
 * If answer message is freed, the query is also freed.
 * If the msg_answ_detach function is called, the association is removed.
 * This is meant to be called from the daemon only.
 *
 * RETURN VALUE:
 *  0 	  : ok
 *  EINVAL: a parameter is invalid
 */
int fd_msg_answ_associate( struct msg * answer, struct msg * query );
int fd_msg_answ_getq     ( struct msg * answer, struct msg ** query );
int fd_msg_answ_detach   ( struct msg * answer );

/*
 * FUNCTION:	fd_msg_anscb_associate, fd_msg_anscb_get
 *
 * PARAMETERS:
 *  msg		: the request message
 *  anscb	: the callback to associate with the message
 *  data	: the data to pass to the callback
 *  expirecb    : the expiration callback to associate with the message
 *  timeout     : (optional, use NULL if no timeout) a timeout associated with calling the cb.
 *
 * DESCRIPTION:
 *  Associate or retrieve callbacks with an message.
 * This is meant to be called from the daemon only.
 *
 * RETURN VALUE:
 *  0 	  : ok
 *  EINVAL: a parameter is invalid
 */
int fd_msg_anscb_associate( struct msg * msg, void ( *anscb)(void *, struct msg **), void  * data, void (*expirecb)(void *, DiamId_t, size_t, struct msg **), const struct timespec *timeout );
int fd_msg_anscb_get( struct msg * msg, void (**anscb)(void *, struct msg **), void (**expirecb)(void *, DiamId_t, size_t, struct msg **), void ** data );
int fd_msg_anscb_reset(struct msg * msg, int clear_anscb, int clear_expirecb);
struct timespec *fd_msg_anscb_gettimeout( struct msg * msg ); /* returns NULL or a valid non-0 timespec */

/*
 * FUNCTION:	fd_msg_rt_associate, fd_msg_rt_get
 *
 * PARAMETERS:
 *  msg		: the query message to be sent
 *  list	: the ordered list of possible next-peers
 *
 * DESCRIPTION:
 *  Associate a routing list with a query, and retrieve it.
 * If the message is freed, the list is also freed.
 *
 * RETURN VALUE:
 *  0 	  : ok
 *  EINVAL: a parameter is invalid
 */
int fd_msg_rt_associate( struct msg * msg, struct rt_data * rtd );
int fd_msg_rt_get      ( struct msg * msg, struct rt_data ** rtd );

/*
 * FUNCTION:	fd_msg_is_routable
 *
 * PARAMETERS:
 *  msg		: A msg object.
 *
 * DESCRIPTION: 
 *   This function returns a boolean telling if a given message is routable in the Diameter network, 
 *  or if it is a local link message only (ex: CER/CEA, DWR/DWA, ...).
 *
 * RETURN VALUE:
 *  0      	: The message is not routable / an error occurred.
 *  1      	: The message is routable.
 */
int fd_msg_is_routable ( struct msg * msg );

/*
 * FUNCTION:	fd_msg_source_(g/s)et
 *
 * PARAMETERS:
 *  msg		: A msg object.
 *  diamid,len	: The diameter id of the peer from which this message was received.
 *  dict	: a dictionary with definition of Route-Record AVP (for fd_msg_source_setrr)
 *
 * DESCRIPTION: 
 *   Store or retrieve the diameted id of the peer from which this message was received.
 * Will be used for example by the routing module to add the Route-Record AVP in forwarded requests,
 * or to direct answers to the appropriate peer.
 *
 * RETURN VALUE:
 *  0      	: Operation complete.
 *  !0      	: an error occurred.
 */
int fd_msg_source_set( struct msg * msg, DiamId_t diamid, size_t diamidlen );
int fd_msg_source_setrr( struct msg * msg, DiamId_t diamid, size_t diamidlen, struct dictionary * dict );
int fd_msg_source_get( struct msg * msg, DiamId_t *diamid, size_t * diamidlen );

/*
 * FUNCTION:	fd_msg_eteid_get
 *
 * PARAMETERS:
 *  -
 *
 * DESCRIPTION: 
 *   Get a new unique end-to-end id value for the local peer.
 *
 * RETURN VALUE:
 *  The new assigned value. No error code is defined.
 */
uint32_t fd_msg_eteid_get ( void );


/*
 * FUNCTION:	fd_msg_sess_get
 *
 * PARAMETERS:
 *  dict	: the dictionary that contains the Session-Id AVP definition
 *  msg		: A valid message.
 *  session	: Location to store the session pointer when retrieved.
 *  isnew	: Indicates if the session has been created.
 *
 * DESCRIPTION:
 *  This function retrieves or creates the session object corresponding to a message.
 * If the message does not contain a Session-Id AVP, *session == NULL on return.
 * Note that the Session-Id AVP must never be modified after created in a message.
 *
 * RETURN VALUE:
 *  0 : success
 * !0 : standard error code.
 */
int fd_msg_sess_get(struct dictionary * dict, struct msg * msg, struct session ** session, int * isnew);

/* This one is used by the libfdcore, you should use fd_msg_new_session rather than fd_sess_new, when possible */
int fd_msg_sess_set(struct msg * msg, struct session * session);


/* Helper for the hooks mechanism, for use from libfdcore */
struct fd_msg_pmdl {
	struct fd_list sentinel; /* if the sentinel.o field is NULL, the structure is not initialized. Otherwise it points to the cleanup function in libfdcore. */
	pthread_mutex_t lock;
};
struct fd_msg_pmdl * fd_msg_pmdl_get(struct msg * msg);


/***************************************/
/*   Manage AVP values                 */
/***************************************/

/*
 * FUNCTION:	fd_msg_avp_setvalue
 *
 * PARAMETERS:
 *  avp 	: Pointer to a valid avp object with a NULL avp_value pointer. The model must be known.
 *  value 	: pointer to an avp_value. The content will be COPIED into the internal storage area. 
 *		 If data type is an octetstring, the data is also copied.
 * 		 If value is a NULL pointer, the previous data is erased and value is unset in the AVP.
 *
 * DESCRIPTION: 
 *   Initialize the avp_value field of an AVP header.
 *
 * RETURN VALUE:
 *  0      	: The avp_value pointer has been set.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_avp_setvalue ( struct avp *avp, union avp_value *value );

/*
 * FUNCTION:	fd_msg_avp_value_encode
 *
 * PARAMETERS:
 *  avp 	: Pointer to a valid avp object with a NULL avp_value. The model must be known.
 *  data 	: Pointer to the data that must be encoded as AVP value and stored in the AVP.
 *		 This is only valid for AVPs of derived type for which type_data_encode callback is set. (ex: Address type)
 *
 * DESCRIPTION: 
 *   Initialize the avp_value field of an AVP object from formatted data, using the AVP's type "type_data_encode" callback.
 *
 * RETURN VALUE:
 *  0      	: The avp_value has been set.
 *  EINVAL 	: A parameter is invalid.
 *  ENOTSUP 	: There is no appropriate callback registered with this AVP's type.
 */
int fd_msg_avp_value_encode ( void *data, struct avp *avp );
/*
 * FUNCTION:	fd_msg_avp_value_interpret
 *
 * PARAMETERS:
 *  avp 	: Pointer to a valid avp object with a non-NULL avp_value value.
 *  data 	: Upon success, formatted interpretation of the AVP value is stored here.
 *
 * DESCRIPTION: 
 *   Interpret the content of an AVP of Derived type and store the result in data pointer. The structure
 * of the data pointer is dependent on the AVP type. This function calls the "type_data_interpret" callback 
 * of the type.
 *
 * RETURN VALUE:
 *  0      	: The avp_value has been set.
 *  EINVAL 	: A parameter is invalid.
 *  ENOTSUP 	: There is no appropriate callback registered with this AVP's type.
 */
int fd_msg_avp_value_interpret ( struct avp *avp, void *data );


/***************************************/
/*   Message parsing functions         */
/***************************************/

/*
 * FUNCTION:	fd_msg_bufferize
 *
 * PARAMETERS:
 *  msg		: A valid msg object. All AVPs must have a value set. 
 *  buffer 	: Upon success, this points to a buffer (malloc'd) containing the message ready for network transmission (or security transformations). 
 *		 The buffer may be freed after use.
 *  len		: if not NULL, the size of the buffer is written here. In any case, this size is updated in the msg header.
 *
 * DESCRIPTION: 
 *   Renders a message in memory as a buffer that can be sent over the network to the next peer.
 *
 * RETURN VALUE:
 *  0      	: The location has been written.
 *  EINVAL 	: The buffer does not contain a valid Diameter message.
 *  ENOMEM	: Unable to allocate enough memory to create the buffer object.
 */
int fd_msg_bufferize ( struct msg * msg, uint8_t ** buffer, size_t * len );

/*
 * FUNCTION:	fd_msg_parse_buffer
 *
 * PARAMETERS:
 *  buffer 	: Pointer to a buffer containing a message received from the network. 
 *  buflen	: the size in bytes of the buffer.
 *  msg		: Upon success, this points to a valid msg object. No AVP value is resolved in this object, nor grouped AVP.
 *
 * DESCRIPTION: 
 *   This function parses a buffer an creates a msg object to represent the structure of the message.
 *  Since no dictionary lookup is performed, the values of the AVPs are not interpreted. To interpret the values,
 *  the returned message object must be passed to fd_msg_parse_dict function.
 *  The buffer pointer is saved inside the message and will be freed when not needed anymore.
 *
 * RETURN VALUE:
 *  0      	: The location has been written.
 *  ENOMEM	: Unable to allocate enough memory to create the msg object.
 *  EBADMSG	: The buffer does not contain a valid Diameter message (or is truncated).
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_parse_buffer ( uint8_t ** buffer, size_t buflen, struct msg ** msg );

/* Parsing Error Information structure */
struct fd_pei {
	char *		pei_errcode;	/* name of the error code to use */
	struct avp *	pei_avp;	/* pointer to invalid (in original message) or missing AVP (to be freed) */
	int		pei_avp_free;	/* Set to 1 if the pei_avp must be freed */
	char *		pei_message;	/* Overwrite default message if needed */
	int		pei_protoerr; 	/* do we set the 'E' bit in the error message ? */
};

/*
 * FUNCTION:	fd_msg_parse_dict
 *
 * PARAMETERS:
 *  object	: A msg or AVP object as returned by fd_msg_parse_buffer.
 *  dict	: the dictionary containing the objects definitions to use for resolving all AVPs.
 *  error_info	: If not NULL, will contain the detail about error upon return. May be used to generate an error reply.
 *
 * DESCRIPTION: 
 *   This function looks up for the command and each children AVP definitions in the dictionary.
 *  If the dictionary definition is found, avp_model is set and the value of the AVP is interpreted accordingly and:
 *   - for grouped AVPs, the children AVP are created and interpreted also.
 *   - for numerical AVPs, the value is converted to host byte order and saved in the avp_value field.
 *   - for octetstring AVPs, the string is copied into a new buffer and its address is saved in avp_value. 
 *  If the dictionary definition is not found, avp_model is set to NULL and
 *  the content of the AVP is saved as an octetstring in an internal structure. avp_value is NULL.
 *  As a result, after this function has been called, there is no more dependency of the msg object to the message buffer, that is freed.
 *
 * RETURN VALUE:
 *  0      	: The message has been fully parsed as described.
 *  EINVAL 	: The msg parameter is invalid for this operation.
 *  ENOMEM	: Unable to allocate enough memory to complete the operation.
 *  ENOTSUP	: No dictionary definition for the command or one of the mandatory AVP was found.
 */
int fd_msg_parse_dict ( msg_or_avp * object, struct dictionary * dict, struct fd_pei * error_info );

/*
 * FUNCTION:	fd_msg_parse_rules
 *
 * PARAMETERS:
 *  object	: A msg or grouped avp object that must be verified.
 *  dict	: The dictionary containing the rules definitions.
 *  error_info	: If not NULL, the first problem information will be saved here.
 *
 * DESCRIPTION: 
 *   Check that the children of the object do not conflict with the dictionary rules (ABNF compliance).
 *
 * RETURN VALUE:
 *  0      	: The message has been fully parsed and complies to the defined rules.
 *  EBADMSG	: A conflict was detected, or a mandatory AVP is unknown in the dictionary.
 *  EINVAL 	: The msg or avp object is invalid for this operation.
 *  ENOMEM	: Unable to allocate enough memory to complete the operation.
 */
int fd_msg_parse_rules ( msg_or_avp * object, struct dictionary * dict, struct fd_pei * error_info);



/*
 * FUNCTION:	fd_msg_update_length
 *
 * PARAMETERS:
 *  object 	: Pointer to a valid msg or avp. 
 *
 * DESCRIPTION: 
 *   Update the length field of the object passed as parameter.
 * As a side effect, all children objects are also updated. Therefore, all avp_value fields of
 * the children AVPs must be set, or an error will occur.
 *
 * RETURN VALUE:
 *  0      	: The size has been recomputed.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_msg_update_length ( msg_or_avp * object );


/*============================================================*/
/*                         DISPATCH                           */
/*============================================================*/

/* Dispatch module (passing incoming messages to extensions registered callbacks)
 * is split between the library and the daemon.
 *
 * The library provides the support for associating dispatch callbacks with
 * dictionary objects.
 *
 * The daemon is responsible for calling the callbacks for a message when appropriate.
 *
 *
 * The dispatch module has two main roles:
 *  - help determine if a message can be handled locally (during the routing step)
 *        This decision involves only the application-id of the message.
 *  - pass the message to the callback(s) that will handle it (during the dispatch step)
 *
 * The first role is handled by the daemon.
 *
 * About the second, these are the possibilities for registering a dispatch callback:
 *
 * -> For All messages.
 *  This callback is called for all messages that are handled locally. This should be used only
 *  for debug purpose.
 *
 * -> by AVP value (constants only).
 *  This callback will be called when a message is received and contains an AVP with a specified enumerated value.
 *
 * -> by AVP.
 *  This callback will be called when the received message contains a certain AVP.
 *
 * -> by command-code.
 *  This callback will be called when the message is a specific command (and 'R' flag).
 *
 * -> by application.
 *  This callback will be called when the message has a specific application-id.
 *
 * ( by vendor: would this be useful? it may be added later)
 */
enum disp_how {
	DISP_HOW_ANY = 1,		/* Any message. This should be only used for debug. */
	DISP_HOW_APPID,			/* Any message with the specified application-id */
	DISP_HOW_CC,			/* Messages of the specified command-code (request or answer). App id may be specified. */
	DISP_HOW_AVP,			/* Messages containing a specific AVP. Command-code and App id may be specified. */
	DISP_HOW_AVP_ENUMVAL		/* Messages containing a specific AVP with a specific enumerated value. Command-code and App id may be specified. */
};
/*
 * Several criteria may be selected at the same time, for example command-code AND application id.
 *
 * If several callbacks are registered for the same object, they are called in the order they were registered.
 * The order in which the callbacks are called is:
 *  DISP_HOW_ANY
 *  DISP_HOW_AVP_ENUMVAL & DISP_HOW_AVP
 *  DISP_HOW_CC
 *  DISP_HOW_APPID
 */

/* When a callback is registered, a "when" argument is passed in addition to the disp_how value,
 * to specify which values the criteria must match. */
struct disp_when {
	struct dict_object *	app;
	struct dict_object *	command;
	struct dict_object *	avp;
	struct dict_object *	value;
};

/* Note that all the dictionary objects should really belong to the same dictionary!
 *
 * Here is the details on this "when" argument, depending on the disp_how value.
 *
 * DISP_HOW_ANY.
 *  In this case, "when" must be NULL.
 *
 * DISP_HOW_APPID.
 *  Only the "app_id" field must be set, other fields are ignored. It points to a dictionary object of type DICT_APPLICATION.
 *
 * DISP_HOW_CC.
 *  The "command" field must be defined and point to a dictionary object of type DICT_COMMAND.
 *  The "app_id" may be also set. In the case it is set, it restricts the callback to be called only with this command-code and app id.
 *  The other fields are ignored.
 *
 * DISP_HOW_AVP.
 *  The "avp" field of the structure must be set and point to a dictionary object of type DICT_AVP.
 *  The "app_id" field may be set to restrict the messages matching to a specific app id.
 *  The "command" field may also be set to a valid DICT_COMMAND object.
 *  The content of the "value" field is ignored.
 *
 * DISP_HOW_AVP_ENUMVAL.
 *  All fields have the same constraints and meaning as in DISP_REG_AVP. In addition, the "value" field must be set
 *  and points to a valid DICT_ENUMVAL object. 
 *
 * Here is a sumary of the fields: ( M : must be set; m : may be set; 0 : ignored )
 *  field:     app_id    command     avp    value
 * APPID :       M          0         0       0
 * CC    :       m          M         0       0
 * AVP   :       m          m         M       0
 * ENUMVA:       m          m         M       M
 */

enum disp_action {
	DISP_ACT_CONT,	/* The next handler should be called, unless *msg == NULL. */
	DISP_ACT_SEND,	/* The updated message must be sent. No further callback is called. */
	DISP_ACT_ERROR	/* An error must be created and sent as a reply -- not valid for callbacks, only for fd_msg_dispatch. */
};
/* The callbacks that are registered have the following prototype:
 *  	int dispatch_callback( struct msg ** msg, struct avp * avp, struct session * session, enum disp_action * action );
 *
 * CALLBACK:	dispatch_callback
 *
 * PARAMETERS:
 *  msg 	: the received message on function entry. may be updated to answer on return (see description)
 *  avp 	: for callbacks registered with DISP_HOW_AVP or DISP_HOW_AVP_ENUMVAL, direct link to the triggering AVP.
 *  session	: if the message contains a Session-Id AVP, the corresponding session object, NULL otherwise.
 *  opaque      : An opaque pointer that is registered along the session handler.
 *  action	: upon return, this tells the daemon what to do next.
 *
 * DESCRIPTION: 
 *   Called when a received message matchs the condition for which the callback was registered.
 * This callback may do any kind of processing on the message, including:
 *  - create an answer for a request.
 *  - proxy a request or message, add / remove the Proxy-Info AVP, then forward the message.
 *  - update a routing table or start a connection with a new peer, then forward the message.
 *  - ...
 * 
 * When *action == DISP_ACT_SEND on callback return, the msg pointed by *msg is passed to the routing module for sending.
 * When *action == DISP_ACT_CONT, the next registered callback is called.
 *  When the last callback gives also DISP_ACT_CONT action value, a default handler is called. It's behavior is as follow:
 *   - if the message is an answer, it is discarded.
 *   - if the message is a request, it is passed again to the routing stack, and marked as non-local handling.
 *
 * RETURN VALUE:
 *  0      	: The callback executed successfully and updated *action appropriately.
 *  !0		: standard errors. In case of error, the message is discarded.
 */

/* This structure represents a handler for a registered callback, allowing its de-registration */
struct disp_hdl;

/*
 * FUNCTION:	fd_disp_register
 *
 * PARAMETERS:
 *  cb 		  : The callback function to register (see dispatch_callback description above).
 *  how	  	  : How the callback must be registered.
 *  when          : Values that must match, depending on the how argument.
 *  opaque        : A pointer that is passed back to the handler. The content is not interpreted by the framework.
 *  handle        : On success, a handler to the registered callback is stored here if not NULL. 
 *		   This handler can be used to unregister the cb.
 *
 * DESCRIPTION: 
 *   Register a new callback to handle messages delivered locally.
 *
 * RETURN VALUE:
 *  0      	: The callback is registered.
 *  EINVAL 	: A parameter is invalid.
 *  ENOMEM	: Not enough memory to complete the operation
 */
int fd_disp_register ( int (*cb)( struct msg **, struct avp *, struct session *, void *, enum disp_action *), 
			enum disp_how how, struct disp_when * when, void * opaque, struct disp_hdl ** handle );

/*
 * FUNCTION:	fd_disp_unregister
 *
 * PARAMETERS:
 *  handle       : Location of the handle of the callback that must be unregistered.
 *  opaque       : If not NULL, the opaque data that was registered is restored here.
 *
 * DESCRIPTION: 
 *   Removes a callback previously registered by fd_disp_register.
 *
 * RETURN VALUE:
 *  0      	: The callback is unregistered.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_disp_unregister ( struct disp_hdl ** handle, void ** opaque );

/* Destroy all handlers */
void fd_disp_unregister_all ( void );

/*
 * FUNCTION:	fd_msg_dispatch
 *
 * PARAMETERS:
 *  msg 	: A msg object that have already been fd_msg_parse_dict.
 *  session	: The session corresponding to this object, if any.
 *  action	: Upon return, the action that must be taken on the message
 *  error_code	: Upon return with action == DISP_ACT_ERROR, contains the error (such as "DIAMETER_UNABLE_TO_COMPLY")
 *  drop_reason : if set on return, the message must be freed for this reason.
 *  drop_msg    : if drop_reason is set, this points to the message to be freed while *msg is NULL.
 *
 * DESCRIPTION: 
 *   Call all handlers registered for a given message.
 *  The session must have already been resolved on entry.
 *  The msg pointed may be updated during this process.
 *  Upon return, the action parameter points to what must be done next.
 *
 * RETURN VALUE:
 *  0      	: Success.
 *  EINVAL 	: A parameter is invalid.
 *  (other errors)
 */
int fd_msg_dispatch ( struct msg ** msg, struct session * session, enum disp_action *action, char ** error_code, char ** drop_reason, struct msg ** drop_msg );



/*============================================================*/
/*                     QUEUES                                 */
/*============================================================*/

/* Management of FIFO queues of elements */

/* A queue is an opaque object */
struct fifo;

/*
 * FUNCTION:	fd_fifo_new
 *
 * PARAMETERS:
 *  queue	: Upon success, a pointer to the new queue is saved here.
 *  max		: max number of items in the queue. Above this number, adding a new item becomes a
 *		  blocking operation. Use 0 to disable this maximum.
 *
 * DESCRIPTION: 
 *  Create a new empty queue.
 *
 * RETURN VALUE :
 *  0		: The queue has been initialized successfully.
 *  EINVAL 	: The parameter is invalid.
 *  ENOMEM	: Not enough memory to complete the creation.  
 */
int fd_fifo_new ( struct fifo ** queue, int max );

/*
 * FUNCTION:	fd_fifo_del
 *
 * PARAMETERS:
 *  queue	: Pointer to an empty queue to delete.
 *
 * DESCRIPTION: 
 *  Destroys a queue. This is only possible if no thread is waiting for an element,
 * and the queue is empty.
 *
 * RETURN VALUE:
 *  0		: The queue has been destroyed successfully.
 *  EINVAL 	: The parameter is invalid.
 */
int fd_fifo_del ( struct fifo  ** queue );

/*
 * FUNCTION:	fd_fifo_move
 *
 * PARAMETERS:
 *  oldq	: Location of a FIFO that is to be emptied.
 *  newq	: A FIFO that will receive the old data.
 *  loc_update	: if non NULL, a place to store the pointer to new FIFO atomically with the move.
 *
 * DESCRIPTION: 
 *  Empties a queue and move its content to another one atomically.
 *
 * RETURN VALUE:
 *  0		: The queue has been destroyed successfully.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_fifo_move ( struct fifo * oldq, struct fifo * newq, struct fifo ** loc_update );

/*
 * FUNCTION:	fd_fifo_getstats
 *
 * PARAMETERS:
 *  queue	  : The queue from which to retrieve the information.
 *  current_count : How many items in the queue at the time of execution. This changes each time an item is pushed or poped.
 *  limit_count   : The maximum number of items allowed in this queue. This is specified during queue creation.
 *  highest_count : The maximum number of items this queue has contained. This enables to see if limit_count count was reached.
 *  total_count   : the total number of items that went through the queue (already pop'd). Always increasing.
 *  total	  : Cumulated time all items spent in this queue, including blocking time (always growing, use deltas for monitoring)
 *  blocking      : Cumulated time threads trying to post new items were blocked (queue full).
 *  last          : For the last element retrieved from the queue, how long it take between posting (including blocking) and poping
 *  
 * DESCRIPTION: 
 *  Retrieve the timing information associated with a queue, for monitoring purpose.
 *
 * RETURN VALUE:
 *  0		: The statistics have been updated.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_fifo_getstats( struct fifo * queue, int * current_count, int * limit_count, int * highest_count, long long * total_count, 
				           struct timespec * total, struct timespec * blocking, struct timespec * last);

/*
 * FUNCTION:	fd_fifo_length
 *
 * PARAMETERS:
 *  queue	: The queue from which to retrieve the number of elements.
 *
 * DESCRIPTION: 
 *  Retrieve the number of elements in a queue, without error checking.
 *
 * RETURN VALUE:
 *  The number of items currently queued.
 */
int fd_fifo_length ( struct fifo * queue );

/*
 * FUNCTION:	fd_fifo_setthrhd
 *
 * PARAMETERS:
 *  queue	: The queue for which the thresholds are being set.
 *  data	: An opaque pointer that is passed to h_cb and l_cb callbacks.
 *  high        : The high-level threshold. If the number of elements in the queue increase to this value, h_cb is called.
 *  h_cb        : if not NULL, a callback to call when the queue lengh is bigger than "high".
 *  low         : The low-level threshold. Must be < high.
 *  l_cb        : If the number of elements decrease to low, this callback is called.
 *
 * DESCRIPTION: 
 *  This function allows to adjust the number of producer / consumer threads of a queue.
 * If the consumer are slower than the producers, the number of elements in the queue increase.
 * By setting a "high" value, we allow a callback to be called when this number is too high.
 * The typical use would be to create an additional consumer thread in this callback.
 * If the queue continues to grow, the callback will be called again when the length is 2 * high, then 3*high, ... N * high
 * (the callback itself should implement a limit on the number of consumers that can be created)
 * When the queue starts to decrease, and the number of elements go under ((N - 1) * high + low, the l_cb callback is called
 * and would typially stop one of the consumer threads. If the queue continues to reduce, l_cb is again called at (N-2)*high + low,
 * and so on.
 *
 * Since there is no destructor for the data pointer, if cleanup operations are required, they should be performed in
 * l_cb when the length of the queue is becoming < low.
 *
 * Note that the callbacks are called synchronously, during fd_fifo_post or fd_fifo_get. Their operation should be quick.
 *
 * RETURN VALUE:
 *  0		: The thresholds have been set
 *  EINVAL 	: A parameter is invalid.
 */
int fd_fifo_setthrhd ( struct fifo * queue, void * data, uint16_t high, void (*h_cb)(struct fifo *, void **), uint16_t low, void (*l_cb)(struct fifo *, void **) );

/*
 * FUNCTION:	fd_fifo_post
 *
 * PARAMETERS:
 *  queue	: The queue in which the element must be posted.
 *  item	: The element that is put in the queue.
 *
 * DESCRIPTION: 
 *  An element is added in a queue. Elements are retrieved from the queue in FIFO order
 *  with the fd_fifo_get, fd_fifo_tryget, or fd_fifo_timedget functions.
 *
 * RETURN VALUE:
 *  0		: The element is queued.
 *  EINVAL 	: A parameter is invalid.
 *  ENOMEM 	: Not enough memory to complete the operation.
 */
int fd_fifo_post_int ( struct fifo * queue, void ** item );
#define fd_fifo_post(queue, item) \
	fd_fifo_post_int((queue), (void *)(item))

/* Similar function but does not block. It can cause the number of items in the queue to exceed the maximum set. Do not use for normal operation,
only for failure recovery for example. */
int fd_fifo_post_noblock( struct fifo * queue, void ** item );

/*
 * FUNCTION:	fd_fifo_get
 *
 * PARAMETERS:
 *  queue	: The queue from which the first element must be retrieved.
 *  item	: On return, the first element of the queue is stored here.
 *
 * DESCRIPTION: 
 *  This function retrieves the first element from a queue. If the queue is empty, the function will block the 
 * thread until a new element is posted to the queue, or until the thread is canceled (in which case the 
 * function does not return).
 *
 * RETURN VALUE:
 *  0		: A new element has been retrieved.
 *  EINVAL 	: A parameter is invalid.
 */
int fd_fifo_get_int ( struct fifo * queue, void ** item );
#define fd_fifo_get(queue, item) \
	fd_fifo_get_int((queue), (void *)(item))

/*
 * FUNCTION:	fd_fifo_tryget
 *
 * PARAMETERS:
 *  queue	: The queue from which the element must be retrieved.
 *  item	: On return, the first element of the queue is stored here.
 *
 * DESCRIPTION: 
 *  This function is similar to fd_fifo_get, except that it will not block if 
 * the queue is empty, but return EWOULDBLOCK instead.
 *
 * RETURN VALUE:
 *  0		: A new element has been retrieved.
 *  EINVAL 	: A parameter is invalid.
 *  EWOULDBLOCK : The queue was empty.
 */
int fd_fifo_tryget_int ( struct fifo * queue, void ** item );
#define fd_fifo_tryget(queue, item) \
	fd_fifo_tryget_int((queue), (void *)(item))

/*
 * FUNCTION:	fd_fifo_timedget
 *
 * PARAMETERS:
 *  queue	: The queue from which the element must be retrieved.
 *  item	: On return, the element is stored here.
 *  abstime	: the absolute time until which we allow waiting for an item.
 *
 * DESCRIPTION: 
 *  This function is similar to fd_fifo_get, except that it will block if the queue is empty 
 * only until the absolute time abstime (see pthread_cond_timedwait for + info).
 * If the queue is still empty when the time expires, the function returns ETIMEDOUT
 *
 * RETURN VALUE:
 *  0		: A new item has been retrieved.
 *  EINVAL 	: A parameter is invalid.
 *  ETIMEDOUT   : The time out has passed and no item has been received.
 */
int fd_fifo_timedget_int ( struct fifo * queue, void ** item, const struct timespec *abstime );
#define fd_fifo_timedget(queue, item, abstime) \
	fd_fifo_timedget_int((queue), (void *)(item), (abstime))


/*
 * FUNCTION:	fd_fifo_select
 *
 * PARAMETERS:
 *  queue	: The queue to test.
 *  abstime	: the absolute time until which we can block waiting for an item. If NULL, the function returns immediatly.
 *
 * DESCRIPTION: 
 *  This function is similar to select(), it waits for data to be available in the queue
 * until the abstime is expired.
 * Upon function entry, even if abstime is already expired the data availability is tested.
 *
 * RETURN VALUE:
 *  0		: timeout expired without available data.
 *  <0		: An error occurred (e.g., -EINVAL...)
 *  >0		: data is available. The next call to fd_fifo_get will not block.
 */
int fd_fifo_select ( struct fifo * queue, const struct timespec *abstime );



/* Dump a fifo list and optionally its inner elements -- beware of deadlocks! */
typedef DECLARE_FD_DUMP_PROTOTYPE((*fd_fifo_dump_item_cb), void * item); /* This function should be 1 line if possible, or use indent level. Ends with '\n' */
DECLARE_FD_DUMP_PROTOTYPE(fd_fifo_dump, char * name, struct fifo * queue, fd_fifo_dump_item_cb dump_item);

/* We ignore the mandatory flag on AVPs from this list of vendors. The list
 * should be 0 terminated. By default there are no such vendors. */
int fd_define_untrusted_avp_vendors(uint32_t* untrusted_avp_vendors);

#ifdef __cplusplus
}
#endif

#endif /* _LIBFDPROTO_H */
