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

/* Delay for garbage collection of expired peers, in seconds */
#define GC_TIME		120

static pthread_t       exp_thr = (pthread_t)NULL;
static pthread_t       gc_thr  = (pthread_t)NULL;
static struct fd_list  exp_list = FD_LIST_INITIALIZER( exp_list );
static pthread_cond_t  exp_cnd  = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t exp_mtx  = PTHREAD_MUTEX_INITIALIZER;

static void * gc_th_fct(void * arg)
{
	fd_log_threadname ( "Peers/garb. col." );
	TRACE_ENTRY( "%p", arg );
	
	do {
		struct fd_list * li, purge = FD_LIST_INITIALIZER(purge);
		
		sleep(GC_TIME);	/* sleep is a cancellation point */
		
		/* Now check in the peers list if any peer can be deleted */
		CHECK_FCT_DO( pthread_rwlock_wrlock(&fd_g_peers_rw), goto error );
		
		for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
			struct fd_peer * peer = (struct fd_peer *)li->o;
			
			if (fd_peer_getstate(peer) != STATE_ZOMBIE)
				continue;
			
			if (peer->p_hdr.info.config.pic_flags.persist == PI_PRST_ALWAYS)
				continue; /* This peer was not supposed to terminate, keep it in the list for debug */
			
			/* Ok, the peer was expired, let's remove it */
			li = li->prev; /* to avoid breaking the loop */
			fd_list_unlink(&peer->p_hdr.chain);
			fd_list_insert_before(&purge, &peer->p_hdr.chain);
		}

		CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), goto error );
		
		/* Now delete peers that are in the purge list */
		while (!FD_IS_LIST_EMPTY(&purge)) {
			struct fd_peer * peer = (struct fd_peer *)(purge.next->o);
			fd_list_unlink(&peer->p_hdr.chain);
			TRACE_DEBUG(INFO, "Garbage Collect: delete zombie peer '%s'", peer->p_hdr.info.pi_diamid);
			CHECK_FCT_DO( fd_peer_free(&peer), /* Continue... what else to do ? */ );
		}
	} while (1);
	
error:
	TRACE_DEBUG(INFO, "An error occurred in peers module! GC thread is terminating...");
	ASSERT(0);
	CHECK_FCT_DO(fd_core_shutdown(), );
	return NULL;
}


static void * exp_th_fct(void * arg)
{
	fd_log_threadname ( "Peers/expire" );
	TRACE_ENTRY( "%p", arg );
	
	CHECK_POSIX_DO( pthread_mutex_lock(&exp_mtx), { ASSERT(0); } );
	pthread_cleanup_push( fd_cleanup_mutex, &exp_mtx );
	
	do {
		struct timespec	now;
		struct fd_peer * first;
		
		/* Check if there are expiring peers available */
		if (FD_IS_LIST_EMPTY(&exp_list)) {
			/* Just wait for a change or cancelation */
			CHECK_POSIX_DO( pthread_cond_wait( &exp_cnd, &exp_mtx ), { ASSERT(0); } );
			/* Restart the loop on wakeup */
			continue;
		}
		
		/* Get the pointer to the peer that expires first */
		first = (struct fd_peer *)(exp_list.next->o);
		ASSERT( CHECK_PEER(first) );
		
		/* Get the current time */
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now),  { ASSERT(0); }  );

		/* If first peer is not expired, we just wait until it happens */
		if ( TS_IS_INFERIOR( &now, &first->p_exp_timer ) ) {
			
			CHECK_POSIX_DO2(  pthread_cond_timedwait( &exp_cnd, &exp_mtx, &first->p_exp_timer ),  
					ETIMEDOUT, /* ETIMEDOUT is a normal return value, continue */,
					/* on other error, */ { ASSERT(0); } );
	
			/* on wakeup, loop */
			continue;
		}
		
		/* Now, the first peer in the list is expired; signal it */
		fd_list_unlink( &first->p_expiry );
		CHECK_FCT_DO( fd_event_send(first->p_events, FDEVP_TERMINATE, 0, "DO_NOT_WANT_TO_TALK_TO_YOU"), break );
		
	} while (1);
	
	pthread_cleanup_pop( 1 );

	TRACE_DEBUG(INFO, "An error occurred in peers module! Expiry thread is terminating...");
	CHECK_FCT_DO(fd_core_shutdown(), );
	return NULL;
}

/* Initialize peers expiry mechanism */
int fd_p_expi_init(void)
{
	TRACE_ENTRY();
	CHECK_FCT( pthread_create( &exp_thr, NULL, exp_th_fct, NULL ) );
	CHECK_FCT( pthread_create( &gc_thr,  NULL, gc_th_fct,  NULL ) );
	return 0;
}

/* Finish peers expiry mechanism */
int fd_p_expi_fini(void)
{
	CHECK_FCT_DO( fd_thr_term(&exp_thr), );
	CHECK_POSIX( pthread_mutex_lock(&exp_mtx) );
	while (!FD_IS_LIST_EMPTY(&exp_list)) {
		struct fd_peer * peer = (struct fd_peer *)(exp_list.next->o);
		fd_list_unlink(&peer->p_expiry );
	}
	CHECK_POSIX( pthread_mutex_unlock(&exp_mtx) );
	
	CHECK_FCT_DO( fd_thr_term(&gc_thr), );
	return 0;
}

/* Add / requeue a peer in the expiry list */
int fd_p_expi_update(struct fd_peer * peer )
{
	TRACE_ENTRY("%p", peer);
	CHECK_PARAMS( CHECK_PEER(peer) );
	
	CHECK_POSIX( pthread_mutex_lock(&exp_mtx) );
	
	fd_list_unlink(&peer->p_expiry );
	
	/* if peer expires */
	if (peer->p_hdr.info.config.pic_flags.exp) {
		struct fd_list * li;
		
		/* update the p_exp_timer value */
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &peer->p_exp_timer), { ASSERT(0); }  );
		peer->p_exp_timer.tv_sec += peer->p_hdr.info.config.pic_lft;
		
		/* add to the expiry list in appropriate position (probably around the end) */
		for (li = exp_list.prev; li != &exp_list; li = li->prev) {
			struct fd_peer * p = (struct fd_peer *)(li->o);
			if (TS_IS_INFERIOR( &p->p_exp_timer, &peer->p_exp_timer ) )
				break;
		}
		
		fd_list_insert_after(li, &peer->p_expiry);
		
		/* signal the expiry thread if we added in first position */
		if (li == &exp_list) {
			CHECK_POSIX( pthread_cond_signal(&exp_cnd) );
		}
	}
	
	CHECK_POSIX( pthread_mutex_unlock(&exp_mtx) );
	return 0;
}

