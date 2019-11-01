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
 * Test-only extension for routing: gives a random positive score to any candidate peer.
 */
#include <freeDiameter/extension.h>
#include <stdlib.h>

/* The callback */
static int tra_out_cb(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct msg * msg = *pmsg;
	struct fd_list * li;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	
	for (li = candidates->next; li != candidates; li = li->next) {
		struct rtd_candidate *c = (struct rtd_candidate *) li;
		c->score = (int)lrand48();
	}
	
	return 0;
}

static struct fd_rt_out_hdl * out_hdl = NULL;

/* Register the callbacks to the daemon */
static int tra_main(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	CHECK_FCT( fd_rt_out_register ( tra_out_cb, NULL, 0 /* we call it late so that it replaces previous scores */, &out_hdl ) );
	return 0;
}

/* Cleanup the callbacks */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	CHECK_FCT_DO( fd_rt_out_unregister ( out_hdl, NULL ), /* continue */ );
	return ;
}

/* Define the entry point function */
EXTENSION_ENTRY("test_rt_any", tra_main);
