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

/* Alloc a new hbh for requests, bufferize the message and send on the connection, save in sentreq if provided */
static int do_send(struct msg ** msg, struct cnxctx * cnx, uint32_t * hbh, struct fd_peer * peer)
{
	struct msg_hdr * hdr;
	int msg_is_a_req;
	uint8_t * buf;
	size_t sz;
	int ret;
	uint32_t bkp_hbh = 0;
	struct msg *cpy_for_logs_only;
	struct fd_cnx_rcvdata msgdata;
	
	TRACE_ENTRY("%p %p %p %p", msg, cnx, hbh, peer);
	
	/* Retrieve the message header */
	CHECK_FCT( fd_msg_hdr(*msg, &hdr) );
	
	msg_is_a_req = (hdr->msg_flags & CMD_FLAG_REQUEST);
	if (msg_is_a_req) {
		CHECK_PARAMS(hbh && peer);
		/* Alloc the hop-by-hop id and increment the value for next message */
		bkp_hbh = hdr->msg_hbhid;
		hdr->msg_hbhid = *hbh;
		*hbh = hdr->msg_hbhid + 1;
	}

	fd_hook_call(HOOK_MESSAGE_PRE_SEND, *msg, peer, &msgdata, fd_msg_pmdl_get(*msg));
	
	/* Create the message buffer */
	CHECK_FCT(fd_msg_bufferize( *msg, &buf, &sz ));
	pthread_cleanup_push( free, buf );
	
	cpy_for_logs_only = *msg;
	
	/* Save a request before sending so that there is no race condition with the answer */
	if (msg_is_a_req) {
		CHECK_FCT_DO( ret = fd_p_sr_store(&peer->p_sr, msg, &hdr->msg_hbhid, bkp_hbh), goto out );
	}
	
	/* Log the message */
	msgdata.length = sz;
	msgdata.buffer = buf;
	fd_hook_call(HOOK_MESSAGE_SENT, cpy_for_logs_only, peer, &msgdata, fd_msg_pmdl_get(cpy_for_logs_only));
	
	pthread_cleanup_push((void *)fd_msg_free, *msg /* might be NULL, no problem */);
	
	/* Send the message */
	CHECK_FCT_DO( ret = fd_cnx_send(cnx, buf, sz), );
	
	pthread_cleanup_pop(0);
	
out:
	;	
	pthread_cleanup_pop(1);
	
	if (ret)
		return ret;
	
	/* Free remaining messages (i.e. answers) */
	if (*msg) {
		CHECK_FCT( fd_msg_free(*msg) );
		*msg = NULL;
	}
	
	return 0;
}

/* The code of the "out" thread */
static void * out_thr(void * arg)
{
	struct fd_peer * peer = arg;
	int stop = 0;
	struct msg * msg;
	ASSERT( CHECK_PEER(peer) );
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "OUT/%s", peer->p_hdr.info.pi_diamid);
		fd_log_threadname ( buf );
	}
	
	/* Loop until cancelation */
	while (!stop) {
		int ret;
		
		/* Retrieve next message to send */
		CHECK_FCT_DO( fd_fifo_get(peer->p_tosend, &msg), goto error );
		
		/* Send the message, log any error */
		CHECK_FCT_DO( ret = do_send(&msg, peer->p_cnxctx, &peer->p_hbh, peer),
			{
				if (msg) {
					char buf[256];
					snprintf(buf, sizeof(buf), "Error while sending this message: %s", strerror(ret));
					fd_hook_call(HOOK_MESSAGE_DROPPED, msg, NULL, buf, fd_msg_pmdl_get(msg));
					fd_msg_free(msg);
				}
				stop = 1;
			} );
			
	}
	
	/* If we're here it means there was an error on the socket. We need to continue to purge the fifo & until we are canceled */
	CHECK_FCT_DO( fd_event_send(peer->p_events, FDEVP_CNX_ERROR, 0, NULL), /* What do we do if it fails? */ );
	
	/* Requeue all routable messages in the global "out" queue, until we are canceled once the PSM deals with the CNX_ERROR sent above */
	while ( fd_fifo_get(peer->p_tosend, &msg) == 0 ) {
		if (fd_msg_is_routable(msg)) {
			CHECK_FCT_DO(fd_fifo_post_noblock(peer->p_tofailover, (void *)&msg), 
				{
					/* fallback: destroy the message */
					fd_hook_call(HOOK_MESSAGE_DROPPED, msg, NULL, "Internal error: unable to requeue this message during failover process", fd_msg_pmdl_get(msg));
					CHECK_FCT_DO(fd_msg_free(msg), /* What can we do more? */)
				} );
		} else {
			/* Just free it */
			/* fd_hook_call(HOOK_MESSAGE_DROPPED, m, NULL, "Non-routable message freed during handover", fd_msg_pmdl_get(m)); */
			CHECK_FCT_DO(fd_msg_free(msg), /* What can we do more? */)
		}
	}

error:
	/* It is not really a connection error, but the effect is the same, we are not able to send anymore message */
	CHECK_FCT_DO( fd_event_send(peer->p_events, FDEVP_CNX_ERROR, 0, NULL), /* What do we do if it fails? */ );
	return NULL;
}

/* Wrapper to sending a message either by out thread (peer in OPEN state) or directly; cnx or peer must be provided. Flags are valid only for direct sending, not through thread (unused) */
int fd_out_send(struct msg ** msg, struct cnxctx * cnx, struct fd_peer * peer, int update_reqin_cnt)
{
	struct msg_hdr * hdr;
	
	TRACE_ENTRY("%p %p %p", msg, cnx, peer);
	CHECK_PARAMS( msg && *msg && (cnx || (peer && peer->p_cnxctx)));
	
	if (update_reqin_cnt && peer) {
		CHECK_FCT( fd_msg_hdr(*msg, &hdr) );
		if (!(hdr->msg_flags & CMD_FLAG_REQUEST)) {
			/* Update the count of pending answers to send */
			CHECK_POSIX( pthread_mutex_lock(&peer->p_state_mtx) );
			peer->p_reqin_count--;
			CHECK_POSIX( pthread_mutex_unlock(&peer->p_state_mtx) );			
		}
	}
	
	if (fd_peer_getstate(peer) == STATE_OPEN) {
		/* Normal case: just queue for the out thread to pick it up */
		CHECK_FCT( fd_fifo_post(peer->p_tosend, msg) );
		
	} else {
		int ret;
		uint32_t *hbh = NULL;
		
		/* In other cases, the thread is not running, so we handle the sending directly */
		if (peer)
			hbh = &peer->p_hbh;

		if (!cnx)
			cnx = peer->p_cnxctx;

		/* Do send the message */
		CHECK_FCT_DO( ret = do_send(msg, cnx, hbh, peer),
			{
				if (msg) {
					char buf[256];
					snprintf(buf, sizeof(buf), "Error while sending this message: %s", strerror(ret));
					fd_hook_call(HOOK_MESSAGE_DROPPED, *msg, NULL, buf, fd_msg_pmdl_get(*msg));
					fd_msg_free(*msg);
					*msg = NULL;
				}
			} );
	}
	
	return 0;
}

/* Start the "out" thread that picks messages in p_tosend and send them on p_cnxctx */
int fd_out_start(struct fd_peer * peer)
{
	TRACE_ENTRY("%p", peer);
	CHECK_PARAMS( CHECK_PEER(peer) && (peer->p_outthr == (pthread_t)NULL) );
	
	CHECK_POSIX( pthread_create(&peer->p_outthr, NULL, out_thr, peer) );
	
	CHECK_FCT( fd_cnx_unordered_delivery(peer->p_cnxctx, 1) );
	
	return 0;
}

/* Stop that thread */
int fd_out_stop(struct fd_peer * peer)
{
	TRACE_ENTRY("%p", peer);
	CHECK_PARAMS( CHECK_PEER(peer) );
	
	CHECK_FCT( fd_cnx_unordered_delivery(peer->p_cnxctx, 0) );
	
	CHECK_FCT( fd_thr_term(&peer->p_outthr) );
	
	return 0;
}
		
