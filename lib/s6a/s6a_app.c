#define TRACE_MODULE _s6a_app

#include "s6a_app.h"

static pthread_t s6a_stats_th = (pthread_t)NULL;

static void s6a_config_dump(void)
{
	LOG_N( "------- s6a configuration dump: ---------");
	LOG_N( " Vendor Id .......... : %u", s6a_config->vendor_id);
	LOG_N( " Application Id ..... : %u", s6a_config->appli_id);
	LOG_N( " Mode ............... : %s%s", 
            s6a_config->mode & MODE_MME ? "MME" : "", 
            s6a_config->mode & MODE_HSS ? "HSS" : "");
	LOG_N( " Destination Realm .. : %s", 
            s6a_config->dest_realm ?: "- none -");
	LOG_N( " Destination Host ... : %s", 
            s6a_config->dest_host ?: "- none -");
	LOG_N( "------- /s6a configuration dump ---------");
}

/* Function to display statistics periodically */
static void * s6a_stats(void * arg) 
{
	struct timespec start, now;
	struct ta_stats copy;
	
	/* Get the start time */
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &start), );
	
	/* Now, loop until canceled */
	while (1) {
		/* Display statistics every XX seconds */
		sleep(10);
		
		/* Now, get the current stats */
		CHECK_POSIX_DO( pthread_mutex_lock(&s6a_config->stats_lock), );
		memcpy(&copy, &s6a_config->stats, sizeof(struct ta_stats));
		CHECK_POSIX_DO( pthread_mutex_unlock(&s6a_config->stats_lock), );
		
		/* Get the current execution time */
		CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );
		
		/* Now, display everything */
		LOG_N( "------- app_test statistics ---------");
		if (now.tv_nsec >= start.tv_nsec) {
			LOG_N( " Executing for: %d.%06ld sec",
					(int)(now.tv_sec - start.tv_sec),
					(long)(now.tv_nsec - start.tv_nsec) / 1000);
		} else {
			LOG_N( " Executing for: %d.%06ld sec",
					(int)(now.tv_sec - 1 - start.tv_sec),
					(long)(now.tv_nsec + 1000000000 - start.tv_nsec) / 1000);
		}
		
		if (s6a_config->mode & MODE_HSS) {
			LOG_N( " HSS: %" C_UINT64_T_FMT " message(s) echoed", copy.nb_echoed);
		}
		if (s6a_config->mode & MODE_MME) {
			LOG_N( " MME:");
			LOG_N( "   %" C_UINT64_T_FMT " message(s) sent", copy.nb_sent);
			LOG_N( "   %" C_UINT64_T_FMT " error(s) received", copy.nb_errs);
			LOG_N( "   %" C_UINT64_T_FMT " answer(s) received", copy.nb_recv);
		}
		LOG_N( "-------------------------------------");
	}
	
	return NULL; /* never called */
}

/* entry point */
int s6a_app_init(int mode)
{
    /* Configure Application Mode(MME, HSS) */
    s6a_config->mode = mode;

	/* Initialize the mutex */
	CHECK_POSIX( pthread_mutex_init(&s6a_config->stats_lock, NULL) );
	
	s6a_config_dump();
	
	/* Install objects definitions for this test application */
	CHECK_FCT( s6a_dict_init() );
	
	/* Start the signal handler thread */
	if (s6a_config->mode & MODE_MME) {
        CHECK_FCT( s6a_mme_init() );
	}
	
	/* Install the handlers for incoming messages */
	if (s6a_config->mode & MODE_HSS) {
		CHECK_FCT( s6a_hss_init() );
	}
	
	/* Advertise the support for the test application in the peer */
	CHECK_FCT( fd_disp_app_support ( s6a_appli, s6a_vendor, 1, 0 ) );
	
	/* Start the statistics thread */
	CHECK_POSIX( pthread_create(&s6a_stats_th, NULL, s6a_stats, NULL) );
	
	return 0;
}

/* Unload */
void s6a_app_final(void)
{
	if (s6a_config->mode & MODE_MME)
		s6a_mme_final();
	if (s6a_config->mode & MODE_HSS)
		s6a_hss_final();

	CHECK_FCT_DO( fd_thr_term(&s6a_stats_th), );
	CHECK_POSIX_DO( pthread_mutex_destroy(&s6a_config->stats_lock), );
}
