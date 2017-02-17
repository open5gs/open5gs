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
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

#include "fdproto-internal.h"

#include <stdarg.h>

pthread_mutex_t fd_log_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_key_t	fd_log_thname;
int fd_g_debug_lvl = FD_LOG_NOTICE;

static void fd_internal_logger( int, const char *, va_list );
static int use_colors = 0; /* 0: not init, 1: yes, 2: no */

/* These may be used to pass specific debug requests via the command-line parameters */
char * fd_debug_one_function = NULL;
char * fd_debug_one_file = NULL;

/* Useless function, only to ease setting up a breakpoint in gdb (break fd_breakhere) -- use TRACE_HERE */
int fd_breaks = 0;
int fd_breakhere(void) { return ++fd_breaks; }

/* Allow passing of the log and debug information from base stack to extensions */
void (*fd_logger)( int loglevel, const char * format, va_list args ) = fd_internal_logger;

/* Register an external call back for tracing and debug */
int fd_log_handler_register( void (*logger)(int loglevel, const char * format, va_list args) )
{
        CHECK_PARAMS( logger );

        if ( fd_logger != fd_internal_logger )
        {
               return EALREADY; /* only one registration allowed */
        }
        else
        {
               fd_logger = logger;
        }

        return 0;
}

/* Implement a simple reset function here */
int fd_log_handler_unregister ( void )
{
        fd_logger = fd_internal_logger;
        return 0; /* Successfull in all cases. */
}

static void fd_cleanup_mutex_silent( void * mutex )
{
	(void)pthread_mutex_unlock((pthread_mutex_t *)mutex);
}


static void fd_internal_logger( int printlevel, const char *format, va_list ap )
{
    char buf[25];

    /* Do we need to trace this ? */
    if (printlevel < fd_g_debug_lvl)
    	return;

    /* add timestamp */
    printf("%s  ", fd_log_time(NULL, buf, sizeof(buf), 
#if (defined(DEBUG) && !defined(DEBUG_WITHOUT_META))
    	1, 1
#else /* (defined(DEBUG) && !defined(DEBUG_WITHOUT_META)) */
        0, 0
#endif /* (defined(DEBUG) && !defined(DEBUG_WITHOUT_META)) */
	    ));
    /* Use colors on stdout ? */
    if (!use_colors) {
	if (isatty(STDOUT_FILENO))
		use_colors = 1;
	else
		use_colors = 2;
    }
    
    switch(printlevel) {
	    case FD_LOG_ANNOYING:  printf("%s	A   ", (use_colors == 1) ? "\e[0;37m" : ""); break;
	    case FD_LOG_DEBUG:     printf("%s DBG   ", (use_colors == 1) ? "\e[0;37m" : ""); break;
	    case FD_LOG_NOTICE:    printf("%sNOTI   ", (use_colors == 1) ? "\e[1;37m" : ""); break;
	    case FD_LOG_ERROR:     printf("%sERROR  ", (use_colors == 1) ? "\e[0;31m" : ""); break;
	    case FD_LOG_FATAL:     printf("%sFATAL! ", (use_colors == 1) ? "\e[0;31m" : ""); break;
	    default:               printf("%s ???   ", (use_colors == 1) ? "\e[0;31m" : "");
    }
    vprintf(format, ap);
    if (use_colors == 1)
	     printf("\e[00m");
    printf("\n");
    
    fflush(stdout);
}

/* Log a debug message */
void fd_log ( int loglevel, const char * format, ... )
{
	va_list ap;
	
	(void)pthread_mutex_lock(&fd_log_lock);
	
	pthread_cleanup_push(fd_cleanup_mutex_silent, &fd_log_lock);
	
	va_start(ap, format);
	fd_logger(loglevel, format, ap);
	va_end(ap);

	pthread_cleanup_pop(0);
	
	(void)pthread_mutex_unlock(&fd_log_lock);
}

/* Log a debug message */
void fd_log_va ( int loglevel, const char * format, va_list args )
{
	(void)pthread_mutex_lock(&fd_log_lock);
	
	pthread_cleanup_push(fd_cleanup_mutex_silent, &fd_log_lock);
	fd_logger(loglevel, format, args);
	pthread_cleanup_pop(0);
	
	(void)pthread_mutex_unlock(&fd_log_lock);
}

/* Function to set the thread's friendly name */
void fd_log_threadname ( const char * name )
{
	void * val = NULL;
	
	TRACE_ENTRY("%p(%s)", name, name?:"/");
	
	/* First, check if a value is already assigned to the current thread */
	val = pthread_getspecific(fd_log_thname);
	if (TRACE_BOOL(ANNOYING)) {
		if (val) {
			fd_log_debug("(Thread '%s' renamed to '%s')", (char *)val, name?:"(nil)");
		} else {
			fd_log_debug("(Thread %p named '%s')", (void *)pthread_self(), name?:"(nil)");
		}
	}
	if (val != NULL) {
		free(val);
	}
	
	/* Now create the new string */
	if (name == NULL) {
		CHECK_POSIX_DO( pthread_setspecific(fd_log_thname, NULL), /* continue */);
		return;
	}
	
	CHECK_MALLOC_DO( val = strdup(name), return );
	
	CHECK_POSIX_DO( pthread_setspecific(fd_log_thname, val), /* continue */);
	return;
}

/* Write time into a buffer */
char * fd_log_time ( struct timespec * ts, char * buf, size_t len, int incl_date, int incl_ms )
{
	int ret;
	size_t offset = 0;
	struct timespec tp;
	struct tm tm;
	
	/* Get current time */
	if (!ts) {
		ret = clock_gettime(CLOCK_REALTIME, &tp);
		if (ret != 0) {
			snprintf(buf, len, "%s", strerror(ret));
			return buf;
		}
		ts = &tp;
	}
	
	offset += strftime(buf + offset, len - offset, incl_date?"%D,%T":"%T", localtime_r( &ts->tv_sec , &tm ));
	if (incl_ms)
		offset += snprintf(buf + offset, len - offset, ".%6.6ld", ts->tv_nsec / 1000);

	return buf;
}


static size_t sys_mempagesz = 0;

static size_t get_mempagesz(void) {
	if (!sys_mempagesz) {
		sys_mempagesz = sysconf(_SC_PAGESIZE); /* We alloc buffer by memory pages for efficiency. This can be readjusted if too memory consuming */
		if (sys_mempagesz <= 0)
			sys_mempagesz = 256; /* default size if above call failed */
	}
	return sys_mempagesz;
}


/* Helper function for fd_*_dump. Prints the format string from 'offset' into '*buf', extends if needed. The location of buf can be updated by this function. */
char * fd_dump_extend(char ** buf, size_t *len, size_t *offset, const char * format, ... )
{
	va_list ap;
	int to_write;
	size_t o = 0;
	size_t mempagesz = get_mempagesz();
	
	/* we do not TRACE_ENTRY this one on purpose */
	
	CHECK_PARAMS_DO(buf && len, return NULL);
	
	if (*buf == NULL) {
		CHECK_MALLOC_DO(*buf = malloc(mempagesz), return NULL);
		*len = mempagesz;
	}
	
	if (offset)
		o = *offset;
	
	va_start(ap, format);
	to_write = vsnprintf(*buf + o, *len - o, format, ap);
	va_end(ap);
	
	if (to_write + o >= *len) {
		/* There was no room in the buffer, we extend and redo */
		size_t new_len = (((to_write + o) / mempagesz) + 1) * mempagesz;
		CHECK_MALLOC_DO(*buf = realloc(*buf, new_len), return NULL);
		*len = new_len;
		
		va_start(ap, format);
		to_write = vsnprintf(*buf + o, *len - o, format, ap);
		va_end(ap);
	}
	
	if (offset)
		*offset += to_write;
	
	return *buf;
}

char * fd_dump_extend_hexdump(char ** buf, size_t *len, size_t *offset, uint8_t *data, size_t datalen, size_t trunc, size_t wrap )
{
	int truncated = 0;
	size_t towrite = 0;
	size_t o = 0;
	int i;
	char * p;
	size_t mempagesz = get_mempagesz();
#define TRUNK_MARK "[...]"

	CHECK_PARAMS_DO(buf && len && data, return NULL);
	
	if (trunc && (datalen > trunc)) {
		datalen = trunc;
		truncated = 1;
	}
	
	towrite = datalen * 2;
	
	if (wrap)
		towrite += datalen / wrap; /* add 1 '\n' every wrap byte */
	
	if (truncated)
		towrite += CONSTSTRLEN(TRUNK_MARK);
	
	
	if (offset)
		o = *offset;
	
	if (*buf == NULL) {
		/* Directly allocate the size we need */
		*len = (((towrite + o) / mempagesz) + 1 ) * mempagesz;
		CHECK_MALLOC_DO(*buf = malloc(*len), return NULL);
	} else if ((towrite + o) >= *len) {
		/* There is no room in the buffer, we extend and redo */
		size_t new_len = (((towrite + o) / mempagesz) + 1) * mempagesz;
		CHECK_MALLOC_DO(*buf = realloc(*buf, new_len), return NULL);
		*len = new_len;
	}
	
	p = *buf + o;
	for (i = 0; i < datalen; i++) {
		sprintf(p, "%02hhX", data[i]);
		p+=2;
		if ((wrap) && ((i+1) % wrap == 0)) {
			*p++='\n'; *p ='\0'; /* we want to ensure the buffer is always 0-terminated */
		}
	}
	
	if (truncated)
		memcpy(p, TRUNK_MARK, CONSTSTRLEN(TRUNK_MARK));
		
	if (offset)
		*offset += towrite;
	
	return *buf;
}
