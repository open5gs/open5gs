#define TRACE_MODULE _s6a_fd

#include "core_debug.h"
#include "core_lib.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "s6a_app.h"

static void s6a_gnutls_log_func(int level, const char *str);
static void s6a_fd_logger(int printlevel, const char *format, va_list ap);

int s6a_fd_init(const char *conffile)
{
    int ret;
    
    d_trace_level(&_s6a_fd, 100);

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
        CHECK_FCT_DO( s6a_config_apply(), goto error );
    }

	/* Start the servers */
	CHECK_FCT_DO( fd_core_start(), goto error );

	CHECK_FCT_DO( fd_core_waitstartcomplete(), goto error );

    return 0;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return -1;
}

void s6a_fd_final()
{
	CHECK_FCT_DO( fd_core_shutdown(), d_error("fd_core_shutdown() failed") );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(), 
            d_error("fd_core_wait_shutdown_complete() failed"));
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
