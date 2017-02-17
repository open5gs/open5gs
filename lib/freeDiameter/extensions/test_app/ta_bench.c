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

/* Create and send a message, and receive it */

#include "test_app.h"

#include <semaphore.h>
#include <stdio.h>

struct ta_mess_info {
	int32_t		randval;	/* a random value to store in Test-AVP */
	struct timespec ts;		/* Time of sending the message */
};

static sem_t ta_sem; /* To handle the concurrency */

/* Cb called when an answer is received */
static void ta_cb_ans(void * data, struct msg ** msg)
{
	struct ta_mess_info * mi = (struct ta_mess_info *)data;
	struct timespec ts;
	struct avp * avp;
	struct avp_hdr * hdr;
	unsigned long dur;
	
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

	/* Value of Result Code */
	CHECK_FCT_DO( fd_msg_search_avp ( *msg, ta_res_code, &avp), return );
	if (avp) {
		CHECK_FCT_DO( fd_msg_avp_hdr( avp, &hdr ), return );
	}
	if (!avp || !hdr || hdr->avp_value->i32 != 2001) {
		/* error */
		CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
		ta_conf->stats.nb_errs++;
		CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );
		goto end;
	}
	
	/* Check value of Test-AVP */
	CHECK_FCT_DO( fd_msg_search_avp ( *msg, ta_avp, &avp), return );
	if (avp) {
		CHECK_FCT_DO( fd_msg_avp_hdr( avp, &hdr ), return );
		ASSERT(hdr->avp_value->i32 == mi->randval);
	}
	
	/* Compute how long it took */
	dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
	
	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
	
	if (ta_conf->stats.nb_recv) {
		/* Ponderate in the avg */
		ta_conf->stats.avg = (ta_conf->stats.avg * ta_conf->stats.nb_recv + dur) / (ta_conf->stats.nb_recv + 1);
		/* Min, max */
		if (dur < ta_conf->stats.shortest)
			ta_conf->stats.shortest = dur;
		if (dur > ta_conf->stats.longest)
			ta_conf->stats.longest = dur;
	} else {
		ta_conf->stats.shortest = dur;
		ta_conf->stats.longest = dur;
		ta_conf->stats.avg = dur;
	}
	ta_conf->stats.nb_recv++;
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );
	
end:	
	/* Free the message */
	CHECK_FCT_DO(fd_msg_free(*msg), );
	*msg = NULL;
	
	free(mi);
	
	/* Post the semaphore */
	CHECK_SYS_DO( sem_post(&ta_sem), );
	
	return;
}

/* Create a test message */
static void ta_bench_test_message()
{
	struct msg * req = NULL;
	struct avp * avp;
	union avp_value val;
	struct ta_mess_info * mi = NULL;
	
	TRACE_DEBUG(FULL, "Creating a new message for sending.");
	
	/* Create the request */
	CHECK_FCT_DO( fd_msg_new( ta_cmd_r, MSGFL_ALLOC_ETEID, &req ), goto out );
	
	/* Create a new session */
	#define TEST_APP_SID_OPT  "app_testb"
	CHECK_FCT_DO( fd_msg_new_session( req, (os0_t)TEST_APP_SID_OPT, CONSTSTRLEN(TEST_APP_SID_OPT) ), goto out );
	
	/* Create the random value to store with the session */
	mi = malloc(sizeof(struct ta_mess_info));
	if (mi == NULL) {
		fd_log_debug("malloc failed: %s", strerror(errno));
		goto out;
	}
	
	mi->randval = (int32_t)random();
	
	/* Now set all AVPs values */
	
	/* Set the Destination-Realm AVP */
	{
		CHECK_FCT_DO( fd_msg_avp_new ( ta_dest_realm, 0, &avp ), goto out  );
		val.os.data = (unsigned char *)(ta_conf->dest_realm);
		val.os.len  = strlen(ta_conf->dest_realm);
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	}
	
	/* Set the Destination-Host AVP if needed*/
	if (ta_conf->dest_host) {
		CHECK_FCT_DO( fd_msg_avp_new ( ta_dest_host, 0, &avp ), goto out  );
		val.os.data = (unsigned char *)(ta_conf->dest_host);
		val.os.len  = strlen(ta_conf->dest_host);
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	}
	
	/* Set Origin-Host & Origin-Realm */
	CHECK_FCT_DO( fd_msg_add_origin ( req, 0 ), goto out  );
	
	/* Set the User-Name AVP if needed*/
	if (ta_conf->user_name) {
		CHECK_FCT_DO( fd_msg_avp_new ( ta_user_name, 0, &avp ), goto out  );
		val.os.data = (unsigned char *)(ta_conf->user_name);
		val.os.len  = strlen(ta_conf->user_name);
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	}
	
	/* Set the Test-AVP AVP */
	{
		CHECK_FCT_DO( fd_msg_avp_new ( ta_avp, 0, &avp ), goto out  );
		val.i32 = mi->randval;
		CHECK_FCT_DO( fd_msg_avp_setvalue( avp, &val ), goto out  );
		CHECK_FCT_DO( fd_msg_avp_add( req, MSG_BRW_LAST_CHILD, avp ), goto out  );
	}
	
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
	
	/* Send the request */
	CHECK_FCT_DO( fd_msg_send( &req, ta_cb_ans, mi ), goto out );
	
	/* Increment the counter */
	CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
	ta_conf->stats.nb_sent++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );

out:
	return;
}

/* The function called when the signal is received */
static void ta_bench_start() {
	struct timespec end_time, now;
	struct ta_stats start, end;
	int nsec = 0;
	
	/* Save the initial stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
	memcpy(&start, &ta_conf->stats, sizeof(struct ta_stats));
	CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );
	
	/* We will run for ta_conf->bench_duration seconds */
	LOG_N("Starting benchmark client, %ds", ta_conf->bench_duration);
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &end_time), );
	end_time.tv_sec += ta_conf->bench_duration;
	
	/* Now loop until timeout is reached */
	do {
		/* Do not create more that NB_CONCURRENT_MESSAGES in paralel */
		int ret = sem_timedwait(&ta_sem, &end_time);
		if (ret == -1) {
			ret = errno;
			if (ret != ETIMEDOUT) {
				CHECK_POSIX_DO(ret, ); /* Just to log it */
			}
			break;
		}
		
		/* Update the current time */
		CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );
		
		if (!TS_IS_INFERIOR(&now, &end_time))
			break;
		
		/* Create and send a new test message */
		ta_bench_test_message();
	} while (1);
	
	do {
		CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
		CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), ); /* Re-read the time because we might have spent some time wiating for the mutex */
		memcpy(&end, &ta_conf->stats, sizeof(struct ta_stats));
		CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );
		
		/* Now, display the statistics */
		LOG_N( "------- app_test Benchmark results, end sending +%ds ---------", nsec);
		if (now.tv_nsec >= end_time.tv_nsec) {
			LOG_N( " Executing for: %d.%06ld sec",
					(int)(now.tv_sec + ta_conf->bench_duration - end_time.tv_sec),
					(long)(now.tv_nsec - end_time.tv_nsec) / 1000);
		} else {
			LOG_N( " Executing for: %d.%06ld sec",
					(int)(now.tv_sec + ta_conf->bench_duration - 1 - end_time.tv_sec),
					(long)(now.tv_nsec + 1000000000 - end_time.tv_nsec) / 1000);
		}
		LOG_N( "   %llu messages sent", end.nb_sent - start.nb_sent);
		LOG_N( "   %llu error(s) received", end.nb_errs - start.nb_errs);
		LOG_N( "   %llu answer(s) received", end.nb_recv - start.nb_recv);
		LOG_N( "   Overall:");
		LOG_N( "     fastest: %ld.%06ld sec.", end.shortest / 1000000, end.shortest % 1000000);
		LOG_N( "     slowest: %ld.%06ld sec.", end.longest / 1000000, end.longest % 1000000);
		LOG_N( "     Average: %ld.%06ld sec.", end.avg / 1000000, end.avg % 1000000);
		LOG_N( "   Throughput: %llu messages / sec", (end.nb_recv - start.nb_recv) / (( now.tv_sec + ta_conf->bench_duration - end_time.tv_sec ) + ((now.tv_nsec - end_time.tv_nsec) / 1000000000)));
		LOG_N( "-------------------------------------");
		
		nsec ++;
		sleep(1);
	} while ( (end.nb_sent - start.nb_sent) > (end.nb_errs - start.nb_errs) + (end.nb_recv - start.nb_recv) );
	LOG_N( "--------------- Test Complete --------------");

}


int ta_bench_init(void)
{
	CHECK_SYS( sem_init( &ta_sem, 0, ta_conf->bench_concur) );

	CHECK_FCT( fd_event_trig_regcb(ta_conf->signal, "test_app.bench", ta_bench_start ) );
	
	return 0;
}

void ta_bench_fini(void)
{
	// CHECK_FCT_DO( fd_sig_unregister(ta_conf->signal), /* continue */ );
	
	CHECK_SYS_DO( sem_destroy(&ta_sem), );
	
	return;
};
