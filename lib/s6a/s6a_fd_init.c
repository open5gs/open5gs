#define TRACE_MODULE _s6a_init

#include "core_debug.h"
#include "core_lib.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "s6a_fd.h"

static pid_t s6a_fd_hss_pid;

static void s6a_gnutls_log_func(int level, const char *str);
static void s6a_fd_logger(int printlevel, const char *format, va_list ap);

static int check_signal(int signum);

static int s6a_fd_init_internal(const char *conffile)
{
    int ret;
    
    d_trace_level(&_s6a_init, 0);

    gnutls_global_set_log_function(s6a_gnutls_log_func);
    gnutls_global_set_log_level(TRACE_MODULE);

    ret = fd_log_handler_register(s6a_fd_logger);
    if (ret != 0) 
    {
        d_error("fd_log_handler_register() failed");
        return ret;
    } 

    ret = fd_core_initialize();
    if (ret != 0) 
    {
        d_error("fd_core_initialize() failed");
        return ret;
    } 
    
	/* Parse the configuration file */
    if (conffile)
    {
        CHECK_FCT_DO( fd_core_parseconf(conffile), goto error );
    }
    else
    {
        CHECK_FCT_DO( s6a_fd_config_apply(), goto error );
    }
	
	/* Start the servers */
	CHECK_FCT_DO( fd_core_start(), goto error );

	CHECK_FCT_DO( fd_core_waitstartcomplete(), goto error );

	CHECK_FCT_DO( s6a_app_init(), goto error );

    return 0;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return -1;
}

int s6a_fd_init()
{
    status_t rv;
    int ret;
    semaphore_id semaphore;

    rv = semaphore_create(&semaphore, 0);
    d_assert(rv == CORE_OK, return -1, "semaphore_create() failed");

    s6a_fd_hss_pid = fork();
    d_assert(s6a_fd_hss_pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (s6a_fd_hss_pid == 0)
    {
        /* Child */
        rv = semaphore_wait(semaphore);
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "semaphore_wait() failed");

        rv = semaphore_delete(semaphore);
        d_assert(rv == CORE_OK, , "semaphore_delete() failed");

        rv = s6a_fd_init_internal(s6a_fd_hss_config());
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "s6a_fd_init() failed");
        signal_thread(check_signal);
        s6a_fd_final();

        _exit(EXIT_SUCCESS);
    }

    /* Parent */
    ret = s6a_fd_init_internal(s6a_fd_mme_config());
    if (ret != 0)
    {
        d_error("s6a_fd_init_internal() failed");
        return ret;
    }

    rv = semaphore_post(semaphore);
    d_assert(rv == CORE_OK, return -1, "semaphore_post() failed");

    return 0;
}

void s6a_fd_final()
{
    s6a_app_final();

	CHECK_FCT_DO( fd_core_shutdown(), d_error("fd_core_shutdown() failed") );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(), 
            d_error("fd_core_wait_shutdown_complete() failed"));

    core_kill(s6a_fd_hss_pid, SIGTERM);
}

static void s6a_gnutls_log_func(int level, const char *str)
{
    d_trace(level, "gnutls[%d]: %s", level, str);
}

static void s6a_fd_logger(int printlevel, const char *format, va_list ap)
{
    char buffer[HUGE_STRING_LEN];
    int  ret = 0;

    ret = vsnprintf(buffer, HUGE_STRING_LEN, format, ap);
    if (ret < 0 || ret > HUGE_STRING_LEN)
    {
        d_error("vsnprintf() failed");
        return;
    }

    switch(printlevel) 
    {
	    case FD_LOG_ANNOYING: 
            d_trace(10, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;  
	    case FD_LOG_DEBUG:
            d_trace(3, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;  
	    case FD_LOG_NOTICE:
            d_trace(1, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;
	    case FD_LOG_ERROR:
            d_error("%s", buffer);
            break;
	    case FD_LOG_FATAL:
            {
                char *except = "Initiating freeDiameter shutdown sequence";
                if (strncmp(buffer, except, strlen(except)) == 0)
                    d_trace(1, "freeDiameter[%d]: %s\n", printlevel, buffer);
                else
                    d_fatal("%s", buffer);
            }
            break;
	    default:
            d_warn("%s", buffer);
            break;
    }
}

static int check_signal(int signum)
{
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
        {
            d_info("%s received", 
                    signum == SIGTERM ? "SIGTERM" : "SIGINT");

            return 1;
        }
        case SIGUSR1:
        {
            void s6a_cli_test_message();
            s6a_cli_test_message();
            break;
        }
        default:
        {
            d_error("Unknown signal number = %d\n", signum);
            break;
        }
            
    }
    return 0;
}
