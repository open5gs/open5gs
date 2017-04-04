#define TRACE_MODULE _mme_init

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "s6a_sm.h"

static thread_id mme_sm_thread;
void *THREAD_FUNC mme_sm_main(thread_id id, void *data);

status_t mme_initialize()
{
    status_t rv;
    int ret;

    rv = mme_ctx_init();
    if (rv != CORE_OK) return rv;

    ret = s6a_sm_init();
    if (ret != 0) return -1;

    rv = thread_create(&mme_sm_thread, NULL, mme_sm_main, NULL);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void mme_terminate(void)
{
    thread_delete(mme_sm_thread);

    s6a_sm_final();

    mme_ctx_final();
}

void *THREAD_FUNC mme_sm_main(thread_id id, void *data)
{
    event_t event;
    mme_sm_t mme_sm;
    c_time_t prev_tm, now_tm;
    status_t rv;

    memset(&event, 0, sizeof(event_t));

    mme_self()->queue_id = event_create();
    d_assert(mme_self()->queue_id, return NULL, 
            "MME event queue creation failed");
    tm_service_init(&mme_self()->tm_service);
    gtp_xact_init(&mme_self()->gtp_xact_ctx, 
            &mme_self()->tm_service, EVT_TM_MME_S11_T3);

    fsm_create(&mme_sm.fsm, mme_state_initial, mme_state_final);
    d_assert(&mme_sm.fsm, return NULL, "MME state machine creation failed");
    fsm_init((fsm_t*)&mme_sm, 0);

    prev_tm = time_now();

    while ((!thread_should_stop()))
    {
        rv = event_timedrecv(mme_self()->queue_id, &event, EVENT_WAIT_TIMEOUT);

        d_assert(rv != CORE_ERROR, continue,
                "While receiving a event message, error occurs");

        now_tm = time_now();

        /* if the gap is over 10 ms, execute preriodic jobs */
        if (now_tm - prev_tm > EVENT_WAIT_TIMEOUT)
        {
            tm_execute_tm_service(
                    &mme_self()->tm_service, mme_self()->queue_id);

            prev_tm = now_tm;
        }

        if (rv == CORE_TIMEUP)
        {
            continue;
        }

        fsm_dispatch((fsm_t*)&mme_sm, (fsm_event_t*)&event);
    }

    fsm_final((fsm_t*)&mme_sm, 0);
    fsm_clear((fsm_t*)&mme_sm);

    gtp_xact_final();
    event_delete(mme_self()->queue_id);

    return NULL;
}

void *THREAD_FUNC mme_net_main(thread_id id, void *data)
{
    while (!thread_should_stop())
    {
        net_fds_read_run(50); 
    }

    return NULL;
}
