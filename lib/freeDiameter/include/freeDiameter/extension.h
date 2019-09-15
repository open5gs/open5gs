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

#ifndef _EXTENSION_H
#define _EXTENSION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include definition of freeDiameter API */
#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdcore.h>

/* Macro that define the entry point of the extension */
#define EXTENSION_ENTRY(_name, _function, _depends...)					\
const char *fd_ext_depends[] = { _name , ## _depends , NULL };				\
static int extension_loaded = 0;							\
int fd_ext_init(int major, int minor, char * conffile) {				\
	if ((major != FD_PROJECT_VERSION_MAJOR)						\
		|| (minor != FD_PROJECT_VERSION_MINOR)) {				\
		TRACE_ERROR("This extension (" _name ") was compiled for a different version of freeDiameter.");	\
		TRACE_DEBUG(INFO, "daemon %d.%d != ext %d.%d",				\
			major, minor,							\
			FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR);		\
		return EINVAL;								\
	}										\
	if (extension_loaded) {								\
		TRACE_ERROR("Extension (" _name ") cannot be loaded twice!");		\
		return ENOTSUP;								\
	}										\
	extension_loaded++;								\
	return (_function)(conffile);							\
}														

#ifdef __cplusplus
}
#endif

#endif /* _EXTENSION_H */
