#define TRACE_MODULE _s6a_init

#include "core_debug.h"
#include "core_lib.h"
#include "core_signal.h"

#include "s6a_app.h"
#include "s6a_lib.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

static pid_t s6a_fd_hss_pid;

static void s6a_gnutls_log_func(int level, const char *str);
static void s6a_fd_logger(int printlevel, const char *format, va_list ap);

static int check_signal(int signum);

static status_t s6a_fd_init_internal(const char *conffile)
{
    int ret;
    
    d_trace_level(&_s6a_init, 0);

    gnutls_global_set_log_function(s6a_gnutls_log_func);
    gnutls_global_set_log_level(TRACE_MODULE);

    ret = fd_log_handler_register(s6a_fd_logger);
    if (ret != 0) 
    {
        d_error("fd_log_handler_register() failed");
        return CORE_ERROR;
    } 

    ret = fd_core_initialize();
    if (ret != 0) 
    {
        d_error("fd_core_initialize() failed");
        return CORE_ERROR;
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

    ret = s6a_app_init();
    if (ret != 0) 
    {
        d_error("s6a_app_init() failed");
        return CORE_ERROR;
    } 

    return CORE_OK;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return CORE_ERROR;
}

status_t s6a_fd_init()
{
    status_t rv;

    s6a_fd_hss_pid = fork();

    d_assert(s6a_fd_hss_pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (s6a_fd_hss_pid == 0)
    {
        /* Child */
        rv = s6a_fd_init_internal(s6a_fd_hss_config());
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "s6a_fd_init() failed");
        signal_thread(check_signal);
        s6a_fd_final();

        _exit(EXIT_SUCCESS);
    }

    /* Parent */
    rv = s6a_fd_init_internal(s6a_fd_mme_config());
    if (rv != CORE_OK) return rv;
    return CORE_OK;
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
        default:
        {
            d_error("Unknown signal number = %d\n", signum);
            break;
        }
            
    }
    return 0;
}
