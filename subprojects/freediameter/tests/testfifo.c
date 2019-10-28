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

#include "tests.h"
#include <unistd.h>
#include <limits.h>

/* Wrapper for pthread_barrier stuff on Mac OS X */
#ifndef HAVE_PTHREAD_BAR

#define PTHREAD_BARRIER_SERIAL_THREAD 1
typedef struct {
	int count;
	int entered;
	int serial;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t * barrier, int * barrier_attr, int count)
{
	memset(barrier, 0, sizeof(pthread_barrier_t));
	barrier->count = count;
	pthread_mutex_init(&barrier->mutex, NULL);
	pthread_cond_init(&barrier->cond, NULL);
	return 0;
}

int pthread_barrier_destroy(pthread_barrier_t * barrier)
{
	pthread_mutex_destroy(&barrier->mutex);
	pthread_cond_destroy(&barrier->cond);
	return 0;
}

int pthread_barrier_wait(pthread_barrier_t * barrier)
{
	int ret = 0;
	int serial;
	pthread_mutex_lock(&barrier->mutex);
	serial = barrier->serial;
	
	/* first thread gets the special value */
	if (barrier->entered++ == 0) 
		ret = PTHREAD_BARRIER_SERIAL_THREAD;
	
	/* Count was achieved? */
	if (barrier->entered == barrier->count) {
		/* Ok, increase serial, reset number of threads, and signal everyone */
		barrier->entered = 0;
		barrier->serial++;
		pthread_cond_broadcast(&barrier->cond);
	} else {
		do {
			pthread_cond_wait(&barrier->cond, &barrier->mutex);
		} while (barrier->serial == serial);
		/* this protects against spurious wakes */
	}
	pthread_mutex_unlock(&barrier->mutex);
	return 0;
}

#endif /* HAVE_PTHREAD_BAR */

/* Structure for testing threshold function */
static struct thrh_test {
	struct fifo *   queue; /* pointer to the queue */
	int		h_calls; /* number of calls of h_cb */
	int		l_calls; /* number of calls of l_cb */
} thrh_td;

/* Callbacks for threasholds test */
void thrh_cb_h(struct fifo *queue, void **data)
{
	if (thrh_td.h_calls == thrh_td.l_calls) {
		CHECK( NULL, *data );
		*data = &thrh_td;
	} else {
		CHECK( *data, &thrh_td );
	}
	CHECK( queue, thrh_td.queue );
	
	/* Update the count */
	thrh_td.h_calls ++;
}
void thrh_cb_l(struct fifo *queue, void **data)
{
	CHECK( 1, data ? 1 : 0 );
	CHECK( *data, &thrh_td );

	/* Check the queue parameter is correct */
	CHECK( queue, thrh_td.queue );
	
	/* Update the count */
	thrh_td.l_calls ++;
	/* Cleanup the data ptr if needed */
	if (thrh_td.l_calls == thrh_td.h_calls)
		*data = NULL;
	/* done */
}


/* Structure that is passed to the test function */
struct test_data {
	struct fifo     * queue; /* pointer to the queue */
	pthread_barrier_t * bar;   /* if not NULL, barrier to synchronize before getting messages */
	struct timespec   * ts;	   /* if not NULL, use a timedget instead of a get */
	int		    nbr;   /* number of messages to retrieve from the queue */
};

/* The test function, to be threaded */
static void * test_fct(void * data)
{
	int ret = 0, i;
	struct msg * msg = NULL;
	struct test_data * td = (struct test_data *) data;
	
	if (td->bar != NULL) {
		ret = pthread_barrier_wait(td->bar);
		if (ret != PTHREAD_BARRIER_SERIAL_THREAD) {
			CHECK( 0, ret);
		} else {
			CHECK( PTHREAD_BARRIER_SERIAL_THREAD, ret); /* just for the traces */
		}
	}
	
	for (i=0; i< td->nbr; i++) {
		if (td->ts != NULL) {
			CHECK( 0, fd_fifo_timedget(td->queue, &msg, td->ts) );
		} else {
			CHECK( 0, fd_fifo_get(td->queue, &msg) );
		}
	}
	
	return NULL;
}

/* The test function, to be threaded */
static int iter  = 0;
static void * test_fct2(void * data)
{
	int i;
	int * item;
	struct test_data * td = (struct test_data *) data;
	
	for (i=0; i< td->nbr; i++) {
		item = malloc(sizeof(int));
		CHECK( 1, item ? 1 : 0 );
		*item = i;
		CHECK( 0, fd_fifo_post(td->queue, &item) );
		iter++;
	}
	
	return NULL;
}


/* Main test routine */
int main(int argc, char *argv[])
{
	struct timespec ts;
	
	struct msg * msg1 = NULL;
	struct msg * msg2 = NULL;
	struct msg * msg3 = NULL;
	
	/* First, initialize the daemon modules */
	INIT_FD();
	
	/* Prolog: create the messages */
	{
		struct dict_object * acr_model = NULL;
		struct dict_object * cer_model = NULL;
		struct dict_object * dwr_model = NULL;

		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Accounting-Request", 			&acr_model, ENOENT ) );
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Capabilities-Exchange-Request", 	&cer_model, ENOENT ) );
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Device-Watchdog-Request",		&dwr_model, ENOENT ) );
		CHECK( 0, fd_msg_new ( acr_model, 0, &msg1 ) );
		CHECK( 0, fd_msg_new ( cer_model, 0, &msg2 ) );
		CHECK( 0, fd_msg_new ( dwr_model, 0, &msg3 ) );
	}
	
	/* Basic operation */
	{
		struct fifo * queue = NULL;
		struct msg * msg  = NULL;
		int max;
		long long count;
		
		/* Create the queue */
		CHECK( 0, fd_fifo_new(&queue, 0) );
		
		/* Check the count is 0 */
		CHECK( 0, fd_fifo_length(queue) );
		
		/* Now enqueue */
		msg = msg1;
		CHECK( 0, fd_fifo_post(queue, &msg) );
		msg = msg2;
		CHECK( 0, fd_fifo_post(queue, &msg) );
		msg = msg3;
		CHECK( 0, fd_fifo_post(queue, &msg) );
		
		/* Check the count is 3 */
		CHECK( 3, fd_fifo_length(queue) );
		
		/* Retrieve the first message using fd_fifo_get */
		CHECK( 0, fd_fifo_get(queue, &msg) );
		CHECK( msg1, msg);
		CHECK( 2, fd_fifo_length(queue) );
		
		/* Retrieve the second message using fd_fifo_timedget */
		CHECK(0, clock_gettime(CLOCK_REALTIME, &ts));
		ts.tv_sec += 1; /* Set the timeout to 1 second */
		CHECK( 0, fd_fifo_timedget(queue, &msg, &ts) );
		CHECK( msg2, msg);
		CHECK( 1, fd_fifo_length(queue) );
		
		/* Retrieve the third message using meq_tryget */
		CHECK( 0, fd_fifo_tryget(queue, &msg) );
		CHECK( msg3, msg);
		CHECK( 0, fd_fifo_length(queue) );
		
		/* Check that another meq_tryget does not block */
		CHECK( EWOULDBLOCK, fd_fifo_tryget(queue, &msg) );
		CHECK( 0, fd_fifo_length(queue) );
		
		/* Check the timedget actually timesout */
		CHECK(0, clock_gettime(CLOCK_REALTIME, &ts));
		ts.tv_nsec += 1000000; /* 1 millisecond */
		if (ts.tv_nsec >= 1000000000L) {
			ts.tv_nsec -= 1000000000L;
			ts.tv_sec += 1;
		}
		CHECK( ETIMEDOUT, fd_fifo_timedget(queue, &msg, &ts) );
		CHECK( 0, fd_fifo_length(queue) );
		
		/* Post & get another message */
		msg = msg1;
		CHECK( 0, fd_fifo_post(queue, &msg) );
		CHECK( 0, fd_fifo_timedget(queue, &msg, &ts) );
		CHECK( msg1, msg);		
		
		/* Check some statistics */
		CHECK( 0, fd_fifo_getstats(queue, NULL, NULL, &max, &count, NULL, NULL, NULL) );
		CHECK( 3, max );
		CHECK( 4, count );	
		
		/* We're done for basic tests */
		CHECK( 0, fd_fifo_del(&queue) );
	}
	
	/* Test robustness, ensure no messages are lost */
	{
#define NBR_MSG		200
#define NBR_THREADS	60
		struct fifo  		*queue = NULL;
		pthread_barrier_t	 bar;
		struct test_data	 td_1;
		struct test_data	 td_2;
		struct msg   		*msgs[NBR_MSG * NBR_THREADS * 2], *msg;
		pthread_t  		 thr [NBR_THREADS * 2];
		struct dict_object	*dwr_model = NULL;
		int			 i;
		int			 nbr_threads;
#ifdef _POSIX_THREAD_THREADS_MAX
		nbr_threads = _POSIX_THREAD_THREADS_MAX;
#else /* _POSIX_THREAD_THREADS_MAX */
		nbr_threads = sysconf(_SC_THREAD_THREADS_MAX);
#endif /* _POSIX_THREAD_THREADS_MAX */
		if ((nbr_threads <= 0) || (nbr_threads > NBR_THREADS * 2)) {
			nbr_threads = NBR_THREADS;
		} else {
			TRACE_DEBUG(INFO, "Local limit on number of threads: %d", nbr_threads);
			/* The local limit is below NBR_THREADS */
			nbr_threads = (nbr_threads / 2) - 1;
			/* Ensure we create at least a few threads! */
			CHECK( 1, nbr_threads >= 10 ? 1 : 0 );
		}
		
		/* Create the queue */
		CHECK( 0, fd_fifo_new(&queue, 0) );
		
		/* Create the barrier */
		CHECK( 0, pthread_barrier_init(&bar, NULL, nbr_threads * 2 + 1) );
		
		/* Initialize the ts */
		CHECK(0, clock_gettime(CLOCK_REALTIME, &ts));
		ts.tv_sec += 20; /* Set the timeout to 20 second */
		
		/* Create the messages */
		CHECK( 0, fd_dict_search ( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Device-Watchdog-Request",		&dwr_model, ENOENT ) );
		for (i = 0; i < NBR_MSG * nbr_threads * 2; i++) {
			CHECK( 0, fd_msg_new ( dwr_model, 0, &msgs[i] ) );
		}
		
		/* Initialize the test data structures */
		td_1.queue = queue;
		td_1.bar = &bar;
		td_1.ts  = &ts;
		td_1.nbr = NBR_MSG;
		td_2.queue = queue;
		td_2.bar = &bar;
		td_2.ts  = NULL;
		td_2.nbr = NBR_MSG;
		
		/* Create the threads */
		for (i=0; i < nbr_threads * 2; i++) {
			CHECK( 0, pthread_create( &thr[i], NULL, test_fct, (i & 1) ? &td_1 : &td_2 ) );
		}
		
		/* Synchronize everyone */
		{
			int ret = pthread_barrier_wait(&bar);
			if (ret != PTHREAD_BARRIER_SERIAL_THREAD) {
				CHECK( 0, ret);
			} else {
				CHECK( PTHREAD_BARRIER_SERIAL_THREAD, ret); /* for trace only */
			}
		}
		
		/* Now post all the messages */
		for (i=0; i < NBR_MSG * nbr_threads * 2; i++) {
			msg = msgs[i];
			CHECK( 0, fd_fifo_post(queue, &msg) );
		}
		
		/* Join all threads. This blocks if messages are lost... */
		for (i=0; i < nbr_threads * 2; i++) {
			CHECK( 0, pthread_join( thr[i], NULL ) );
		}
		
		/* Check the count of the queue is back to 0 */
		CHECK( 0, fd_fifo_length(queue) );
		
		/* Destroy this queue and the messages */
		CHECK( 0, fd_fifo_del(&queue) );
		for (i=0; i < NBR_MSG * nbr_threads * 2; i++) {
			CHECK( 0, fd_msg_free(  msgs[i] ) );
		}
	}
	
	/* Test thread cancelation */
	{
		struct fifo      	*queue = NULL;
		pthread_barrier_t	 bar;
		struct test_data	 td;
		pthread_t		 th;
		
		/* Create the queue */
		CHECK( 0, fd_fifo_new(&queue, 0) );
		
		/* Create the barrier */
		CHECK( 0, pthread_barrier_init(&bar, NULL, 2) );
		
		/* Initialize the ts */
		CHECK(0, clock_gettime(CLOCK_REALTIME, &ts));
		ts.tv_sec += 10; /* Set the timeout to 10 second */
		
		/* Initialize the test data structures */
		td.queue = queue;
		td.bar = &bar;
		td.ts  = &ts;
		td.nbr = 1;
		
		/* Create the thread */
		CHECK( 0, pthread_create( &th, NULL, test_fct, &td ) );
		
		/* Wait for the thread to be running */
		{
			int ret = pthread_barrier_wait(&bar);
			if (ret != PTHREAD_BARRIER_SERIAL_THREAD) {
				CHECK( 0, ret);
			} else {
				CHECK( PTHREAD_BARRIER_SERIAL_THREAD, ret );
			}
		}
		
		/* Now cancel the thread */
		CHECK( 0, pthread_cancel( th ) );
		
		/* Join it */
		CHECK( 0, pthread_join( th, NULL ) );
		
		/* Do the same with the other function */
		td.ts  = NULL;
		
		/* Create the thread */
		CHECK( 0, pthread_create( &th, NULL, test_fct, &td ) );
		
		/* Wait for the thread to be running */
		{
			int ret = pthread_barrier_wait(&bar);
			if (ret != PTHREAD_BARRIER_SERIAL_THREAD) {
				CHECK( 0, ret);
			} else {
				CHECK( PTHREAD_BARRIER_SERIAL_THREAD, ret );
			}
		}
		
		/* Now cancel the thread */
		CHECK( 0, pthread_cancel( th ) );
		
		/* Join it */
		CHECK( 0, pthread_join( th, NULL ) );
		
		/* Destroy the queue */
		CHECK( 0, fd_fifo_del(&queue) );
	}
	
	/* Test the threashold function */
	{
		struct fifo * queue = NULL;
		int i;
		struct msg * msg  = NULL;
		
		/* Create the queue */
		CHECK( 0, fd_fifo_new(&queue, 0) );
		
		/* Prepare the test data */
		memset(&thrh_td, 0, sizeof(thrh_td));
		thrh_td.queue = queue;
		
		/* Set the thresholds for the queue */
		CHECK( 0, fd_fifo_setthrhd ( queue, NULL, 6, thrh_cb_h, 4, thrh_cb_l ) );
		
		/* Post 5 messages, no cb must be called. */
		for (i=0; i<5; i++) {
			msg = msg1;
			CHECK( 0, fd_fifo_post(queue, &msg) );
		} /* 5 msg in queue */
		CHECK( 0, thrh_td.h_calls );
		CHECK( 0, thrh_td.l_calls );
		
		/* Get all these messages, and check again */
		for (i=0; i<5; i++) {
			CHECK( 0, fd_fifo_get(queue, &msg) );
		} /* 0 msg in queue */
		CHECK( 0, thrh_td.h_calls );
		CHECK( 0, thrh_td.l_calls );
		
		/* Now, post 6 messages, the high threashold */
		for (i=0; i<6; i++) {
			msg = msg1;
			CHECK( 0, fd_fifo_post(queue, &msg) );
		} /* 6 msg in queue */
		CHECK( 1, thrh_td.h_calls );
		CHECK( 0, thrh_td.l_calls );
		
		/* Remove 2 messages, to reach the low threshold */
		for (i=0; i<2; i++) {
			CHECK( 0, fd_fifo_get(queue, &msg) );
		} /* 4 msg in queue */
		CHECK( 1, thrh_td.h_calls );
		CHECK( 1, thrh_td.l_calls );
		
		/* Come again at the high threshold */
		for (i=0; i<2; i++) {
			msg = msg1;
			CHECK( 0, fd_fifo_post(queue, &msg) );
		} /* 6 msg in queue */
		CHECK( 2, thrh_td.h_calls );
		CHECK( 1, thrh_td.l_calls );
		
		/* Suppose the queue continues to grow */
		for (i=0; i<6; i++) {
			msg = msg1;
			CHECK( 0, fd_fifo_post(queue, &msg) );
		} /* 12 msg in queue */
		CHECK( 3, thrh_td.h_calls );
		CHECK( 1, thrh_td.l_calls );
		for (i=0; i<5; i++) {
			msg = msg1;
			CHECK( 0, fd_fifo_post(queue, &msg) );
		} /* 17 msg in queue */
		CHECK( 3, thrh_td.h_calls );
		CHECK( 1, thrh_td.l_calls );
		
		/* Now the queue goes back to 0 messages */
		for (i=0; i<17; i++) {
			CHECK( 0, fd_fifo_get(queue, &msg) );
		} /* 0 msg in queue */
		CHECK( 3, thrh_td.h_calls );
		CHECK( 3, thrh_td.l_calls );
		
		/* We're done for this test */
		CHECK( 0, fd_fifo_del(&queue) );
	}
	
	/* Test max queue limit */
	{
		struct fifo      	*queue = NULL;
		struct test_data	 td;
		pthread_t		 th;
		int *			item, i;
		
		/* Create the queue */
		CHECK( 0, fd_fifo_new(&queue, 10) );
		
		/* Initialize the test data structures */
		td.queue = queue;
		td.nbr = 15;
		
		CHECK( 0, pthread_create( &th, NULL, test_fct2, &td ) );
		
		usleep(100000); /* 100 millisec */
		
		CHECK( 10, iter );
		
		CHECK( 0, fd_fifo_tryget(queue, &item) );
		CHECK( 0, *item);
		free(item);
		
		usleep(100000); /* 100 millisec */
		
		CHECK( 11, iter );
		
		for (i=1; i<4; i++) {
			CHECK( 0, fd_fifo_get(queue, &item) );
			CHECK( i, *item);
			free(item);
		}
		
		usleep(100000); /* 100 millisec */
		
		CHECK( 14, iter );
		
		for (; i < td.nbr; i++) {
			CHECK( 0, fd_fifo_tryget(queue, &item) );
			CHECK( i, *item);
			free(item);
		}
		
		CHECK( 0, pthread_join( th, NULL ) );
		CHECK( 15, iter );
		
	}
	
	/* Delete the messages */
	CHECK( 0, fd_msg_free( msg1 ) );
	CHECK( 0, fd_msg_free( msg2 ) );
	CHECK( 0, fd_msg_free( msg3 ) );

	/* That's all for the tests yet */
	PASSTEST();
} 
