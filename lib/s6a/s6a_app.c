#define TRACE_MODULE _s6a_app

#include "core_debug.h"

#include "s6a_app.h"

static pthread_t s6a_stats_th = (pthread_t)NULL;

static void s6a_config_dump(void)
{
	d_trace(1, "------- s6a configuration dump: ---------\n");
	d_trace(1, " Mode ............... : %s%s\n", 
            s6a_config->mode & MODE_MME ? "MME" : "", 
            s6a_config->mode & MODE_HSS ? "HSS" : "");
	d_trace(1, " Vendor Id .......... : %u\n", s6a_config->vendor_id);
	d_trace(1, " Application Id ..... : %u\n", s6a_config->appli_id);
	d_trace(1, " Duration ........... : %d(sec)\n", s6a_config->duration);
	d_trace(1, "------- /s6a configuration dump ---------\n");
}

/* Function to display statistics periodically */
static void * s6a_stats(void * arg) 
{
	struct timespec start, now;
	struct s6a_stats copy;
	
	/* Get the start time */
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &start), );
	
	/* Now, loop until canceled */
	while (1) {
		/* Display statistics every XX seconds */
		sleep(s6a_config->duration);
		
		/* Now, get the current stats */
		CHECK_POSIX_DO( pthread_mutex_lock(&s6a_config->stats_lock), );
		memcpy(&copy, &s6a_config->stats, sizeof(struct s6a_stats));
		CHECK_POSIX_DO( pthread_mutex_unlock(&s6a_config->stats_lock), );
		
		/* Get the current execution time */
		CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );
		
		/* Now, display everything */
		d_trace(1, "------- app_test statistics ---------\n");
		if (now.tv_nsec >= start.tv_nsec) 
        {
			d_trace(1, " Executing for: %d.%06ld sec\n",
					(int)(now.tv_sec - start.tv_sec),
					(long)(now.tv_nsec - start.tv_nsec) / 1000);
		} 
        else 
        {
			d_trace(1, " Executing for: %d.%06ld sec\n",
					(int)(now.tv_sec - 1 - start.tv_sec),
					(long)(now.tv_nsec + 1000000000 - start.tv_nsec) / 1000);
		}
		
		if (s6a_config->mode & MODE_HSS) 
        {
			d_trace(1, " HSS: %llu message(s) echoed\n", 
                    copy.nb_echoed);
		}
		if (s6a_config->mode & MODE_MME) 
        {
			d_trace(1, " MME:\n");
			d_trace(1, "   %llu message(s) sent\n", copy.nb_sent);
			d_trace(1, "   %llu error(s) received\n", copy.nb_errs);
			d_trace(1, "   %llu answer(s) received\n", copy.nb_recv);
			d_trace(1, "     fastest: %ld.%06ld sec.\n", 
                    copy.shortest / 1000000, copy.shortest % 1000000);
			d_trace(1, "     slowest: %ld.%06ld sec.\n", 
                    copy.longest / 1000000, copy.longest % 1000000);
			d_trace(1, "     Average: %ld.%06ld sec.\n", 
                    copy.avg / 1000000, copy.avg % 1000000);
		}
		d_trace(1, "-------------------------------------\n");
	}
	
	return NULL; /* never called */
}

/* entry point */
int s6a_app_init(int mode)
{
    d_trace_level(&_s6a_app, 0);

    /* Configure Application Mode(MME, HSS) */
    s6a_config->mode = mode;

	/* Initialize the mutex */
	CHECK_POSIX( pthread_mutex_init(&s6a_config->stats_lock, NULL) );
	
	s6a_config_dump();
	
	/* Install objects definitions for this test application */
	CHECK_FCT( s6a_dict_init() );
	
	/* Start the signal handler thread */
	if (s6a_config->mode & MODE_MME) {
        CHECK_FCT( mme_init() );
	}
	
	/* Install the handlers for incoming messages */
	if (s6a_config->mode & MODE_HSS) {
		CHECK_FCT( hss_init() );
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
		mme_final();
	if (s6a_config->mode & MODE_HSS)
		hss_final();

	CHECK_FCT_DO( fd_thr_term(&s6a_stats_th), );
	CHECK_POSIX_DO( pthread_mutex_destroy(&s6a_config->stats_lock), );
}
