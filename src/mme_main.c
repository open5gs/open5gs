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
#include "core_msgq.h"

#include "context.h"

#define EVENT_WAIT_TIMEOUT 10000 /* 10 msec */

void *THREAD_FUNC mme_main(void *data)
{
    event_t event;
    msgq_id queue_id;
    mme_sm_t mme_sm;
    c_time_t prev_tm, now_tm;
    int r;

    memset(&event, 0, sizeof(event_t));

    queue_id = event_create();
    d_assert(queue_id, return NULL, "MME event queue creation failed");

    fsm_create(&mme_sm.fsm, mme_state_initial, mme_state_final);
    d_assert(&mme_sm.fsm, return NULL, "MME state machine creation failed");
    mme_sm.queue_id = queue_id;
    tm_service_init(&mme_sm.tm_service);

    fsm_init((fsm_t*)&mme_sm, 0);

    prev_tm = time_now();

    while (!thread_should_stop())
    {
        r = event_timedrecv(queue_id, &event, EVENT_WAIT_TIMEOUT);

        d_assert(r != CORE_ERROR, continue,
                "While receiving a event message, error occurs");

        now_tm = time_now();

        /* if the gap is over 10 ms, execute preriodic jobs */
        if (now_tm - prev_tm > EVENT_WAIT_TIMEOUT)
        {
            event_timer_execute(&mme_sm.tm_service);

            prev_tm = now_tm;
        }

        if (r == CORE_TIMEUP)
        {
            continue;
        }

        fsm_dispatch((fsm_t*)&mme_sm, (fsm_event_t*)&event);
    }

    fsm_final((fsm_t*)&mme_sm, 0);
    fsm_clear((fsm_t*)&mme_sm);

    event_delete(queue_id);

    return NULL;
}
