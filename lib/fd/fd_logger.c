#define TRACE_MODULE _fd_logger

#include "core_debug.h"

#include "fd_logger.h"

static struct fd_logger_t self;

static struct fd_hook_hdl *logger_hdl = NULL;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_t fd_stats_th = (pthread_t)NULL;

static fd_logger_user_handler user_handler = NULL;

static void fd_logger_cb(enum fd_hook_type type, struct msg * msg, 
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, 
    void * regdata);
static void * fd_stats_worker(void * arg);

int fd_logger_init(int mode)
{
	uint32_t mask_peers = HOOK_MASK( HOOK_PEER_CONNECT_SUCCESS );

    memset(&self, 0, sizeof(struct fd_logger_t));

    self.mode = mode;
	self.duration = 60;       /* 60 seconds */
	
    CHECK_FCT( fd_hook_register( 
            mask_peers, fd_logger_cb, NULL, NULL, &logger_hdl) );

	CHECK_POSIX( pthread_mutex_init(&self.stats_lock, NULL) );

	return 0;
}

void fd_logger_final()
{
	CHECK_FCT_DO( fd_thr_term(&fd_stats_th), );
	CHECK_POSIX_DO( pthread_mutex_destroy(&self.stats_lock), );

	if (logger_hdl) { CHECK_FCT_DO( fd_hook_unregister( logger_hdl ), ); }
}

struct fd_logger_t* fd_logger_self()
{
    return &self;
}

int fd_logger_stats_start()
{
	/* Start the statistics thread */
	CHECK_POSIX( pthread_create(&fd_stats_th, NULL, fd_stats_worker, NULL) );

    return 0;
}

void fd_logger_register(fd_logger_user_handler instance)
{
    user_handler = instance;
}

void fd_logger_unregister(void)
{
    user_handler = NULL;
}

/* The callback called when messages are received and sent */
static void fd_logger_cb(enum fd_hook_type type, struct msg * msg, 
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, 
    void * regdata)
{
	char * peer_name = peer ? peer->info.pi_diamid : "<unknown peer>";
	
	CHECK_POSIX_DO( pthread_mutex_lock(&mtx), );

    if (user_handler)
    {
        user_handler(type, msg, peer, other, pmd, regdata);
    }

	switch (type) {
/* peers */
	case HOOK_PEER_CONNECT_SUCCESS:
		{
			char protobuf[40];
			if (peer) {
				CHECK_FCT_DO(fd_peer_cnx_proto_info(peer, protobuf, sizeof(protobuf)), break );
			} else {
				protobuf[0] = '-';
				protobuf[1] = '\0';
			}
			d_info("CONNECTED TO '%s' (%s):", peer_name, protobuf);
		}
		break;
    default:
        d_warn("Unknown type(%d)", type);
		break;
	}
	
	CHECK_POSIX_DO( pthread_mutex_unlock(&mtx), );
}

/* Function to display statistics periodically */
static void * fd_stats_worker(void * arg) 
{
	struct timespec start, now;
	struct fd_stats copy;
	
	/* Get the start time */
	CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &start), );
	
	/* Now, loop until canceled */
	while (1) {
		/* Display statistics every XX seconds */
		sleep(self.duration);
		
		/* Now, get the current stats */
		CHECK_POSIX_DO( pthread_mutex_lock(&self.stats_lock), );
		memcpy(&copy, &self.stats, sizeof(struct fd_stats));
		CHECK_POSIX_DO( pthread_mutex_unlock(&self.stats_lock), );
		
		/* Get the current execution time */
		CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );
		
		/* Now, display everything */
		d_trace(15, "------- fd statistics ---------\n");
		if (now.tv_nsec >= start.tv_nsec) 
        {
			d_trace(15, " Executing for: %d.%06ld sec\n",
					(int)(now.tv_sec - start.tv_sec),
					(long)(now.tv_nsec - start.tv_nsec) / 1000);
		} 
        else 
        {
			d_trace(15, " Executing for: %d.%06ld sec\n",
					(int)(now.tv_sec - 1 - start.tv_sec),
					(long)(now.tv_nsec + 1000000000 - start.tv_nsec) / 1000);
		}
		
        if (self.mode & FD_MODE_SERVER) {
            d_trace(15, " Server: %llu message(s) echoed\n", 
                    copy.nb_echoed);
        }
        if (self.mode & FD_MODE_CLIENT) {
            d_trace(15, " Client:\n");
            d_trace(15, "   %llu message(s) sent\n", copy.nb_sent);
            d_trace(15, "   %llu error(s) received\n", copy.nb_errs);
            d_trace(15, "   %llu answer(s) received\n", copy.nb_recv);
            d_trace(15, "     fastest: %ld.%06ld sec.\n", 
                    copy.shortest / 1000000, copy.shortest % 1000000);
            d_trace(15, "     slowest: %ld.%06ld sec.\n", 
                    copy.longest / 1000000, copy.longest % 1000000);
            d_trace(15, "     Average: %ld.%06ld sec.\n", 
                    copy.avg / 1000000, copy.avg % 1000000);
        }
		d_trace(15, "-------------------------------------\n");
	}
	
	return NULL; /* never called */
}

