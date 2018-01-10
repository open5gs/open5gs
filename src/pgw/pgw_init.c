#define TRACE_MODULE _pgw_init

#include "core_debug.h"
#include "core_thread.h"

#include "gtp/gtp_xact.h"

#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_sm.h"

#include "pgw_fd_path.h"

static thread_id pgw_thread;
static void *THREAD_FUNC pgw_main(thread_id id, void *data);

static int initialized = 0;

status_t pgw_initialize()
{
    status_t rv;

    rv = pgw_context_init();
    if (rv != CORE_OK) return rv;

    rv = pgw_context_parse_config();
    if (rv != CORE_OK) return rv;

    rv = pgw_context_setup_trace_module();
    if (rv != CORE_OK) return rv;

    rv = pgw_ue_pool_generate();
    if (rv != CORE_OK) return rv;

    rv = pgw_fd_init();
    if (rv != 0) return CORE_ERROR;

    rv = thread_create(&pgw_thread, NULL, pgw_main, NULL);
    if (rv != CORE_OK) return rv;

    initialized = 1;

    return CORE_OK;
}

void pgw_terminate(void)
{
    if (!initialized) return;

    thread_delete(pgw_thread);

    pgw_fd_final();

    pgw_context_final();

    gtp_xact_final();
}

static void *THREAD_FUNC pgw_main(thread_id id, void *data)
{
    event_t event;
    fsm_t pgw_sm;
    c_time_t prev_tm, now_tm;
    status_t rv;

    memset(&event, 0, sizeof(event_t));

    pgw_self()->queue_id = event_create(MSGQ_O_NONBLOCK);
    d_assert(pgw_self()->queue_id, return NULL, 
            "PGW event queue creation failed");
    tm_service_init(&pgw_self()->tm_service);
    gtp_xact_init(&pgw_self()->tm_service,
            PGW_EVT_S5C_T3_RESPONSE, PGW_EVT_S5C_T3_HOLDING);

    fsm_create(&pgw_sm, pgw_state_initial, pgw_state_final);
    fsm_init(&pgw_sm, 0);

    prev_tm = time_now();

#define EVENT_LOOP_TIMEOUT 10   /* 10ms */
    while ((!thread_should_stop()))
    {
        sock_select_loop(EVENT_LOOP_TIMEOUT); 
        do
        {
            rv = event_recv(pgw_self()->queue_id, &event);

            d_assert(rv != CORE_ERROR, continue,
                    "While receiving a event message, error occurs");

            now_tm = time_now();

            /* if the gap is over event_loop timeout, execute preriodic jobs */
            if (now_tm - prev_tm > (EVENT_LOOP_TIMEOUT * 1000))
            {
                tm_execute_tm_service(
                        &pgw_self()->tm_service, pgw_self()->queue_id);

                prev_tm = now_tm;
            }

            if (rv == CORE_EAGAIN)
            {
                continue;
            }

            fsm_dispatch(&pgw_sm, (fsm_event_t*)&event);
        } while(rv == CORE_OK);
    }

    fsm_final(&pgw_sm, 0);
    fsm_clear(&pgw_sm);

    event_delete(pgw_self()->queue_id);

    return NULL;
}
