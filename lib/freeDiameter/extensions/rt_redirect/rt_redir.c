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

#include "rt_redir.h"

struct dict_object * redir_dict_dr = NULL;
struct dict_object * redir_dict_un = NULL;

static struct fd_rt_fwd_hdl * fwd_hdl = NULL;
static struct fd_rt_out_hdl * out_hdl = NULL;
static pthread_t exp_thr  = (pthread_t)NULL;

/* Initialize the module */
static int redir_entry(char * conffile)
{
	TRACE_ENTRY("");
	
	/* Dictionary objects */
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Destination-Realm", &redir_dict_dr, ENOENT)  );
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Name", &redir_dict_un, ENOENT)  );

	/* Initialize the entries array */
	CHECK_FCT( redir_entry_init() );
	
	/* Start the expire thread */
	CHECK_POSIX( pthread_create( &exp_thr, NULL, redir_exp_thr_fct, NULL ) );
	
	/* Register the callback that receives the answers and processes when it contains a Redirect indication. */
	CHECK_FCT( fd_rt_fwd_register ( redir_fwd_cb, NULL, RT_FWD_ANS, &fwd_hdl ) );

	/* Register the callback that applies the saved Redirect rules to outgoing messages. */
	CHECK_FCT( fd_rt_out_register ( redir_out_cb, NULL, 10, &out_hdl ) );

	return 0;
}

EXTENSION_ENTRY("rt_redirect", redir_entry);

/* And terminate it */
void fd_ext_fini(void)
{
	int i;
	
	/* Unregister the callbacks */
	if (fwd_hdl) {
		CHECK_FCT_DO( fd_rt_fwd_unregister(fwd_hdl, NULL), );
	}
	if (out_hdl) {
		CHECK_FCT_DO( fd_rt_out_unregister(out_hdl, NULL), );
	}
	
	/* Stop the expiry thread */
	CHECK_FCT_DO( fd_thr_term(&exp_thr), );
	
	/* Empty all entries */
	CHECK_POSIX_DO( pthread_mutex_lock(&redir_exp_peer_lock),   );
	for (i = 0; i <= H_U_MAX; i++) {
		CHECK_POSIX_DO( pthread_rwlock_wrlock( &redirects_usages[i].lock), );
		while (!FD_IS_LIST_EMPTY(&redirects_usages[i].sentinel)) {
			struct redir_entry * e = redirects_usages[i].sentinel.next->o;
			fd_list_unlink(&e->redir_list);
			CHECK_FCT_DO( redir_entry_destroy(e), );
		}
		CHECK_POSIX_DO( pthread_rwlock_unlock( &redirects_usages[i].lock), );
		CHECK_POSIX_DO( pthread_rwlock_destroy( &redirects_usages[i].lock), );
	}
	CHECK_POSIX_DO( pthread_mutex_unlock(&redir_exp_peer_lock),   );
	
	return;
}
