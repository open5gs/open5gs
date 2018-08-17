#define TRACE_MODULE _fd_init

#include "core_debug.h"
#include "core_param.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "fd_logger.h"
#include "fd_lib.h"

static void fd_gnutls_log_func(int level, const char *str);
static void fd_log_func(int printlevel, const char *format, va_list ap);

int fd_init(int mode, const char *conffile, fd_config_t *fd_config)
{
    int ret;

    fd_g_debug_lvl = FD_LOG_ERROR;
    if (g_trace_mask)
    {
        if (TRACE_MODULE >= 25)
            gnutls_global_set_log_level(TRACE_MODULE-24);

        if (TRACE_MODULE >= 25 && TRACE_MODULE < 27)
            fd_g_debug_lvl = FD_LOG_NOTICE;
        else if (TRACE_MODULE >= 27 && TRACE_MODULE < 29)
            fd_g_debug_lvl = FD_LOG_DEBUG;
        else if (TRACE_MODULE >= 29)
            fd_g_debug_lvl = FD_LOG_ANNOYING;
    }

    gnutls_global_set_log_function(fd_gnutls_log_func);
    ret = fd_log_handler_register(fd_log_func);
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
        CHECK_FCT_DO( fd_config_init(fd_config), goto error );
    }

    /* Initialize FD Message */
    CHECK_FCT( fd_message_init() );

    /* Initialize FD logger */
    CHECK_FCT_DO( fd_logger_init(mode), goto error );

	/* Start the servers */
	CHECK_FCT_DO( fd_core_start(), goto error );

	CHECK_FCT_DO( fd_core_waitstartcomplete(), goto error );

    CHECK_FCT( fd_logger_stats_start() );

    return 0;
error:
	CHECK_FCT_DO( fd_core_shutdown(),  );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(),  );

	return -1;
}

void fd_final()
{
    fd_logger_final();

	CHECK_FCT_DO( fd_core_shutdown(), d_error("fd_core_shutdown() failed") );
	CHECK_FCT_DO( fd_core_wait_shutdown_complete(), 
            d_error("fd_core_wait_shutdown_complete() failed"));
}

static void fd_gnutls_log_func(int level, const char *str)
{
    d_trace(25-level, "gnutls[%d]: %s", level, str);
}

static void fd_log_func(int printlevel, const char *format, va_list ap)
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
            d_trace(29, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;  
	    case FD_LOG_DEBUG:
            d_trace(27, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;  
	    case FD_LOG_NOTICE:
            d_trace(25, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;
	    case FD_LOG_ERROR:
            d_error("%s", buffer);
            if (!strcmp(buffer, " - The certificate is expired."))
            {
                d_error("You can renew CERT as follows:");
                d_error("./support/freeDiameter/make_certs.sh "
                        "./install/etc/nextepc/freeDiameter");
            }
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
