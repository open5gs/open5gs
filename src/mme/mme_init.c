#include "ogs-sctp.h"

#include "gtp/gtp_xact.h"
#include "app/context.h"

#include "mme_context.h"
#include "mme_sm.h"
#include "mme_event.h"

#include "mme_fd_path.h"
#include "s1ap-path.h"

static ogs_thread_t *thread;
static void mme_main(void *data);

static int initialized = 0;

int mme_initialize()
{
    int rv;

    mme_context_init();
    mme_event_init();

    rv = gtp_xact_init(mme_self()->timer_mgr);
    if (rv != OGS_OK) return rv;

    rv = mme_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = context_setup_log_module();
    if (rv != OGS_OK) return rv;

    rv = mme_m_tmsi_pool_generate();
    if (rv != OGS_OK) return rv;

    rv = mme_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

#define USRSCTP_LOCAL_UDP_PORT 9899
    rv = ogs_sctp_init(USRSCTP_LOCAL_UDP_PORT);
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(mme_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void mme_terminate(void)
{
    if (!initialized) return;

    mme_event_term();

    ogs_thread_destroy(thread);

    mme_fd_final();

    mme_context_final();

    ogs_sctp_final();

    gtp_xact_final();

    mme_event_final();
}

static void mme_main(void *data)
{
    ogs_fsm_t mme_sm;
    int rv;

    ogs_fsm_create(&mme_sm, mme_state_initial, mme_state_final);
    ogs_fsm_init(&mme_sm, 0);

    for ( ;; )
    {
        ogs_pollset_poll(mme_self()->pollset,
                ogs_timer_mgr_next(mme_self()->timer_mgr));

        ogs_timer_mgr_expire(mme_self()->timer_mgr);

        for ( ;; )
        {
            mme_event_t *e = NULL;

            rv = ogs_queue_trypop(mme_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&mme_sm, e);
            mme_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&mme_sm, 0);
    ogs_fsm_delete(&mme_sm);
}
