#define TRACE_MODULE _pcrf_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "context.h"
#include "app.h"

const char *app_name = "pcrf";

status_t app_initialize(const char *config_path, const char *log_path)
{
    status_t rv;
    int others = 0;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    others = context_self()->trace_level.others;
    if (others)
    {
        d_trace_level(&_pcrf_main, others);
    }

    d_trace(1, "PCRF try to initialize\n");
    rv = pcrf_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize PCRF");
    d_trace(1, "PCRF initialize...done\n");

    rv = app_did_initialize();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    d_trace(1, "PCRF try to terminate\n");
    pcrf_terminate();
    d_trace(1, "PCRF terminate...done\n");

    app_did_terminate();
}
