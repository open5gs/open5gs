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

#include "rt_redir.h"

/* This structure contains the data to keep when a new peer's connection is attempted (for later) */
struct redir_task {
	struct msg * answer; /* the message that was being processed */
	uint32_t     rhu;    /* Redirect-Host-Usage value */
	uint32_t     rmct;   /* Redirect-Max-Cache-Time value */
	struct fd_list rh;   /* the list of Redirect-Hosts */
};

/* Received answers FWD callback */
int redir_fwd_cb(void * cbdata, struct msg ** msg)
{
	struct msg * m, * q;
	struct rt_data *rtd;
	struct msg_hdr * hdr;
	union avp_value *a_rc = NULL, *a_rhu = NULL, *a_rmct = NULL, *a_oh = NULL;
	int known = 0, actives = 0;
	struct fd_list * li;
	struct avp * avp;
	struct redir_task task = { .answer = NULL, .rhu = 0, .rmct = 0, .rh = FD_LIST_INITIALIZER(task.rh) };
	DiamId_t nh;
	size_t   nhlen;
	int nbrh = 0;
	struct redir_entry * entry;
	
	TRACE_ENTRY("%p %p", cbdata, msg);
	
	CHECK_PARAMS(msg && *msg);
	
	m = *msg;
	
	/* First get the header */
	CHECK_FCT( fd_msg_hdr(m, &hdr) );
	
	/* Check if we have an error */
	ASSERT(!(hdr->msg_flags & CMD_FLAG_REQUEST));
	if (!(hdr->msg_flags & CMD_FLAG_ERROR)) {
		/* This answer does not have the E flag, no need to process further */
		return 0;
	}
	
	/* Now get the AVPs we are interested in */
	CHECK_FCT(  fd_msg_browse(m, MSG_BRW_FIRST_CHILD, &avp, NULL)  );
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
					
					if (a_rc->u32 != ER_DIAMETER_REDIRECT_INDICATION) {
						/* It is not a REDIRECT error, we don't do anything */
						goto out;
					}
					break;
					
				case AC_REDIRECT_HOST:
					{
						struct redir_host * h = NULL;
						DiamId_t id = NULL;
						size_t	 len = 0;
						int 	 secure = 0;
						uint16_t port = 0;
						int	 l4 = 0;
						char	 proto = 0;
						
						/* Parse this AVP */
						CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
						ASSERT( ahdr->avp_value );
						
						nbrh++;
						
						CHECK_FCT_DO( fd_os_parse_DiameterURI(ahdr->avp_value->os.data, ahdr->avp_value->os.len, 
									&id, &len, &secure, &port, &l4, &proto),
							{
								TRACE_DEBUG(INFO, "Received an invalid Redirect-Host AVP value ('%.*s'), ignored", (int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
								break;
							} );
						
						/* Now check if the transport & protocol are supported */
						if (proto && (proto != 'd')) {
							TRACE_DEBUG(FULL, "Ignored unsupported non-Diameter Redirect-Host AVP (%.*s)", (int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
							free(id);
							break;
						}
						if (l4 && (l4 == IPPROTO_UDP)) {
							TRACE_DEBUG(FULL, "Ignored unsupported UDP Redirect-Host AVP (%.*s)", (int)ahdr->avp_value->os.len, ahdr->avp_value->os.data);
							free(id);
							break;
						}
						
						/* It looks OK, save this entry. */
						
						CHECK_MALLOC( h = malloc(sizeof(struct redir_host)) );
						memset(h, 0, sizeof(struct redir_host));
						fd_list_init(&h->chain, h);
						h->id = id;
						h->len = len;
						/* later: secure, port */
						
						/* The list is kept ordered by id so that it is faster to compare to candidates later */
						for (li = task.rh.next; li != &task.rh; li = li->next) {
							struct redir_host * nhost = li->o;
							if ( fd_os_cmp(id, len, nhost->id, nhost->len) <= 0 )
								break;
						}
						fd_list_insert_before(li, &h->chain);
					}
					break;

				case AC_REDIRECT_HOST_USAGE:
					/* Parse this AVP */
					CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
					ASSERT( ahdr->avp_value );
					a_rhu = ahdr->avp_value;
					if (a_rhu->u32 > H_U_MAX) {
						TRACE_DEBUG(INFO, "Received unsupported Redirect-Host-Usage value (%d), defaulting to DONT_CACHE", a_rhu->u32);
					} else {
						task.rhu = a_rhu->u32;
					}
					break;

				case AC_REDIRECT_MAX_CACHE_TIME:
					/* Parse this AVP */
					CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
					ASSERT( ahdr->avp_value );
					a_rmct = ahdr->avp_value;
					task.rmct = a_rmct->u32;
					break;

			}
		}

		/* Go to next AVP */
		CHECK_FCT(  fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL)  );
	}
	
	/* Check we have received the necessary information */
	if (!a_rc) {
		TRACE_DEBUG(FULL, "Invalid Diameter answer without a Result-Code AVP, Redirect module gave up");
		goto out;
	}
	
	if (!a_oh) {
		TRACE_DEBUG(FULL, "Invalid Diameter answer without an Origin-Host AVP, Redirect module gave up");
		goto out;
	}
	
	if (FD_IS_LIST_EMPTY(&task.rh)) {
		TRACE_DEBUG(FULL, "Diameter answer with a DIAMETER_REDIRECT_INDICATION Result-Code AVP but no valid/supported Redirect-Host AVP, Redirect module gave up");
		goto out;
	}

	if (a_rhu && (task.rhu != DONT_CACHE) && !a_rmct) {
		TRACE_DEBUG(FULL, "Invalid Diameter Redirect answer with a Redirect-Host-Usage AVP but no Redirect-Max-Cache-Time, Redirect module gave up");
		goto out;
	}
	
	/* It looks like we can process the Redirect indication */
	
	/* Search for the peers we already know */
	for (li = task.rh.next; li != &task.rh; li = li->next) {
		struct redir_host * h = li->o;
		struct peer_hdr * peer;
		
		CHECK_FCT( fd_peer_getbyid( h->id, h->len, 1, &peer ) );
		if (peer) {
			known ++;
			memcpy(h->id, peer->info.pi_diamid, h->len); /* Overwrite the case so we can search case-sensitive from here on */
			if (fd_peer_get_state(peer) == STATE_OPEN) {
				actives ++;
			}
		}
	}
	
	TRACE_DEBUG(FULL, "Redirect module: received %d Redirect-Hosts, %d are known peers, %d have an OPEN connection", nbrh, known, actives);
	
	/* in this version, we only redirect when there are known active peers. TODO: add new peers via fd_peer_add when no active peer is available */
	
	if (!actives) {
		TRACE_DEBUG(INFO, "Unable to comply to Redirect indication: none of the peers included is in OPEN state");
		goto out;
	}
	
	/* From this point, we will re-send the query to a different peer, so stop forwarding the answer here */
	*msg = NULL;
	
	/* Get the query's routing data & add the new error */
	CHECK_FCT( fd_msg_answ_getq(m, &q) );
	CHECK_FCT( fd_msg_rt_get(q, &rtd) );
	CHECK_FCT( fd_msg_source_get( m, &nh, &nhlen ) );
	CHECK_FCT( fd_rtd_error_add(rtd, nh, nhlen, a_oh->os.data, a_oh->os.len, a_rc->u32, NULL, NULL) );
	
	/* Create a redir_rule  */
	CHECK_FCT( redir_entry_new(&entry, &task.rh, task.rhu, q, nh, nhlen, a_oh->os.data, a_oh->os.len) );
		
	CHECK_POSIX(  pthread_mutex_lock(&redir_exp_peer_lock)  );
	/* Insert in the split list */
	CHECK_FCT( redir_entry_insert(entry) );
	/* Set the expiry */
	CHECK_FCT( redir_exp_set(entry, task.rmct ?: DEFAULT_EXPIRE_TIME) );
	CHECK_POSIX(  pthread_mutex_unlock(&redir_exp_peer_lock)  );

	/* Now we can get rid of the received answer and send again the query. */
	CHECK_FCT( fd_msg_answ_detach(m) );
	CHECK_FCT( fd_msg_free(m) );
	
	/* Send it */
	CHECK_FCT( fd_msg_send(&q, NULL, NULL) );
	
	/* Done! */
	
out:
	while (!FD_IS_LIST_EMPTY(&task.rh)) {
		struct redir_host * h = task.rh.next->o;
		fd_list_unlink(&h->chain);
		free(h->id);
		free(h);
	}
		
	return 0;

}

