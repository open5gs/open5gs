#define TRACE_MODULE _pgw_init

#include "core_debug.h"
#include "core_thread.h"

#include "pgw_context.h"
#include "pgw_event.h"

#include "pgw_gx_handler.h"

static thread_id sm_thread;
static void *THREAD_FUNC sm_main(thread_id id, void *data);

static thread_id net_thread;
static void *THREAD_FUNC net_main(thread_id id, void *data);

static int initialized = 0;

status_t pgw_initialize()
{
    status_t rv;
    int ret;

    rv = pgw_context_init();
    if (rv != CORE_OK) return rv;

    rv = pgw_context_parse_config();
    if (rv != CORE_OK) return rv;

    rv = pgw_context_setup_trace_module();
    if (rv != CORE_OK) return rv;

    rv = pgw_ip_pool_generate();
    if (rv != CORE_OK) return rv;

    ret = pgw_gx_init();
    if (ret != 0) return CORE_ERROR;

    rv = thread_create(&sm_thread, NULL, sm_main, NULL);
    if (rv != CORE_OK) return rv;
    rv = thread_create(&net_thread, NULL, net_main, NULL);
    if (rv != CORE_OK) return rv;

    initialized = 1;

    return CORE_OK;
}

void pgw_terminate(void)
{
    if (!initialized) return;

    thread_delete(net_thread);
    thread_delete(sm_thread);

    pgw_gx_final();

    pgw_context_final();

    gtp_xact_final();
}

static void *THREAD_FUNC sm_main(thread_id id, void *data)
{
    event_t event;
    fsm_t pgw_sm;
    c_time_t prev_tm, now_tm;
    int r;

    memset(&event, 0, sizeof(event_t));

    pgw_self()->queue_id = event_create();
    d_assert(pgw_self()->queue_id, return NULL, 
            "PGW event queue creation failed");
    tm_service_init(&pgw_self()->tm_service);
    gtp_xact_init(&pgw_self()->gtp_xact_ctx, 
            &pgw_self()->tm_service, PGW_EVT_TRANSACTION_T3);

    fsm_create(&pgw_sm, pgw_state_initial, pgw_state_final);
    fsm_init(&pgw_sm, 0);

    prev_tm = time_now();

    while ((!thread_should_stop()))
    {
        r = event_timedrecv(pgw_self()->queue_id, &event, EVENT_WAIT_TIMEOUT);

        d_assert(r != CORE_ERROR, continue,
                "While receiving a event message, error occurs");

        now_tm = time_now();

        /* if the gap is over 10 ms, execute preriodic jobs */
        if (now_tm - prev_tm > EVENT_WAIT_TIMEOUT)
        {
            tm_execute_tm_service(
                    &pgw_self()->tm_service, pgw_self()->queue_id);

            prev_tm = now_tm;
        }

        if (r == CORE_TIMEUP)
        {
            continue;
        }

        fsm_dispatch(&pgw_sm, (fsm_event_t*)&event);
    }

    fsm_final(&pgw_sm, 0);
    fsm_clear(&pgw_sm);

    event_delete(pgw_self()->queue_id);

    return NULL;
}

static void *THREAD_FUNC net_main(thread_id id, void *data)
{
    while (!thread_should_stop())
    {
        net_fds_read_run(50); 
    }

    return NULL;
}
