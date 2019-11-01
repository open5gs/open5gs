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
 * Configurable routing of messages for freeDiameter.
 */

#include "app_redir.h"

static struct fd_rt_fwd_hdl * ard_fwd_cb = NULL;

/* entry point */
static int ard_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	/* Parse the configuration file */
	CHECK_FCT( ard_conf_handle(conffile) );
	
	/* Resolve the dictionary objects we use */
	CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Redirect-Host", &avp_Redirect_Host, ENOENT ));
	CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Redirect-Host-Usage", &avp_Redirect_Host_Usage, ENOENT ));
	CHECK_FCT( fd_dict_search ( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Redirect-Max-Cache-Time", &avp_Redirect_Max_Cache_Time, ENOENT ));
	
	/* Register the proxy callback */
	CHECK_FCT( fd_rt_fwd_register(ard_rule_apply, NULL, RT_FWD_REQ, &ard_fwd_cb) );
	
	/* We're done */
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	
	/* Unregister the proxy callback */
	if (ard_fwd_cb) {
		CHECK_FCT_DO( fd_rt_fwd_unregister(ard_fwd_cb, NULL), );
	}
	
	/* Destroy all rules */
	while (!FD_IS_LIST_EMPTY(&ard_conf->rules)) {
		struct ard_rule * r = ard_conf->rules.next->o;
		fd_list_unlink(&r->chain);
		while (!FD_IS_LIST_EMPTY(&r->criteria)) {
			struct ard_criteria * c = r->criteria.next->o;
			fd_list_unlink(&c->chain);
			if (c->is_regex) {
				regfree(&c->preg);
			}
			free(c->s);
			free(c);
		}
		while (!FD_IS_LIST_EMPTY(&r->targets)) {
			struct ard_target * t = r->targets.next->o;
			fd_list_unlink(&t->chain);
			free(t->s);
			free(t);
		}
		free(r);
	}
	
	/* Done */
	return ;
}

EXTENSION_ENTRY("app_redirect", ard_entry);
