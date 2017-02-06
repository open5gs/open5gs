/**
 * @file init.c
 */

/* Core library */
#define TRACE_MODULE _init
#include "core_debug.h"
#include "context.h"

#include <syslog.h>

status_t cellwire_initialize(char *config_path)
{
    status_t rv;
    openlog("cellwire", 0, LOG_DAEMON);

    srand(time(NULL)*getpid());

    rv = context_init();
    if (rv != CORE_OK)
        return rv;

    return CORE_OK;
}

void cellwire_terminate(void)
{
    context_final();
}
