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

/* 
 * This extension provides a simple Diameter network emulator mechanism.
 * It allows the introduction of delays and duplicates of Diameter messages.
 * See test_netemul.conf.sample file for the format of the configuration file.
 */

#include "test_netemul.h"

/* The configuration structure */
struct tne_conf tne_conf;



/* Proxying callback */
static int tne_fwd_cb(void * cbdata, struct msg ** msg)
{
	struct msg * m;
	
	TRACE_ENTRY("%p %p", cbdata, msg);
	
	m = *msg;
	*msg = NULL;
	
	TRACE_DEBUG(FULL, "[tne] Processing message %p", m);
	CHECK_FCT( tne_process_message(m) );
	
	return 0;
}


static struct fd_rt_fwd_hdl * fwd_hdl = NULL;

/* Register the callbacks to the daemon */
static int tne_main(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	/* Initialize the configuration */
	memset(&tne_conf, 0, sizeof(tne_conf));
	tne_conf.lat_avg = 500;
	tne_conf.lat_dev = 20;
	tne_conf.dupl_proba = 1E-2;
	
	/* Parse the configuration file */
	CHECK_FCT( tne_conf_handle(conffile) );
	
	/* Initialize the process module */
	CHECK_FCT( tne_process_init() );
	
	/* Now, register the proxying callback */
	CHECK_FCT( fd_rt_fwd_register ( tne_fwd_cb, NULL, RT_FWD_REQ, &fwd_hdl ) );

	return 0;
}

/* Cleanup the callbacks */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	
	/* Unregister the module */
	CHECK_FCT_DO( fd_rt_fwd_unregister ( fwd_hdl, NULL ), /* continue */ );
	
	/* Destroy the process thread */
	CHECK_FCT_DO( tne_process_fini (  ), /* continue */ );
	
	return ;
}

/* Define the entry point function */
EXTENSION_ENTRY("test_netemul", tne_main);
