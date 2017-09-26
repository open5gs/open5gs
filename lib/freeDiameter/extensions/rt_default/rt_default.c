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
 * Configurable routing of messages for freeDiameter.
 */

#include "rt_default.h"

/* The callback called on new messages */
static int rtd_out(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct msg * msg = *pmsg;
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	
	CHECK_PARAMS(msg && candidates);
	
	/* Simply pass it to the appropriate function */
	if (FD_IS_LIST_EMPTY(candidates)) {
		return 0;
	} else {
		return rtd_process( msg, candidates );
	}
}

/* handler */
static struct fd_rt_out_hdl * rtd_hdl = NULL;

/* entry point */
static int rtd_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	/* Initialize the repo */
	CHECK_FCT( rtd_init() );
	
	/* Parse the configuration file */
	CHECK_FCT( rtd_conf_handle(conffile) );
	
#if 0
	/* Dump the rules */
	rtd_dump();
#endif /* 0 */
	
	/* Register the callback */
	CHECK_FCT( fd_rt_out_register( rtd_out, NULL, 5, &rtd_hdl ) );
	
	/* We're done */
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	
	/* Unregister the cb */
	CHECK_FCT_DO( fd_rt_out_unregister ( rtd_hdl, NULL ), /* continue */ );
	
	/* Destroy the data */
	rtd_fini();
	
	/* Done */
	return ;
}

EXTENSION_ENTRY("rt_default", rtd_entry);
