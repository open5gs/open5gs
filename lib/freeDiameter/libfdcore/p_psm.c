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

/*
This file implement a Peer State Machine which is a mix of:
 - the state machine described in rfc3588bis
 - the state machine described in rfc3539#section-3.4
 - the following observations.
 
The delivery of Diameter messages must not always be unordered: order is important at
begining and end of a connection lifetime. It means we need agility to
switch between "ordering enforced" and "ordering not enforced to counter
Head of the Line Blocking" modes of operation.

The connection state machine represented in RFC3588 (and RFC6733) is
incomplete, because it lacks the SUSPECT state and the 3 DWR/DWA
exchanges (section 5.1) when the peer recovers from this state.
Personnally I don't see the rationale for exchanging 3 messages (why 3?)
but, if we require at least 1 DWR/DWA exchange to be always performed
after the CER/CEA exchange (and initiated by the peer that sent the
CEA), we have a simple way to deal with our ordering problem, as resumed
below. Peers are: [i]nitiator, [r]esponder.
 (1) [i] SCTP connection attempt.
 (2) [r] accept the connection.
 (3) [i,r] (if secure port) DTLS handshake, close on failure.
 (4) [i] Send CER
 (5) [r] Receive CER, send CEA using stream 0, flag "unordered" cleared.
       [r] Immediately send a DWR after the CEA, also using stream 0,
flag "unordered" cleared.
       [r] Move to STATE_OPEN_NEW state -- equivalent to OPEN except
that all messages are sent ordered at the moment.
 (6) [i] receive CEA, move to OPEN state. All messages can be sent
unordered in OPEN state.
       [i] As per normal operation, reply with DWA to the DWR.
 (7) [r] Upon reception of the DWA, move to OPEN state, messages can be
sent unordered from this point.

Note about (5) and (6): if the Diameter Identity received in CER or CEA
does not match the credentials from the certificate presented during
TLS handshake, we may need to specify a path of clean disconnection
(not blocking the remote peer waiting for something).

This proposed mechanism removes the problem of application messages
received before the CEA by the initiator. Note that if the "old" inband
TLS handshake is used, this handshake plays the same synchronization
role than the new DWR/DWA, which becomes useless.


The other time where ordering is important is by the end of connection
lifetime, when one peer is shutting down the link for some reason
(reboot, overload, no activity, etc...). In case of unordered delivery,
we may have:
- peer A sends an application message followed by a DPR. Peer B receives
the DPR first and tears down the connection. Application message is lost.
- Peer B sends an application message, then receives a DPR and answers a
DPA. Peer A receives the DPA before the application message. The
application message is lost.

This situation is actually happening easily because DPR/DPA messages are
very short, while application messages can be quite large. Therefore,
they require much more time to deliver.

I really cannot see a way to counter this effect by using the ordering
of the messages, except by applying a timer (state STATE_CLOSING_GRACE).
This timer can be also useful when we detect that some messages has not 
yet received an answer on this link, to give time to the application to 
complete the exchange ongoing.

However, this problem must be balanced with the fact that the message
that is lost will be in many cases sent again as the failover mechanism
specifies.
*/

/* The actual declaration of peer_state_str */
DECLARE_STATE_STR();

/* Helper for next macro */
#define case_str( _val )		\
	case _val : return #_val

DECLARE_PEV_STR();

/************************************************************************/
/*                      Delayed startup                                 */
/************************************************************************/
static int started = 0;
static pthread_mutex_t  started_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t   started_cnd = PTHREAD_COND_INITIALIZER;

/* Wait for start signal */
static int fd_psm_waitstart()
{
	int ret = 0;
	TRACE_ENTRY("");
	CHECK_POSIX( pthread_mutex_lock(&started_mtx) );
awake:	
	if (!ret && !started) {
		pthread_cleanup_push( fd_cleanup_mutex, &started_mtx );
		CHECK_POSIX_DO( ret = pthread_cond_wait(&started_cnd, &started_mtx), );
		pthread_cleanup_pop( 0 );
		goto awake;
	}
	CHECK_POSIX( pthread_mutex_unlock(&started_mtx) );
	return ret;
}

/* Allow the state machines to start */
int fd_psm_start()
{
	TRACE_ENTRY("");
	CHECK_POSIX( pthread_mutex_lock(&started_mtx) );
	started = 1;
	CHECK_POSIX( pthread_cond_broadcast(&started_cnd) );
	CHECK_POSIX( pthread_mutex_unlock(&started_mtx) );
	return 0;
}


/************************************************************************/
/*                 Manage the list of active peers                      */
/************************************************************************/

/* Enter/leave OPEN state */
static int enter_open_state(struct fd_peer * peer)
{
	struct fd_list * li;
	CHECK_PARAMS( FD_IS_LIST_EMPTY(&peer->p_actives) );
	
	/* Callback registered by the credential validator (fd_peer_validate_register) */
	if (peer->p_cb2) {
		CHECK_FCT_DO( (*peer->p_cb2)(&peer->p_hdr.info),
			{
				TRACE_DEBUG(FULL, "Validation failed, terminating the connection");
				fd_psm_terminate(peer, "DO_NOT_WANT_TO_TALK_TO_YOU" );
			} );
		peer->p_cb2 = NULL;
		return 0;
	}
	
	/* Insert in the active peers list */
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_g_activ_peers_rw) );
	for (li = fd_g_activ_peers.next; li != &fd_g_activ_peers; li = li->next) {
		struct fd_peer * next_p = (struct fd_peer *)li->o;
		int cmp = fd_os_cmp(peer->p_hdr.info.pi_diamid, peer->p_hdr.info.pi_diamidlen, 
					next_p->p_hdr.info.pi_diamid, next_p->p_hdr.info.pi_diamidlen);
		if (cmp < 0)
			break;
	}
	fd_list_insert_before(li, &peer->p_actives);
	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_activ_peers_rw) );
	
	/* Callback registered when the peer was added, by fd_peer_add */
	if (peer->p_cb) {
		TRACE_DEBUG(FULL, "Calling add callback for peer %s", peer->p_hdr.info.pi_diamid);
		(*peer->p_cb)(&peer->p_hdr.info, peer->p_cb_data); /* TODO: do this in a separate detached thread? */
		peer->p_cb = NULL;
		peer->p_cb_data = NULL;
	}
	
	/* Start the thread to handle outgoing messages */
	CHECK_FCT( fd_out_start(peer) );
	
	/* Update the expiry timer now */
	CHECK_FCT( fd_p_expi_update(peer) );
	
	return 0;
}
static int leave_open_state(struct fd_peer * peer, int skip_failover)
{
	/* Remove from active peers list */
	CHECK_POSIX( pthread_rwlock_wrlock(&fd_g_activ_peers_rw) );
	fd_list_unlink( &peer->p_actives );
	CHECK_POSIX( pthread_rwlock_unlock(&fd_g_activ_peers_rw) );
	
	/* Stop the "out" thread */
	CHECK_FCT( fd_out_stop(peer) );
	
	/* Failover the messages */
	if (!skip_failover) {
		fd_peer_failover_msg(peer);
	}
	
	return 0;
}


/************************************************************************/
/*                      Helpers for state changes                       */
/************************************************************************/

/* Cleanup pending events in the peer */
void fd_psm_events_free(struct fd_peer * peer)
{
	struct fd_event * ev;
	/* Purge all events, and free the associated data if any */
	while (fd_fifo_tryget( peer->p_events, &ev ) == 0) {
		switch (ev->code) {
			case FDEVP_CNX_ESTABLISHED: {
				fd_cnx_destroy(ev->data);
			}
			break;
			
			case FDEVP_TERMINATE:
				/* Do not free the string since it is a constant */
			break;
			
			case FDEVP_CNX_INCOMING: {
				struct cnx_incoming * evd = ev->data;
				fd_hook_call(HOOK_MESSAGE_DROPPED, evd->cer, NULL, "Message discarded while cleaning peer state machine queue.", fd_msg_pmdl_get(evd->cer));
				CHECK_FCT_DO( fd_msg_free(evd->cer), /* continue */);
				fd_cnx_destroy(evd->cnx);
			}
			default:
				free(ev->data);
		}
		free(ev);
	}
}

/* Read state */
int fd_peer_get_state(struct peer_hdr *peer)
{
	int ret;
	
	struct fd_peer * p = (struct fd_peer *)peer;
	
	if (!CHECK_PEER(p))
		return -1;
	
	CHECK_POSIX_DO( pthread_mutex_lock(&p->p_state_mtx), return -1 );
	ret = p->p_state;
	CHECK_POSIX_DO( pthread_mutex_unlock(&p->p_state_mtx), return -1 );
	
	return ret;
}


/* Change state */
int fd_psm_change_state(struct fd_peer * peer, int new_state)
{
	int old;
	
	TRACE_ENTRY("%p %d(%s)", peer, new_state, STATE_STR(new_state));
	CHECK_PARAMS( CHECK_PEER(peer) );
	
	old = fd_peer_getstate(peer);
	if (old == new_state)
		return 0;
	
	LOG(((old == STATE_OPEN) || (new_state == STATE_OPEN)) ? FD_LOG_NOTICE : FD_LOG_DEBUG, "'%s'\t-> '%s'\t'%s'",
			STATE_STR(old),
			STATE_STR(new_state),
			peer->p_hdr.info.pi_diamid);
	
	
	CHECK_POSIX( pthread_mutex_lock(&peer->p_state_mtx) );
	peer->p_state = new_state;
	CHECK_POSIX( pthread_mutex_unlock(&peer->p_state_mtx) );
	
	if (old == STATE_OPEN) {
		CHECK_FCT( leave_open_state(peer, new_state == STATE_CLOSING_GRACE) );
	}
	if (old == STATE_CLOSING_GRACE) {
		fd_peer_failover_msg(peer);
	}

	if (new_state == STATE_OPEN) {
		CHECK_FCT( enter_open_state(peer) );
	}
	
	if (new_state == STATE_CLOSED) {
		/* Purge event list */
		fd_psm_events_free(peer);
		
		/* Reset the counter of pending anwers to send */
		peer->p_reqin_count = 0;
		
		/* If the peer is not persistant, we destroy it */
		if (peer->p_hdr.info.config.pic_flags.persist == PI_PRST_NONE) {
			CHECK_FCT( fd_event_send(peer->p_events, FDEVP_TERMINATE, 0, NULL) );
		}
	}
	
	return 0;
}

/* Set timeout timer of next event */
void fd_psm_next_timeout(struct fd_peer * peer, int add_random, int delay)
{
	TRACE_DEBUG(FULL, "Peer timeout reset to %d seconds%s", delay, add_random ? " (+/- 2)" : "" );
	
	/* Initialize the timer */
	CHECK_POSIX_DO(  clock_gettime( CLOCK_REALTIME,  &peer->p_psm_timer ), ASSERT(0) );
	
	if (add_random) {
		if (delay > 2)
			delay -= 2;
		else
			delay = 0;

		/* Add a random value between 0 and 4sec */
		peer->p_psm_timer.tv_sec += random() % 4;
		peer->p_psm_timer.tv_nsec+= random() % 1000000000L;
		if (peer->p_psm_timer.tv_nsec >= 1000000000L) {
			peer->p_psm_timer.tv_nsec -= 1000000000L;
			peer->p_psm_timer.tv_sec ++;
		}
	}
	
	peer->p_psm_timer.tv_sec += delay;
	
#ifdef SLOW_PSM
	/* temporary for debug */
	peer->p_psm_timer.tv_sec += 10;
#endif
}

/* Cleanup the peer */
void fd_psm_cleanup(struct fd_peer * peer, int terminate)
{
	/* Move to CLOSED state: failover messages, stop OUT thread, unlink peer from active list */
	if (fd_peer_getstate(peer) != STATE_ZOMBIE) {
		CHECK_FCT_DO( fd_psm_change_state(peer, STATE_CLOSED), /* continue */ );
	}
	
	fd_p_cnx_abort(peer, terminate);
	
	fd_p_ce_clear_cnx(peer, NULL);
	
	if (peer->p_receiver) {
		fd_cnx_destroy(peer->p_receiver);
		peer->p_receiver = NULL;
	}
	
	if (terminate) {
		fd_psm_events_free(peer);
		CHECK_FCT_DO( fd_fifo_del(&peer->p_events), /* continue */ );
	}
	
}


/************************************************************************/
/*                      The PSM thread                                  */
/************************************************************************/
/* Cancelation cleanup : set ZOMBIE state in the peer */
void cleanup_setstate(void * arg) 
{
	struct fd_peer * peer = (struct fd_peer *)arg;
	CHECK_PARAMS_DO( CHECK_PEER(peer), return );
	CHECK_POSIX_DO( pthread_mutex_lock(&peer->p_state_mtx), );
	peer->p_state = STATE_ZOMBIE;
	CHECK_POSIX_DO( pthread_mutex_unlock(&peer->p_state_mtx), );
	return;
}

/* The state machine thread (controler) */
static void * p_psm_th( void * arg )
{
	struct fd_peer * peer = (struct fd_peer *)arg;
	int created_started = started ? 1 : 0;
	int event;
	size_t ev_sz;
	void * ev_data;
	int cur_state;
	
	CHECK_PARAMS_DO( CHECK_PEER(peer), ASSERT(0) );
	
	pthread_cleanup_push( cleanup_setstate, arg );
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "PSM/%s", peer->p_hdr.info.pi_diamid);
		fd_log_threadname ( buf );
	}
	
	/* The state machine starts in CLOSED state */
	CHECK_POSIX_DO( pthread_mutex_lock(&peer->p_state_mtx), goto psm_end );
	peer->p_state = STATE_CLOSED;
	CHECK_POSIX_DO( pthread_mutex_unlock(&peer->p_state_mtx), goto psm_end );

	/* Wait that the PSM are authorized to start in the daemon */
	CHECK_FCT_DO( fd_psm_waitstart(), goto psm_end );
	
	/* Initialize the timer */
	if (peer->p_flags.pf_responder) {
		fd_psm_next_timeout(peer, 0, INCNX_TIMEOUT);
	} else {
		fd_psm_next_timeout(peer, created_started, 0);
	}
	
psm_loop:
	/* Get next event */
	TRACE_DEBUG(FULL, "'%s' in state '%s' waiting for next event.",
			peer->p_hdr.info.pi_diamid, STATE_STR(fd_peer_getstate(peer)));
	CHECK_FCT_DO( fd_event_timedget(peer->p_events, &peer->p_psm_timer, FDEVP_PSM_TIMEOUT, &event, &ev_sz, &ev_data), goto psm_end );
	
	cur_state = fd_peer_getstate(peer);
	if (cur_state == -1)
		goto psm_end;
	
	TRACE_DEBUG(FULL, "'%s'\t<-- '%s'\t(%p,%zd)\t'%s'",
			STATE_STR(cur_state),
			fd_pev_str(event), ev_data, ev_sz,
			peer->p_hdr.info.pi_diamid);

	/* Now, the action depends on the current state and the incoming event */

	/* The following states are impossible */
	ASSERT( cur_state != STATE_NEW );
	ASSERT( cur_state != STATE_ZOMBIE );
	ASSERT( cur_state != STATE_OPEN_HANDSHAKE ); /* because it should exist only between two loops */

	/* Purge invalid events */
	if (!CHECK_PEVENT(event)) {
		TRACE_DEBUG(INFO, "Invalid event received in PSM '%s' : %d", peer->p_hdr.info.pi_diamid, event);
		ASSERT(0); /* we should investigate this situation */
		goto psm_loop;
	}

	/* Requests to terminate the peer object */
	if (event == FDEVP_TERMINATE) {
		switch (cur_state) {
			case STATE_OPEN:
			case STATE_OPEN_NEW:
			case STATE_REOPEN:
				/* We cannot just close the connection, we have to send a DPR first */
				CHECK_FCT_DO( fd_p_dp_initiate(peer, ev_data), goto psm_end );
				goto psm_loop;
			
			/*	
			case STATE_CLOSING:
			case STATE_CLOSING_GRACE:
			case STATE_WAITCNXACK:
			case STATE_WAITCNXACK_ELEC:
			case STATE_WAITCEA:
			case STATE_SUSPECT:
			case STATE_CLOSED:
			*/
			default:
				/* In these cases, we just cleanup the peer object (if needed) and terminate */
				goto psm_end;
		}
	}
	
	/* A message was received */
	if (event == FDEVP_CNX_MSG_RECV) {
		struct msg * msg = NULL;
		struct msg_hdr * hdr;
		struct fd_cnx_rcvdata rcv_data;
		struct fd_msg_pmdl * pmdl = NULL;
		
		rcv_data.buffer = ev_data;
		rcv_data.length = ev_sz;
		pmdl = fd_msg_pmdl_get_inbuf(rcv_data.buffer, rcv_data.length);
		
		/* Parse the received buffer */
		CHECK_FCT_DO( fd_msg_parse_buffer( (void *)&ev_data, ev_sz, &msg), 
			{
				fd_hook_call(HOOK_MESSAGE_PARSING_ERROR, NULL, peer, &rcv_data, pmdl );
				free(ev_data);
				CHECK_FCT_DO( fd_event_send(peer->p_events, FDEVP_CNX_ERROR, 0, NULL), goto psm_reset );
				goto psm_loop;
			} );
			
		fd_hook_associate(msg, pmdl);
		CHECK_FCT_DO( fd_msg_source_set( msg, peer->p_hdr.info.pi_diamid, peer->p_hdr.info.pi_diamidlen), goto psm_end);
	
		/* If the current state does not allow receiving messages, just drop it */
		if (cur_state == STATE_CLOSED) {
			/* In such case, just discard the message */
			fd_hook_call(HOOK_MESSAGE_DROPPED, msg, peer, "Message purged from queue, peer in CLOSED state", fd_msg_pmdl_get(msg));
			fd_msg_free(msg);
			goto psm_loop;
		}
		
		/* Extract the header */
		CHECK_FCT_DO( fd_msg_hdr(msg, &hdr), goto psm_end );
		
		/* If it is an answer, associate with the request or drop */
		if (!(hdr->msg_flags & CMD_FLAG_REQUEST)) {
			struct msg * req;
			/* Search matching request (same hbhid) */
			CHECK_FCT_DO( fd_p_sr_fetch(&peer->p_sr, hdr->msg_hbhid, &req), goto psm_end );
			if (req == NULL) {
				fd_hook_call(HOOK_MESSAGE_DROPPED, msg, peer, "Answer received with no corresponding sent request.", fd_msg_pmdl_get(msg));
				fd_msg_free(msg);
				goto psm_loop;
			}
			
			/* Associate */
			CHECK_FCT_DO( fd_msg_answ_associate( msg, req ), goto psm_end );
			
		}
		
		/* Log incoming message */
		fd_hook_call(HOOK_MESSAGE_RECEIVED, msg, peer, &rcv_data, fd_msg_pmdl_get(msg));
		
		if (cur_state == STATE_OPEN_NEW) {
			/* OK, we have received something, so the connection is supposedly now in OPEN state at the remote site */
			fd_psm_change_state(peer, STATE_OPEN );
		}
		
		/* Now handle non-link-local messages */
		if (fd_msg_is_routable(msg)) {
			switch (cur_state) {
				/* To maximize compatibility -- should not be a security issue here */
				case STATE_REOPEN:
				case STATE_SUSPECT:
				case STATE_CLOSING:
				case STATE_CLOSING_GRACE:
					TRACE_DEBUG(FULL, "Accepted a message while not in OPEN state... ");
				/* The standard situation : */
				case STATE_OPEN_NEW:
				case STATE_OPEN:
					/* We received a valid routable message, update the expiry timer */
					CHECK_FCT_DO( fd_p_expi_update(peer), goto psm_end );

					/* Set the message source and add the Route-Record */
					CHECK_FCT_DO( fd_msg_source_setrr( msg, peer->p_hdr.info.pi_diamid, peer->p_hdr.info.pi_diamidlen, fd_g_config->cnf_dict ), goto psm_end);

					if ((hdr->msg_flags & CMD_FLAG_REQUEST)) {
						/* Mark the incoming request so that we know we have pending answers for this peer */
						CHECK_POSIX_DO( pthread_mutex_lock(&peer->p_state_mtx), goto psm_end  );
						peer->p_reqin_count++;
						CHECK_POSIX_DO( pthread_mutex_unlock(&peer->p_state_mtx), goto psm_end  );
					}
						
					/* Requeue to the global incoming queue */
					CHECK_FCT_DO(fd_fifo_post(fd_g_incoming, &msg), goto psm_end );

					/* Update the peer timer (only in OPEN state) */
					if ((cur_state == STATE_OPEN) && (!peer->p_flags.pf_dw_pending)) {
						fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_twtimer ?: fd_g_config->cnf_timer_tw);
					}
					break;
					
				/* In other states, we discard the message, it is either old or invalid to send it for the remote peer */
				case STATE_WAITCNXACK:
				case STATE_WAITCNXACK_ELEC:
				case STATE_WAITCEA:
				case STATE_CLOSED:
				default: {
					/* In such case, just discard the message */
					char buf[128];
					snprintf(buf, sizeof(buf), "Received while peer state machine was in state %s.", STATE_STR(cur_state));
					fd_hook_call(HOOK_MESSAGE_DROPPED, msg, peer, buf, fd_msg_pmdl_get(msg));
					fd_msg_free(msg);
				}
			}
			goto psm_loop;
		}
		
		/* Link-local message: They must be understood by our dictionary, otherwise we return an error */
		{
			struct msg * error = NULL;
			int ret = fd_msg_parse_or_error( &msg, &error );
			if (ret != EBADMSG) {
				CHECK_FCT_DO( ret, 
					{
						char buf[256];
						snprintf(buf, sizeof(buf), "%s: An unexpected error occurred while parsing a link-local message", peer->p_hdr.info.pi_diamid); 
						fd_hook_call(HOOK_MESSAGE_DROPPED, msg, peer, buf, fd_msg_pmdl_get(msg));
						fd_msg_free(msg); 
						goto psm_end; 
					} );
			} else {
				if (msg == NULL) {
					/* Send the error back to the peer */
					CHECK_FCT_DO( ret = fd_out_send(&error, NULL, peer, 0),  );
					if (error) {
						char buf[256];
						/* Only if an error occurred & the message was not saved / dumped */
						snprintf(buf, sizeof(buf), "%s: error sending a message", peer->p_hdr.info.pi_diamid); 
						fd_hook_call(HOOK_MESSAGE_DROPPED, error, peer, buf, fd_msg_pmdl_get(error));
						CHECK_FCT_DO( fd_msg_free(error), goto psm_end);
					}
				} else {
					char buf[256];
					/* We received an invalid answer, let's disconnect */
					snprintf(buf, sizeof(buf), "%s: Received invalid answer to Base protocol message, disconnecting...", peer->p_hdr.info.pi_diamid);
					fd_hook_call(HOOK_MESSAGE_DROPPED, msg, peer, buf, fd_msg_pmdl_get(msg));
					CHECK_FCT_DO( fd_msg_free(msg), goto psm_end);
					CHECK_FCT_DO( fd_event_send(peer->p_events, FDEVP_CNX_ERROR, 0, NULL), goto psm_reset );
				}
				goto psm_loop;
			}
		}
		
		/* Handle the LL message and update the expiry timer appropriately */
		switch (hdr->msg_code) {
			case CC_CAPABILITIES_EXCHANGE:
				CHECK_FCT_DO( fd_p_ce_msgrcv(&msg, (hdr->msg_flags & CMD_FLAG_REQUEST), peer), 
					{
						if (msg)
							CHECK_FCT_DO( fd_msg_free(msg), );
						goto psm_reset;
					} );
				break;
			
			case CC_DISCONNECT_PEER:
				CHECK_FCT_DO( fd_p_dp_handle(&msg, (hdr->msg_flags & CMD_FLAG_REQUEST), peer), goto psm_reset );
				if (fd_peer_getstate(peer) == STATE_CLOSING)
				{
					fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Connection closed", NULL);
					goto psm_end;
				}

				break;
			
			case CC_DEVICE_WATCHDOG:
				CHECK_FCT_DO( fd_p_dw_handle(&msg, (hdr->msg_flags & CMD_FLAG_REQUEST), peer), goto psm_reset );
				break;
			
			default:
				/* Unknown / unexpected / invalid message -- but validated by our dictionary */
				TRACE_DEBUG(INFO, "Invalid non-routable command received: %u.", hdr->msg_code);
				if (hdr->msg_flags & CMD_FLAG_REQUEST) {
					do {
						/* Reply with an error code */
						CHECK_FCT_DO( fd_msg_new_answer_from_req ( fd_g_config->cnf_dict, &msg, MSGFL_ANSW_ERROR ), break );

						/* Set the error code */
						CHECK_FCT_DO( fd_msg_rescode_set(msg, "DIAMETER_COMMAND_UNSUPPORTED", "Or maybe the P-bit or application Id are erroneous.", NULL, 1 ), break );

						/* Send the answer */
						CHECK_FCT_DO( fd_out_send(&msg, peer->p_cnxctx, peer, 0), break );
					} while (0);
				} else {
					/* We did ASK for it ??? */
					TRACE_DEBUG(INFO, "Received answer with erroneous 'is_routable' result...");
				}
				
				/* Cleanup the message if not done */
				if (msg) {
					char buf[256];
					snprintf(buf, sizeof(buf), "Received un-handled non-routable command from peer '%s'.", peer->p_hdr.info.pi_diamid);
					fd_hook_call(HOOK_MESSAGE_DROPPED, msg, NULL, buf, fd_msg_pmdl_get(msg));
					CHECK_FCT_DO( fd_msg_free(msg), /* continue */);
					msg = NULL;
				}
		};
		
		/* At this point the message must have been fully handled already */
		if (msg) {
			char buf[256];
			snprintf(buf, sizeof(buf), "Internal error ('%s'): unhandled message.", peer->p_hdr.info.pi_diamid);
			fd_hook_call(HOOK_MESSAGE_DROPPED, msg, NULL, buf, fd_msg_pmdl_get(msg));
			fd_msg_free(msg);
		}
		
		goto psm_loop;
	}
	
	/* The connection object is broken */
	if (event == FDEVP_CNX_ERROR) {
		switch (cur_state) {
			case STATE_WAITCNXACK_ELEC:
				/* Abort the initiating side */
				fd_p_cnx_abort(peer, 0);
				/* Process the receiver side */
				CHECK_FCT_DO( fd_p_ce_process_receiver(peer), goto psm_end );
				break;
			
			case STATE_WAITCEA:
			case STATE_OPEN:
			case STATE_OPEN_NEW:
			case STATE_REOPEN:
			case STATE_WAITCNXACK:
			case STATE_SUSPECT:
			default:
				/* Mark the connection problem */
				peer->p_flags.pf_cnx_pb = 1;
			
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "The connection was broken", NULL);
				
				/* Destroy the connection, restart the timer to a new connection attempt */
				fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_tctimer ?: fd_g_config->cnf_timer_tc);
				
			case STATE_CLOSED:
				goto psm_reset;
				
			case STATE_CLOSING:
				/* We sent a DPR so we are terminating, do not wait for DPA */
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Connection closed", NULL);
				goto psm_end;
				
			case STATE_CLOSING_GRACE:
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Connection closed", NULL);
				if (peer->p_flags.pf_localterm) /* initiated here */
					goto psm_end;
				
				fd_psm_cleanup(peer, 0);
				
				/* Reset the timer for next connection attempt */
				fd_psm_next_timeout(peer, 1, fd_p_dp_newdelay(peer));
				goto psm_loop;
		}
		goto psm_loop;
	}
	
	/* The connection notified a change in endpoints */
	if (event == FDEVP_CNX_EP_CHANGE) {
		/* We actually don't care if we are in OPEN state here... */
		
		/* Cleanup the remote LL and primary addresses */
		CHECK_FCT_DO( fd_ep_filter( &peer->p_hdr.info.pi_endpoints, EP_FL_CONF | EP_FL_DISC | EP_FL_ADV ), /* ignore the error */);
		CHECK_FCT_DO( fd_ep_clearflags( &peer->p_hdr.info.pi_endpoints, EP_FL_PRIMARY ), /* ignore the error */);
		
		/* Get the new ones */
		CHECK_FCT_DO( fd_cnx_getremoteeps(peer->p_cnxctx, &peer->p_hdr.info.pi_endpoints), /* ignore the error */);
		
		/* We do not support local endpoints change currently, but it could be added here if needed (refresh fd_g_config->cnf_endpoints) */
		{
			char * buf = NULL;
			size_t len = 0;
			LOG_D("Got low layer notification (IGNORED): remote endpoint(s) changed: %s",  fd_ep_dump(&buf, &len, NULL, 0, 0, &peer->p_hdr.info.pi_endpoints) ?: "error");
			free(buf);
		}
		
		/* Done */
		goto psm_loop;
	}
	
	/* A new connection was established and CER containing this peer id was received */
	if (event == FDEVP_CNX_INCOMING) {
		struct cnx_incoming * params = ev_data;
		ASSERT(params);
		
		/* Handle the message */
		CHECK_FCT_DO( fd_p_ce_handle_newCER(&params->cer, peer, &params->cnx, params->validate), goto psm_end );
		
		/* Cleanup if needed */
		if (params->cnx) {
			fd_cnx_destroy(params->cnx);
			params->cnx = NULL;
		}
		if (params->cer) {
			CHECK_FCT_DO( fd_msg_free(params->cer), );
			params->cer = NULL;
		}
		
		/* Loop */
		free(ev_data);
		goto psm_loop;
	}
	
	/* A new connection has been established with the remote peer */
	if (event == FDEVP_CNX_ESTABLISHED) {
		struct cnxctx * cnx = ev_data;
		
		/* Release the resources of the connecting thread */
		CHECK_POSIX_DO( pthread_join( peer->p_ini_thr, NULL), /* ignore, it is not a big deal */);
		peer->p_ini_thr = (pthread_t)NULL;
		
		switch (cur_state) {
			case STATE_WAITCNXACK_ELEC:
			case STATE_WAITCNXACK:
				LOG_D("%s: Connection established, %s", peer->p_hdr.info.pi_diamid, fd_cnx_getid(cnx));
				fd_p_ce_handle_newcnx(peer, cnx);
				break;
				
			default:
				/* Just abort the attempt and continue */
				TRACE_DEBUG(FULL, "Connection attempt successful but current state is %s, closing... (too slow?)", STATE_STR(cur_state));
				fd_cnx_destroy(cnx);
		}
		
		goto psm_loop;
	}
	
	/* A new connection has not been established with the remote peer */
	if (event == FDEVP_CNX_FAILED) {
		
		/* Release the resources of the connecting thread */
		CHECK_POSIX_DO( pthread_join( peer->p_ini_thr, NULL), /* ignore, it is not a big deal */);
		peer->p_ini_thr = (pthread_t)NULL;
		
		switch (cur_state) {
			case STATE_WAITCNXACK_ELEC:
				/* Abort the initiating side */
				fd_p_cnx_abort(peer, 0);
				/* Process the receiver side */
				CHECK_FCT_DO( fd_p_ce_process_receiver(peer), goto psm_end );
				break;
				
			case STATE_WAITCNXACK:
				/* Go back to CLOSE */
				fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_tctimer ?: fd_g_config->cnf_timer_tc);
				goto psm_reset;
				
			default:
				/* Just ignore */
				TRACE_DEBUG(FULL, "Connection attempt failed but current state is %s, ignoring...", STATE_STR(cur_state));
		}
		
		goto psm_loop;
	}
	
	/* The timeout for the current state has been reached */
	if (event == FDEVP_PSM_TIMEOUT) {
		switch (cur_state) {
			case STATE_OPEN:
			case STATE_REOPEN:
			case STATE_OPEN_NEW:
				CHECK_FCT_DO( fd_p_dw_timeout(peer), goto psm_end );
				goto psm_loop;
				
			case STATE_CLOSED:
				LOG_D("%s: Connecting...", peer->p_hdr.info.pi_diamid);
				CHECK_FCT_DO( fd_psm_change_state(peer, STATE_WAITCNXACK), goto psm_end );
				fd_psm_next_timeout(peer, 0, CNX_TIMEOUT);
				CHECK_FCT_DO( fd_p_cnx_init(peer), goto psm_end );
				goto psm_loop;
				
			case STATE_SUSPECT:
				/* Mark the connection problem */
				peer->p_flags.pf_cnx_pb = 1;
			case STATE_WAITCNXACK:
			case STATE_WAITCEA:
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Timeout while waiting for remote peer", NULL);
				/* Destroy the connection, restart the timer to a new connection attempt */
				fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_tctimer ?: fd_g_config->cnf_timer_tc);
				goto psm_reset;

			case STATE_CLOSING:
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Connection closed", NULL);
				/* Destroy the connection, restart the timer to a new connection attempt */
				fd_psm_next_timeout(peer, 1, peer->p_hdr.info.config.pic_tctimer ?: fd_g_config->cnf_timer_tc);
				goto psm_reset;
				
			case STATE_CLOSING_GRACE:
				/* The grace period is completed, now close */
				fd_hook_call(HOOK_PEER_CONNECT_FAILED, NULL, peer, "Connection closed", NULL);
				if (peer->p_flags.pf_localterm)
					goto psm_end;
				
				fd_psm_cleanup(peer, 0);
				/* Reset the timer for next connection attempt */
				fd_psm_next_timeout(peer, 1, fd_p_dp_newdelay(peer));
				goto psm_loop;
				
			case STATE_WAITCNXACK_ELEC:
				/* Abort the initiating side */
				fd_p_cnx_abort(peer, 0);
				/* Process the receiver side */
				CHECK_FCT_DO( fd_p_ce_process_receiver(peer), goto psm_end );
				goto psm_loop;
			
			default:
				ASSERT(0); /* implementation problem, we did not foresee this case? */
		}
	}
	
	/* Default action : the handling has not yet been implemented. [for debug only] */
	TRACE_DEBUG(INFO, "Missing handler in PSM for '%s'\t<-- '%s'", STATE_STR(cur_state), fd_pev_str(event));
psm_reset:
	if (peer->p_flags.pf_delete)
		goto psm_end;
	fd_psm_cleanup(peer, 0);
	goto psm_loop;
	
psm_end:
	LOG_N("%s: Going to ZOMBIE state (no more activity)", peer->p_hdr.info.pi_diamid);
	fd_psm_cleanup(peer, 1);
	TRACE_DEBUG(INFO, "'%s'\t-> STATE_ZOMBIE (terminated)\t'%s'",
			STATE_STR(fd_peer_getstate(peer)),
			peer->p_hdr.info.pi_diamid);
	pthread_cleanup_pop(1); /* set STATE_ZOMBIE */
	peer->p_psm = (pthread_t)NULL;
	pthread_detach(pthread_self());
	return NULL;
}


/************************************************************************/
/*                      Functions to control the PSM                    */
/************************************************************************/
/* Create the PSM thread of one peer structure */
int fd_psm_begin(struct fd_peer * peer )
{
	TRACE_ENTRY("%p", peer);
	
	/* Check the peer and state are OK */
	CHECK_PARAMS( fd_peer_getstate(peer) == STATE_NEW );
	
	/* Create the FIFO for events */
	CHECK_FCT( fd_fifo_new(&peer->p_events, 0) );
	
	/* Create the PSM controler thread */
	CHECK_POSIX( pthread_create( &peer->p_psm, NULL, p_psm_th, peer ) );
	
	/* We're done */
	return 0;
}

/* End the PSM (clean ending) */
int fd_psm_terminate(struct fd_peer * peer, char * reason )
{
	TRACE_ENTRY("%p", peer);
	CHECK_PARAMS( CHECK_PEER(peer) );
	
	if (fd_peer_getstate(peer) != STATE_ZOMBIE) {
		CHECK_FCT( fd_event_send(peer->p_events, FDEVP_TERMINATE, 0, reason) );
	} else {
		TRACE_DEBUG(FULL, "Peer '%s' was already terminated", peer->p_hdr.info.pi_diamid);
	}
	return 0;
}

/* End the PSM & cleanup the peer structure */
void fd_psm_abord(struct fd_peer * peer )
{
	TRACE_ENTRY("%p", peer);
	
	/* Cancel PSM thread */
	CHECK_FCT_DO( fd_thr_term(&peer->p_psm), /* continue */ );
	
	/* Cleanup the data */
	fd_psm_cleanup(peer, 1);
	
	/* Destroy the event list */
	CHECK_FCT_DO( fd_fifo_del(&peer->p_events), /* continue */ );
	
	/* Remaining cleanups are performed in fd_peer_free */
	return;
}

