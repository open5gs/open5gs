/**
 * @file cp_main.c
 */

/* Server */
#include "event.h"
#include "sm.h"

/* Core library */
#include "core.h"
#define TRACE_MODULE _smmain
#include "core_debug.h"
#include "core_thread.h"

#include "context.h"

static master_sm_t g_master_sm;

#define EVENT_WAIT_TIMEOUT 10000 /* 10 msec */

void *THREAD_FUNC sm_main(void *data)
{
    event_t e;
    c_time_t prev_tm, now_tm;
    int r;

    fsm_create(&g_master_sm.fsm, master_state_initial, master_state_final);
    d_assert(&g_master_sm.fsm, return NULL,
            "Master state machine creation failed");

    fsm_init((fsm_t*)&g_master_sm, 0);

    prev_tm = time_now();

    while (!thread_should_stop())
    {
        r = event_timedrecv(&e, EVENT_WAIT_TIMEOUT);

        d_assert(r != CORE_ERROR, continue,
                "While receiving a event message, error occurs");

        now_tm = time_now();

        /* if the gap is over 10 ms, execute preriodic jobs */
        if (now_tm - prev_tm > EVENT_WAIT_TIMEOUT)
        {
            event_timer_execute();

            prev_tm = now_tm;
        }

        if (r == CORE_TIMEUP)
        {
            continue;
        }

        fsm_dispatch((fsm_t*)&g_master_sm, (fsm_event_t*)&e);
    }

    fsm_final((fsm_t*)&g_master_sm, 0);
    fsm_clear((fsm_t*)&g_master_sm);

    return NULL;
}
