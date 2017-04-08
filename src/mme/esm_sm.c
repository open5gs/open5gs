#define TRACE_MODULE _esm_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "nas_security.h"
#include "emm_handler.h"

void esm_state_initial(esm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &esm_state_operational);
}

void esm_state_final(esm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void esm_state_operational(esm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_esm_t *esm = s->ctx;
    mme_ue_t *ue = NULL;
    d_assert(esm, return, "Null param");

    ue = esm->ue;
    d_assert(ue, return, "Null param");

    mme_sm_trace(1, e);

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        case EVT_MSG_MME_ESM:
        {
            nas_message_t message;
            status_t rv;
            pkbuf_t *recvbuf = (pkbuf_t *)event_get_param2(e);
            d_assert(recvbuf, break, "Null param");

            rv = nas_security_decode(&message, ue, recvbuf);
            if (rv != CORE_OK) 
            {
                d_error("Can't parse NAS_PDU");
                break;
            }

            switch(message.emm.h.message_type)
            {
                case NAS_PDN_CONNECTIVITY_REQUEST:
                {
                    break;
                }
                default:
                {
                    d_warn("Not implemented(type:%d)", message.emm.h.message_type);
                    break;
                }
            }

            pkbuf_free(recvbuf);
            break;
        }

        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void esm_state_exception(esm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(1, e);

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}
