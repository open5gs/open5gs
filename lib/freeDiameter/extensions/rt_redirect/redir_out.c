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


/* Find the data pertinent to a type in the input data */
static int get_data_to_match(enum redir_h_u type, struct msg *msg, union matchdata * data, int * nodata)
{
	TRACE_ENTRY("%d %p %p %p", type, msg, data, nodata);
	
	/* Initialize the data area */
	memset(data, 0, sizeof(union matchdata));
	*nodata = 0;
	
	/* Now, find the appropriate information, depending on type */
	switch (type) {
		case DONT_CACHE:
			data->message.msg = msg;
			break;
			
		case ALL_SESSION:
			{
				/* Get the sid from the message */
				struct session * sess;
				CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, msg, &sess, NULL) );
				if (!sess) {
					TRACE_DEBUG(ANNOYING, "Message %p cannot match any ALL_SESSION rule since it does not have a Session-Id", msg);
					*nodata = 1;
				} else {
					CHECK_FCT( fd_sess_getsid(sess, &data->session.s, &data->session.l) );
				}
			}
			break;
		
		case ALL_REALM:
			{
				/* Search the Destination-Realm in the message */
				struct avp * dr;
				CHECK_FCT( fd_msg_search_avp(msg, redir_dict_dr, &dr) );
				if (!dr) {
					TRACE_DEBUG(ANNOYING, "Message %p cannot match any ALL_REALM rule since it does not have a Destination-Realm", msg);
					*nodata = 1;
				} else {
					struct avp_hdr * ahdr;
					CHECK_FCT(  fd_msg_avp_hdr( dr, &ahdr )  );
					data->realm.s = ahdr->avp_value->os.data;
					data->realm.l = ahdr->avp_value->os.len;
				}
			}
			break;
				
		case REALM_AND_APPLICATION:
			{
				/* Search the Destination-Realm of the message */
				struct avp * dr;
				CHECK_FCT( fd_msg_search_avp(msg, redir_dict_dr, &dr) );
				if (!dr) {
					TRACE_DEBUG(ANNOYING, "Message %p cannot match any REALM_AND_APPLICATION rule since it does not have a Destination-Realm", msg);
					*nodata = 1;
				} else {
					struct avp_hdr * ahdr;
					CHECK_FCT(  fd_msg_avp_hdr( dr, &ahdr )  );
					data->realm_app.s = ahdr->avp_value->os.data;
					data->realm_app.l = ahdr->avp_value->os.len;
					
					/* and then the application */
					{
						struct msg_hdr * hdr;
						CHECK_FCT( fd_msg_hdr(msg, &hdr) );
						data->realm_app.a = hdr->msg_appl;
						/* Should we forbid application 0? */
					}
				}
			}
			break;
				
		case ALL_APPLICATION:
			{
				/* Retrieve the application from the message */
				struct msg_hdr * hdr;
				CHECK_FCT( fd_msg_hdr(msg, &hdr) );
				data->app.a = hdr->msg_appl;
			}
			break;
			
		case ALL_HOST:
				/* This is more complex, we need to match with all candidates in each rule, it'll be done later */
			break;
			
		case ALL_USER:
			{
				/* Search the User-Name of the message */
				struct avp * un;
				CHECK_FCT( fd_msg_search_avp(msg, redir_dict_un, &un) );
				if (!un) {
					TRACE_DEBUG(ANNOYING, "Message %p cannot match any ALL_USER rule since it does not have a User-Name", msg);
					*nodata = 1;
				} else {
					struct avp_hdr * ahdr;
					CHECK_FCT(  fd_msg_avp_hdr( un, &ahdr )  );
					data->user.s = ahdr->avp_value->os.data;
					data->user.l = ahdr->avp_value->os.len;
				}
			}
			break;
		
		default:
			ASSERT(0);
			return EINVAL;
	}

	return 0;
}


/* Apply the score from a rule if the candidate list is appropriate */
static int apply_rule(struct redir_entry * e, struct msg * msg, struct fd_list * candidates)
{
	struct fd_list * lic, *lirh;
	struct rtd_candidate * c_oh = NULL;
	int cmp;
	
	TRACE_ENTRY("%p %p %p", e, msg, candidates);
	ASSERT( e && msg && candidates );
	
	if (FD_IS_LIST_EMPTY(candidates)) {
		TRACE_DEBUG(ANNOYING, "Skip Redirect rule since candidates list is empty");
		return 0;
	}
	
	/* Now search common peers between e->target_peers_list and candidates */
	TRACE_DEBUG(ANNOYING, "Message %p matches a Redirect rule (t:%d, @%p), processing candidates list", msg, e->type, e);
	
	/* First, decrease the score of the host that we received the previous Redirect from, in case it is in the list */
	for (lic = candidates->next; lic != candidates; lic = lic->next) {
		struct rtd_candidate * cand = (struct rtd_candidate *) lic;
		
		/* Special case: ALL_HOST rules: we decrease the score of the Origin-Host if present */
		if (e->type == ALL_HOST) {
			cmp = fd_os_almostcasesrch(cand->diamid, cand->diamidlen, e->data.host.s, e->data.host.l, NULL);
			if (!cmp) {
				TRACE_DEBUG(FULL, "Redirect msg %p: peer '%.*s' += %d (previous ALL_HOST Redirect originated from this peer)", msg, (int)cand->diamidlen, cand->diamid, FD_SCORE_SENT_REDIRECT);
				cand->score += FD_SCORE_SENT_REDIRECT;
				c_oh = cand;
				continue;
			}
		}
		
		cmp = fd_os_cmp(cand->diamid, cand->diamidlen, e->from.s, e->from.l);
		if (!cmp) {
			TRACE_DEBUG(FULL, "Redirect msg %p: peer '%.*s' += %d (previous Redirect received from this peer)", msg, (int)cand->diamidlen, cand->diamid, FD_SCORE_SENT_REDIRECT);
			cand->score += FD_SCORE_SENT_REDIRECT;
		}
		
	}
	
	if ((e->type == ALL_HOST) && (c_oh == NULL)) {
		/* The rule does not apply, we're done */
		return 0;
	}
	
	/* for each candidate, if it is found in the target_peers list, we add the rule's score to this candidate */
	for (lic = candidates->next; lic != candidates; lic = lic->next) {
		/* the candidates list is not guaranteed to be ordered at this time, so we cannot avoid the two imbricated loops */
		struct rtd_candidate * cand = (struct rtd_candidate *) lic;
		
		/* Is this candidate in the "Redirect-Host" list ? We must search caseinsentive here. */
		for (lirh = e->target_peers_list.next; lirh != &e->target_peers_list; lirh = lirh->next) {
			struct redir_host * host = lirh->o;
			int cont;
			
			cmp = fd_os_almostcasesrch( cand->diamid, cand->diamidlen, host->id, host->len, &cont );
			
			if (cmp == 0) {
				TRACE_DEBUG(FULL, "Redirect msg %p: peer '%.*s' += %d (rule t:%d @%p)", msg, (int)cand->diamidlen, cand->diamid, redirects_usages[e->type].score, e->type, e);
				cand->score += redirects_usages[e->type].score;
				break;
			}
			if (!cont)
				break;
		}
	}
	
	return 0;
}


/* OUT callback */
int redir_out_cb(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	int i, ret = 0;
	struct msg * msg = *pmsg;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	
	for (i = 0; i <= H_U_MAX; i++) {
		/* Lock the line. We write lock in case of DONT_CACHE so we can directly unlink the entry. read in other cases is sufficient */
		if (i == DONT_CACHE) {
			CHECK_POSIX( pthread_rwlock_wrlock( &redirects_usages[i].lock ) );
		} else {
			CHECK_POSIX( pthread_rwlock_rdlock( &redirects_usages[i].lock ) );
		}
		
		if (!FD_IS_LIST_EMPTY(&redirects_usages[i].sentinel)) {
			union matchdata data;
			int nodata; /* The message does not allow to apply this rule, skip */
			
			/* Retrieve the data that may match in the message */
			CHECK_FCT_DO( ret = get_data_to_match(i, msg, &data, &nodata), goto out );
			
			/* If this message may match some of our rules */
			if (!nodata) {
				struct fd_list * li;
				/* Attempt each rule we have stored */
				for (li = redirects_usages[i].sentinel.next; li != &redirects_usages[i].sentinel; li = li->next) {
					struct redir_entry * e = li->o;
					
					/* Does it match ? */
					if (i != ALL_HOST) { /* this one is an exception, we handle it separately */
						int cmp = redir_entry_cmp_key[i](&data, &e->data);
						if (cmp > 0)
							continue;
						if (cmp < 0)
							break;
					}
					
					/* This rule matches (or we are in ALL_HOST), apply */
					CHECK_FCT_DO( ret = apply_rule(e, msg, candidates), goto out );
					
					/* If this was a DONT_CACHE rule, we unlink it, so that it will not be used again */
					if (i == DONT_CACHE) {
						li=li->prev;
						fd_list_unlink( li->next );
						/* We cannot delete here without taking the mutex, which would mean we have first to release the lock... 
							just let expiry garbage collet the rule */
					}
				}
			}
		
		}
out:		
		CHECK_POSIX( pthread_rwlock_unlock( &redirects_usages[i].lock ) );
		if (ret)
			return ret;
	}
	
	return 0;
}

