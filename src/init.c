/**
 * @file init.c
 */

/* Core library */
#define TRACE_MODULE _init
#include "core_debug.h"
#include "core_thread.h"

#include "context.h"
#include "event.h"

static thread_id thr_sm;
#define THREAD_SM_STACK_SIZE
#define THREAD_SM_PRIORITY
extern void *THREAD_FUNC sm_main(void *data);

void threads_start()
{
    status_t rv;

    rv = thread_create(&thr_sm, NULL, sm_main, NULL);
    d_assert(rv == CORE_OK, return,
            "State machine thread creation failed");
}

void threads_stop()
{
    thread_delete(thr_sm);
}

status_t cellwire_initialize(char *config_path)
{
    status_t rv;

    srand(time(NULL)*getpid());

    rv = event_init();
    if (rv != CORE_OK)
        return rv;

    rv = context_init();
    if (rv != CORE_OK)
        return rv;

    return CORE_OK;
}

void cellwire_terminate(void)
{
    context_final();

    event_final();
}
