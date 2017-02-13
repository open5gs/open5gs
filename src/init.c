/**
 * @file init.c
 */

/* Core library */
#define TRACE_MODULE _init
#include "core_debug.h"
#include "core_thread.h"

#include "context.h"
#include "event.h"

static thread_id mme_thread;
extern void *THREAD_FUNC mme_main(void *data);

void threads_start()
{
    status_t rv;

    rv = thread_create(&mme_thread, NULL, mme_main, NULL);
    d_assert(rv == CORE_OK, return,
            "MME State machine thread creation failed");
}

void threads_stop()
{
    thread_delete(mme_thread);
}

status_t cellwire_initialize(char *config_path)
{
    status_t rv;

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
