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

/* The array with all entries ordered by their data */
struct redir_line redirects_usages[H_U_MAX + 1];

/* Initialize the array */
int redir_entry_init()
{
	int i;
	
	TRACE_ENTRY("");
	
	/* redirects_usages */
	memset(&redirects_usages, 0, sizeof(redirects_usages));
	
	for (i = 0; i <= H_U_MAX; i++) {
		CHECK_POSIX( pthread_rwlock_init( &redirects_usages[i].lock, NULL) );
		fd_list_init( &redirects_usages[i].sentinel, &redirects_usages[i] );
	}
	
	/* initialize the scores */
	redirects_usages[ DONT_CACHE		].score = FD_SCORE_REDIR_ONCE;
	redirects_usages[ ALL_SESSION		].score = FD_SCORE_REDIR_SESSION;
	redirects_usages[ ALL_REALM		].score = FD_SCORE_REDIR_REALM;
	redirects_usages[ REALM_AND_APPLICATION	].score = FD_SCORE_REDIR_REALM_APP;
	redirects_usages[ ALL_APPLICATION	].score = FD_SCORE_REDIR_APP;
	redirects_usages[ ALL_HOST		].score = FD_SCORE_REDIR_HOST;
	redirects_usages[ ALL_USER		].score = FD_SCORE_REDIR_USER;

	return 0;
}


/* Create a new redir_entry and add the correct data */
int redir_entry_new(struct redir_entry ** e, struct fd_list * targets, uint32_t rhu, struct msg * qry, DiamId_t nh, size_t nhlen, os0_t oh, size_t ohlen)
{
	struct redir_entry * entry = NULL;
	os0_t s;
	size_t l;
	
	TRACE_ENTRY("%p %p %d %p %p %zd %p %zd", e, targets, rhu, qry, nh, nhlen, oh, ohlen)
	ASSERT(e && targets && (rhu <= H_U_MAX) && qry && nh && nhlen && oh && ohlen);
	
	CHECK_MALLOC( entry = malloc(sizeof(struct redir_entry)) );
	memset(entry, 0, sizeof(struct redir_entry));
	
	entry->eyec = REDIR_ENTRY_EYEC;
	
	CHECK_MALLOC( entry->from.s = os0dup(nh, nhlen) );
	entry->from.l = nhlen;
	
	fd_list_init(&entry->target_peers_list, entry);
	fd_list_move_end(&entry->target_peers_list, targets);
	
	fd_list_init(&entry->exp_list, entry);
	
	entry->type = rhu;
	fd_list_init(&entry->redir_list, entry);
	/* finally initialize the data */
	switch (rhu) {
		case DONT_CACHE:
			entry->data.message.msg = qry;
			break;
			
		case ALL_SESSION:
			{
				/* There is a good chance that the session is already cached in the message, so retrieve it */
				struct session * sess;
				CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, qry, &sess, NULL) );
				if (!sess) {
					TRACE_DEBUG(INFO, "Received a Redirect indication with usage ALL_SESSION but no Session-Id AVP in the message, defaulting to DONT_CACHE");
					entry->type = DONT_CACHE;
					entry->data.message.msg = qry;
					break;
				}
				CHECK_FCT( fd_sess_getsid(sess, &s, &l) );
				CHECK_MALLOC( entry->data.session.s = os0dup(s, l) );
				entry->data.session.l = l;
			}
			break;
		
		case ALL_REALM:
			{
				/* Search the Destination-Realm of the message */
				struct avp * dr;
				struct avp_hdr * ahdr;
				CHECK_FCT( fd_msg_search_avp(qry, redir_dict_dr, &dr) );
				if (!dr) {
					TRACE_DEBUG(INFO, "Received a Redirect indication with usage ALL_REALM but no Destination-Realm AVP in the message, defaulting to DONT_CACHE");
					entry->type = DONT_CACHE;
					entry->data.message.msg = qry;
					break;
				}
				CHECK_FCT(  fd_msg_avp_hdr( dr, &ahdr )  );
				CHECK_MALLOC( entry->data.realm.s = os0dup(ahdr->avp_value->os.data, ahdr->avp_value->os.len) );
				entry->data.realm.l = ahdr->avp_value->os.len;
			}
			break;
				
		case REALM_AND_APPLICATION:
			{
				/* Search the Destination-Realm of the message */
				struct avp * dr;
				struct avp_hdr * ahdr;
				CHECK_FCT( fd_msg_search_avp(qry, redir_dict_dr, &dr) );
				if (!dr) {
					TRACE_DEBUG(INFO, "Received a Redirect indication with usage REALM_AND_APPLICATION but no Destination-Realm AVP in the message, defaulting to DONT_CACHE");
					entry->type = DONT_CACHE;
					entry->data.message.msg = qry;
					break;
				}
				CHECK_FCT(  fd_msg_avp_hdr( dr, &ahdr )  );
				CHECK_MALLOC( entry->data.realm_app.s = os0dup(ahdr->avp_value->os.data, ahdr->avp_value->os.len) );
				entry->data.realm_app.l = ahdr->avp_value->os.len;
			}
			/* and then the application */
			{
				struct msg_hdr * hdr;
				CHECK_FCT( fd_msg_hdr(qry, &hdr) );
				entry->data.realm_app.a = hdr->msg_appl;
			}
			break;
				
		case ALL_APPLICATION:
			{
				struct msg_hdr * hdr;
				CHECK_FCT( fd_msg_hdr(qry, &hdr) );
				entry->data.app.a = hdr->msg_appl;
			}
			break;
			
		case ALL_HOST:
			CHECK_MALLOC( entry->data.host.s = os0dup(oh, ohlen) );
			entry->data.host.l = ohlen;
			break;
			
		case ALL_USER:
			{
				/* Search the User-Name of the message */
				struct avp * un;
				struct avp_hdr * ahdr;
				CHECK_FCT( fd_msg_search_avp(qry, redir_dict_un, &un) );
				if (!un) {
					TRACE_DEBUG(INFO, "Received a Redirect indication with usage ALL_USER but no User-Name AVP in the message, defaulting to DONT_CACHE");
					entry->type = DONT_CACHE;
					entry->data.message.msg = qry;
					break;
				}
				CHECK_FCT(  fd_msg_avp_hdr( un, &ahdr )  );
				CHECK_MALLOC( entry->data.user.s = os0dup(ahdr->avp_value->os.data, ahdr->avp_value->os.len) );
				entry->data.user.l = ahdr->avp_value->os.len;
			}
			break;
		
		default:
			ASSERT(0);
			return EINVAL;
	}
	
	/* We're done */
	*e = entry;
	return 0;
}


/* Compares two pointers (DONT_CACHE) */
static int compare_entries_ptr(union matchdata * d1, union matchdata * d2) {
	unsigned long v1 = (unsigned long) d1->message.msg;
	unsigned long v2 = (unsigned long) d2->message.msg;
	if (v1 > v2) 
		return 1;
	if (v1 < v2)
		return -1;
	return 0;
}
/* Compare two applications (REALM_AND_APPLICATION and ALL_APPLICATION) */
static int compare_entries_appl(union matchdata * d1, union matchdata * d2) {
	if (d1->app.a > d2->app.a) 
		return 1;
	if (d1->app.a < d2->app.a) 
		return -1;
	return 0;
}

/* Compare two strings (ALL_SESSION, ALL_REALM, ALL_HOST, ALL_USER) */
static int compare_entries_ostr(union matchdata * d1, union matchdata * d2) {
	return fd_os_cmp(d1->session.s, d1->session.l, d2->session.s, d2->session.l);
}

/* The array of callbacks */
int (*redir_entry_cmp_key[H_U_MAX +1])(union matchdata * , union matchdata * ) = {
	compare_entries_ptr,  /* DONT_CACHE */
	compare_entries_ostr, /* ALL_SESSION */
	compare_entries_ostr, /* ALL_REALM */
	compare_entries_appl, /* REALM_AND_APPLICATION */
	compare_entries_appl, /* ALL_APPLICATION */
	compare_entries_ostr, /* ALL_HOST */
	compare_entries_ostr  /* ALL_USER */
};

/* Link the newly created entry into the correct redirects_usages list. The mutex must be held */
int redir_entry_insert(struct redir_entry * e)
{
	struct fd_list * li;
	
	TRACE_ENTRY("%p", e);
	CHECK_PARAMS(e && (e->eyec == REDIR_ENTRY_EYEC));
	
	/* Write-Lock the line */
	CHECK_POSIX( pthread_rwlock_wrlock( RWLOCK_REDIR(e) ) );
	
	for (li = redirects_usages[e->type].sentinel.next; li != &redirects_usages[e->type].sentinel; li = li->next) {
		struct redir_entry * n = li->o;
		int cmp = redir_entry_cmp_key[e->type](&e->data, &n->data);
		if (cmp <= 0)
			break;
	}
	
	fd_list_insert_before(li, &e->redir_list);
	
	/* unLock the line */
	CHECK_POSIX( pthread_rwlock_unlock( RWLOCK_REDIR(e) ) );

	return 0;
}

/* Destroy -- the exp_peer_lock must be held when this function is called */
int redir_entry_destroy(struct redir_entry * e)
{
	TRACE_ENTRY("%p", e);
	CHECK_PARAMS(e && (e->eyec == REDIR_ENTRY_EYEC));
	
	/* If the entry is linked, lock the rwlock also */
	if (!FD_IS_LIST_EMPTY(&e->redir_list)) {
		CHECK_POSIX( pthread_rwlock_wrlock( RWLOCK_REDIR(e) ) );
		fd_list_unlink(&e->redir_list);
		CHECK_POSIX( pthread_rwlock_unlock( RWLOCK_REDIR(e) ) );
	}
	
	/* Now unlink from other list */
	fd_list_unlink(&e->exp_list);
	
	/* Empty the targets list */
	while (!FD_IS_LIST_EMPTY(&e->target_peers_list)) {
		struct redir_host * h = (struct redir_host *)e->target_peers_list.next->o;
		
		fd_list_unlink(&h->chain);
		free(h->id);
		free(h);
	}
	
	/* Now we can destroy the data safely */
	switch (e->type) {
		case DONT_CACHE:
			/* nothing special */
			break;
		case ALL_SESSION:
			free(e->data.session.s);
			break;
		case ALL_REALM:
			free(e->data.realm.s);
			break;
		case REALM_AND_APPLICATION:
			free(e->data.realm_app.s);
			break;
		case ALL_APPLICATION:
			break;
		case ALL_HOST:
			free(e->data.host.s);
			break;
		case ALL_USER:
			free(e->data.user.s);
			break;
		default:
			TRACE_DEBUG(INFO, "Invalid redirect type was saved");
			ASSERT(0);
			return EINVAL;
	}
	
	free(e->from.s);
	
	free(e);
	return 0;
}
