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

/* FIFO queues module.
 *
 * The threads that call these functions must be in the cancellation state PTHREAD_CANCEL_ENABLE and type PTHREAD_CANCEL_DEFERRED.
 * This is the default state and type on thread creation.
 *
 * In order to destroy properly a queue, the application must:
 *  -> shutdown any process that can add into the queue first.
 *  -> pthread_cancel any thread that could be waiting on the queue.
 *  -> consume any element that is in the queue, using fd_qu_tryget_int.
 *  -> then destroy the queue using fd_mq_del.
 */

#include "fdproto-internal.h"

/* Definition of a FIFO queue object */
struct fifo {
	int		eyec;	/* An eye catcher, also used to check a queue is valid. FIFO_EYEC */
	
	pthread_mutex_t	mtx;	/* Mutex protecting this queue */
	pthread_cond_t	cond_pull;	/* condition variable for pulling threads */
	pthread_cond_t	cond_push;	/* condition variable for pushing threads */
	
	struct fd_list	list;	/* sentinel for the list of elements */
	int		count;	/* number of objects in the list */
	int		thrs;	/* number of threads waiting for a new element (when count is 0) */
	
	int 		max;	/* maximum number of items to accept if not 0 */
	int		thrs_push; /* number of threads waitnig to push an item */
	
	uint16_t	high;	/* High level threshold (see libfreeDiameter.h for details) */
	uint16_t	low;	/* Low level threshhold */
	void 		*data;	/* Opaque pointer for threshold callbacks */
	void		(*h_cb)(struct fifo *, void **); /* The callbacks */
	void		(*l_cb)(struct fifo *, void **);
	int 		highest;/* The highest count value for which h_cb has been called */
	int		highest_ever; /* The max count value this queue has reached (for tweaking) */
	
	long long	total_items;   /* Cumulated number of items that went through this fifo (excluding current count), always increasing. */
	struct timespec total_time;    /* Cumulated time all items spent in this queue, including blocking time (always growing, use deltas for monitoring) */
	struct timespec blocking_time; /* Cumulated time threads trying to post new items were blocked (queue full). */
	struct timespec last_time;     /* For the last element retrieved from the queue, how long it take between posting (including blocking) and poping */
	
};

struct fifo_item {
	struct fd_list   item;
	struct timespec  posted_on;
};

/* The eye catcher value */
#define FIFO_EYEC	0xe7ec1130

/* Macro to check a pointer */
#define CHECK_FIFO( _queue ) (( (_queue) != NULL) && ( (_queue)->eyec == FIFO_EYEC) )


/* Create a new queue, with max number of items -- use 0 for no max */
int fd_fifo_new ( struct fifo ** queue, int max )
{
	struct fifo * new;
	
	TRACE_ENTRY( "%p", queue );
	
	CHECK_PARAMS( queue );
	
	/* Create a new object */
	CHECK_MALLOC( new = malloc (sizeof (struct fifo) )  );
	
	/* Initialize the content */
	memset(new, 0, sizeof(struct fifo));
	
	new->eyec = FIFO_EYEC;
	CHECK_POSIX( pthread_mutex_init(&new->mtx, NULL) );
	CHECK_POSIX( pthread_cond_init(&new->cond_pull, NULL) );
	CHECK_POSIX( pthread_cond_init(&new->cond_push, NULL) );
	new->max = max;
	
	fd_list_init(&new->list, NULL);
	
	/* We're done */
	*queue = new;
	return 0;
}

/* Dump the content of a queue */
DECLARE_FD_DUMP_PROTOTYPE(fd_fifo_dump, char * name, struct fifo * queue, fd_fifo_dump_item_cb dump_item)
{
	FD_DUMP_HANDLE_OFFSET();
	
	if (name) {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "'%s'(@%p): ", name, queue), return NULL);	
	} else {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{fifo}(@%p): ", queue), return NULL);
	}
	
	if (!CHECK_FIFO( queue )) {
		return fd_dump_extend(FD_DUMP_STD_PARAMS, "INVALID/NULL");
	}
	
	CHECK_POSIX_DO(  pthread_mutex_lock( &queue->mtx ), /* continue */  );
	CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "items:%d,%d,%d threads:%d,%d stats:%lld/%ld.%06ld,%ld.%06ld,%ld.%06ld thresholds:%d,%d,%d,%p,%p,%p", 
						queue->count, queue->highest_ever, queue->max,
						queue->thrs, queue->thrs_push,
						queue->total_items,(long)queue->total_time.tv_sec,(long)(queue->total_time.tv_nsec/1000),(long)queue->blocking_time.tv_sec,(long)(queue->blocking_time.tv_nsec/1000),(long)queue->last_time.tv_sec,(long)(queue->last_time.tv_nsec/1000),
						queue->high, queue->low, queue->highest, queue->h_cb, queue->l_cb, queue->data), 
			 goto error);
	
	if (dump_item) {
		struct fd_list * li;
		int i = 0;
		for (li = queue->list.next; li != &queue->list; li = li->next) {
			struct fifo_item * fi = (struct fifo_item *)li;
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "\n [#%i](@%p)@%ld.%06ld: ", 
						i++, fi->item.o, (long)fi->posted_on.tv_sec,(long)(fi->posted_on.tv_nsec/1000)), 
					 goto error);
			CHECK_MALLOC_DO( (*dump_item)(FD_DUMP_STD_PARAMS, fi->item.o), goto error);
		}
	}
	CHECK_POSIX_DO(  pthread_mutex_unlock( &queue->mtx ), /* continue */  );
	
	return *buf;
error:
	CHECK_POSIX_DO(  pthread_mutex_unlock( &queue->mtx ), /* continue */  );
	return NULL;
}

/* Delete a queue. It must be empty. */ 
int fd_fifo_del ( struct fifo  ** queue )
{
	struct fifo * q;
	int loops = 0;
	
	TRACE_ENTRY( "%p", queue );

	CHECK_PARAMS( queue && CHECK_FIFO( *queue ) );
	
	q = *queue;
	
	CHECK_POSIX(  pthread_mutex_lock( &q->mtx )  );
	
	if ((q->count != 0) || (q->data != NULL)) {
		TRACE_DEBUG(INFO, "The queue cannot be destroyed (%d, %p)", q->count, q->data);
		CHECK_POSIX_DO(  pthread_mutex_unlock( &q->mtx ), /* no fallback */  );
		return EINVAL;
	}
	
	/* Ok, now invalidate the queue */
	q->eyec = 0xdead;
	
	/* Have all waiting threads return an error */
	while (q->thrs) {
		CHECK_POSIX(  pthread_mutex_unlock( &q->mtx ));
		CHECK_POSIX(  pthread_cond_signal(&q->cond_pull)  );
		usleep(1000);
		
		CHECK_POSIX(  pthread_mutex_lock( &q->mtx )  );
		ASSERT( ++loops < 20 ); /* detect infinite loops */
	}
	
	/* sanity check */
	ASSERT(FD_IS_LIST_EMPTY(&q->list));
	
	/* And destroy it */
	CHECK_POSIX(  pthread_mutex_unlock( &q->mtx )  );
	
	CHECK_POSIX_DO(  pthread_cond_destroy( &q->cond_pull ),  );
	
	CHECK_POSIX_DO(  pthread_cond_destroy( &q->cond_push ),  );
	
	CHECK_POSIX_DO(  pthread_mutex_destroy( &q->mtx ),  );
	
	free(q);
	*queue = NULL;
	
	return 0;
}

/* Move the content of old into new, and update loc_update atomically. We leave the old queue empty but valid */
int fd_fifo_move ( struct fifo * old, struct fifo * new, struct fifo ** loc_update )
{
	int loops = 0;
	
	TRACE_ENTRY("%p %p %p", old, new, loc_update);
	CHECK_PARAMS( CHECK_FIFO( old ) && CHECK_FIFO( new ));
	
	CHECK_PARAMS( ! old->data );
	if (new->high) {
		TODO("Implement support for thresholds in fd_fifo_move...");
	}
	
	/* Update loc_update */
	if (loc_update)
		*loc_update = new;
	
	/* Lock the queues */
	CHECK_POSIX(  pthread_mutex_lock( &old->mtx )  );
	
	CHECK_PARAMS_DO( (! old->thrs_push), {
			pthread_mutex_unlock( &old->mtx );
			return EINVAL;
		} );
	
	CHECK_POSIX(  pthread_mutex_lock( &new->mtx )  );
	
	/* Any waiting thread on the old queue returns an error */
	old->eyec = 0xdead;
	while (old->thrs) {
		CHECK_POSIX(  pthread_mutex_unlock( &old->mtx ));
		CHECK_POSIX(  pthread_cond_signal( &old->cond_pull )  );
		usleep(1000);
		
		CHECK_POSIX(  pthread_mutex_lock( &old->mtx )  );
		ASSERT( loops < 20 ); /* detect infinite loops */
	}
	
	/* Move all data from old to new */
	fd_list_move_end( &new->list, &old->list );
	if (old->count && (!new->count)) {
		CHECK_POSIX(  pthread_cond_signal(&new->cond_pull)  );
	}
	new->count += old->count;
	
	/* Reset old */
	old->count = 0;
	old->eyec = FIFO_EYEC;
	
	/* Merge the stats in the new queue */
	new->total_items += old->total_items;
	old->total_items = 0;
	
	new->total_time.tv_nsec += old->total_time.tv_nsec;
	new->total_time.tv_sec += old->total_time.tv_sec + (new->total_time.tv_nsec / 1000000000);
	new->total_time.tv_nsec %= 1000000000;
	old->total_time.tv_nsec = 0;
	old->total_time.tv_sec = 0;
	
	new->blocking_time.tv_nsec += old->blocking_time.tv_nsec;
	new->blocking_time.tv_sec += old->blocking_time.tv_sec + (new->blocking_time.tv_nsec / 1000000000);
	new->blocking_time.tv_nsec %= 1000000000;
	old->blocking_time.tv_nsec = 0;
	old->blocking_time.tv_sec = 0;
	
	/* Unlock, we're done */
	CHECK_POSIX(  pthread_mutex_unlock( &new->mtx )  );
	CHECK_POSIX(  pthread_mutex_unlock( &old->mtx )  );
	
	return 0;
}

/* Get the information on the queue */
int fd_fifo_getstats( struct fifo * queue, int * current_count, int * limit_count, int * highest_count, long long * total_count, 
				           struct timespec * total, struct timespec * blocking, struct timespec * last)
{
	TRACE_ENTRY( "%p %p %p %p %p %p %p %p", queue, current_count, limit_count, highest_count, total_count, total, blocking, last);
	
	/* Check the parameters */
	CHECK_PARAMS( CHECK_FIFO( queue ) );
	
	/* lock the queue */
	CHECK_POSIX(  pthread_mutex_lock( &queue->mtx )  );
	
	if (current_count)
		*current_count = queue->count;
	
	if (limit_count)
		*limit_count = queue->max;
	
	if (highest_count)
		*highest_count = queue->highest_ever;
	
	if (total_count)
		*total_count = queue->total_items;
	
	if (total)
		memcpy(total, &queue->total_time, sizeof(struct timespec));
	
	if (blocking)
		memcpy(blocking, &queue->blocking_time, sizeof(struct timespec));
	
	if (last)
		memcpy(last, &queue->last_time, sizeof(struct timespec));
	
	/* Unlock */
	CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
	
	/* Done */
	return 0;
}


/* alternate version with no error checking */
int fd_fifo_length ( struct fifo * queue )
{
	if ( !CHECK_FIFO( queue ) )
		return 0;
	
	return queue->count; /* Let's hope it's read atomically, since we are not locking... */
}

/* Set the thresholds of the queue */
int fd_fifo_setthrhd ( struct fifo * queue, void * data, uint16_t high, void (*h_cb)(struct fifo *, void **), uint16_t low, void (*l_cb)(struct fifo *, void **) )
{
	TRACE_ENTRY( "%p %p %hu %p %hu %p", queue, data, high, h_cb, low, l_cb );
	
	/* Check the parameters */
	CHECK_PARAMS( CHECK_FIFO( queue ) && (high > low) && (queue->data == NULL) );
	
	/* lock the queue */
	CHECK_POSIX(  pthread_mutex_lock( &queue->mtx )  );
	
	/* Save the values */
	queue->high = high;
	queue->low  = low;
	queue->data = data;
	queue->h_cb = h_cb;
	queue->l_cb = l_cb;
	
	/* Unlock */
	CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
	
	/* Done */
	return 0;
}


/* This handler is called when a thread is blocked on a queue, and cancelled */
static void fifo_cleanup_push(void * queue)
{
	struct fifo * q = (struct fifo *)queue;
	TRACE_ENTRY( "%p", queue );
	
	/* The thread has been cancelled, therefore it does not wait on the queue anymore */
	q->thrs_push--;
	
	/* Now unlock the queue, and we're done */
	CHECK_POSIX_DO(  pthread_mutex_unlock( &q->mtx ),  /* nothing */  );
	
	/* End of cleanup handler */
	return;
}


/* Post a new item in the queue */
int fd_fifo_post_internal ( struct fifo * queue, void ** item, int skip_max )
{
	struct fifo_item * new;
	int call_cb = 0;
	struct timespec posted_on, queued_on;
	
	/* Get the timing of this call */
	CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &posted_on)  );
	
	/* lock the queue */
	CHECK_POSIX(  pthread_mutex_lock( &queue->mtx )  );
	
	if ((!skip_max) && (queue->max)) {
		while (queue->count >= queue->max) {
			int ret = 0;
			
			/* We have to wait for an item to be pulled */
			queue->thrs_push++ ;
			pthread_cleanup_push( fifo_cleanup_push, queue);
			ret = pthread_cond_wait( &queue->cond_push, &queue->mtx );
			pthread_cleanup_pop(0);
			queue->thrs_push-- ;
			
			ASSERT( ret == 0 );
		}
	}
	
	/* Create a new list item */
	CHECK_MALLOC_DO(  new = malloc (sizeof (struct fifo_item)) , {
			pthread_mutex_unlock( &queue->mtx );
			return ENOMEM;
		} );
	
	fd_list_init(&new->item, *item);
	*item = NULL;
	
	/* Add the new item at the end */
	fd_list_insert_before( &queue->list, &new->item);
	queue->count++;
	if (queue->highest_ever < queue->count)
		queue->highest_ever = queue->count;
	if (queue->high && ((queue->count % queue->high) == 0)) {
		call_cb = 1;
		queue->highest = queue->count;
	}
	
	/* store timing */
	memcpy(&new->posted_on, &posted_on, sizeof(struct timespec));
	
	/* update queue timing info "blocking time" */
	{
		long long blocked_ns;
		CHECK_SYS(  clock_gettime(CLOCK_REALTIME, &queued_on)  );
		blocked_ns = (queued_on.tv_sec - posted_on.tv_sec) * 1000000000;
		blocked_ns += (queued_on.tv_nsec - posted_on.tv_nsec);
		blocked_ns += queue->blocking_time.tv_nsec;
		queue->blocking_time.tv_sec += blocked_ns / 1000000000;
		queue->blocking_time.tv_nsec = blocked_ns % 1000000000;
	}
	
	/* Signal if threads are asleep */
	if (queue->thrs > 0) {
		CHECK_POSIX(  pthread_cond_signal(&queue->cond_pull)  );
	}
	if (queue->thrs_push > 0) {
		/* cascade */
		CHECK_POSIX(  pthread_cond_signal(&queue->cond_push)  );
	}
	
	/* Unlock */
	CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
	
	/* Call high-watermark cb as needed */
	if (call_cb && queue->h_cb)
		(*queue->h_cb)(queue, &queue->data);
	
	/* Done */
	return 0;
}

/* Post a new item in the queue */
int fd_fifo_post_int ( struct fifo * queue, void ** item )
{
	TRACE_ENTRY( "%p %p", queue, item );
	
	/* Check the parameters */
	CHECK_PARAMS( CHECK_FIFO( queue ) && item && *item );
	
	return fd_fifo_post_internal ( queue,item, 0 );
	
}

/* Post a new item in the queue, not blocking */
int fd_fifo_post_noblock ( struct fifo * queue, void ** item )
{
	TRACE_ENTRY( "%p %p", queue, item );
	
	/* Check the parameters */
	CHECK_PARAMS( CHECK_FIFO( queue ) && item && *item );
	
	return fd_fifo_post_internal ( queue,item, 1 );
	
}

/* Pop the first item from the queue */
static void * mq_pop(struct fifo * queue)
{
	void * ret = NULL;
	struct fifo_item * fi;
	struct timespec now;
	
	ASSERT( ! FD_IS_LIST_EMPTY(&queue->list) );
	
	fi = (struct fifo_item *)(queue->list.next);
	ret = fi->item.o;
	fd_list_unlink(&fi->item);
	queue->count--;
	queue->total_items++;
	
	/* Update the timings */
	CHECK_SYS_DO(  clock_gettime(CLOCK_REALTIME, &now), goto skip_timing  );
	{
		long long elapsed = (now.tv_sec - fi->posted_on.tv_sec) * 1000000000;
		elapsed += now.tv_nsec - fi->posted_on.tv_nsec;
		
		queue->last_time.tv_sec = elapsed / 1000000000;
		queue->last_time.tv_nsec = elapsed % 1000000000;
		
		elapsed += queue->total_time.tv_nsec;
		queue->total_time.tv_sec += elapsed / 1000000000;
		queue->total_time.tv_nsec = elapsed % 1000000000;
	}
skip_timing:	
	free(fi);
	
	if (queue->thrs_push) {
		CHECK_POSIX_DO( pthread_cond_signal( &queue->cond_push ), );
	}
	
	return ret;
}

/* Check if the low watermark callback must be called. */
static __inline__ int test_l_cb(struct fifo * queue)
{
	if ((queue->high == 0) || (queue->low == 0) || (queue->l_cb == 0))
		return 0;
	
	if (((queue->count % queue->high) == queue->low) && (queue->highest > queue->count)) {
		queue->highest -= queue->high;
		return 1;
	}
	
	return 0;
}

/* Try poping an item */
int fd_fifo_tryget_int ( struct fifo * queue, void ** item )
{
	int wouldblock = 0;
	int call_cb = 0;
	
	TRACE_ENTRY( "%p %p", queue, item );
	
	/* Check the parameters */
	CHECK_PARAMS( CHECK_FIFO( queue ) && item );
	
	/* lock the queue */
	CHECK_POSIX(  pthread_mutex_lock( &queue->mtx )  );
	
	/* Check queue status */
	if (queue->count > 0) {
got_item:
		/* There are elements in the queue, so pick the first one */
		*item = mq_pop(queue);
		call_cb = test_l_cb(queue);
	} else {
		if (queue->thrs_push > 0) {
			/* A thread is trying to push something, let's give it a chance */
			CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
			CHECK_POSIX(  pthread_cond_signal( &queue->cond_push )  );
			usleep(1000);
			CHECK_POSIX(  pthread_mutex_lock( &queue->mtx )  );
			if (queue->count > 0)
				goto got_item;
		}
		
		wouldblock = 1;
		*item = NULL;
	}
		
	/* Unlock */
	CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
	
	/* Call low watermark callback as needed */
	if (call_cb)
		(*queue->l_cb)(queue, &queue->data);
	
	/* Done */
	return wouldblock ? EWOULDBLOCK : 0;
}

/* This handler is called when a thread is blocked on a queue, and cancelled */
static void fifo_cleanup(void * queue)
{
	struct fifo * q = (struct fifo *)queue;
	TRACE_ENTRY( "%p", queue );
	
	/* The thread has been cancelled, therefore it does not wait on the queue anymore */
	q->thrs--;
	
	/* Now unlock the queue, and we're done */
	CHECK_POSIX_DO(  pthread_mutex_unlock( &q->mtx ),  /* nothing */  );
	
	/* End of cleanup handler */
	return;
}

/* The internal function for fd_fifo_timedget and fd_fifo_get */
static int fifo_tget ( struct fifo * queue, void ** item, int istimed, const struct timespec *abstime)
{
	int call_cb = 0;
	int ret = 0;
	
	/* Check the parameters */
	CHECK_PARAMS( CHECK_FIFO( queue ) && item && (abstime || !istimed) );
	
	/* Initialize the return value */
	*item = NULL;
	
	/* lock the queue */
	CHECK_POSIX(  pthread_mutex_lock( &queue->mtx )  );
	
awaken:
	/* Check queue status */
	if (!CHECK_FIFO( queue )) {
		/* The queue is being destroyed */
		CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
		TRACE_DEBUG(FULL, "The queue is being destroyed -> EPIPE");
		return EPIPE;
	}
	
	if (queue->count > 0) {
		/* There are items in the queue, so pick the first one */
		*item = mq_pop(queue);
		call_cb = test_l_cb(queue);
	} else {
		/* We have to wait for a new item */
		queue->thrs++ ;
		pthread_cleanup_push( fifo_cleanup, queue);
		if (istimed) {
			ret = pthread_cond_timedwait( &queue->cond_pull, &queue->mtx, abstime );
		} else {
			ret = pthread_cond_wait( &queue->cond_pull, &queue->mtx );
		}
		pthread_cleanup_pop(0);
		queue->thrs-- ;
		if (ret == 0)
			goto awaken;  /* test for spurious wake-ups */
		
		/* otherwise (ETIMEDOUT / other error) just continue */
	}
	
	/* Unlock */
	CHECK_POSIX(  pthread_mutex_unlock( &queue->mtx )  );
	
	/* Call low watermark callback as needed */
	if (call_cb)
		(*queue->l_cb)(queue, &queue->data);
	
	/* Done */
	return ret;
}

/* Get the next available item, block until there is one */
int fd_fifo_get_int ( struct fifo * queue, void ** item )
{
	TRACE_ENTRY( "%p %p", queue, item );
	return fifo_tget(queue, item, 0, NULL);
}

/* Get the next available item, block until there is one, or the timeout expires */
int fd_fifo_timedget_int ( struct fifo * queue, void ** item, const struct timespec *abstime )
{
	TRACE_ENTRY( "%p %p %p", queue, item, abstime );
	return fifo_tget(queue, item, 1, abstime);
}

/* Test if data is available in the queue, without pulling it */
int fd_fifo_select ( struct fifo * queue, const struct timespec *abstime )
{
	int ret = 0;
	TRACE_ENTRY( "%p %p", queue, abstime );
	
	CHECK_PARAMS_DO( CHECK_FIFO( queue ), return -EINVAL );
	
	/* lock the queue */
	CHECK_POSIX_DO(  pthread_mutex_lock( &queue->mtx ), return -__ret__  );
	
awaken:	
	ret = (queue->count > 0 ) ? queue->count : 0;
	if ((ret == 0) && (abstime != NULL)) {
		/* We have to wait for a new item */
		queue->thrs++ ;
		pthread_cleanup_push( fifo_cleanup, queue);
		ret = pthread_cond_timedwait( &queue->cond_pull, &queue->mtx, abstime );
		pthread_cleanup_pop(0);
		queue->thrs-- ;
		if (ret == 0)
			goto awaken;  /* test for spurious wake-ups */
		
		if (ret == ETIMEDOUT)
			ret = 0;
		else 
			ret = -ret;
	}
	
	/* Unlock */
	CHECK_POSIX_DO(  pthread_mutex_unlock( &queue->mtx ), return -__ret__  );
	
	return ret;
}
