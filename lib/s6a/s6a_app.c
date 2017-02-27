#define TRACE_MODULE _s6a_app

#include "s6a_app.h"

struct s6a_conf *s6a_conf = NULL;
static struct s6a_conf _conf;
static pthread_t s6a_stats_th = (pthread_t)NULL;

static int s6a_conf_init(void)
{
	s6a_conf = &_conf;
	memset(s6a_conf, 0, sizeof(struct s6a_conf));
	
	/* Set the default values */
	s6a_conf->vendor_id  = 10415;	/* 3GPP Vendor ID */
	s6a_conf->appli_id   = 16777251;	/* 3GPP S6A Application ID */
	s6a_conf->mode       = MODE_SERV | MODE_CLI;
	s6a_conf->dest_realm = strdup(fd_g_config->cnf_diamrlm);
	s6a_conf->dest_host  = NULL;
	
	/* Initialize the mutex */
	CHECK_POSIX( pthread_mutex_init(&s6a_conf->stats_lock, NULL) );
	
	return 0;
}

static void s6a_conf_dump(void)
{
	LOG_N( "------- s6a configuration dump: ---------");
	LOG_N( " Vendor Id .......... : %u", s6a_conf->vendor_id);
	LOG_N( " Application Id ..... : %u", s6a_conf->appli_id);
	LOG_N( " Mode ............... : %s%s", 
            s6a_conf->mode & MODE_SERV ? "Serv" : "", 
            s6a_conf->mode & MODE_CLI ? "Cli" : "");
	LOG_N( " Destination Realm .. : %s", 
            s6a_conf->dest_realm ?: "- none -");
	LOG_N( " Destination Host ... : %s", 
            s6a_conf->dest_host ?: "- none -");
	LOG_N( "------- /s6a configuration dump ---------");
}

/* Function to display statistics periodically */
static void * s6a_stats(void * arg) {

	struct timespec start, now;
	struct ta_stats copy;
	
	/* Get the start time */
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &start), );
	
	/* Now, loop until canceled */
	while (1) {
		/* Display statistics every XX seconds */
		sleep(10);
		
		/* Now, get the current stats */
		CHECK_POSIX_DO( pthread_mutex_lock(&s6a_conf->stats_lock), );
		memcpy(&copy, &s6a_conf->stats, sizeof(struct ta_stats));
		CHECK_POSIX_DO( pthread_mutex_unlock(&s6a_conf->stats_lock), );
		
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
		
		if (s6a_conf->mode & MODE_SERV) {
			fd_log_debug( " Server: %llu message(s) echoed", copy.nb_echoed);
		}
		if (s6a_conf->mode & MODE_CLI) {
			fd_log_debug( " Client:");
			fd_log_debug( "   %llu message(s) sent", copy.nb_sent);
			fd_log_debug( "   %llu error(s) received", copy.nb_errs);
			fd_log_debug( "   %llu answer(s) received", copy.nb_recv);
		}
		fd_log_debug( "-------------------------------------");
	}
	
	return NULL; /* never called */
}

/* entry point */
int s6a_app_init(void)
{
	/* Initialize configuration */
	CHECK_FCT( s6a_conf_init() );
	
	s6a_conf_dump();
	
	/* Install objects definitions for this test application */
	CHECK_FCT( s6a_dict_init() );
#if 0
	
	/* Install the handlers for incoming messages */
	if (ta_conf->mode & MODE_SERV) {
		CHECK_FCT( ta_serv_init() );
	}
	
	/* Start the signal handler thread */
	if (ta_conf->mode & MODE_CLI) {
        CHECK_FCT( ta_cli_init() );
	}
	
	/* Advertise the support for the test application in the peer */
	CHECK_FCT( fd_disp_app_support ( ta_appli, ta_vendor, 1, 0 ) );
	
#endif
	/* Start the statistics thread */
	CHECK_POSIX( pthread_create(&s6a_stats_th, NULL, s6a_stats, NULL) );
	
	return 0;
}

/* Unload */
void s6a_app_final(void)
{
#if 0
	if (ta_conf->mode & MODE_CLI)
		ta_cli_fini();
	if (ta_conf->mode & MODE_SERV)
		ta_serv_fini();
#endif

	CHECK_FCT_DO( fd_thr_term(&s6a_stats_th), );
	CHECK_POSIX_DO( pthread_mutex_destroy(&s6a_conf->stats_lock), );
}
