#define TRACE_MODULE _app_init

#include "core_debug.h"
#include "core_thread.h"
#include "core_file.h"

#include "context.h"
#include "logger.h"

#include "app.h"

#define DEFAULT_PID_DIR_PATH LOCALSTATE_DIR "run/" PACKAGE
#define DEFAULT_CONFIG_FILE_PATH SYSCONF_DIR PACKAGE "/nextepc.conf"

static thread_id logger_thread = 0;
static void *THREAD_FUNC logger_main(thread_id id, void *data);

status_t app_will_initialize(char *config_path, char *log_path)
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

    if (context_self()->db_uri)
    {
        rv = context_db_init(context_self()->db_uri);
        if (rv != CORE_OK) return rv;
    }

    return CORE_OK;
}

status_t app_did_initialize(char *config_path, char *log_path)
{
    status_t rv;

    if (log_path) 
        context_self()->log_path = log_path;

    if (context_self()->log_path)
    {
        d_print("  Logging '%s'\n", context_self()->log_path);
        rv = thread_create(&logger_thread, NULL, 
                logger_main, context_self()->log_path);
        if (rv != CORE_OK) return rv;
    }

    return CORE_OK;
}

void app_will_terminate(void)
{
    if (logger_thread)
    {
        thread_delete(logger_thread);
    }

}

void app_did_terminate(void)
{
    if (context_self()->db_uri)
    {
        context_db_final();
    }

    context_final();
}

static void *THREAD_FUNC logger_main(thread_id id, void *data)
{
    int ret;
    char *path = data;

    ret = logger_start(path);
    if (ret != 0)
    {
        d_error("Failed to initialize logger.");
        d_error("Check file permission for `%s`", path); 
    }

    return NULL;
}

status_t logger_restart()
{
    status_t rv;

    if (logger_thread)
    {
        thread_delete(logger_thread);
        
        if (context_self()->log_path)
        {
            d_print("  Logging '%s'\n", context_self()->log_path);
            rv = thread_create(&logger_thread, NULL, 
                    logger_main, context_self()->log_path);
            if (rv != CORE_OK) return rv;
        }
    }

    return CORE_OK;
}

status_t app_log_pid(const char *name)
{
    file_t *pid_file = NULL;
    file_info_t finfo;
    static pid_t saved_pid = -1;
    pid_t mypid;
    status_t rv;
    char fname[MAX_FILEPATH_LEN];
    char buf[128];

    d_assert(name, return CORE_ERROR, );

    snprintf(fname, sizeof(fname), "%s/%sd.pid", DEFAULT_PID_DIR_PATH, name);
    mypid = getpid();
    if (mypid != saved_pid
        && file_stat(&finfo, fname, FILE_INFO_MTIME) == CORE_OK)
    {
#if 0 /* FIXME : we need to check pid file overwritten */
        d_warn("pid file %s overwritten -- Unclean "
                "shutdown of previous NextEPC run?", fname);
#endif
    }

    if ((rv = file_open(&pid_file, fname,
            FILE_WRITE | FILE_CREATE | FILE_TRUNCATE,
            FILE_UREAD | FILE_UWRITE | FILE_GREAD | FILE_WREAD)) != CORE_OK)
    {
        d_error("could not create %s", fname);
        return CORE_ERROR;
    }
    snprintf(buf, sizeof(buf), "%" C_PID_T_FMT "\r\n", mypid);
    file_puts(buf, pid_file);
    file_close(pid_file);
    saved_pid = mypid;

    return CORE_OK;
}
