#include "mme_event.h"
#include "mme_context.h"

#include "s1ap-path.h"

#define EVENT_POOL 32 /* FIXME : 32 */
void mme_event_init(void)
{
    mme_self()->queue = ogs_queue_create(EVENT_POOL);
    ogs_assert(mme_self()->queue);
    mme_self()->timer_mgr = ogs_timer_mgr_create();
    ogs_assert(mme_self()->timer_mgr);
    mme_self()->pollset = ogs_pollset_create();
    ogs_assert(mme_self()->pollset);
}

void mme_event_term(void)
{
    ogs_queue_term(mme_self()->queue);
    ogs_pollset_notify(mme_self()->pollset);
}

void mme_event_final(void)
{
    if (mme_self()->pollset)
        ogs_pollset_destroy(mme_self()->pollset);
    if (mme_self()->timer_mgr)
        ogs_timer_mgr_destroy(mme_self()->timer_mgr);
    if (mme_self()->queue)
        ogs_queue_destroy(mme_self()->queue);
}

mme_event_t *mme_event_new(mme_event_e id)
{
    mme_event_t *e = NULL;

    e = ogs_calloc(1, sizeof *e);
    ogs_assert(e);
    e->id = id;

    return e;
}

void mme_event_free(mme_event_t *e)
{
    ogs_assert(e);
    ogs_free(e);
}

void mme_event_timeout(void *data)
{
    int rc;
    mme_event_t *e = data;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(e);

    enb_ue = e->enb_ue;
    ogs_assert(enb_ue);
    pkbuf = e->pkbuf;
    ogs_assert(pkbuf);

    rc = s1ap_send_to_enb_ue(enb_ue, pkbuf);
    ogs_assert(rc == OGS_OK);

    ogs_timer_delete(e->timer);
    mme_event_free(e);
}

const char *mme_event_get_name(mme_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id)
    {
        case OGS_FSM_ENTRY_SIG: 
            return OGS_FSM_NAME_ENTRY_SIG;
        case OGS_FSM_EXIT_SIG: 
            return OGS_FSM_NAME_EXIT_SIG;

        case MME_EVT_S1AP_MESSAGE:
            return "MME_EVT_S1AP_MESSAGE";
        case MME_EVT_S1AP_DELAYED_SEND:
            return "MME_EVT_S1AP_DELAYED_SEND";
        case MME_EVT_S1AP_LO_ACCEPT:
            return "MME_EVT_S1AP_LO_ACCEPT";
        case MME_EVT_S1AP_LO_SCTP_COMM_UP:
            return "MME_EVT_S1AP_LO_SCTP_COMM_UP";
        case MME_EVT_S1AP_LO_CONNREFUSED:
            return "MME_EVT_S1AP_LO_CONNREFUSED";

        case MME_EVT_EMM_MESSAGE:
            return "MME_EVT_EMM_MESSAGE";

        case MME_EVT_ESM_MESSAGE:
            return "MME_EVT_ESM_MESSAGE";

        case MME_EVT_S11_MESSAGE:
            return "MME_EVT_S11_MESSAGE";

        case MME_EVT_S6A_MESSAGE:
            return "MME_EVT_S6A_MESSAGE";

        default: 
           break;
    }

    return "UNKNOWN_EVENT";
}
