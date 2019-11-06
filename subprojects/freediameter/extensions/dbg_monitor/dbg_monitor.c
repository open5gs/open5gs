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

/* Monitoring extension:
 - periodically display queues and peers information
 - upon SIGUSR2, display additional debug information
 */

#include <freeDiameter/extension.h>
#include <signal.h>

#ifndef MONITOR_SIGNAL
#define MONITOR_SIGNAL	SIGUSR2
#endif /* MONITOR_SIGNAL */

static int 	 monitor_main(char * conffile);

EXTENSION_ENTRY("dbg_monitor", monitor_main);



/* Display information about a queue */
static void display_info(char * queue_desc, char * peer, int current_count, int limit_count, int highest_count, long long total_count,
			struct timespec * total, struct timespec * blocking, struct timespec * last)
{
	long long us = (total->tv_sec * 1000000) + (total->tv_nsec / 1000);
	long double throughput = (long double)total_count * 1000000;
	throughput /= us;
	if (peer) {
		TRACE_DEBUG(INFO, "'%s'@'%s': cur:%d/%d, h:%d, T:%lld in %ld.%06lds (%.2LFitems/s), blocked:%ld.%06lds, last processing:%ld.%06lds",
			queue_desc, peer, current_count, limit_count, highest_count,
			total_count, total->tv_sec, total->tv_nsec/1000, throughput,
			blocking->tv_sec, blocking->tv_nsec/1000, last->tv_sec, last->tv_nsec/1000);
	} else {
		TRACE_DEBUG(INFO, "Global '%s': cur:%d/%d, h:%d, T:%lld in %ld.%06lds (%.2LFitems/s), blocked:%ld.%06lds, last processing:%ld.%06lds",
			queue_desc, current_count, limit_count, highest_count,
			total_count, total->tv_sec, total->tv_nsec/1000, throughput,
			blocking->tv_sec, blocking->tv_nsec/1000, last->tv_sec, last->tv_nsec/1000);
	}
}

/* Thread to display periodical debug information */
static pthread_t thr;
static void * mn_thr(void * arg)
{
	int i = 0;
	fd_log_threadname("Monitor thread");
	char * buf = NULL;
	size_t len;
	
	/* Loop */
	while (1) {
		int current_count, limit_count, highest_count;
		long long total_count;
		struct timespec total, blocking, last;
		struct fd_list * li;
	
		#ifdef DEBUG
		for (i++; i % 30; i++) {
			fd_log_debug("[dbg_monitor] %ih%*im%*is", i/3600, 2, (i/60) % 60 , 2, i%60); /* This makes it easier to detect inactivity periods in the log file */
			sleep(1);
		}
		#else /* DEBUG */
		sleep(3599); /* 1 hour */
		#endif /* DEBUG */
		TRACE_DEBUG(INFO, "[dbg_monitor] Dumping queues statistics");
		
		CHECK_FCT_DO( fd_stat_getstats(STAT_G_LOCAL, NULL, &current_count, &limit_count, &highest_count, &total_count, &total, &blocking, &last), );
		display_info("Local delivery", NULL, current_count, limit_count, highest_count, total_count, &total, &blocking, &last);
		
		CHECK_FCT_DO( fd_stat_getstats(STAT_G_INCOMING, NULL, &current_count, &limit_count, &highest_count, &total_count, &total, &blocking, &last), );
		display_info("Total received", NULL, current_count, limit_count, highest_count, total_count, &total, &blocking, &last);
		
		CHECK_FCT_DO( fd_stat_getstats(STAT_G_OUTGOING, NULL, &current_count, &limit_count, &highest_count, &total_count, &total, &blocking, &last), );
		display_info("Total sending", NULL, current_count, limit_count, highest_count, total_count, &total, &blocking, &last);
		
		
		CHECK_FCT_DO( pthread_rwlock_rdlock(&fd_g_peers_rw), /* continue */ );

		for (li = fd_g_peers.next; li != &fd_g_peers; li = li->next) {
			struct peer_hdr * p = (struct peer_hdr *)li->o;
			
			TRACE_DEBUG(INFO, "%s", fd_peer_dump(&buf, &len, NULL, p, 1));
			
			CHECK_FCT_DO( fd_stat_getstats(STAT_P_PSM, p, &current_count, &limit_count, &highest_count, &total_count, &total, &blocking, &last), );
			display_info("Events, incl. recept", p->info.pi_diamid, current_count, limit_count, highest_count, total_count, &total, &blocking, &last);
			
			CHECK_FCT_DO( fd_stat_getstats(STAT_P_TOSEND, p, &current_count, &limit_count, &highest_count, &total_count, &total, &blocking, &last), );
			display_info("Outgoing", p->info.pi_diamid, current_count, limit_count, highest_count, total_count, &total, &blocking, &last);
			
		}

		CHECK_FCT_DO( pthread_rwlock_unlock(&fd_g_peers_rw), /* continue */ );
		
		TRACE_DEBUG(INFO, "[dbg_monitor] Dumping servers information");
		TRACE_DEBUG(INFO, "%s", fd_servers_dump(&buf, &len, NULL, 1));
		
		sleep(1);
	}
	
	free(buf);
	return NULL;
}

/* Function called on receipt of MONITOR_SIGNAL */
static void got_sig()
{
	char * buf = NULL;
	size_t len;
	TRACE_DEBUG(INFO, "[dbg_monitor] Dumping config information");
	TRACE_DEBUG(INFO, "%s", fd_conf_dump(&buf, &len, NULL));
	TRACE_DEBUG(INFO, "[dbg_monitor] Dumping extensions information");
	TRACE_DEBUG(INFO, "%s", fd_ext_dump(&buf, &len, NULL));
	TRACE_DEBUG(INFO, "[dbg_monitor] Dumping dictionary information");
	TRACE_DEBUG(INFO, "%s", fd_dict_dump(&buf, &len, NULL, fd_g_config->cnf_dict));
	free(buf);
}

/* Entry point */
static int monitor_main(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	/* Catch signal SIGUSR1 */
	CHECK_FCT( fd_event_trig_regcb(MONITOR_SIGNAL, "dbg_monitor", got_sig));
	
	CHECK_POSIX( pthread_create( &thr, NULL, mn_thr, NULL ) );
	return 0;
}

/* Cleanup */
void fd_ext_fini(void)
{
	TRACE_ENTRY();
	CHECK_FCT_DO( fd_thr_term(&thr), /* continue */ );
	return ;
}

