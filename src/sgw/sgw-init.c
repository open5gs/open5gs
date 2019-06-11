#include "gtp/gtp-xact.h"

#include "app/context.h"
#include "sgw-context.h"
#include "sgw-sm.h"
#include "sgw-event.h"

static ogs_thread_t *thread;
static void sgw_main(void *data);

static int initialized = 0;

int sgw_initialize()
{
    int rv;

    sgw_context_init();
    sgw_event_init();

    rv = gtp_xact_init(sgw_self()->timer_mgr);
    if (rv != OGS_OK) return rv;

    rv = sgw_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = context_setup_log_module();
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(sgw_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void sgw_terminate(void)
{
    if (!initialized) return;

    sgw_event_term();

    ogs_thread_destroy(thread);

    sgw_context_final();

    gtp_xact_final();

    sgw_event_final();
}

static void sgw_main(void *data)
{
    ogs_fsm_t sgw_sm;
    int rv;

    ogs_fsm_create(&sgw_sm, sgw_state_initial, sgw_state_final);
    ogs_fsm_init(&sgw_sm, 0);

    for ( ;; )
    {
        ogs_pollset_poll(sgw_self()->pollset,
                ogs_timer_mgr_next(sgw_self()->timer_mgr));

        ogs_timer_mgr_expire(sgw_self()->timer_mgr);

        for ( ;; )
        {
            sgw_event_t *e = NULL;

            rv = ogs_queue_trypop(sgw_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&sgw_sm, e);
            sgw_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&sgw_sm, 0);
    ogs_fsm_delete(&sgw_sm);
}
