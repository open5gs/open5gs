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
 * Debug-only extension for routing module; 
 * displays state information at the end of routing information process.
 */
#include <freeDiameter/extension.h>

static struct fd_rt_fwd_hdl * fwd_hdl = NULL;
static struct fd_rt_out_hdl * out_hdl = NULL;

/* Proxying debug callback */
static int dbgrt_fwd_cb(void * cbdata, struct msg ** msg)
{
	char * buf = NULL; size_t buflen;
	TRACE_ENTRY("%p %p", cbdata, msg);
	
	LOG_D("[dbg_rt] FWD routing message: %p", msg ? *msg : NULL);
	if (msg) {
		CHECK_MALLOC( fd_msg_dump_treeview(&buf, &buflen, NULL, *msg, NULL, 0, 1) );
		LOG_D("%s", buf);
	}
	free(buf);
	return 0;
}

/* Path selection debug callback */
static int dbgrt_out_cb(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct fd_list * li;
	struct msg * msg = *pmsg;
	char * buf = NULL; size_t buflen;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	
	LOG_D("[dbg_rt] OUT routing message: %p", msg);
	CHECK_MALLOC( fd_msg_dump_treeview(&buf, &buflen, NULL, msg, NULL, 0, 1) );
	LOG_D("%s", buf);
	LOG_D("[dbg_rt] Current list of candidates (%p): (score - id)", msg);
	
	for (li = candidates->next; li != candidates; li = li->next) {
		struct rtd_candidate *c = (struct rtd_candidate *) li;
		LOG_D("[dbg_rt]   %d -\t%s", c->score, c->diamid);
	}
	
	return 0;
}

/* Register the callbacks to the daemon */
static int dbgrt_main(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	CHECK_FCT( fd_rt_fwd_register ( dbgrt_fwd_cb, NULL, RT_FWD_ALL, &fwd_hdl ) );
	CHECK_FCT( fd_rt_out_register ( dbgrt_out_cb, NULL, -1 /* so that it is called late */, &out_hdl ) );

	return 0;
}

/* Cleanup the callbacks */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	
	/* Unregister the modules */
	CHECK_FCT_DO( fd_rt_fwd_unregister ( fwd_hdl, NULL ), /* continue */ );
	CHECK_FCT_DO( fd_rt_out_unregister ( out_hdl, NULL ), /* continue */ );
	
	return ;
}

/* Define the entry point function */
EXTENSION_ENTRY("dbg_rt", dbgrt_main);
