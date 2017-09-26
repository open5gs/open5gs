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

/* Structure to store a sent request */
struct sentreq {
	struct fd_list	chain; 	/* the "o" field points directly to the (new) hop-by-hop of the request (uint32_t *)  */
	struct msg	*req;	/* A request that was sent and not yet answered. */
	uint32_t	prevhbh;/* The value to set back in the hbh header when the message is retrieved */
	struct fd_list  expire; /* the list of expiring requests */
	struct timespec timeout; /* Cache the expire date of the request so that the timeout thread does not need to get it each time. */
	struct timespec added_on; /* the time the request was added */
};

/* Find an element in the hbh list, or the following one */
static struct fd_list * find_or_next(struct fd_list * srlist, uint32_t hbh, int * match)
{
	struct fd_list * li;
	*match = 0;
	for (li = srlist->next; li != srlist; li = li->next) {
		uint32_t * nexthbh = li->o;
		if (*nexthbh < hbh)
			continue;
		if (*nexthbh == hbh)
			*match = 1;
		break;
	}
	return li;
}

/* Similar but start from the end, since we add requests in growing hbh order usually */
static struct fd_list * find_or_prev(struct fd_list * srlist, uint32_t hbh, int * match)
{
	struct fd_list * li;
	*match = 0;
	for (li = srlist->prev; li != srlist; li = li->prev) {
		uint32_t * prevhbh = li->o;
		if (*prevhbh > hbh)
			continue;
		if (*prevhbh == hbh)
			*match = 1;
		break;
	}
	return li;
}

static void srl_dump(const char * text, struct fd_list * srlist)
{
	struct fd_list * li;
	struct timespec now;
	
	LOG_D("%sSentReq list @%p:", text, srlist);
	
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );
	
	for (li = srlist->next; li != srlist; li = li->next) {
		struct sentreq * sr = (struct sentreq *)li;
		uint32_t * nexthbh = li->o;
		
		LOG_D(" - Next req (hbh:0x%x, prev:0x%x): [since %ld.%06ld sec]", *nexthbh, sr->prevhbh,
			(long)((now.tv_nsec >= sr->added_on.tv_nsec) ? (now.tv_sec - sr->added_on.tv_sec) : (now.tv_sec - sr->added_on.tv_sec - 1)),
			(long)((now.tv_nsec >= sr->added_on.tv_nsec) ? ((now.tv_nsec - sr->added_on.tv_nsec) / 1000) : ((now.tv_nsec - sr->added_on.tv_nsec + 1000000000) / 1000)));
	}
}

/* thread that handles messages expiring. The thread is started only when needed */
static void * sr_expiry_th(void * arg) {
	struct sr_list * srlist = arg;
	
	TRACE_ENTRY("%p", arg);
	CHECK_PARAMS_DO( arg, return NULL );
	
	/* Set the thread name */
	{
		char buf[48];
		snprintf(buf, sizeof(buf), "ReqExp/%s", ((struct fd_peer *)(srlist->exp.o))->p_hdr.info.pi_diamid);
		fd_log_threadname ( buf );
	}
	
	do {
		struct timespec	now;
		struct sentreq * first;
		struct msg * request;
		struct fd_peer * sentto;
		void (*expirecb)(void *, DiamId_t, size_t, struct msg **);
		void * data;
		int no_error;

		CHECK_POSIX_DO( pthread_mutex_lock(&srlist->mtx),  return NULL );
		pthread_cleanup_push( fd_cleanup_mutex, &srlist->mtx );

loop:	
		no_error = 0;

		/* Check if there are expiring requests available */
		if (FD_IS_LIST_EMPTY(&srlist->exp)) {
			/* Just wait for a change or cancelation */
			CHECK_POSIX_DO( pthread_cond_wait( &srlist->cnd, &srlist->mtx ), goto unlock );
			/* Restart the loop on wakeup */
			goto loop;
		}
		
		/* Get the pointer to the request that expires first */
		first = (struct sentreq *)(srlist->exp.next->o);
		
		/* Get the current time */
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now),  goto unlock  );

		/* If first request is not expired, we just wait until it happens */
		if ( TS_IS_INFERIOR( &now, &first->timeout ) ) {
			
			CHECK_POSIX_DO2(  pthread_cond_timedwait( &srlist->cnd, &srlist->mtx, &first->timeout ),  
					ETIMEDOUT, /* ETIMEDOUT is a normal return value, continue */,
					/* on other error, */ goto unlock );
	
			/* on wakeup, loop */
			goto loop;
		}
		
		/* Now, the first request in the list is expired; remove it and call the expirecb for it */
		request = first->req;
		sentto = first->chain.head->o;
		
		TRACE_DEBUG(FULL, "Request %x was not answered by %s within the timer delay", *((uint32_t *)first->chain.o), sentto->p_hdr.info.pi_diamid);
		
		/* Restore the hbhid */
		*((uint32_t *)first->chain.o) = first->prevhbh; 
		
		/* Free the sentreq information */
		fd_list_unlink(&first->chain);
		srlist->cnt--;
		srlist->cnt_lost++; /* We are not waiting for this answer anymore, but the remote peer may still be processing it. */
		fd_list_unlink(&first->expire);
		free(first);
		
		no_error = 1;
unlock:
		; /* pthread_cleanup_pop sometimes expands as "} ..." and the label before this cause some compilers to complain... */
		pthread_cleanup_pop( 1 ); /* unlock the mutex */
		if (!no_error)
			break;

		
		/* Retrieve callback in the message */
		CHECK_FCT_DO( fd_msg_anscb_get( request, NULL, &expirecb, &data ), break);
		ASSERT(expirecb);
	
		/* Clean up this expirecb from the message */
		CHECK_FCT_DO( fd_msg_anscb_reset( request, 0, 1 ), break);

		/* Call it */
		(*expirecb)(data, sentto->p_hdr.info.pi_diamid, sentto->p_hdr.info.pi_diamidlen, &request);
	
		/* If the callback did not dispose of the message, do it now */
		if (request) {
			fd_hook_call(HOOK_MESSAGE_DROPPED, request, NULL, "Expiration period completed without an answer, and the expiry callback did not dispose of the message.", fd_msg_pmdl_get(request));
			CHECK_FCT_DO( fd_msg_free(request), /* ignore */ );
		}
	
	} while (1);
	
	ASSERT(0); /* we have encountered a problem, maybe time to signal the framework to terminate? */
	return NULL;
}


/* Store a new sent request */
int fd_p_sr_store(struct sr_list * srlist, struct msg **req, uint32_t *hbhloc, uint32_t hbh_restore)
{
	struct sentreq * sr;
	struct fd_list * prev;
	int match;
	struct timespec * ts;
	
	TRACE_ENTRY("%p %p %p %x", srlist, req, hbhloc, hbh_restore);
	CHECK_PARAMS(srlist && req && *req && hbhloc);
	
	CHECK_MALLOC( sr = malloc(sizeof(struct sentreq)) );
	memset(sr, 0, sizeof(struct sentreq));
	fd_list_init(&sr->chain, hbhloc);
	sr->req = *req;
	sr->prevhbh = hbh_restore;
	fd_list_init(&sr->expire, sr);
	CHECK_SYS( clock_gettime(CLOCK_REALTIME, &sr->added_on) );
	
	/* Search the place in the list */
	CHECK_POSIX( pthread_mutex_lock(&srlist->mtx) );
	prev = find_or_prev(&srlist->srs, *hbhloc, &match);
	if (match) {
		TRACE_DEBUG(INFO, "A request with the same hop-by-hop Id (0x%x) was already sent: error", *hbhloc);
		free(sr);
		srl_dump("Current list of SR: ", &srlist->srs);
		CHECK_POSIX_DO( pthread_mutex_unlock(&srlist->mtx), /* ignore */ );
		return EINVAL;
	}
	
	/* Save in the list */
	*req = NULL;
	fd_list_insert_after(prev, &sr->chain);
	srlist->cnt++;
	
	/* In case of request with a timeout, also store in the timeout list */
	ts = fd_msg_anscb_gettimeout( sr->req );
	if (ts) {
		struct fd_list * li;
		
		memcpy(&sr->timeout, ts, sizeof(struct timespec));
		
		/* browse srlist->exp from the end */
		for (li = srlist->exp.prev; li != &srlist->exp; li = li->prev) {
			struct sentreq * s = (struct sentreq *)(li->o);
			if (TS_IS_INFERIOR(&s->timeout, ts))
				break;
		}
		
		fd_list_insert_after(li, &sr->expire);
	
		/* if the thread does not exist yet, create it */
		if (srlist->thr == (pthread_t)NULL) {
			CHECK_POSIX_DO( pthread_create(&srlist->thr, NULL, sr_expiry_th, srlist), /* continue anyway */);
		} else {
			/* or, if added in first position, signal the condvar to update the sleep time of the thread */
			if (li == &srlist->exp) {
				CHECK_POSIX_DO( pthread_cond_signal(&srlist->cnd), /* continue anyway */);
			}
		}
	}
	
	CHECK_POSIX( pthread_mutex_unlock(&srlist->mtx) );
	return 0;
}

/* Fetch a request by hbh */
int fd_p_sr_fetch(struct sr_list * srlist, uint32_t hbh, struct msg **req)
{
	struct sentreq * sr;
	int match;
	
	TRACE_ENTRY("%p %x %p", srlist, hbh, req);
	CHECK_PARAMS(srlist && req);
	
	/* Search the request in the list */
	CHECK_POSIX( pthread_mutex_lock(&srlist->mtx) );
	sr = (struct sentreq *)find_or_next(&srlist->srs, hbh, &match);
	if (!match) {
		TRACE_DEBUG(INFO, "There is no saved request with this hop-by-hop id (%x)", hbh);
		srl_dump("Current list of SR: ", &srlist->srs);
		*req = NULL;
		if (srlist->cnt_lost > 0) {
			srlist->cnt_lost--; /* This is probably an answer for a request we already timedout. */
		} /* else, probably a bug in the remote peer */
	} else {
		/* Restore hop-by-hop id */
		*((uint32_t *)sr->chain.o) = sr->prevhbh;
		/* Unlink */
		fd_list_unlink(&sr->chain);
		srlist->cnt--;
		fd_list_unlink(&sr->expire);
		*req = sr->req;
		free(sr);
	}
	CHECK_POSIX( pthread_mutex_unlock(&srlist->mtx) );
	
	/* do not stop the expire thread here, it might cause creating/destroying it very often otherwise */

	/* Done */
	return 0;
}

/* Failover requests (free or requeue routables) */
void fd_p_sr_failover(struct sr_list * srlist)
{
	CHECK_POSIX_DO( pthread_mutex_lock(&srlist->mtx), /* continue anyway */ );
	while (!FD_IS_LIST_EMPTY(&srlist->srs)) {
		struct sentreq * sr = (struct sentreq *)(srlist->srs.next);
		fd_list_unlink(&sr->chain);
		srlist->cnt--;
		fd_list_unlink(&sr->expire);
		if (fd_msg_is_routable(sr->req)) {
			struct msg_hdr * hdr = NULL;
			int ret;
			
			/* Set the 'T' flag */
			CHECK_FCT_DO(fd_msg_hdr(sr->req, &hdr), /* continue */);
			if (hdr)
				hdr->msg_flags |= CMD_FLAG_RETRANSMIT;
			
			/* Restore the original hop-by-hop id of the request */
			*((uint32_t *)sr->chain.o) = sr->prevhbh;
			
			fd_hook_call(HOOK_MESSAGE_FAILOVER, sr->req, (struct fd_peer *)srlist->srs.o, NULL, fd_msg_pmdl_get(sr->req));
			
			/* Requeue for sending to another peer */
			CHECK_FCT_DO( ret = fd_fifo_post_noblock(fd_g_outgoing, (void *)&sr->req),
				{
					char buf[256];
					snprintf(buf, sizeof(buf), "Internal error: error while requeuing during failover: %s", strerror(ret));
					fd_hook_call(HOOK_MESSAGE_DROPPED, sr->req, NULL, buf, fd_msg_pmdl_get(sr->req));
					CHECK_FCT_DO(fd_msg_free(sr->req), /* What can we do more? */)
				});
		} else {
			/* Just free the request. */
			/* fd_hook_call(HOOK_MESSAGE_DROPPED, sr->req, NULL, "Sent & unanswered local message discarded during failover.", fd_msg_pmdl_get(sr->req)); */
			CHECK_FCT_DO(fd_msg_free(sr->req), /* Ignore */);
		}
		free(sr);
	}
	/* The list of expiring requests must be empty now */
	ASSERT( FD_IS_LIST_EMPTY(&srlist->exp) );
	ASSERT( srlist->cnt == 0 ); /* debug the counter management if needed */
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&srlist->mtx), /* continue anyway */ );
	
	/* Terminate the expiry thread (must be done when the lock can be taken) */
	CHECK_FCT_DO( fd_thr_term(&srlist->thr), /* ignore error */ );
}

