/**
 * @file init.c
 */

/* Core library */
#include "core.h"
#define TRACE_MODULE _init
#include "core_debug.h"

#include <syslog.h>

status_t cellwire_initialize(char *config_path)
{
    openlog("cellwire", 0, LOG_DAEMON);

    srand(time(NULL)*getpid());

    return CORE_OK;
}

void cellwire_terminate(void)
{
}
