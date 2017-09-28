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
/* 
 * Main file of the app_radgw extension.
 */

#include "rgw.h"

/* Extension entry point called by freeDiameter */
static int rgw_main(char * conffile) 
{
	CHECK_FCT( rgw_clients_init() );
	
	CHECK_FCT( rgw_servers_init() );
	
	CHECK_FCT( rgw_conf_handle(conffile) );
	
	LOG_D( "Extension RADIUS Gateway initialized with configuration: '%s'", conffile);
	rgw_servers_dump();
	rgw_clients_dump();
	rgw_plg_dump();
	
	/* Start making extension list accelerators */
	rgw_plg_start_cache();
	
	/* Start the worker threads */
	CHECK_FCT( rgw_work_start() );
	
	/* Start the servers */
	CHECK_FCT( rgw_servers_start() );
	
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	rgw_servers_fini();
	rgw_work_fini();
	rgw_plg_fini();
	rgw_clients_fini();
}

EXTENSION_ENTRY("app_radgw", rgw_main);
