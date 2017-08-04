#define TRACE_MODULE _sgw_main

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
        d_trace_level(&_sgw_main, others);
    }

    d_trace(1, "SGW try to initialize\n");
    rv = sgw_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize SGW");
    d_trace(1, "SGW initialize...done\n");

    rv = app_did_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    d_trace(1, "SGW try to terminate\n");
    sgw_terminate();
    d_trace(1, "SGW terminate...done\n");

    app_did_terminate();
}
