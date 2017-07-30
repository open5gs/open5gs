#define TRACE_MODULE _app

#include "core_general.h"
#include "core_debug.h"
#include "core_thread.h"
#include "core_net.h"
#include "core_signal.h"

#include "context.h"
#include "logger.h"

#include "app.h"

static pid_t logger_pid = 0;
static thread_id net_thread;

void *THREAD_FUNC net_main(thread_id id, void *data);

status_t app_will_initialize(char *config_path, char *log_path)
{
    status_t rv;

    core_initialize();
    context_init();

    rv = context_read_file(config_path);
    if (rv != CORE_OK) return rv;

    rv = context_parse_config();
    if (rv != CORE_OK) return rv;

    if (log_path) 
        context_self()->log_path = log_path;

    if (context_self()->log_path)
    {
        logger_pid = logger_start(context_self()->log_path);
        d_assert(logger_pid > 0, return -1, "logger_start() failed");
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

    rv = thread_create(&net_thread, NULL, net_main, NULL);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void app_will_terminate(void)
{
    if (logger_pid)
        core_kill(logger_pid, SIGTERM);

    thread_delete(net_thread);
}

void app_did_terminate(void)
{
    context_final();
    core_terminate();
}

void *THREAD_FUNC net_main(thread_id id, void *data)
{
    while (!thread_should_stop())
    {
        net_fds_read_run(50); 
    }

    return NULL;
}
