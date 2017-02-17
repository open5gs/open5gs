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

#include "fdcore-internal.h"

/********************************************************************************/
/*              First part : handling the extensions callbacks                  */
/********************************************************************************/

/* Lists of the callbacks, and locks to protect them */
static pthread_rwlock_t rt_fwd_lock = PTHREAD_RWLOCK_INITIALIZER;
static struct fd_list 	rt_fwd_list = FD_LIST_INITIALIZER_O(rt_fwd_list, &rt_fwd_lock);

static pthread_rwlock_t rt_out_lock = PTHREAD_RWLOCK_INITIALIZER;
static struct fd_list 	rt_out_list = FD_LIST_INITIALIZER_O(rt_out_list, &rt_out_lock);

/* Items in the lists are the same */
struct rt_hdl {
	struct fd_list	chain;	/* link in the rt_fwd_list or rt_out_list */
	void *		cbdata;	/* the registered data */
	union {
		int	order;	/* This value is used to sort the list */
		int 	dir;	/* It is the direction for FWD handlers */
		int	prio;	/* and the priority for OUT handlers */
	};
	union {
		int (*rt_fwd_cb)(void * cbdata, struct msg ** msg);
		int (*rt_out_cb)(void * cbdata, struct msg ** msg, struct fd_list * candidates);
	};
};	

/* Add a new entry in the list */
static int add_ordered(struct rt_hdl * new, struct fd_list * list)
{
	/* The list is ordered by prio parameter */
	struct fd_list * li;
	
	CHECK_POSIX( pthread_rwlock_wrlock(list->o) );
	
	for (li = list->next; li != list; li = li->next) {
		struct rt_hdl * h = (struct rt_hdl *) li;
		if (new->order <= h->order)
			break;
	}
	
	fd_list_insert_before(li, &new->chain);
	
	CHECK_POSIX( pthread_rwlock_unlock(list->o) );
	
	return 0;
}

/* Register a new FWD callback */
int fd_rt_fwd_register ( int (*rt_fwd_cb)(void * cbdata, struct msg ** msg), void * cbdata, enum fd_rt_fwd_dir dir, struct fd_rt_fwd_hdl ** handler )
{
	struct rt_hdl * new;
	
	TRACE_ENTRY("%p %p %d %p", rt_fwd_cb, cbdata, dir, handler);
	CHECK_PARAMS( rt_fwd_cb );
	CHECK_PARAMS( (dir >= RT_FWD_REQ) && ( dir <= RT_FWD_ANS) );
	
	/* Create a new container */
	CHECK_MALLOC(new = malloc(sizeof(struct rt_hdl)));
	memset(new, 0, sizeof(struct rt_hdl));
	
	/* Write the content */
	fd_list_init(&new->chain, NULL);
	new->cbdata 	= cbdata;
	new->dir    	= dir;
	new->rt_fwd_cb 	= rt_fwd_cb;
	
	/* Save this in the list */
	CHECK_FCT( add_ordered(new, &rt_fwd_list) );
	
	/* Give it back to the extension if needed */
	if (handler)
		*handler = (void *)new;
	
	return 0;
}

/* Remove it */
int fd_rt_fwd_unregister ( struct fd_rt_fwd_hdl * handler, void ** cbdata )
{
	struct rt_hdl * del;
	TRACE_ENTRY( "%p %p", handler, cbdata);
	CHECK_PARAMS( handler );
	
	del = (struct rt_hdl *)handler;
	CHECK_PARAMS( del->chain.head == &rt_fwd_list );
	
	/* Unlink */
	CHECK_POSIX( pthread_rwlock_wrlock(&rt_fwd_lock) );
	fd_list_unlink(&del->chain);
	CHECK_POSIX( pthread_rwlock_unlock(&rt_fwd_lock) );
	
	if (cbdata)
		*cbdata = del->cbdata;
	
	free(del);
	return 0;
}

/* Register a new OUT callback */
int fd_rt_out_register ( int (*rt_out_cb)(void * cbdata, struct msg ** pmsg, struct fd_list * candidates), void * cbdata, int priority, struct fd_rt_out_hdl ** handler )
{
	struct rt_hdl * new;
	
	TRACE_ENTRY("%p %p %d %p", rt_out_cb, cbdata, priority, handler);
	CHECK_PARAMS( rt_out_cb );
	
	/* Create a new container */
	CHECK_MALLOC(new = malloc(sizeof(struct rt_hdl)));
	memset(new, 0, sizeof(struct rt_hdl));
	
	/* Write the content */
	fd_list_init(&new->chain, NULL);
	new->cbdata 	= cbdata;
	new->prio    	= priority;
	new->rt_out_cb 	= rt_out_cb;
	
	/* Save this in the list */
	CHECK_FCT( add_ordered(new, &rt_out_list) );
	
	/* Give it back to the extension if needed */
	if (handler)
		*handler = (void *)new;
	
	return 0;
}

/* Remove it */
int fd_rt_out_unregister ( struct fd_rt_out_hdl * handler, void ** cbdata )
{
	struct rt_hdl * del;
	TRACE_ENTRY( "%p %p", handler, cbdata);
	CHECK_PARAMS( handler );
	
	del = (struct rt_hdl *)handler;
	CHECK_PARAMS( del->chain.head == &rt_out_list );
	
	/* Unlink */
	CHECK_POSIX( pthread_rwlock_wrlock(&rt_out_lock) );
	fd_list_unlink(&del->chain);
	CHECK_POSIX( pthread_rwlock_unlock(&rt_out_lock) );
	
	if (cbdata)
		*cbdata = del->cbdata;
	
	free(del);
	return 0;
}

/********************************************************************************/
/*                      Some default OUT routing callbacks                      */
/********************************************************************************/

/* Prevent sending to peers that do not support the message application */
static int dont_send_if_no_common_app(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct msg * msg = *pmsg;
	struct fd_list * li;
	struct msg_hdr * hdr;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	CHECK_PARAMS(msg && candidates);
	
	CHECK_FCT( fd_msg_hdr(msg, &hdr) );
	
	/* For Base Diameter Protocol, every peer is supposed to support it, so skip */
	if (hdr->msg_appl == 0)
		return 0;
	
	/* Otherwise, check that the peers support the application */
	for (li = candidates->next; li != candidates; li = li->next) {
		struct rtd_candidate *c = (struct rtd_candidate *) li;
		struct fd_peer * peer;
		struct fd_app *found;
		CHECK_FCT( fd_peer_getbyid( c->diamid, c->diamidlen, 0, (void *)&peer ) );
		if (peer && !peer->p_hdr.info.runtime.pir_relay) {
			/* Check if the remote peer advertised the message's appli */
			CHECK_FCT( fd_app_check(&peer->p_hdr.info.runtime.pir_apps, hdr->msg_appl, &found) );
			if (!found)
				c->score += FD_SCORE_NO_DELIVERY;
		}
	}

	return 0;
}

/* Detect if the Destination-Host and Destination-Realm match the peer */
static int score_destination_avp(void * cbdata, struct msg ** pmsg, struct fd_list * candidates)
{
	struct msg * msg = *pmsg;
	struct fd_list * li;
	struct avp * avp;
	union avp_value *dh = NULL, *dr = NULL;
	
	TRACE_ENTRY("%p %p %p", cbdata, msg, candidates);
	CHECK_PARAMS(msg && candidates);
	
	/* Search the Destination-Host and Destination-Realm AVPs -- we could also use fd_msg_search_avp here, but this one is slightly more efficient */
	CHECK_FCT(  fd_msg_browse(msg, MSG_BRW_FIRST_CHILD, &avp, NULL) );
	while (avp) {
		struct avp_hdr * ahdr;
		CHECK_FCT(  fd_msg_avp_hdr( avp, &ahdr ) );

		if (! (ahdr->avp_flags & AVP_FLAG_VENDOR)) {
			switch (ahdr->avp_code) {
				case AC_DESTINATION_HOST:
					/* Parse this AVP */
					CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
					ASSERT( ahdr->avp_value );
					dh = ahdr->avp_value;
					break;

				case AC_DESTINATION_REALM:
					/* Parse this AVP */
					CHECK_FCT( fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, NULL ) );
					ASSERT( ahdr->avp_value );
					dr = ahdr->avp_value;
					break;
			}
		}

		if (dh && dr)
			break;

		/* Go to next AVP */
		CHECK_FCT(  fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL) );
	}
	
	/* Now, check each candidate against these AVP values */
	for (li = candidates->next; li != candidates; li = li->next) {
		struct rtd_candidate *c = (struct rtd_candidate *) li;
		
	    #if 0 /* this is actually useless since the sending process will also ensure that the peer is still available */
		struct fd_peer * peer;
		/* Since the candidates list comes from the peers list, we do not have any issue with upper/lower case to find the peer object */
		CHECK_FCT( fd_peer_getbyid( c->diamid, c->diamidlen, 0, (void *)&peer ) );
		if (!peer)
			continue; /* it has been deleted since the candidate list was generated; avoid sending to this one in that case. */
	    #endif /* 0 */
		
		/* In the AVPs, the value comes from the network, so let's be case permissive */
		if (dh && !fd_os_almostcasesrch(dh->os.data, dh->os.len, c->diamid, c->diamidlen, NULL) ) {
			/* The candidate is the Destination-Host */
			c->score += FD_SCORE_FINALDEST;
		} else {
			if (dr && !fd_os_almostcasesrch(dr->os.data, dr->os.len, c->realm, c->realmlen, NULL) ) {
				/* The candidate's realm matchs the Destination-Realm */
				c->score += FD_SCORE_REALM;
			}
		}
	}

	return 0;
}

/********************************************************************************/
/*                        Helper functions                                      */
/********************************************************************************/

/* Find (first) '!' and '@' positions in a UTF-8 encoded string (User-Name AVP value) */
static void nai_get_indexes(union avp_value * un, int * excl_idx, int * at_idx)
{
	int i;
	
	TRACE_ENTRY("%p %p %p", un, excl_idx, at_idx);
	CHECK_PARAMS_DO( un && excl_idx && at_idx, return );
	
	*excl_idx = 0;
	*at_idx = 0;
	
	/* Search if there is a '!' before any '@' -- do we need to check it contains a '.' ? */
	for (i = 0; i < un->os.len; i++) {
		/* The '!' marks the decorated NAI */
		if ( un->os.data[i] == (unsigned char) '!' ) {
			if (!*excl_idx)
				*excl_idx = i;
			continue;
		}
		/* If we reach the realm part, we can stop */
		if ( un->os.data[i] == (unsigned char) '@' ) {
			*at_idx = i;
			break;
		}
		/* Stop if we find a \0 in the middle */
		if ( un->os.data[i] == 0 ) {
			return;
		}
		/* Skip escaped characters */
		if ( un->os.data[i] == (unsigned char) '\\' ) {
			i++;
			continue;
		}
	}
	
	return;
}	

/* Test if a User-Name AVP contains a Decorated NAI -- RFC4282, RFC5729 */
/* Create new User-Name and Destination-Realm values */
static int process_decorated_NAI(int * was_nai, union avp_value * un, union avp_value * dr)
{
	int at_idx, sep_idx;
	unsigned char * old_un;
	TRACE_ENTRY("%p %p %p", was_nai, un, dr);
	CHECK_PARAMS(was_nai && un && dr);
	
	/* Save the decorated User-Name, for example 'homerealm.example.net!user@otherrealm.example.net' */
	old_un = un->os.data;
	
	/* Search the positions of the first '!' and the '@' in the string */
	nai_get_indexes(un, &sep_idx, &at_idx);
	if ((!sep_idx) || (sep_idx > at_idx) || !fd_os_is_valid_DiameterIdentity(old_un, sep_idx /* this is the new realm part */)) {
		*was_nai = 0;
		return 0;
	}
	
	*was_nai = 1;
	
	/* Create the new User-Name value */
	CHECK_MALLOC( un->os.data = malloc( at_idx ) );
	memcpy( un->os.data, old_un + sep_idx + 1, at_idx - sep_idx ); /* user@ */
	memcpy( un->os.data + at_idx - sep_idx, old_un, sep_idx ); /* homerealm.example.net */
	
	/* Create the new Destination-Realm value */
	CHECK_MALLOC( dr->os.data = realloc(dr->os.data, sep_idx) );
	memcpy( dr->os.data, old_un, sep_idx );
	dr->os.len = sep_idx;
	
	TRACE_DEBUG(FULL, "Processed Decorated NAI : '%.*s' became '%.*s' (%.*s)",
				(int)un->os.len, old_un,
				(int)at_idx, un->os.data,
				(int)dr->os.len, dr->os.data);
	
	un->os.len = at_idx;
	free(old_un);
	
	return 0;
}


/* Function to return an error to an incoming request */
static int return_error(struct msg ** pmsg, char * error_code, char * error_message, struct avp * failedavp)
{
	struct fd_peer * peer;
	int is_loc = 0;

	/* Get the source of the message */
	{
		DiamId_t id;
		size_t   idlen;
		CHECK_FCT( fd_msg_source_get( *pmsg, &id, &idlen ) );
		
		if (id == NULL) {
			is_loc = 1; /* The message was issued locally */
		} else {
		
			/* Search the peer with this id */
			CHECK_FCT( fd_peer_getbyid( id, idlen, 0, (void *)&peer ) );

			if (!peer) {
				char buf[256];
				snprintf(buf, sizeof(buf), "Unable to send error '%s' to deleted peer '%s' in reply to this message.", error_code, id);
				fd_hook_call(HOOK_MESSAGE_DROPPED, *pmsg, NULL, buf, fd_msg_pmdl_get(*pmsg));
				fd_msg_free(*pmsg);
				*pmsg = NULL;
				return 0;
			}
		}
	}
	
	/* Create the error message */
	CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, pmsg, MSGFL_ANSW_ERROR ) );

	/* Set the error code */
	CHECK_FCT( fd_msg_rescode_set(*pmsg, error_code, error_message, failedavp, 1 ) );

	/* Send the answer */
	if (is_loc) {
		CHECK_FCT( fd_fifo_post(fd_g_incoming, pmsg) );
	} else {
		CHECK_FCT( fd_out_send(pmsg, NULL, peer, 1) );
	}
	
	/* Done */
	return 0;
}


/****************************************************************************/
/*         Second part : threads moving messages in the daemon              */
/****************************************************************************/

/* The DISPATCH message processing */
static int msg_dispatch(struct msg * msg)
{
	struct msg_hdr * hdr;
	int is_req = 0;
	struct session * sess;
	enum disp_action action;
	char * ec = NULL;
	char * em = NULL;
	struct msg *msgptr = msg, *error = NULL;

	/* Read the message header */
	CHECK_FCT( fd_msg_hdr(msg, &hdr) );
	is_req = hdr->msg_flags & CMD_FLAG_REQUEST;
	
	/* Note: if the message is for local delivery, we should test for duplicate
	  (draft-asveren-dime-dupcons-00). This may conflict with path validation decisions, no clear answer yet */

	/* At this point, we need to understand the message content, so parse it */
	CHECK_FCT_DO( fd_msg_parse_or_error( &msgptr, &error ),
		{
			int rescue = 0;
			if (__ret__ != EBADMSG) {
				fd_hook_call(HOOK_MESSAGE_DROPPED, msgptr, NULL, "Error while parsing received answer", fd_msg_pmdl_get(msgptr));
				fd_msg_free(msgptr);
			} else {
				if (!msgptr) {
					/* error now contains the answer message to send back */
					CHECK_FCT( fd_fifo_post(fd_g_outgoing, &error) );
				} else if (!error) {
					/* We have received an invalid answer to our query */
					fd_hook_call(HOOK_MESSAGE_DROPPED, msgptr, NULL, "Received answer failed the dictionary / rules parsing", fd_msg_pmdl_get(msgptr));
					fd_msg_free(msgptr);
				} else {
					/* We will pass the invalid received error to the application */
					rescue = 1;
				}
			}
			if (!rescue)
				return 0; /* We are done with this message, go to the next */
		} );

	/* First, if the original request was registered with a callback and we receive the answer, call it. */
	if ( ! is_req ) {
		struct msg * qry;
		void (*anscb)(void *, struct msg **) = NULL;
		void * data = NULL;

		/* Retrieve the corresponding query */
		CHECK_FCT( fd_msg_answ_getq( msgptr, &qry ) );

		/* Retrieve any registered handler */
		CHECK_FCT( fd_msg_anscb_get( qry, &anscb, NULL, &data ) );

		/* If a callback was registered, pass the message to it */
		if (anscb != NULL) {

			TRACE_DEBUG(FULL, "Calling callback registered when query was sent (%p, %p)", anscb, data);
			(*anscb)(data, &msgptr);
			
			/* If the message is processed, we're done */
			if (msgptr == NULL) {
				return 0;
			}
			
			/* otherwise continue the dispatching --hoping that the anscb callback did not mess with our message :) */
		}
	}
	
	/* Retrieve the session of the message */
	CHECK_FCT( fd_msg_sess_get(fd_g_config->cnf_dict, msgptr, &sess, NULL) );

	/* Now, call any callback registered for the message */
	CHECK_FCT( fd_msg_dispatch ( &msgptr, sess, &action, &ec, &em, &error) );

	/* Now, act depending on msg and action and ec */
	if (msgptr) {
		switch ( action ) {
			case DISP_ACT_CONT:
				/* No callback has handled the message, let's reply with a generic error or relay it */
				if (!fd_g_config->cnf_flags.no_fwd) {
					/* requeue to fd_g_outgoing */
					fd_hook_call(HOOK_MESSAGE_ROUTING_FORWARD, msgptr, NULL, NULL, fd_msg_pmdl_get(msgptr));
					CHECK_FCT( fd_fifo_post(fd_g_outgoing, &msgptr) );
					break;
				}
				/* We don't relay => reply error */
				em = "The message was not handled by any extension callback";
				ec = "DIAMETER_COMMAND_UNSUPPORTED";
				/* and continue as if an error occurred... */
			case DISP_ACT_ERROR:
				/* We have a problem with delivering the message */
				if (ec == NULL) {
					ec = "DIAMETER_UNABLE_TO_COMPLY";
				}
				
				if (!is_req) {
					fd_hook_call(HOOK_MESSAGE_DROPPED, msgptr, NULL, "Internal error: Answer received to locally issued request, but not handled by any handler.", fd_msg_pmdl_get(msgptr));
					fd_msg_free(msgptr);
					break;
				}
				
				/* Create an answer with the error code and message */
				CHECK_FCT( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, &msgptr, 0 ) );
				CHECK_FCT( fd_msg_rescode_set(msgptr, ec, em, NULL, 1 ) );
				
			case DISP_ACT_SEND:
				/* Now, send the message */
				CHECK_FCT( fd_fifo_post(fd_g_outgoing, &msgptr) );
		}
	} else if (em) {
		fd_hook_call(HOOK_MESSAGE_DROPPED, error, NULL, em, fd_msg_pmdl_get(error));
		fd_msg_free(error);
	}
	
	/* We're done with dispatching this message */
	return 0;
}

/* The ROUTING-IN message processing */
static int msg_rt_in(struct msg * msg)
{
	struct msg_hdr * hdr;
	int is_req = 0;
	int is_err = 0;
	DiamId_t qry_src = NULL;
	struct msg *msgptr = msg;

	/* Read the message header */
	CHECK_FCT( fd_msg_hdr(msg, &hdr) );
	is_req = hdr->msg_flags & CMD_FLAG_REQUEST;
	is_err = hdr->msg_flags & CMD_FLAG_ERROR;

	/* Handle incorrect bits */
	if (is_req && is_err) {
		fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "R & E bits were set", fd_msg_pmdl_get(msgptr));
		CHECK_FCT( return_error( &msgptr, "DIAMETER_INVALID_HDR_BITS", "R & E bits were set", NULL) );
		return 0;
	}
	
	/* If it is a request, we must analyze its content to decide what we do with it */
	if (is_req) {
		struct avp * avp, *un = NULL;
		union avp_value * un_val = NULL, *dr_val = NULL;
		enum status { UNKNOWN, YES, NO };
		/* Are we Destination-Host? */
		enum status is_dest_host = UNKNOWN;
		/* Are we Destination-Realm? */
		enum status is_dest_realm = UNKNOWN;
		/* Do we support the application of the message? */
		enum status is_local_app = UNKNOWN;

		/* Check if we have local support for the message application */
		if ( (hdr->msg_appl == 0) || (hdr->msg_appl == AI_RELAY) ) {
			fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Received a routable message with application id 0 or " _stringize(AI_RELAY) " (relay)", fd_msg_pmdl_get(msgptr));
			CHECK_FCT( return_error( &msgptr, "DIAMETER_APPLICATION_UNSUPPORTED", "Routable message with application id 0 or relay", NULL) );
			return 0;
		} else {
			struct fd_app * app;
			CHECK_FCT( fd_app_check(&fd_g_config->cnf_apps, hdr->msg_appl, &app) );
			is_local_app = (app ? YES : NO);
		}

		/* Parse the message for Dest-Host, Dest-Realm, and Route-Record */
		CHECK_FCT(  fd_msg_browse(msgptr, MSG_BRW_FIRST_CHILD, &avp, NULL)  );
		while (avp) {
			struct avp_hdr * ahdr;
			struct fd_pei error_info;
			int ret;
			
			memset(&error_info, 0, sizeof(struct fd_pei)); 
			
			CHECK_FCT(  fd_msg_avp_hdr( avp, &ahdr )  );

			if (! (ahdr->avp_flags & AVP_FLAG_VENDOR)) {
				switch (ahdr->avp_code) {
					case AC_DESTINATION_HOST:
						/* Parse this AVP */
						CHECK_FCT_DO( ret = fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, &error_info ),
							{
								if (error_info.pei_errcode) {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, error_info.pei_message ?: error_info.pei_errcode, fd_msg_pmdl_get(msgptr));
									CHECK_FCT( return_error( &msgptr, error_info.pei_errcode, error_info.pei_message, error_info.pei_avp) );
									if (error_info.pei_avp_free) { fd_msg_free(error_info.pei_avp); }
									return 0;
								} else {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Unspecified error while parsing Destination-Host AVP", fd_msg_pmdl_get(msgptr));
									return ret;
								}
							} );
						ASSERT( ahdr->avp_value );
						/* Compare the Destination-Host AVP of the message with our identity */
						if (!fd_os_almostcasesrch(ahdr->avp_value->os.data, ahdr->avp_value->os.len, fd_g_config->cnf_diamid, fd_g_config->cnf_diamid_len, NULL)) {
							is_dest_host = YES;
						} else {
							is_dest_host = NO;
						}
						break;

					case AC_DESTINATION_REALM:
						/* Parse this AVP */
						CHECK_FCT_DO( ret = fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, &error_info ),
							{
								if (error_info.pei_errcode) {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, error_info.pei_message ?: error_info.pei_errcode, fd_msg_pmdl_get(msgptr));
									CHECK_FCT( return_error( &msgptr, error_info.pei_errcode, error_info.pei_message, error_info.pei_avp) );
									if (error_info.pei_avp_free) { fd_msg_free(error_info.pei_avp); }
									return 0;
								} else {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Unspecified error while parsing Destination-Realm AVP", fd_msg_pmdl_get(msgptr));
									return ret;
								}
							} );
						ASSERT( ahdr->avp_value );
						dr_val = ahdr->avp_value;
						/* Compare the Destination-Realm AVP of the message with our identity */
						if (!fd_os_almostcasesrch(dr_val->os.data, dr_val->os.len, fd_g_config->cnf_diamrlm, fd_g_config->cnf_diamrlm_len, NULL)) {
							is_dest_realm = YES;
						} else {
							is_dest_realm = NO;
						}
						break;

					/* we also use User-Name for decorated NAI */
					case AC_USER_NAME:
						/* Parse this AVP */
						CHECK_FCT_DO( ret = fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, &error_info ),
							{
								if (error_info.pei_errcode) {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, error_info.pei_message ?: error_info.pei_errcode, fd_msg_pmdl_get(msgptr));
									CHECK_FCT( return_error( &msgptr, error_info.pei_errcode, error_info.pei_message, error_info.pei_avp) );
									if (error_info.pei_avp_free) { fd_msg_free(error_info.pei_avp); }
									return 0;
								} else {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Unspecified error while parsing User-Name AVP", fd_msg_pmdl_get(msgptr));
									return ret;
								}
							} );
						ASSERT( ahdr->avp_value );
						un = avp;
						un_val = ahdr->avp_value;
						break;
						
					case AC_ROUTE_RECORD:
						/* Parse this AVP */
						CHECK_FCT_DO( ret = fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, &error_info ),
							{
								if (error_info.pei_errcode) {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, error_info.pei_message ?: error_info.pei_errcode, fd_msg_pmdl_get(msgptr));
									CHECK_FCT( return_error( &msgptr, error_info.pei_errcode, error_info.pei_message, error_info.pei_avp) );
									if (error_info.pei_avp_free) { fd_msg_free(error_info.pei_avp); }
									return 0;
								} else {
									fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Unspecified error while parsing Route-Record AVP", fd_msg_pmdl_get(msgptr));
									return ret;
								}
							} );
						ASSERT( ahdr->avp_value );
						/* Is this our own name ? */
						if (!fd_os_almostcasesrch(ahdr->avp_value->os.data, ahdr->avp_value->os.len, fd_g_config->cnf_diamid, fd_g_config->cnf_diamid_len, NULL)) {
							/* Yes: then we must return DIAMETER_LOOP_DETECTED according to Diameter RFC */
							char * error = "DIAMETER_LOOP_DETECTED";
							fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, error, fd_msg_pmdl_get(msgptr));
							CHECK_FCT( return_error( &msgptr, error, NULL, NULL) );
							return 0;
						}
						break;
						
					
				}
			}

			/* Stop when we found all 3 AVPs -- they are supposed to be at the beginning of the message, so this should be fast */
			if ((is_dest_host != UNKNOWN) && (is_dest_realm != UNKNOWN) && un)
				break;

			/* Go to next AVP */
			CHECK_FCT(  fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL)  );
		}

		/* OK, now decide what we do with the request */

		/* Handle the missing routing AVPs first */
		if ( is_dest_realm == UNKNOWN ) {
			fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Non-routable message not supported (invalid bit ? missing Destination-Realm ?)", fd_msg_pmdl_get(msgptr));
			CHECK_FCT( return_error( &msgptr, "DIAMETER_COMMAND_UNSUPPORTED", "Non-routable message not supported (invalid bit ? missing Destination-Realm ?)", NULL) );
			return 0;
		}

		/* If we are listed as Destination-Host */
		if (is_dest_host == YES) {
			if (is_local_app == YES) {
				/* Ok, give the message to the dispatch thread */
				fd_hook_call(HOOK_MESSAGE_ROUTING_LOCAL, msgptr, NULL, NULL, fd_msg_pmdl_get(msgptr));
				CHECK_FCT( fd_fifo_post(fd_g_local, &msgptr) );
			} else {
				/* We don't support the application, reply an error */
				fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Application unsupported", fd_msg_pmdl_get(msgptr));
				CHECK_FCT( return_error( &msgptr, "DIAMETER_APPLICATION_UNSUPPORTED", NULL, NULL) );
			}
			return 0;
		}

		/* If the message is explicitely for someone else */
		if ((is_dest_host == NO) || (is_dest_realm == NO)) {
			char* error = "Message for another realm";
			if (is_dest_host == NO) {
				error = "Message for another host";
			}
  
			if (fd_g_config->cnf_flags.no_fwd) {
				fd_hook_call(HOOK_MESSAGE_ROUTING_ERROR, msgptr, NULL, error, fd_msg_pmdl_get(msgptr));
				CHECK_FCT( return_error( &msgptr, "DIAMETER_UNABLE_TO_DELIVER", "I am not a Diameter agent", NULL) );
				return 0;
			}
		} else {
		/* Destination-Host was not set, and Destination-Realm is matching/unset : we may handle or pass to a fellow peer */
			int is_nai = 0;

			/* test for decorated NAI  (RFC5729 section 4.4) */
			/* Handle the decorated NAI */
			if (un_val) {
				CHECK_FCT_DO( process_decorated_NAI(&is_nai, un_val, dr_val),
					{
						/* If the process failed, we assume it is because of the AVP format */
						fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, msgptr, NULL, "Failed to process decorated NAI", fd_msg_pmdl_get(msgptr));
						CHECK_FCT( return_error( &msgptr, "DIAMETER_INVALID_AVP_VALUE", "Failed to process decorated NAI", un) );
						return 0;
					} );
			}
				
			if (is_nai) {
				/* We have transformed the AVP, now submit it again in the queue */
				CHECK_FCT(fd_fifo_post(fd_g_incoming, &msgptr) );
				return 0;
			}

			if (is_local_app == YES) {
				/* Handle localy since we are able to */
				fd_hook_call(HOOK_MESSAGE_ROUTING_LOCAL, msgptr, NULL, NULL, fd_msg_pmdl_get(msgptr));
				CHECK_FCT(fd_fifo_post(fd_g_local, &msgptr) );
				return 0;
			}

			if (fd_g_config->cnf_flags.no_fwd) {
				/* We return an error */
				fd_hook_call(HOOK_MESSAGE_ROUTING_ERROR, msgptr, NULL, "Application unsupported", fd_msg_pmdl_get(msgptr));
				CHECK_FCT( return_error( &msgptr, "DIAMETER_APPLICATION_UNSUPPORTED", NULL, NULL) );
				return 0;
			}
		}

		/* From that point, for requests, we will call the registered callbacks, then forward to another peer */

	} else {
		/* The message is an answer */
		struct msg * qry;

		/* Retrieve the corresponding query and its origin */
		CHECK_FCT( fd_msg_answ_getq( msgptr, &qry ) );
		CHECK_FCT( fd_msg_source_get( qry, &qry_src, NULL ) );

		if ((!qry_src) && (!is_err)) {
			/* The message is a normal answer to a request issued localy, we do not call the callbacks chain on it. */
			fd_hook_call(HOOK_MESSAGE_ROUTING_LOCAL, msgptr, NULL, NULL, fd_msg_pmdl_get(msgptr));
			CHECK_FCT(fd_fifo_post(fd_g_local, &msgptr) );
			return 0;
		}
		
		/* From that point, for answers, we will call the registered callbacks, then pass it to the dispatch module or forward it */
	}

	/* Call all registered callbacks for this message */
	{
		struct fd_list * li;

		CHECK_FCT( pthread_rwlock_rdlock( &rt_fwd_lock ) );
		pthread_cleanup_push( fd_cleanup_rwlock, &rt_fwd_lock );

		/* requests: dir = 1 & 2 => in order; answers = 3 & 2 => in reverse order */
		for (	li = (is_req ? rt_fwd_list.next : rt_fwd_list.prev) ; msgptr && (li != &rt_fwd_list) ; li = (is_req ? li->next : li->prev) ) {
			struct rt_hdl * rh = (struct rt_hdl *)li;
			int ret;

			if (is_req && (rh->dir > RT_FWD_ALL))
				break;
			if ((!is_req) && (rh->dir < RT_FWD_ALL))
				break;

			/* Ok, call this cb */
			TRACE_DEBUG(ANNOYING, "Calling next FWD callback on %p : %p", msgptr, rh->rt_fwd_cb);
			CHECK_FCT_DO( ret = (*rh->rt_fwd_cb)(rh->cbdata, &msgptr),
				{
					char buf[256];
					snprintf(buf, sizeof(buf), "A FWD routing callback returned an error: %s", strerror(ret));
					fd_hook_call(HOOK_MESSAGE_ROUTING_ERROR, msgptr, NULL, buf, fd_msg_pmdl_get(msgptr));
					fd_hook_call(HOOK_MESSAGE_DROPPED, msgptr, NULL, buf, fd_msg_pmdl_get(msgptr));
					fd_msg_free(msgptr);
					msgptr = NULL;
					break;
				} );
		}

		pthread_cleanup_pop(0);
		CHECK_FCT( pthread_rwlock_unlock( &rt_fwd_lock ) );

		/* If a callback has handled the message, we stop now */
		if (!msgptr)
			return 0;
	}

	/* Now pass the message to the next step: either forward to another peer, or dispatch to local extensions */
	if (is_req || qry_src) {
		fd_hook_call(HOOK_MESSAGE_ROUTING_FORWARD, msgptr, NULL, NULL, fd_msg_pmdl_get(msgptr));
		CHECK_FCT(fd_fifo_post(fd_g_outgoing, &msgptr) );
	} else {
		fd_hook_call(HOOK_MESSAGE_ROUTING_LOCAL, msgptr, NULL, NULL, fd_msg_pmdl_get(msgptr));
		CHECK_FCT(fd_fifo_post(fd_g_local, &msgptr) );
	}

	/* We're done with this message */
	return 0;
}
		

/* The ROUTING-OUT message processing */
static int msg_rt_out(struct msg * msg)
{
	struct rt_data * rtd = NULL;
	struct msg_hdr * hdr;
	int is_req = 0;
	int ret;
	struct fd_list * li, *candidates;
	struct avp * avp;
	struct rtd_candidate * c;
	struct msg *msgptr = msg;
	DiamId_t qry_src = NULL;
	size_t qry_src_len = 0;
	
	/* Read the message header */
	CHECK_FCT( fd_msg_hdr(msgptr, &hdr) );
	is_req = hdr->msg_flags & CMD_FLAG_REQUEST;
	
	/* For answers, the routing is very easy */
	if ( ! is_req ) {
		struct msg * qry;
		struct msg_hdr * qry_hdr;
		struct fd_peer * peer = NULL;

		/* Retrieve the corresponding query and its origin */
		CHECK_FCT( fd_msg_answ_getq( msgptr, &qry ) );
		CHECK_FCT( fd_msg_source_get( qry, &qry_src, &qry_src_len ) );

		ASSERT( qry_src ); /* if it is NULL, the message should have been in the LOCAL queue! */

		/* Find the peer corresponding to this name */
		CHECK_FCT( fd_peer_getbyid( qry_src, qry_src_len, 0, (void *) &peer ) );
		if (fd_peer_getstate(peer) != STATE_OPEN) {
			char buf[128];
			snprintf(buf, sizeof(buf), "Unable to forward answer to deleted / closed peer '%s'.", qry_src);
			fd_hook_call(HOOK_MESSAGE_ROUTING_ERROR, msgptr, NULL, buf, fd_msg_pmdl_get(msgptr));
			fd_hook_call(HOOK_MESSAGE_DROPPED, msgptr, NULL, buf, fd_msg_pmdl_get(msgptr));
			fd_msg_free(msgptr);
			return 0;
		}

		/* We must restore the hop-by-hop id */
		CHECK_FCT( fd_msg_hdr(qry, &qry_hdr) );
		hdr->msg_hbhid = qry_hdr->msg_hbhid;

		/* Push the message into this peer */
		CHECK_FCT( fd_out_send(&msgptr, NULL, peer, 1) );

		/* We're done with this answer */
		return 0;
	}
	
	/* From that point, the message is a request */
	CHECK_FCT( fd_msg_source_get( msgptr, &qry_src, &qry_src_len ) );
	/* if qry_src != NULL, this message is relayed, otherwise it is locally issued */

	/* Get the routing data out of the message if any (in case of re-transmit) */
	CHECK_FCT( fd_msg_rt_get ( msgptr, &rtd ) );

	/* If there is no routing data already, let's create it */
	if (rtd == NULL) {
		CHECK_FCT( fd_rtd_init(&rtd) );

		/* Add all peers currently in OPEN state */
		CHECK_FCT( pthread_rwlock_rdlock(&fd_g_activ_peers_rw) );
		for (li = fd_g_activ_peers.next; li != &fd_g_activ_peers; li = li->next) {
			struct fd_peer * p = (struct fd_peer *)li->o;
			CHECK_FCT_DO( ret = fd_rtd_candidate_add(rtd,
								 p->p_hdr.info.pi_diamid,
								 p->p_hdr.info.pi_diamidlen,
								 p->p_hdr.info.runtime.pir_host,
								 p->p_hdr.info.runtime.pir_hostlen,
								 p->p_hdr.info.runtime.pir_realm,
								 p->p_hdr.info.runtime.pir_realmlen),
				{ CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_activ_peers_rw), ); return ret; } );
		}
		CHECK_FCT( pthread_rwlock_unlock(&fd_g_activ_peers_rw) );

		/* Now let's remove all peers from the Route-Records */
		CHECK_FCT(  fd_msg_browse(msgptr, MSG_BRW_FIRST_CHILD, &avp, NULL)  );
		while (avp) {
			struct avp_hdr * ahdr;
			struct fd_pei error_info;
			CHECK_FCT(  fd_msg_avp_hdr( avp, &ahdr )  );

			if ((ahdr->avp_code == AC_ROUTE_RECORD) && (! (ahdr->avp_flags & AVP_FLAG_VENDOR)) ) {
				/* Parse this AVP */
				CHECK_FCT_DO( ret = fd_msg_parse_dict ( avp, fd_g_config->cnf_dict, &error_info ),
					{
						if (error_info.pei_errcode) {
							CHECK_FCT( return_error( &msgptr, error_info.pei_errcode, error_info.pei_message, error_info.pei_avp) );
							if (error_info.pei_avp_free) { fd_msg_free(error_info.pei_avp); }
							return 0;
						} else {
							return ret;
						}
					} );
				ASSERT( ahdr->avp_value );
				/* Remove this value from the list. We don't need to pay special attention to the contents here. */
				fd_rtd_candidate_del(rtd, ahdr->avp_value->os.data, ahdr->avp_value->os.len);
			}

			/* Go to next AVP */
			CHECK_FCT(  fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL)  );
		}
		
		/* Save the routing information in the message */
		CHECK_FCT( fd_msg_rt_associate ( msgptr, rtd ) );
	}

	/* Note: we reset the scores and pass the message to the callbacks, maybe we could re-use the saved scores when we have received an error ? -- TODO */

	/* Ok, we have our list in rtd now, let's (re)initialize the scores */
	fd_rtd_candidate_extract(rtd, &candidates, FD_SCORE_INI);

	/* Pass the list to registered callbacks (even if it is empty list) */
	{
		CHECK_FCT( pthread_rwlock_rdlock( &rt_out_lock ) );
		pthread_cleanup_push( fd_cleanup_rwlock, &rt_out_lock );

		/* We call the cb by reverse priority order */
		for (	li = rt_out_list.prev ; (msgptr != NULL) && (li != &rt_out_list) ; li = li->prev ) {
			struct rt_hdl * rh = (struct rt_hdl *)li;

			TRACE_DEBUG(ANNOYING, "Calling next OUT callback on %p : %p (prio %d)", msgptr, rh->rt_out_cb, rh->prio);
			CHECK_FCT_DO( ret = (*rh->rt_out_cb)(rh->cbdata, &msgptr, candidates),
				{
					char buf[256];
					snprintf(buf, sizeof(buf), "An OUT routing callback returned an error: %s", strerror(ret));
					fd_hook_call(HOOK_MESSAGE_ROUTING_ERROR, msgptr, NULL, buf, fd_msg_pmdl_get(msgptr));
					fd_hook_call(HOOK_MESSAGE_DROPPED, msgptr, NULL, buf, fd_msg_pmdl_get(msgptr));
					fd_msg_free(msgptr);
					msgptr = NULL;
				} );
		}

		pthread_cleanup_pop(0);
		CHECK_FCT( pthread_rwlock_unlock( &rt_out_lock ) );

		/* If an error occurred or the callback disposed of the message, go to next message */
		if (! msgptr) {
			return 0;
		}
	}
	
	/* Order the candidate peers by score attributed by the callbacks */
	CHECK_FCT( fd_rtd_candidate_reorder(candidates) );

	/* Now try sending the message */
	for (li = candidates->prev; li != candidates; li = li->prev) {
		struct fd_peer * peer;

		c = (struct rtd_candidate *) li;

		/* Stop when we have reached the end of valid candidates */
		if (c->score < 0)
			break;

		/* Search for the peer */
		CHECK_FCT( fd_peer_getbyid( c->cfg_diamid, c->cfg_diamidlen, 0, (void *)&peer ) );

		if (fd_peer_getstate(peer) == STATE_OPEN) {
			/* Send to this one */
			CHECK_FCT_DO( fd_out_send(&msgptr, NULL, peer, 1), continue );
			
			/* If the sending was successful */
			break;
		}
	}

	/* If the message has not been sent, return an error */
	if (msgptr) {
		fd_hook_call(HOOK_MESSAGE_ROUTING_ERROR, msgptr, NULL, "No remaining suitable candidate to route the message to", fd_msg_pmdl_get(msgptr));
		return_error( &msgptr, "DIAMETER_UNABLE_TO_DELIVER", "No suitable candidate to route the message to", NULL);
	}

	/* We're done with this message */
	
	return 0;
}


/********************************************************************************/
/*                     Management of the threads                                */
/********************************************************************************/

/* Note: in the first version, we only create one thread of each kind.
 We could improve the scalability by using the threshold feature of the queues
 to create additional threads if a queue is filling up, or at least giving a configurable
 number of threads of each kind.
 */

/* Control of the threads */
static enum { RUN = 0, STOP = 1 } order_val = RUN;
static pthread_mutex_t order_state_lock = PTHREAD_MUTEX_INITIALIZER;

/* Threads report their status */
enum thread_state { NOTRUNNING = 0, RUNNING = 1 };
static void cleanup_state(void * state_loc)
{
	CHECK_POSIX_DO( pthread_mutex_lock(&order_state_lock), );
	*(enum thread_state *)state_loc = NOTRUNNING;
	CHECK_POSIX_DO( pthread_mutex_unlock(&order_state_lock), );
}

/* This is the common thread code (same for routing and dispatching) */
static void * process_thr(void * arg, int (*action_cb)(struct msg * msg), struct fifo * queue, char * action_name)
{
	TRACE_ENTRY("%p %p %p %p", arg, action_cb, queue, action_name);
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "%s (%p)", action_name, arg);
		fd_log_threadname ( buf );
	}
	
	/* The thread reports its status when canceled */
	CHECK_PARAMS_DO(arg, return NULL);
	pthread_cleanup_push( cleanup_state, arg );
	
	/* Mark the thread running */
	CHECK_POSIX_DO( pthread_mutex_lock(&order_state_lock), );
	*(enum thread_state *)arg = RUNNING;
	CHECK_POSIX_DO( pthread_mutex_unlock(&order_state_lock), );
	
	do {
		struct msg * msg;
	
		/* Test the current order */
		{
			int must_stop;
			CHECK_POSIX_DO( pthread_mutex_lock(&order_state_lock), { ASSERT(0); } ); /* we lock to flush the caches */
			must_stop = (order_val == STOP);
			CHECK_POSIX_DO( pthread_mutex_unlock(&order_state_lock), { ASSERT(0); } );
			if (must_stop)
				goto end;
			
			pthread_testcancel();
		}
		
		/* Ok, we are allowed to run */
		
		/* Get the next message from the queue */
		{
			int ret;
			struct timespec ts;
			
			CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), goto fatal_error );
			ts.tv_sec += 1;
			
			ret = fd_fifo_timedget ( queue, &msg, &ts );
			if (ret == ETIMEDOUT)
				/* loop, check if the thread must stop now */
				continue;
			if (ret == EPIPE)
				/* The queue was destroyed, we are probably exiting */
				goto end;
			
			/* check if another error occurred */
			CHECK_FCT_DO( ret, goto fatal_error );
		}
		
		LOG_A("%s: Picked next message", action_name);

		/* Now process the message */
		CHECK_FCT_DO( (*action_cb)(msg), goto fatal_error);

		/* We're done with this message */
	
	} while (1);
	
fatal_error:
	TRACE_DEBUG(INFO, "An unrecoverable error occurred, %s thread is terminating...", action_name);
	CHECK_FCT_DO(fd_core_shutdown(), );
	
end:	
	; /* noop so that we get rid of "label at end of compund statement" warning */
	/* Mark the thread as terminated */
	pthread_cleanup_pop(1);
	return NULL;
}

/* The dispatch thread */
static void * dispatch_thr(void * arg)
{
	return process_thr(arg, msg_dispatch, fd_g_local, "Dispatch");
}

/* The (routing-in) thread -- see description in freeDiameter.h */
static void * routing_in_thr(void * arg)
{
	return process_thr(arg, msg_rt_in, fd_g_incoming, "Routing-IN");
}

/* The (routing-out) thread -- see description in freeDiameter.h */
static void * routing_out_thr(void * arg)
{
	return process_thr(arg, msg_rt_out, fd_g_outgoing, "Routing-OUT");
}


/********************************************************************************/
/*                     The functions for the other files                        */
/********************************************************************************/

static pthread_t * dispatch = NULL;
static enum thread_state * disp_state = NULL;

/* Later: make this more dynamic */
static pthread_t rt_out = (pthread_t)NULL;
static enum thread_state out_state = NOTRUNNING;

static pthread_t rt_in  = (pthread_t)NULL;
static enum thread_state in_state = NOTRUNNING;

/* Initialize the routing and dispatch threads */
int fd_rtdisp_init(void)
{
	int i;
	
	/* Prepare the array for dispatch */
	CHECK_MALLOC( disp_state = calloc(fd_g_config->cnf_dispthr, sizeof(enum thread_state)) );
	CHECK_MALLOC( dispatch = calloc(fd_g_config->cnf_dispthr, sizeof(pthread_t)) );
	
	/* Create the threads */
	for (i=0; i < fd_g_config->cnf_dispthr; i++) {
		CHECK_POSIX( pthread_create( &dispatch[i], NULL, dispatch_thr, &disp_state[i] ) );
	}
	CHECK_POSIX( pthread_create( &rt_out, NULL, routing_out_thr, &out_state) );
	CHECK_POSIX( pthread_create( &rt_in,  NULL, routing_in_thr,  &in_state) );
	
	/* Later: TODO("Set the thresholds for the queues to create more threads as needed"); */
	
	/* Register the built-in callbacks */
	CHECK_FCT( fd_rt_out_register( dont_send_if_no_common_app, NULL, 10, NULL ) );
	CHECK_FCT( fd_rt_out_register( score_destination_avp, NULL, 10, NULL ) );
	
	return 0;
}

/* Ask the thread to terminate after next iteration */
int fd_rtdisp_cleanstop(void)
{
	CHECK_POSIX_DO( pthread_mutex_lock(&order_state_lock), );
	order_val = STOP;
	CHECK_POSIX_DO( pthread_mutex_unlock(&order_state_lock), );

	return 0;
}

static void stop_thread_delayed(enum thread_state *st, pthread_t * thr, char * th_name)
{
	TRACE_ENTRY("%p %p", st, thr);
	CHECK_PARAMS_DO(st && thr, return);
	int terminated;
	
	CHECK_POSIX_DO( pthread_mutex_lock(&order_state_lock), );
	terminated = (*st == NOTRUNNING);
	CHECK_POSIX_DO( pthread_mutex_unlock(&order_state_lock), );
	

	/* Wait for a second for the thread to complete, by monitoring my_state */
	if (!terminated) {
		TRACE_DEBUG(INFO, "Waiting for the %s thread to have a chance to terminate", th_name);
		do {
			struct timespec	 ts, ts_final;

			CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), break );
			
			ts_final.tv_sec = ts.tv_sec + 1;
			ts_final.tv_nsec = ts.tv_nsec;
			
			while (TS_IS_INFERIOR( &ts, &ts_final )) {
			
				CHECK_POSIX_DO( pthread_mutex_lock(&order_state_lock), );
				terminated = (*st == NOTRUNNING);
				CHECK_POSIX_DO( pthread_mutex_unlock(&order_state_lock), );
				if (terminated)
					break;
				
				usleep(100000);
				CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), break );
			}
		} while (0);
	}

	/* Now stop the thread and reclaim its resources */
	CHECK_FCT_DO( fd_thr_term(thr ), /* continue */);
	
}

/* Stop the thread after up to one second of wait */
int fd_rtdisp_fini(void)
{
	int i;
	
	/* Destroy the incoming queue */
	CHECK_FCT_DO( fd_queues_fini(&fd_g_incoming), /* ignore */);
	
	/* Stop the routing IN thread */
	stop_thread_delayed(&in_state, &rt_in, "IN routing");
	
	/* Destroy the outgoing queue */
	CHECK_FCT_DO( fd_queues_fini(&fd_g_outgoing), /* ignore */);
	
	/* Stop the routing OUT thread */
	stop_thread_delayed(&out_state, &rt_out, "OUT routing");
	
	/* Destroy the local queue */
	CHECK_FCT_DO( fd_queues_fini(&fd_g_local), /* ignore */);
	
	/* Stop the Dispatch threads */
	if (dispatch != NULL) {
		for (i=0; i < fd_g_config->cnf_dispthr; i++) {
			stop_thread_delayed(&disp_state[i], &dispatch[i], "Dispatching");
		}
		free(dispatch);
		dispatch = NULL;
	}
	if (disp_state != NULL) {
		free(disp_state);
		disp_state = NULL;
	}
	
	return 0;
}

/* Cleanup handlers */
int fd_rtdisp_cleanup(void)
{
	/* Cleanup all remaining handlers */
	while (!FD_IS_LIST_EMPTY(&rt_fwd_list)) {
		CHECK_FCT_DO( fd_rt_fwd_unregister ( (void *)rt_fwd_list.next, NULL ), /* continue */ );
	}
	while (!FD_IS_LIST_EMPTY(&rt_out_list)) {
		CHECK_FCT_DO( fd_rt_out_unregister ( (void *)rt_out_list.next, NULL ), /* continue */ );
	}
	
	fd_disp_unregister_all(); /* destroy remaining handlers */

	return 0;
}


/********************************************************************************/
/*                     For extensions to register a new appl                    */
/********************************************************************************/

/* Add an application into the peer's supported apps */
int fd_disp_app_support ( struct dict_object * app, struct dict_object * vendor, int auth, int acct )
{
	application_id_t aid = 0;
	vendor_id_t	 vid = 0;
	
	TRACE_ENTRY("%p %p %d %d", app, vendor, auth, acct);
	CHECK_PARAMS( app && (auth || acct) );
	
	{
		enum dict_object_type type = 0;
		struct dict_application_data data;
		CHECK_FCT( fd_dict_gettype(app, &type) );
		CHECK_PARAMS( type == DICT_APPLICATION );
		CHECK_FCT( fd_dict_getval(app, &data) );
		aid = data.application_id;
	}

	if (vendor) {
		enum dict_object_type type = 0;
		struct dict_vendor_data data;
		CHECK_FCT( fd_dict_gettype(vendor, &type) );
		CHECK_PARAMS( type == DICT_VENDOR );
		CHECK_FCT( fd_dict_getval(vendor, &data) );
		vid = data.vendor_id;
	}
	
	return fd_app_merge(&fd_g_config->cnf_apps, aid, vid, auth, acct);
}



