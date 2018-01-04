#define TRACE_MODULE _mme_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "app/context.h"
#include "app/app.h"

const char *app_name = "mme";

status_t app_initialize(const char *config_path, const char *log_path)
{
    status_t rv;
    int others = 0;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    others = context_self()->logger.trace.others;
    if (others)
    {
        d_trace_level(&_mme_main, others);
    }

    d_trace(1, "MME try to initialize\n");
    rv = mme_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize MME");
    d_trace(1, "MME initialize...done\n");

    rv = app_did_initialize();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    d_trace(1, "MME try to terminate\n");
    mme_terminate();
    d_trace(1, "MME terminate...done\n");

    app_did_terminate();
}

