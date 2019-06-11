#include "gtp/gtp-xact.h"

#include "app/context.h"
#include "pgw-context.h"
#include "pgw-event.h"
#include "pgw-sm.h"

#include "pgw-fd-path.h"

static ogs_thread_t *thread;
static void pgw_main(void *data);

static int initialized = 0;

int pgw_initialize()
{
    int rv;

    pgw_context_init();
    pgw_event_init();

    rv = gtp_xact_init(pgw_self()->timer_mgr);
    if (rv != OGS_OK) return rv;

    rv = pgw_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = context_setup_log_module();
    if (rv != OGS_OK) return rv;

    rv = pgw_ue_pool_generate();
    if (rv != OGS_OK) return rv;

    rv = pgw_fd_init();
    if (rv != 0) return OGS_ERROR;

    thread = ogs_thread_create(pgw_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void pgw_terminate(void)
{
    if (!initialized) return;

    pgw_event_term();

    ogs_thread_destroy(thread);

    pgw_fd_final();

    pgw_context_final();

    gtp_xact_final();

    pgw_event_final();
}

static void pgw_main(void *data)
{
    ogs_fsm_t pgw_sm;
    int rv;

    ogs_fsm_create(&pgw_sm, pgw_state_initial, pgw_state_final);
    ogs_fsm_init(&pgw_sm, 0);

    for ( ;; )
    {
        ogs_pollset_poll(pgw_self()->pollset,
                ogs_timer_mgr_next(pgw_self()->timer_mgr));

        ogs_timer_mgr_expire(pgw_self()->timer_mgr);

        for ( ;; )
        {
            pgw_event_t *e = NULL;

            rv = ogs_queue_trypop(pgw_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&pgw_sm, e);
            pgw_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&pgw_sm, 0);
    ogs_fsm_delete(&pgw_sm);
}
