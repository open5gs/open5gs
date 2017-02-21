#define TRACE_MODULE _s6a_init

#include "core_debug.h"
#include "core_lib.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#include "s6a_message.h"

static void s6a_gnutls_log_func(int level, const char *str);
static void s6a_fd_logger(int printlevel, const char *format, va_list ap);

static status_t s6a_ext_init();
static status_t s6a_dict_init();

status_t s6a_initialize()
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

    ret = s6a_ext_init();
    if (ret != 0) 
    {
        d_error("s6a_ext_init() failed");
        return CORE_ERROR;
    } 

    ret = s6a_dict_init();
    if (ret != 0) 
    {
        d_error("s6a_conf_init() failed");
        return CORE_ERROR;
    } 

    return CORE_OK;
}

status_t s6a_ext_init()
{
    int ret;

    int fd_ext_init_dnr_entry(int major, int minor, char *conffile);
    int fd_ext_init_dict_dcca_entry(int major, int minor, char *conffile);
    int fd_ext_init_dict_dcca_3gpp_entry(int major, int minor, char *conffile);

    ret = fd_ext_init_dnr_entry(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_dnr_entry() failed");
        return CORE_ERROR;
    } 
    ret = fd_ext_init_dict_dcca_entry(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_dict_dcca_entry() failed");
        return CORE_ERROR;
    } 
    ret = fd_ext_init_dict_dcca_3gpp_entry(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_dict_dcca_entry() failed");
        return CORE_ERROR;
    } 

    return CORE_OK;
}

status_t s6a_dict_init()
{
    return CORE_OK;
}

status_t s6a_conf_handle(s6a_conf_t *conf)
{
    int ret;
    fd_g_config->cnf_diamid = conf->identity;

    ret = fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamid, 
            &fd_g_config->cnf_diamid_len, 1);
    if (ret != 0)
    {
        return CORE_ERROR;
    }

    fd_g_config->cnf_diamrlm = conf->realm;

    ret = fd_os_validate_DiameterIdentity(&fd_g_config->cnf_diamrlm, 
            &fd_g_config->cnf_diamrlm_len, 1);

    if (ret != 0)
    {
        return CORE_ERROR;
    }

    fd_g_config->cnf_flags.no_fwd = 1;
    fd_g_config->cnf_flags.no_ip6 = 1;
    fd_g_config->cnf_flags.no_sctp = 1;
    fd_g_config->cnf_flags.pr_tcp = 1;

    return CORE_OK;
}

status_t s6a_thread_start()
{
    int ret;

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

    return CORE_OK;
}

void s6a_thread_stop()
{
    int ret;

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

void s6a_conf_show()
{
    char *buf = NULL;
    size_t len;

    if (g_trace_mask && TRACE_MODULE >= 1)
    {
        printf("%s\n", fd_conf_dump(&buf, &len, NULL));
        free(buf);
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


