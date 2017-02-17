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

/* Expiration management */


/* Entries by their ascending expiration date, to accelerate the work of the expire thread */
static struct fd_list  expire_list = FD_LIST_INITIALIZER(expire_list);
static pthread_cond_t  exp_cnd  = PTHREAD_COND_INITIALIZER; 

pthread_mutex_t redir_exp_peer_lock = PTHREAD_MUTEX_INITIALIZER;

/* The thread that handles expired entries cleanup. */
void * redir_exp_thr_fct(void * arg) 
{
	fd_log_threadname ( "Redirects/expire" );
	TRACE_ENTRY( "" );

	CHECK_POSIX_DO( pthread_mutex_lock(&redir_exp_peer_lock),  goto fatal_error );
	pthread_cleanup_push( fd_cleanup_mutex, &redir_exp_peer_lock );
	
	do {
		struct timespec	now;
		struct redir_entry * first;
again:		
		/* Check if there are expiring entries available */
		if (FD_IS_LIST_EMPTY(&expire_list)) {
			/* Just wait for a change or cancelation */
			CHECK_POSIX_DO( pthread_cond_wait( &exp_cnd, &redir_exp_peer_lock ), break /* this might not pop the cleanup handler, but since we ASSERT(0), it is not the big issue... */ );
			/* Restart the loop on wakeup */
			goto again;
		}
		
		/* Get the pointer to the entry that expires first */
		first = (struct redir_entry *)(expire_list.next->o);
		
		/* Get the current time */
		CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now),  break  );

		/* If first session is not expired, we just wait until it happens */
		if ( TS_IS_INFERIOR( &now, &first->timeout ) ) {
			
			CHECK_POSIX_DO2(  pthread_cond_timedwait( &exp_cnd, &redir_exp_peer_lock, &first->timeout ),  
					ETIMEDOUT, /* ETIMEDOUT is a normal error, continue */,
					/* on other error, */ break );
	
			/* on wakeup, loop */
			goto again;
		}
		
		/* Now, the first entry in the list is expired; destroy it */
		
		CHECK_FCT_DO( redir_entry_destroy( first ), break );
		
	} while (1);
	
	pthread_cleanup_pop( 0 );
	CHECK_POSIX_DO( pthread_mutex_unlock(&redir_exp_peer_lock),  );
	
fatal_error:
	TRACE_DEBUG(INFO, "A system error occurred in redirect module! Expiry thread is terminating...");
	ASSERT(0);
	return NULL;
}

/* Sets the timeout value & link in expiry list. The mutex must be held on calling */
int redir_exp_set(struct redir_entry * e, uint32_t duration)
{
	struct fd_list * li;
	TRACE_ENTRY("%p %d", e, duration);
	CHECK_PARAMS(e && (e->eyec == REDIR_ENTRY_EYEC) && duration );
	
	/* Unlink in case it was already set before */
	fd_list_unlink(&e->exp_list);
	
	/* Get current time */
	CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &e->timeout)  );
	
	/* Add the duration */
	e->timeout.tv_sec += duration;
	
	/* now search the next element in the list */
	for (li = expire_list.next; li != &expire_list; li = li->next) {
		struct redir_entry * n = li->o;
		
		if ( TS_IS_INFERIOR( &e->timeout, &n->timeout ) )
			break;
	
	}
	
	/* Insert before this element */
	fd_list_insert_before(li, &e->exp_list);
	
	/* Signal the expiry thread if needed */
	if (e->exp_list.prev == &expire_list) { /* it is the first element */
		CHECK_POSIX( pthread_cond_signal(&exp_cnd) );
	}
	
	/* Done */
	return 0;
}

