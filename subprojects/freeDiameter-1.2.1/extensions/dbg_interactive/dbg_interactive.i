/* This interface file is processed by SWIG to create a python wrapper interface to freeDiameter framework. */
%module fDpy
%begin %{
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
%}

%{
/* This text is included in the generated wrapper verbatim */
#define SWIG
#include <freeDiameter/extension.h>
%}


/* Include standard types & functions used in freeDiameter headers */
%include <stdint.i>
//%include <cdata.i>
%include <cstring.i>
%include <typemaps.i>


/* Inline functions seems to give problems to SWIG -- just remove the inline definition */
%define __inline__ 
%enddef


/* Make some global-variables read-only (mainly to avoid warnings) */
%immutable fd_g_config;
%immutable peer_state_str;

#pragma SWIG nowarn=451

/*****************
 *  Exceptions  *
*****************/
%{
/* This is not thread-safe etc. but it should work /most of the time/. */
static int wrapper_errno;
static PyObject* wrapper_errno_py;
static const char * wrapper_error_txt; /* if NULL, use strerror(errno) */
#define DI_ERROR(code, pycode, str) {	\
	fd_log_debug("[dbg_interactive] ERROR: %s: %s", __PRETTY_FUNCTION__, str ? str : strerror(code)); \
	wrapper_errno = code;		\
	wrapper_errno_py = pycode;	\
	wrapper_error_txt = str;	\
}

#define DI_ERROR_MALLOC	\
	 DI_ERROR(ENOMEM, PyExc_MemoryError, NULL)

%}

%exception {
	/* reset the errno */
	wrapper_errno = 0;
	/* Call the function  -- it will use DI_ERROR macro in case of error */
	$action
	/* Now, test for error */
	if (wrapper_errno) {
		const char * str = wrapper_error_txt ? wrapper_error_txt : strerror(wrapper_errno);
		PyObject * exc = wrapper_errno_py;
		if (!exc) {
			switch (wrapper_errno) {
				case ENOMEM: exc = PyExc_MemoryError; break;
				case EINVAL: exc = PyExc_ValueError; break;
				default: exc = PyExc_RuntimeError;
			}
		}
		SWIG_PYTHON_THREAD_BEGIN_BLOCK;
		PyErr_SetString(exc, str);
		SWIG_fail;
		SWIG_PYTHON_THREAD_END_BLOCK;
	}
}


/***********************************
 Some types & typemaps for usability 
 ***********************************/

%apply (char *STRING, size_t LENGTH) { ( char * string, size_t len ) };
%apply (char *STRING, size_t LENGTH) { ( uint8_t * string, size_t len ) };

/* Generic typemap for functions that create something */
%typemap(in, numinputs=0,noblock=1) SWIGTYPE ** OUTPUT (void *temp = NULL) {
	$1 = (void *)&temp;
}
%typemap(argout,noblock=1) SWIGTYPE ** OUTPUT {
	%append_output(SWIG_NewPointerObj(*$1, $*1_descriptor, 0));
}

/* Case of the fd_*_dump functions */
%typemap(in,noblock=1,numinputs=0) (char ** buf, size_t *len, size_t *offset) ($*1_ltype temp = NULL, $*2_ltype tempn = 0) {
	$1 = &temp; $2 = &tempn; $3 = NULL;
}
%typemap(freearg,match="in") (char ** buf, size_t *len, size_t *offset) "";
%typemap(argout,noblock=1,fragment="SWIG_FromCharPtr")(char ** buf, size_t *len, size_t *offset) { 
  if (*$1) {
    %append_output(SWIG_FromCharPtr(*$1));
    free(*$1);					  	     
  }					  	     
}							     

/* Typemap to return a boolean value as output parameter */
%typemap(in, numinputs=0,noblock=1) int * BOOL_OUT (int temp) {
	$1 = &temp;
}
%typemap(argout,noblock=1) int * BOOL_OUT {
	PyObject * r;
	if (*$1)
		r = Py_True;
	else
		r = Py_False;
	Py_XINCREF(r);
	%append_output(r);
}

/* To allow passing callback functions defined in python */
%typemap(in) PyObject *PyCb {
	if (!$input || ($input == Py_None)) {
		$1 = NULL;
	} else {
		if (!PyCallable_Check($input)) {
			PyErr_SetString(PyExc_TypeError, "Need a callable object!");
			SWIG_fail;
		}
		$1 = $input;
	}
}

%{
/* Forward declaration for the peers module */
static void fd_add_cb(struct peer_info *peer, void *data);

/* This one gives problems when included from the header file */
void fd_log_va( int, const char *, va_list);
void fd_log_deprecated( int level, const char *format, ... ) MARK_DEPRECATED
{ 
	va_list ap;
	va_start(ap, format);
	fd_log_va(level, format, ap);
	va_end(ap);
}

%}

/* Overwrite declaration to apply typemaps */
int fd_sess_fromsid ( uint8_t * string, size_t len, struct session ** OUTPUT, int * BOOL_OUT);



/*********************************************************
 Now, create wrappers for (almost) all objects from fD API 
 *********************************************************/
%include "freeDiameter/freeDiameter-host.h"
%include "freeDiameter/libfdproto.h"
%include "freeDiameter/libfdcore.h"

/* Most of the functions from the API are not directly usable "as is".
See the specific following files and the dbg_interactive.py.sample file
for more usable python-style versions.
*/

%include "lists.i"
%include "dictionary.i"
%include "sessions.i"
%include "routing.i"
%include "messages.i"
%include "dispatch.i"
%include "queues.i"

%include "peers.i"
%include "events.i"
%include "endpoints.i"
%include "hooks.i"

%include "posix.i"
