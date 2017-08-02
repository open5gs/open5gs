#define TRACE_MODULE _mme_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "app.h"

status_t app_initialize(char *config_path, char *log_path)
{
    status_t rv;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    rv = mme_initialize();
    if (rv != CORE_OK) return rv;

    rv = app_did_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    mme_terminate();

    app_did_terminate();
}
