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

/* See doc/rt_busypeers.conf.sample for more details about the features of this extension */
#include "rtbusy.h"

/* The configuration structure */
struct rtbusy_conf rtbusy_conf;

static struct fd_rt_fwd_hdl * rt_busy_hdl = NULL;

static void rtbusy_expirecb(void * data, DiamId_t sentto, size_t senttolen, struct msg ** req);

/* The function that does the actual work */
int rt_busy_process_busy(struct msg ** pmsg, int is_req, DiamId_t sentto, size_t senttolen, union avp_value *oh)
{
	struct msg * qry = NULL;
	struct rt_data * rtd = NULL;
	struct fd_list * candidates = NULL;
	int sendingattemtps;
	int resend = 1;
	
	
	TRACE_ENTRY("%p(%p) %d %p %p", pmsg, pmsg?*pmsg:NULL, is_req, sentto, oh);
	
	if (is_req) {
		qry = *pmsg;
	} else {
		CHECK_FCT( fd_msg_answ_getq( *pmsg, &qry ) );
	}
	
	CHECK_FCT( fd_msg_rt_get ( qry, &rtd ) );
	ASSERT(rtd);
	
	/* rtd is the routing data associated with the query that was sent */
	
	/* Store the error in this routing data, this avoids sending the message to the same peer again */
	CHECK_FCT( fd_rtd_error_add(rtd, 
				    sentto, senttolen, 
				    (uint8_t *)(oh ? (DiamId_t)oh->os.data : fd_g_config->cnf_diamid), oh ? oh->os.len : fd_g_config->cnf_diamid_len , 
	                            ER_DIAMETER_TOO_BUSY, 
	                            &candidates, 
	                            &sendingattemtps) );
	
	/* Now we need to decide if we re-send this query to a different peer or return an error to upstream */
	
	/* First, are we exceeding the allowed attempts? */
	if (rtbusy_conf.RetryMaxPeers != 0) {
		if (sendingattemtps >= rtbusy_conf.RetryMaxPeers) {
			TRACE_DEBUG(FULL, "Maximum number of sending attempts reached for message %p, returning an error upstream", qry);
			resend = 0;
		}
	}
	
	if (resend) {
		/* Check if we have any valid candidate left for sending the message. This may not be 100% accurate but there should not be
		any situation where this is actually an issue. */
		if (FD_IS_LIST_EMPTY(candidates)) {
			resend = 0;
		} else {
			struct rtd_candidate * first = candidates->next->o;
			if (first->score < 0) /* No more candidate available */
				resend = 0;
		}
	}
	
	/* Ok, now act on the message, we know what to do */
	if (resend) {
		if (!is_req) {
			/* We must free the answer we received, and send the query again */
			CHECK_FCT( fd_msg_answ_detach(*pmsg) );
			CHECK_FCT( fd_msg_free(*pmsg) );
			*pmsg = qry;
		}
		/* Send the query again. We  need to re-associate the expirecb which was cleaned, if it is used */
		if (rtbusy_conf.RelayTimeout) {
			struct timespec expire;
			CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &expire)  );
			expire.tv_sec += rtbusy_conf.RelayTimeout/1000 + ((expire.tv_nsec + (1000000LL * (rtbusy_conf.RelayTimeout % 1000))) / 1000000000LL);
			expire.tv_nsec = (expire.tv_nsec + (1000000LL * (rtbusy_conf.RelayTimeout % 1000))) % 1000000000LL;
			CHECK_FCT( fd_msg_send_timeout( pmsg, NULL, NULL, rtbusy_expirecb, &expire ) );
		} else {
			CHECK_FCT( fd_msg_send(pmsg, NULL, NULL) );
		}
	
	} else {
		if (is_req) {
			/* We must create an answer */
			CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, pmsg, MSGFL_ANSW_ERROR ) );
			
			CHECK_FCT( fd_msg_rescode_set(*pmsg, "DIAMETER_TOO_BUSY", "[rt_busypeers] Timeout reached while waiting for an answer", NULL, 1 ) );
		
			CHECK_FCT( fd_msg_send(pmsg, NULL, NULL) );
		}
		/* Otherwise, we have nothing to do at all, the answer will be forwarded upstream as part of the normal processing */
	
	}
	
	return 0;
}

/* Callback called on expiry of the timeout timer */
static void rtbusy_expirecb(void * data, DiamId_t sentto, size_t senttolen, struct msg ** preq)
{
	CHECK_FCT_DO( rt_busy_process_busy(preq, 1, sentto, senttolen, NULL), /* continue */ );
}

/* the routing callback that handles all the tasks of this extension */
static int rtbusy_fwd_cb(void * cbdata, struct msg ** pmsg)
{
	struct msg_hdr * hdr;
	struct avp * avp;
	union avp_value *a_rc = NULL, *a_oh = NULL;
	DiamId_t sentto = NULL;
	size_t senttolen;
	
	/* Get the header of the message */
	CHECK_FCT( fd_msg_hdr(*pmsg, &hdr) );
	
	/* If the message is a request, we only associate the timeout */
	if (hdr->msg_flags & CMD_FLAG_REQUEST) {
		struct timespec expire;
		CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &expire)  );
		expire.tv_sec += rtbusy_conf.RelayTimeout/1000 + ((expire.tv_nsec + (1000000LL * (rtbusy_conf.RelayTimeout % 1000))) / 1000000000LL);
		expire.tv_nsec = (expire.tv_nsec + (1000000LL * (rtbusy_conf.RelayTimeout % 1000))) % 1000000000LL;
		CHECK_FCT( fd_msg_anscb_associate( *pmsg, NULL, NULL, rtbusy_expirecb, &expire ) );
		return 0;
	}
	
	/* From this point, the message is an answer; we need to check if the E flag is set and if the Result-Code is DIAMETER_TOO_BUSY */
	
	if (!(hdr->msg_flags & CMD_FLAG_ERROR)) {
		/* This answer does not have the E flag, no need to process further */
		return 0;
	}
	
	CHECK_FCT( fd_msg_source_get( *pmsg, &sentto, &senttolen ) );
	
	/* Now get the AVPs we are interested in */
	CHECK_FCT(  fd_msg_browse(*pmsg, MSG_BRW_FIRST_CHILD, &avp, NULL)  );
	while (avp) {
		struct avp_hdr * ahdr;
			
		CHECK_FCT(  fd_msg_avp_hdr( avp, &ahdr )  );
		if (! (ahdr->avp_flags & AVP_FLAG_VENDOR)) {
			switch (ahdr->avp_code) {
				case AC_ORIGIN_HOST:
					/* Parse this AVP */
					CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
					ASSERT( ahdr->avp_value );
					a_oh = ahdr->avp_value;
					break;
					
				case AC_RESULT_CODE:
					/* Parse this AVP */
					CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
					ASSERT( ahdr->avp_value );
					a_rc = ahdr->avp_value;
					
					if (a_rc->u32 != ER_DIAMETER_TOO_BUSY) {
						/* It is not a TOO_BUSY error, we don't do anything */
						goto out;
					}
					break;
			}
			
			if (a_rc && a_oh)
				break;
		}

		/* Go to next AVP */
		CHECK_FCT(  fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL)  );
	}
	
	/* Check we have received the necessary information */
	if (!a_rc) {
		TRACE_ERROR( "Invalid Diameter answer without a Result-Code AVP, rt_busypeer module gave up processing");
		goto out;
	}
	
	if (!a_oh) {
		TRACE_ERROR( "Invalid Diameter answer without an Origin-Host AVP, rt_busypeer module gave up processing");
		goto out;
	}
	
	/* Pass this error to the function that processes BUSY status */
	CHECK_FCT( rt_busy_process_busy(pmsg, 0, sentto, senttolen, a_oh) );
	
out:
	return 0;
}


/* entry point */
static int rtbusy_entry(char * conffile)
{
	enum fd_rt_fwd_dir dir = RT_FWD_ANS;
	TRACE_ENTRY("%p", conffile);
	
	/* Initialize the configuration */
	memset(&rtbusy_conf, 0, sizeof(rtbusy_conf));
	
	/* Parse the configuration file */
	CHECK_FCT( rtbusy_conf_handle(conffile) );
	
	if (rtbusy_conf.SkipTooBusyErrors && !rtbusy_conf.RelayTimeout) {
		TRACE_NOTICE("[rt_busypeers] Configuration file does not specify any behavior (no effect)!");
		return 0;
	}
	
	if (rtbusy_conf.SkipTooBusyErrors)
		dir = RT_FWD_REQ; /* in this case, RelayTimeout is not 0 */
	else if (rtbusy_conf.RelayTimeout)
		dir = RT_FWD_ALL;
	
	/* Register the callback */
	CHECK_FCT( fd_rt_fwd_register ( rtbusy_fwd_cb, NULL, dir, &rt_busy_hdl ) );
	
	/* We're done */
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	
	/* Unregister the cb */
	if (rt_busy_hdl) 
		CHECK_FCT_DO( fd_rt_fwd_unregister ( rt_busy_hdl, NULL), /* continue */);
	
	/* Done */
	return ;
}

EXTENSION_ENTRY("rt_busypeers", rtbusy_entry);
