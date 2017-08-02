#define TRACE_MODULE _app_init

#include "core_debug.h"
#include "core_thread.h"
#include "core_net.h"
#include "core_signal.h"
#include "core_proc.h"
#include "core_semaphore.h"

#include "context.h"
#include "logger.h"

#include "app.h"

static proc_id logger_proc;
static semaphore_id logger_sem;
static void *PROC_FUNC logger_main(proc_id id, void *data);
static void check_signal(int signum);

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

    if (log_path) 
        context_self()->log_path = log_path;

    if (context_self()->log_path)
    {
        d_assert(semaphore_create(&logger_sem, 0) == CORE_OK, 
                return CORE_ERROR, "semaphore_create() failed");
        rv = proc_create(&logger_proc, logger_main, context_self()->log_path);
        if (rv != CORE_OK) return rv;
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
    return CORE_OK;
}

void app_will_terminate(void)
{
}

void app_did_terminate(void)
{
    if (context_self()->log_path)
    {
        d_assert(semaphore_post(logger_sem) == CORE_OK,,
                "semaphore_post() failed");
        proc_delete(logger_proc);
    }

    context_final();
}

static void *PROC_FUNC logger_main(proc_id id, void *data)
{
    status_t rv;
    char *path = data;

    umask(027);

    signal_unblock(SIGINT);
    signal_unblock(SIGTERM);
    core_signal(SIGINT, check_signal);
    core_signal(SIGTERM, check_signal);

    d_print("  Logging '%s'\n", path);
    rv = logger_start(path);
    if (rv != CORE_OK) return NULL;

    d_assert(semaphore_wait(logger_sem) == CORE_OK, return NULL,
            "semaphore_wait() failed");
    d_assert(semaphore_delete(logger_sem) == CORE_OK, return NULL,
            "semaphore_delete() failed");

    d_trace(1, "LOGGER terminate...done\n");

    return NULL;
}

static void check_signal(int signum)
{
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
        {
            d_info("%s received", 
                    signum == SIGTERM ? "SIGTERM" : "SIGINT");

            logger_stop();
            break;
        }
        default:
        {
            d_error("Unknown signal number = %d\n", signum);
            break;
        }
    }
}
