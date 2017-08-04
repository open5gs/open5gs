#define TRACE_MODULE _app_init

#include "core_debug.h"
#include "core_thread.h"

#include "context.h"
#include "logger.h"

#include "app.h"

static thread_id logger_thread = 0;
static void *THREAD_FUNC logger_main(thread_id id, void *data);

status_t app_will_initialize(char *config_path, char *log_path)
{
    status_t rv;
    int others = 0;

    context_init();

    rv = context_read_file(config_path);
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
    d_assert(ret == 0, return NULL, "Failed to intialize Logger");

    return NULL;
}
