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

/* 
 * Test application for freeDiameter.
 */

#include "test_app.h"

/* Initialize the configuration */
struct ta_conf * ta_conf = NULL;
static struct ta_conf _conf;
static pthread_t ta_stats_th = (pthread_t)NULL;

static int ta_conf_init(void)
{
	ta_conf = &_conf;
	memset(ta_conf, 0, sizeof(struct ta_conf));
	
	/* Set the default values */
	ta_conf->vendor_id  = 999999;		/* Dummy value */
	ta_conf->appli_id   = 0xffffff;	/* dummy value */
	ta_conf->cmd_id     = 0xfffffe;	/* Experimental */
	ta_conf->avp_id     = 0xffffff;	/* dummy value */
	ta_conf->long_avp_len = 5000;
	ta_conf->mode       = MODE_SERV | MODE_CLI;
	ta_conf->dest_realm = strdup(fd_g_config->cnf_diamrlm);
	ta_conf->dest_host  = NULL;
	ta_conf->signal     = TEST_APP_DEFAULT_SIGNAL;
	ta_conf->bench_concur   = 100;
	ta_conf->bench_duration = 10;
	
	/* Initialize the mutex */
	CHECK_POSIX( pthread_mutex_init(&ta_conf->stats_lock, NULL) );
	
	return 0;
}

static void ta_conf_dump(void)
{
	if (!TRACE_BOOL(INFO))
		return;
	fd_log_debug( "------- app_test configuration dump: ---------");
	fd_log_debug( " Vendor Id .......... : %u", ta_conf->vendor_id);
	fd_log_debug( " Application Id ..... : %u", ta_conf->appli_id);
	fd_log_debug( " Command Id ......... : %u", ta_conf->cmd_id);
	fd_log_debug( " AVP Id ............. : %u", ta_conf->avp_id);
	fd_log_debug( " Long AVP Id ........ : %u", ta_conf->long_avp_id);
	fd_log_debug( " Long AVP len ....... : %zu", ta_conf->long_avp_len);
	fd_log_debug( " Mode ............... : %s%s%s", ta_conf->mode & MODE_SERV ? "Serv" : "", ta_conf->mode & MODE_CLI ? "Cli" : "",  ta_conf->mode & MODE_BENCH ? " (Benchmark)" : "");
	fd_log_debug( " Destination Realm .. : %s", ta_conf->dest_realm ?: "- none -");
	fd_log_debug( " Destination Host ... : %s", ta_conf->dest_host ?: "- none -");
	fd_log_debug( " Signal ............. : %i", ta_conf->signal);
	fd_log_debug( "------- /app_test configuration dump ---------");
}

/* Function to display statistics periodically */
static void * ta_stats(void * arg) {

	struct timespec start, now;
	struct ta_stats copy;
	
	/* Get the start time */
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &start), );
	
	/* Now, loop until canceled */
	while (1) {
		/* Display statistics every XX seconds */
		sleep(ta_conf->bench_duration + 3);
		
		/* Now, get the current stats */
		CHECK_POSIX_DO( pthread_mutex_lock(&ta_conf->stats_lock), );
		memcpy(&copy, &ta_conf->stats, sizeof(struct ta_stats));
		CHECK_POSIX_DO( pthread_mutex_unlock(&ta_conf->stats_lock), );
		
		/* Get the current execution time */
		CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );
		
		/* Now, display everything */
		fd_log_debug( "------- app_test statistics ---------");
		if (now.tv_nsec >= start.tv_nsec) {
			fd_log_debug( " Executing for: %d.%06ld sec",
					(int)(now.tv_sec - start.tv_sec),
					(long)(now.tv_nsec - start.tv_nsec) / 1000);
		} else {
			fd_log_debug( " Executing for: %d.%06ld sec",
					(int)(now.tv_sec - 1 - start.tv_sec),
					(long)(now.tv_nsec + 1000000000 - start.tv_nsec) / 1000);
		}
		
		if (ta_conf->mode & MODE_SERV) {
			fd_log_debug( " Server: %llu message(s) echoed", copy.nb_echoed);
		}
		if (ta_conf->mode & MODE_CLI) {
			fd_log_debug( " Client:");
			fd_log_debug( "   %llu message(s) sent", copy.nb_sent);
			fd_log_debug( "   %llu error(s) received", copy.nb_errs);
			fd_log_debug( "   %llu answer(s) received", copy.nb_recv);
			fd_log_debug( "     fastest: %ld.%06ld sec.", copy.shortest / 1000000, copy.shortest % 1000000);
			fd_log_debug( "     slowest: %ld.%06ld sec.", copy.longest / 1000000, copy.longest % 1000000);
			fd_log_debug( "     Average: %ld.%06ld sec.", copy.avg / 1000000, copy.avg % 1000000);
		}
		fd_log_debug( "-------------------------------------");
	}
	
	return NULL; /* never called */
}

static struct fd_hook_hdl * hookhdl[2] = { NULL, NULL };
static void ta_hook_cb_silent(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata) {
}
static void ta_hook_cb_oneline(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata) {
	char * buf = NULL;
	size_t len;
	
	CHECK_MALLOC_DO( fd_msg_dump_summary(&buf, &len, NULL, msg, NULL, 0, 0), 
		{ LOG_E("Error while dumping a message"); return; } );
	
	LOG_N("{%d} %s: %s", type, (char *)other ?:"<nil>", buf ?:"<nil>");

	free(buf);
}


/* entry point */
static int ta_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	/* Initialize configuration */
	CHECK_FCT( ta_conf_init() );
	
	/* Parse configuration file */
	if (conffile != NULL) {
		CHECK_FCT( ta_conf_handle(conffile) );
	}
	
	TRACE_DEBUG(INFO, "Extension Test_App initialized with configuration: '%s'", conffile);
	ta_conf_dump();
	
	/* Install objects definitions for this test application */
	CHECK_FCT( ta_dict_init() );
	
	/* Install the handlers for incoming messages */
	if (ta_conf->mode & MODE_SERV) {
		CHECK_FCT( ta_serv_init() );
	}
	
	/* Start the signal handler thread */
	if (ta_conf->mode & MODE_CLI) {
		if (ta_conf->mode & MODE_BENCH) {
			CHECK_FCT( ta_bench_init() );
		} else {
			CHECK_FCT( ta_cli_init() );
		}
	}
	
	/* Advertise the support for the test application in the peer */
	CHECK_FCT( fd_disp_app_support ( ta_appli, ta_vendor, 1, 0 ) );
	
	if (ta_conf->mode & MODE_BENCH) {
		/* Register an empty hook to disable the default handling */
		CHECK_FCT( fd_hook_register( HOOK_MASK( HOOK_DATA_RECEIVED, HOOK_MESSAGE_RECEIVED, HOOK_MESSAGE_LOCAL, HOOK_MESSAGE_SENT, HOOK_MESSAGE_ROUTING_FORWARD, HOOK_MESSAGE_ROUTING_LOCAL ), 
					ta_hook_cb_silent, NULL, NULL, &hookhdl[0]) );
		CHECK_FCT( fd_hook_register( HOOK_MASK( HOOK_MESSAGE_ROUTING_ERROR, HOOK_MESSAGE_DROPPED ), 
					ta_hook_cb_oneline, NULL, NULL, &hookhdl[1]) );
		
	}
	
	/* Start the statistics thread */
	CHECK_POSIX( pthread_create(&ta_stats_th, NULL, ta_stats, NULL) );
	
	return 0;
}

/* Unload */
void fd_ext_fini(void)
{
	if (ta_conf->mode & MODE_CLI)
		ta_cli_fini();
	if (ta_conf->mode & MODE_SERV)
		ta_serv_fini();
	if (hookhdl[0])
		fd_hook_unregister( hookhdl[0] );
	if (hookhdl[1])
		fd_hook_unregister( hookhdl[1] );
	CHECK_FCT_DO( fd_thr_term(&ta_stats_th), );
	CHECK_POSIX_DO( pthread_mutex_destroy(&ta_conf->stats_lock), );
}

EXTENSION_ENTRY("test_app", ta_entry);
