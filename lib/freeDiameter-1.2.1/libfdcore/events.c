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

/* Events are a subset of fifo queues, with a known type */

int fd_event_send(struct fifo *queue, int code, size_t datasz, void * data)
{
	struct fd_event * ev;
	CHECK_MALLOC( ev = malloc(sizeof(struct fd_event)) );
	ev->code = code;
	ev->size = datasz;
	ev->data = data;
	CHECK_FCT( fd_fifo_post(queue, &ev) );
	return 0;
}

int fd_event_get(struct fifo *queue, int *code, size_t *datasz, void ** data)
{
	struct fd_event * ev;
	CHECK_FCT( fd_fifo_get(queue, &ev) );
	if (code)
		*code = ev->code;
	if (datasz)
		*datasz = ev->size;
	if (data)
		*data = ev->data;
	free(ev);
	return 0;
}

int fd_event_timedget(struct fifo *queue, struct timespec * timeout, int timeoutcode, int *code, size_t *datasz, void ** data)
{
	struct fd_event * ev;
	int ret = 0;
	ret = fd_fifo_timedget(queue, &ev, timeout);
	if (ret == ETIMEDOUT) {
		if (code)
			*code = timeoutcode;
		if (datasz)
			*datasz = 0;
		if (data)
			*data = NULL;
	} else {
		CHECK_FCT( ret );
		if (code)
			*code = ev->code;
		if (datasz)
			*datasz = ev->size;
		if (data)
			*data = ev->data;
		free(ev);
	}
	return 0;
}

void fd_event_destroy(struct fifo **queue, void (*free_cb)(void * data))
{
	struct fd_event * ev;
	/* Purge all events, and free the associated data if any */
	while (fd_fifo_tryget( *queue, &ev ) == 0) {
		(*free_cb)(ev->data);
		free(ev);
	}
	CHECK_FCT_DO( fd_fifo_del(queue), /* continue */ );
	return ;
} 

const char * fd_ev_str(int event)
{
	switch (event) {
	#define case_str( _val )\
		case _val : return #_val
		case_str(FDEV_TERMINATE_INT);
		case_str(FDEV_TRIGGER);
		
		default:
			TRACE_DEBUG(FULL, "Unknown event : %d", event);
			return "Unknown event";
	}
}

/**********************************************************************/
/* Trigged events */
/* This allows extensions to register for / send triggers to other extensions */
/* It is used for example for users interactions (through signals or ...) */

/* Because the number of triggers is not expected to grow, we use a simple ordered chained list */
static pthread_rwlock_t trig_rwl = PTHREAD_RWLOCK_INITIALIZER;
static struct fd_list   trig_list = FD_LIST_INITIALIZER(trig_list); /* The list of triggers ordered by trigger value */
struct trig_item {
	struct fd_list 	chain;
	int  		trig_value;
	const char * 	trig_module;
	void 		(*cb)(void);
};	

/* Add a new entry in the trigger list */
int fd_event_trig_regcb(int trigger_val, const char * module, void (*cb)(void))
{
	struct trig_item * ti;
	struct fd_list * li;
	
	TRACE_ENTRY("%d %p %p", trigger_val, module, cb);
	CHECK_PARAMS( trigger_val && cb );
	
	/* Create a new trig_item */
	CHECK_MALLOC( ti = malloc(sizeof(struct trig_item)) );
	memset(ti, 0, sizeof(struct trig_item));
	fd_list_init(&ti->chain, ti);
	ti->trig_value = trigger_val;
	ti->trig_module = module;
	ti->cb = cb;
	
	/* Now insert in the list */
	CHECK_POSIX( pthread_rwlock_wrlock(&trig_rwl) );
	
	for (li = trig_list.next; li != &trig_list; li = li->next) {
		struct trig_item *t = li->o;
		if (t->trig_value >= trigger_val)
			break;
	}
	
	fd_list_insert_before(li, &ti->chain);
	
	CHECK_POSIX( pthread_rwlock_unlock(&trig_rwl) );

	return 0;
}

DECLARE_FD_DUMP_PROTOTYPE(fd_event_trig_dump)
{
	struct fd_list * li;
	FD_DUMP_HANDLE_OFFSET();
	
	CHECK_POSIX_DO( pthread_rwlock_rdlock(&trig_rwl),  );
	
	for (li = trig_list.next; li != &trig_list; li = li->next) {
		struct trig_item *t = li->o;
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "{signal:%d}'%s'->%p ", t->trig_value, t->trig_module, t->cb), break);
	}
	
	CHECK_POSIX_DO( pthread_rwlock_unlock(&trig_rwl),  );
	return *buf;
}

static void *call_cb_detached(void * arg)
{
	void (*cb)(void) = arg;
	fd_log_threadname("Trig'd callback thread");
	TRACE_ENTRY("%p", arg);
	(*cb)();
	TRACE_DEBUG(ANNOYING, "Callback %p completed", cb);
	return NULL;
}

int fd_event_trig_call_cb(int trigger_val)
{
	struct fd_list * li;
	pthread_attr_t detached;
	pthread_t th;
	
	CHECK_POSIX( pthread_attr_init(&detached) );
	CHECK_POSIX( pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED) );
	
	CHECK_POSIX( pthread_rwlock_rdlock(&trig_rwl)  );
	
	for (li = trig_list.next; li != &trig_list; li = li->next) {
		struct trig_item *t = li->o;
		if (t->trig_value == trigger_val) {
			TRACE_DEBUG(FULL, "Trigger %d: Calling %p in %s", t->trig_value, t->cb, t->trig_module);
			CHECK_POSIX_DO( pthread_create( &th, &detached, call_cb_detached, t->cb ), break );
		}
		if (t->trig_value > trigger_val)
			break;
	}
	
	CHECK_POSIX( pthread_rwlock_unlock(&trig_rwl)  );
	
	return 0;
}

int fd_event_trig_fini(void) {
	
	TRACE_ENTRY("");
	
	CHECK_POSIX( pthread_rwlock_wrlock(&trig_rwl) );
	
	while (!FD_IS_LIST_EMPTY(&trig_list)) {
		struct fd_list * li = trig_list.next;
		fd_list_unlink(li);
		free(li);
	}
	
	CHECK_POSIX( pthread_rwlock_unlock(&trig_rwl) );
	
	return 0;
}
