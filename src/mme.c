#define TRACE_MODULE _mme_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "context.h"
#include "app.h"

status_t app_initialize(char *config_path, char *log_path)
{
    status_t rv;
    int others = 0;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    others = context_self()->trace_level.others;
    if (others)
    {
        d_trace_level(&_mme_main, others);
    }

    d_trace(1, "MME try to initialize\n");
    rv = mme_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize MME");
    d_trace(1, "MME initialize...done\n");

    rv = app_did_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void app_terminate(void)
{
    printf("1\n");
    app_will_terminate();
    printf("2\n");

    d_trace(1, "MME try to terminate\n");
    mme_terminate();
    d_trace(1, "MME terminate...done\n");

    app_did_terminate();
}
