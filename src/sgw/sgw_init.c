#define TRACE_MODULE _sgw_init

#include "core_debug.h"
#include "core_thread.h"

#include "gtp/gtp_xact.h"

#include "sgw_context.h"
#include "sgw_sm.h"
#include "sgw_event.h"

static thread_id sgw_thread;
static void *THREAD_FUNC sgw_main(thread_id id, void *data);

static int initialized = 0;

status_t sgw_initialize()
{
    status_t rv;

    rv = sgw_context_init();
    if (rv != CORE_OK) return rv;

    rv = sgw_context_parse_config();
    if (rv != CORE_OK) return rv;

    rv = sgw_context_setup_trace_module();
    if (rv != CORE_OK) return rv;

    rv = thread_create(&sgw_thread, NULL, sgw_main, NULL);
    if (rv != CORE_OK) return rv;

    initialized = 1;

    return CORE_OK;
}

void sgw_terminate(void)
{
    if (!initialized) return;

    thread_delete(sgw_thread);

    sgw_context_final();

    gtp_xact_final();
}

static void *THREAD_FUNC sgw_main(thread_id id, void *data)
{
    event_t event;
    fsm_t sgw_sm;
    c_time_t prev_tm, now_tm;
    status_t rv;

    memset(&event, 0, sizeof(event_t));

    sgw_self()->queue_id = event_create(MSGQ_O_NONBLOCK);
    d_assert(sgw_self()->queue_id, return NULL, 
            "SGW event queue creation failed");
    tm_service_init(&sgw_self()->tm_service);
    gtp_xact_init(&sgw_self()->tm_service,
            SGW_EVT_T3_RESPONSE, SGW_EVT_T3_HOLDING);

    fsm_create(&sgw_sm, sgw_state_initial, sgw_state_final);
    fsm_init(&sgw_sm, 0);

    prev_tm = time_now();

#define EVENT_LOOP_TIMEOUT 10   /* 10ms */
    while ((!thread_should_stop()))
    {
        sock_select_loop(EVENT_LOOP_TIMEOUT); 
        do
        {
            rv = event_recv(sgw_self()->queue_id, &event);

            d_assert(rv != CORE_ERROR, continue,
                    "While receiving a event message, error occurs");

            now_tm = time_now();

            /* if the gap is over event_loop timeout, execute preriodic jobs */
            if (now_tm - prev_tm > (EVENT_LOOP_TIMEOUT * 1000))
            {
                tm_execute_tm_service(
                        &sgw_self()->tm_service, sgw_self()->queue_id);

                prev_tm = now_tm;
            }

            if (rv == CORE_EAGAIN)
            {
                continue;
            }

            fsm_dispatch(&sgw_sm, (fsm_event_t*)&event);
        } while(rv == CORE_OK);
    }

    fsm_final(&sgw_sm, 0);
    fsm_clear(&sgw_sm);

    event_delete(sgw_self()->queue_id);

    return NULL;
}
