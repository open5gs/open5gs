#define TRACE_MODULE _app_init

#include "core_debug.h"
#include "core_thread.h"
#include "core_file.h"

#include "context.h"

#include "app.h"

#define DEFAULT_CONFIG_FILE_PATH SYSCONF_DIR PACKAGE "/nextepc.conf"
#define DEFAULT_RUNTIME_DIR_PATH LOCALSTATE_DIR "run/"

static status_t app_logger_init();
static status_t app_logger_final();
static status_t app_logger_start();
static status_t app_logger_stop();

status_t app_will_initialize(const char *config_path, const char *log_path)
{
    status_t rv;
    int others = 0;

    context_init();

    context_self()->config.path = config_path;
    if (context_self()->config.path == NULL)
        context_self()->config.path = DEFAULT_CONFIG_FILE_PATH;

    rv = context_read_file();
    if (rv != CORE_OK) return rv;

    rv = context_parse_config();
    if (rv != CORE_OK) return rv;

    others = context_self()->trace_level.others;
    if (others)
    {
        d_trace_level(&_app_init, others);
    }
    
    context_self()->log.path = log_path;
    rv = app_logger_init();
    if (rv != CORE_OK) return rv;

    if (context_self()->db_uri)
    {
        rv = context_db_init(context_self()->db_uri);
        if (rv != CORE_OK) return rv;
    }

    return rv;
}

status_t app_did_initialize(void)
{
    status_t rv = app_logger_start();
    if (rv != CORE_OK) return rv;

    return rv;
}

void app_will_terminate(void)
{
    app_logger_stop();
}

void app_did_terminate(void)
{
    if (context_self()->db_uri)
    {
        context_db_final();
    }

    app_logger_final();

    context_final();
}

status_t app_log_pid(const char *pid_path)
{
    file_t *pid_file = NULL;
    file_info_t finfo;
    static pid_t saved_pid = -1;
    pid_t mypid;
    status_t rv;
    char default_pid_path[MAX_FILEPATH_LEN];
    char buf[128];

    if (pid_path == NULL)
    {
        snprintf(default_pid_path, sizeof(default_pid_path),
                "%snextepc-%sd/pid", DEFAULT_RUNTIME_DIR_PATH, app_name);
        pid_path = default_pid_path;
    }

    mypid = getpid();
    if (mypid != saved_pid
        && file_stat(&finfo, pid_path, FILE_INFO_MTIME) == CORE_OK)
    {
        d_warn("pid file %s overwritten -- Unclean "
                "shutdown of previous NextEPC run?", pid_path);
    }

    if ((rv = file_open(&pid_file, pid_path,
            FILE_WRITE | FILE_CREATE | FILE_TRUNCATE,
            FILE_UREAD | FILE_UWRITE | FILE_GREAD | FILE_WREAD)) != CORE_OK)
    {
        d_error("could not create %s", pid_path);
        return CORE_ERROR;
    }
    snprintf(buf, sizeof(buf), "%" C_PID_T_FMT "\r\n", mypid);
    file_puts(buf, pid_file);
    file_close(pid_file);
    saved_pid = mypid;

    d_print("  PID[%" C_PID_T_FMT "] : '%s'\n", saved_pid, pid_path);

    return CORE_OK;
}

status_t app_logger_restart()
{
    app_logger_stop();
    app_logger_final();

    app_logger_init();
    app_logger_start();

    return CORE_OK;
}

static status_t app_logger_init()
{
    status_t rv;

    if (context_self()->log.console >= 0)
    {
        rv = d_msg_console_init(context_self()->log.console);
        if (rv != CORE_OK) 
        {
            d_error("console logger init failed : (file:%d)",
                context_self()->log.console);
            return rv;
        }
        d_print("  Console Logging '%d'\n", context_self()->log.console);
    }
    if (context_self()->log.syslog)
    {
        d_msg_syslog_init(context_self()->log.syslog);
        d_print("  Syslog Logging '%s'\n", context_self()->log.syslog);
    }
    if (context_self()->log.socket.file &&
        context_self()->log.socket.unix_domain)
    {
        if (context_self()->log.path)
            context_self()->log.socket.file = context_self()->log.path;

        rv = d_msg_socket_init(context_self()->log.socket.unix_domain);
        if (rv != CORE_OK) 
        {
            d_error("socket logger init failed : (unix_domain:%s, file:%s)",
                context_self()->log.socket.unix_domain,
                context_self()->log.socket.file);
            return rv;
        }
        d_print("  Socket Logging '%s' on %s\n",
                context_self()->log.socket.file,
                context_self()->log.socket.unix_domain);
    }
    if (context_self()->log.file)
    {
        if (context_self()->log.path)
            context_self()->log.file = context_self()->log.path;

        rv = d_msg_file_init(context_self()->log.file);
        if (rv != CORE_OK) 
        {
            d_error("file logger init failed : (file:%s)",
                context_self()->log.file);
            return rv;
        }
        d_print("  File Logging '%s'\n", context_self()->log.file);
    }

    return CORE_OK;
}


static status_t app_logger_start()
{
    status_t rv;

    if (context_self()->log.socket.file &&
        context_self()->log.socket.unix_domain)
    {
        rv = d_msg_socket_start(context_self()->log.socket.file);
        if (rv != CORE_OK) 
        {
            d_error("socket logger start failed : (unix_domain:%s, file:%s)",
                context_self()->log.socket.unix_domain,
                context_self()->log.socket.file);
            return rv;
        }
    }

    return CORE_OK;
}

static status_t app_logger_stop()
{
    if (context_self()->log.socket.file &&
        context_self()->log.socket.unix_domain)
    {
        d_msg_socket_stop();
    }
    
    return CORE_OK;
}

static status_t app_logger_final()
{
    if (context_self()->log.console >= 0)
    {
        d_msg_console_final();
    }
    if (context_self()->log.syslog)
    {
        d_msg_syslog_final();
    }
    if (context_self()->log.socket.file &&
        context_self()->log.socket.unix_domain)
    {
        d_msg_socket_final();
    }
    if (context_self()->log.file)
    {
        d_msg_file_final();
    }
    
    return CORE_OK;
}

