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

#include "test_netemul.h"
#include <math.h>

/* This file implements the real processing of the message.
 The entry point is tne_process_message().
 
 First, the duplicate filter is applied: with the configured
 probability, a copy of the message is created. Then, with 
 the tenth probability, a second copy is created, and so on,
 until the random value tells not to create a copy.
 
 The original message + all copies are stored in a list, for next step.
 
 Second step is the latency filter. For each message in the list, a
 latency value is randomly generated (with a lognormal shape of the 
 random distribution) and stored in the list.
 
 Finally, when the latency time is over, the message is sent.
 */

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cnd = PTHREAD_COND_INITIALIZER;
static pthread_t       thr = (pthread_t)NULL;

/* The lists below are all protected by the same mutex mtx */
static struct fd_list input = FD_LIST_INITIALIZER(input); /* messages received from network */
static struct fd_list forlat = FD_LIST_INITIALIZER(forlat); /* messages after duplicate filter */
static struct fd_list waitlist = FD_LIST_INITIALIZER(waitlist); /* messages waiting for sending */

struct process_item {
	struct fd_list chain; /* link into one of the lists. "o" points to the message. */
	struct timespec ts; /* when the message must be sent */
};

/******************************************************************/
/* helper functions */

/* Process all pi in input list and queue in forlat, duplicating when needed */
static int do_duplicates()
{
	TRACE_ENTRY("");
	
	while (!FD_IS_LIST_EMPTY(&input)) {
		struct msg * m;
		struct process_item * pi = (struct process_item *)(input.next);
		
		/* Take out this pi from input */
		fd_list_unlink(&pi->chain);
		
		/* store it in forlat */
		fd_list_insert_before(&forlat, &pi->chain);
		
		/* Duplicate eventually, unless deactivated */
		if (tne_conf.dupl_proba != 0.0) {
			DiamId_t src;
			size_t srclen;
			/* Pick a random value in [0, 1] */
			double my_rand = drand48();
			m = pi->chain.o;
			CHECK_FCT( fd_msg_source_get(m, &src, &srclen) );
			
			while (my_rand < (double) tne_conf.dupl_proba) {
				/* create the duplicate */
				struct process_item * npi;
				struct msg * nm;
				struct msg_hdr * nh;
				unsigned char * buf;
				size_t len;
				
				/* Duplicate the message */
				CHECK_FCT( fd_msg_bufferize(m, &buf, &len) );
				CHECK_FCT( fd_msg_parse_buffer(&buf, len, &nm) );
				CHECK_FCT( fd_msg_source_set(nm, src, srclen) );
				CHECK_FCT( fd_msg_hdr(nm, &nh) );
				nh->msg_flags |= CMD_FLAG_RETRANSMIT; /* Add the 'T' flag */
				TRACE_DEBUG(FULL, "[tne] Duplicated message %p as %p", m, nm);
				
				/* Duplicate the pi */
				CHECK_MALLOC( npi = malloc(sizeof(struct process_item)) );
				memset(npi, 0, sizeof(struct process_item));
				fd_list_init(&npi->chain, nm);
				memcpy(&npi->ts, &pi->ts, sizeof(struct timespec));
				
				/* Enqueue the candidate in forlat */
				fd_list_insert_before(&forlat, &npi->chain);
				
				/* loop for another duplicate */
				if (!my_rand)
					break; /* otherwise, infinite loop */
				my_rand *= 10.0;
			}
		}
	}
	
	/* Done */
	return 0;
}

/* Generate a random value with a normal distribution, mean 0, variance 1 */
/* Using Box-Muller algo from Numerical Recipes in C++, 2nd Ed. */
static double get_rand_norm()
{
	double ru1, ru2; /* two random uniform values in -1..1 */
	double rsq;      /* ru1^2 + ru2^2, to ensure we are in the circle */
	
	/* Get our appropriate 2 random uniform values */
	do {
		ru1 = 2.0 * drand48() - 1.0;
		ru2 = 2.0 * drand48() - 1.0;
		rsq = ru1 * ru1 + ru2 * ru2;
	} while ((rsq >= 1.0) || (rsq == 0.0));
	
	/* Do the Box-Muller transform -- we don't use the 2nd value generated */
	return ru1 * sqrt( -2.0 * log(rsq) / rsq );
}

/* Return the latency to add, in ms. */
static __inline__ unsigned long get_latency()
{
	unsigned long lat = tne_conf.lat_avg;

	if (tne_conf.lat_dev) {
		/* We randomize the value to add */
		double rn;

		rn = get_rand_norm(); /* this is normal random value with mean = 0 and variance = 1 */
		rn = rn * ((double)tne_conf.lat_dev) / 100.0; /* now the variance is lat_dev */
		rn = exp(rn); /* and now, we have a lognormal random value, with geometric mean = 1 */

		lat = (unsigned long)(rn * (double)lat); /* Apply to our mean latency */
	}
	
	return lat;
}
			
/* Process all pi in forlat and add a random latency, then requeue in order into waitlist */
static int do_latency()
{
	TRACE_ENTRY("");
	
	while (!FD_IS_LIST_EMPTY(&forlat)) {
		struct process_item * pi = (struct process_item *)(forlat.next);
		struct fd_list * li;
		
		/* Take out this pi from forlat */
		fd_list_unlink(&pi->chain);
		
		/* If there is a latency to add */
		if (tne_conf.lat_avg) {
			unsigned long l = get_latency();
			TRACE_DEBUG(FULL, "[tne] Set %lu ms latency for %p", l, pi->chain.o);
			pi->ts.tv_sec += l / 1000;
			l %= 1000;
			pi->ts.tv_nsec += l * 1000000;
			if (pi->ts.tv_nsec >= 1000000000) {
				pi->ts.tv_sec += 1;
				pi->ts.tv_nsec -= 1000000000;
			}
		}

		for (li = waitlist.prev; li != &waitlist; li=li->prev) {
			struct process_item * p = (struct process_item *)li;
			if (TS_IS_INFERIOR( &p->ts, &pi->ts ))
				break; /* we must insert after this one */
		}
		
		/* store it */
		fd_list_insert_after(li, &pi->chain);
	}
	
	/* Done */
	return 0;
}

/* Send all messages in waitlist that have passed their latency period */
static int send_all_ready()
{
	struct timespec now;
	
	TRACE_ENTRY("");
	
	CHECK_SYS( clock_gettime(CLOCK_REALTIME, &now) );
	
	while (!FD_IS_LIST_EMPTY(&waitlist)) {
		struct msg * m;
		struct process_item * pi = (struct process_item *)(waitlist.next);
		
		if (!TS_IS_INFERIOR( &pi->ts, &now))
			break; /* We sent already all we could */
		
		/* Take out this pi and send the message */
		fd_list_unlink(&pi->chain);
		m = pi->chain.o;
		free(pi);
		
		TRACE_DEBUG(FULL, "[tne] Sending now %p", m);
		CHECK_FCT( fd_msg_send(&m, NULL, NULL) );
	}
	
	return 0;

}


/******************************************************************/
/* the processing thread */
static void * tne_process_th(void * arg) 
{
	TRACE_ENTRY("%p", arg);
	
	/* Name the thread */
	fd_log_threadname ( "test_netemul/process" );
	
	CHECK_POSIX_DO( pthread_mutex_lock(&mtx), goto error );
	pthread_cleanup_push( fd_cleanup_mutex, &mtx );
	
	/* The loop */
	while (1) {
		/* First, test if we are canceled */
		pthread_testcancel();
		
		/* Send all messages that are ready (free resources before using new ones) */
		CHECK_FCT_DO( send_all_ready(), break );
		
		/* Now process the new messages in input list for duplicate filter */
		CHECK_FCT_DO( do_duplicates(), break );
		
		/* Now compute the latency for each new item */
		CHECK_FCT_DO( do_latency(), break );
		
		/* Now, wait then loop */
		if (FD_IS_LIST_EMPTY(&waitlist)) {
			CHECK_POSIX_DO( pthread_cond_wait(&cnd, &mtx), break );
		} else {
			CHECK_POSIX_DO2( pthread_cond_timedwait(&cnd, &mtx, &((struct process_item *)(waitlist.next))->ts), 
				ETIMEDOUT, /* ETIMEDOUT is a normal return value, continue */,
					/* on other error, */ break );
		}
		
		/* loop */
	}

	pthread_cleanup_pop( 0 );
	CHECK_POSIX_DO( pthread_mutex_unlock(&mtx),  );
error:
	TRACE_DEBUG(INFO, "A fatal error occurred in test_netemul/process thread!");
	ASSERT(0);
	CHECK_FCT_DO(fd_core_shutdown(), );
	return NULL;
}

/******************************************************************/
/* functions visible from outside this file */
int tne_process_init() 
{
	CHECK_POSIX( pthread_create(&thr, NULL, tne_process_th, NULL) );
	
	#if 0 /* debug */
	int i;
	for (i=0; i< 20; i++) {
		printf("LAT: %lu\n", get_latency()); 
	}
	#endif /* 0 */
	
	return 0;
}


int tne_process_fini() 
{
	CHECK_FCT( fd_thr_term(&thr) );
	return 0;
}


int tne_process_message(struct msg * msg) 
{
	struct process_item * pi;
	
	TRACE_ENTRY("%p", msg);
	
	/* Create a new pi for this message */
	CHECK_MALLOC( pi = malloc(sizeof(struct process_item)) );
	memset(pi, 0, sizeof(struct process_item));
	fd_list_init(&pi->chain, msg);
	CHECK_SYS(clock_gettime(CLOCK_REALTIME, &pi->ts));
	
	/* Store it in the input list */
	CHECK_POSIX( pthread_mutex_lock(&mtx) );
	fd_list_insert_before(&input, &pi->chain);
	CHECK_POSIX( pthread_mutex_unlock(&mtx) );
	
	/* Wake up the process thread so that it processes the message */
	CHECK_POSIX( pthread_cond_signal(&cnd) );
	
	/* done */
	return 0;
}
