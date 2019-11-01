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

/* Install the dispatch callbacks */

#include "test_app.h"

static struct disp_hdl * ta_hdl_fb = NULL; /* handler for fallback cb */
static struct disp_hdl * ta_hdl_tr = NULL; /* handler for Test-Request req cb */

/* Default callback for the application. */
static int ta_fb_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act)
{
	/* This CB should never be called */
	TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
	
	fd_log_debug("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Test-Request messages */
static int ta_tr_cb( struct msg ** msg, struct avp * avp, struct session * sess, void * opaque, enum disp_action * act)
{
	struct msg *ans, *qry;
	struct avp * a;
	
	TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
	
	if (msg == NULL)
		return EINVAL;
	
	/* Value of Origin-Host */
	if (! (ta_conf->mode & MODE_BENCH)) {
		fprintf(stderr, "ECHO Test-Request received from ");
		CHECK_FCT( fd_msg_search_avp ( *msg, ta_origin_host, &a) );
		if (a) {
			struct avp_hdr * hdr;
			CHECK_FCT( fd_msg_avp_hdr( a, &hdr ) );
			fprintf(stderr, "'%.*s'", (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
		} else {
			fprintf(stderr, "no_Origin-Host");
		}
		fprintf(stderr, ", replying...\n");
	}
	
	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, msg, 0 ) );
	ans = *msg;
	
	/* Set the Test-AVP AVP */
	{
		struct avp * src = NULL;
		struct avp_hdr * hdr = NULL;
		
		CHECK_FCT( fd_msg_search_avp ( qry, ta_avp, &src) );
		CHECK_FCT( fd_msg_avp_hdr( src, &hdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( ta_avp, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, hdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	/* Set the Test-Payload-AVP AVP */
	if (ta_conf->long_avp_id) {
		struct avp * src = NULL;
		struct avp_hdr * hdr = NULL;
		
		CHECK_FCT( fd_msg_search_avp ( qry, ta_avp_long, &src) );
		CHECK_FCT( fd_msg_avp_hdr( src, &hdr )  );
		
		CHECK_FCT( fd_msg_avp_new ( ta_avp_long, 0, &avp ) );
		CHECK_FCT( fd_msg_avp_setvalue( avp, hdr->avp_value ) );
		CHECK_FCT( fd_msg_avp_add( ans, MSG_BRW_LAST_CHILD, avp ) );
	}
	
	
	/* Set the Origin-Host, Origin-Realm, Result-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set( ans, "DIAMETER_SUCCESS", NULL, NULL, 1 ) );
	
	/* Send the answer */
	CHECK_FCT( fd_msg_send( msg, NULL, NULL ) );
	
	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
	ta_conf->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );
	
	return 0;
}

int ta_serv_init(void)
{
	struct disp_when data;
	
	TRACE_DEBUG(FULL, "Initializing dispatch callbacks for test");
	
	memset(&data, 0, sizeof(data));
	data.app = ta_appli;
	data.command = ta_cmd_r;
	
	/* fallback CB if command != Test-Request received */
	CHECK_FCT( fd_disp_register( ta_fb_cb, DISP_HOW_APPID, &data, NULL, &ta_hdl_fb ) );
	
	/* Now specific handler for Test-Request */
	CHECK_FCT( fd_disp_register( ta_tr_cb, DISP_HOW_CC, &data, NULL, &ta_hdl_tr ) );
	
	return 0;
}

void ta_serv_fini(void)
{
	if (ta_hdl_fb) {
		(void) fd_disp_unregister(&ta_hdl_fb, NULL);
	}
	if (ta_hdl_tr) {
		(void) fd_disp_unregister(&ta_hdl_tr, NULL);
	}
	
	return;
}
