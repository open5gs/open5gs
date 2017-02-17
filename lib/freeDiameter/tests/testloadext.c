/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2011, WIDE Project and NICT								 *
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

#include "tests.h"

#ifndef BUILD_DIR
#error "Missing BUILD_DIR information"
#endif /* BUILD_DIR */

#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <dlfcn.h>

/* Main test routine */
int main(int argc, char *argv[])
{
	DIR *dir;
	struct dirent *dp;
	char fullname[512];
	int pathlen;

	/* First, initialize the daemon modules */
	INIT_FD();
	CHECK( 0, fd_queues_init()  );
	CHECK( 0, fd_msg_init()  );
	CHECK( 0, fd_rtdisp_init()  );
	
	/* Find all extensions which have been compiled along the test */
	TRACE_DEBUG(INFO, "Loading from: '%s'", BUILD_DIR "/extensions");
	CHECK( 0, (dir = opendir (BUILD_DIR "/extensions")) == NULL ? 1 : 0 );
	pathlen = snprintf(fullname, sizeof(fullname), BUILD_DIR "/extensions/");
	
	while ((dp = readdir (dir)) != NULL) {
		char * dot = strrchr(dp->d_name, '.');
		if (dot && !(strcmp(dot, ".fdx"))) {
			/* We found a file with name *.fdx, attempt to load it */
			void *hdl, * ep;
			snprintf(fullname + pathlen, sizeof(fullname) - pathlen, "%s", dp->d_name);
			
			TRACE_DEBUG(INFO, "Extension: '%s'", dp->d_name);
			
			/* load */
			hdl = dlopen(fullname, RTLD_NOW | RTLD_GLOBAL);
			if (!hdl) {
				TRACE_DEBUG(INFO, "Unable to load '%s': %s.", fullname, dlerror());
			}
			CHECK( 0, hdl == NULL ? 1 : 0 );
			
			/* resolve entry */
			ep = dlsym( hdl, "fd_ext_init" );
			if (!ep) {
				TRACE_DEBUG(INFO, "No 'fd_ext_init' entry point in '%s': %s.", fullname, dlerror());
			}
			CHECK( 0, ep == NULL ? 1 : 0 );
			
			/* Done, now unload */
#ifndef SKIP_DLCLOSE
			CHECK( 0, dlclose(hdl) );
#endif /* SKIP_DLCLOSE */
		}
	}
	
	CHECK( 0, closedir(dir) );
	
	PASSTEST();
} 
	
