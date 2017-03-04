/**
 * @file init.c
 */

/* Core library */
#define TRACE_MODULE _cellwire_init
#include "core_general.h"
#include "core_thread.h"
#include "core_net.h"

#include "logger.h"

#include "cellwire.h"

static thread_id net_thread;

void *THREAD_FUNC net_main(void *data);

status_t cellwire_initialize(char *config_path, char *log_path)
{
    status_t rv;

    core_initialize();

    if (config_path)
    {
        /* TODO */
    }

    if (log_path)
        logger_start(log_path);

    rv = mme_initialize();
    if (rv != CORE_OK) return rv;

    rv = thread_create(&net_thread, NULL, net_main, NULL);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void cellwire_terminate(void)
{
    thread_delete(net_thread);

    mme_terminate();
    core_terminate();
}

void *THREAD_FUNC net_main(void *data)
{
    while (!thread_should_stop())
    {
        net_fds_read_run(50); 
    }

    return NULL;
}
