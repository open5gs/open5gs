#define TRACE_MODULE _s6a_init

#include "core_debug.h"
#include "core_lib.h"

#include "s6a_app.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

static void s6a_gnutls_log_func(int level, const char *str);
static void s6a_fd_logger(int printlevel, const char *format, va_list ap);

status_t s6a_thread_start(int hss)
{
    int ret;
    
    d_trace_level(&_s6a_init, 100);

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
    
    ret = s6a_config_init(hss);
    if (ret != 0) 
    {
        d_error("s6a_config_init() failed");
        return CORE_ERROR;
    } 
	
    ret = fd_core_start();
    if (ret != 0) 
    {
        d_error("fd_core_start() failed");
        return CORE_ERROR;
    } 

    ret = fd_core_waitstartcomplete();
    if (ret != 0) 
    {
        d_error("fd_core_waitstartcomplete() failed");
        return CORE_ERROR;
    } 

    ret = s6a_app_init();
    if (ret != 0) 
    {
        d_error("s6a_app_init() failed");
        return CORE_ERROR;
    } 

    return CORE_OK;
}

void s6a_thread_stop()
{
    int ret;
    
    s6a_app_final();

    ret = fd_core_shutdown();
    if (ret != 0) 
    {
        d_error("fd_core_shutdown() failed");
    }

    ret = fd_core_wait_shutdown_complete();
    if (ret != 0) 
    {
        d_error("fd_core_wait_shutdown_complete() failed");
    }
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
            d_trace(1, "freeDiameter[%d]: %s\n", printlevel, buffer);
            break;  
	    case FD_LOG_NOTICE:
            d_info("%s", buffer);
            break;
	    case FD_LOG_ERROR:
            d_error("%s", buffer);
            break;
	    case FD_LOG_FATAL:
            {
                char *except = "Initiating freeDiameter shutdown sequence";
                if (strncmp(buffer, except, strlen(except)) == 0)
                    d_info("%s", buffer);
                else
                    d_fatal("%s", buffer);
            }
            break;
	    default:
            d_warn("%s", buffer);
            break;
    }
}
