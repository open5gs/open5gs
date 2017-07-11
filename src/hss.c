#define TRACE_MODULE _hss_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "init.h"

status_t epc_initialize(char *config_path, char *log_path)
{
    status_t rv;

    will_initialize(config_path, log_path);

    rv = hss_initialize();
    if (rv != CORE_OK) return rv;

    did_initialize(config_path, log_path);

    return CORE_OK;
}

void epc_terminate(void)
{
    will_terminate();

    hss_terminate();

    did_terminate();
}
