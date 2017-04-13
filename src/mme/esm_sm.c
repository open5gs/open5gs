#define TRACE_MODULE _bearer_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "esm_handler.h"
#include "esm_build.h"

void esm_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &esm_state_operational);
}

void esm_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void esm_state_operational(fsm_t *s, event_t *e)
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
        case MME_EVT_ESM_BEARER_LO_INFO_REQ:
        {
            index_t index = event_get_param1(e);
            mme_bearer_t *bearer = NULL;
            mme_ue_t *ue = NULL;

            d_assert(index, return, "Null param");
            bearer = mme_bearer_find(index);
            d_assert(bearer, return, "Null param");
            ue = bearer->ue;
            d_assert(ue, return, "Null param");

            switch(event_get(e))
            {
                case MME_EVT_ESM_BEARER_LO_INFO_REQ:
                {
                    d_info("[NAS] ESM information request : "
                            "UE[%s] <--- ESM[%d]", ue->imsi_bcd, bearer->pti);
                    esm_handle_lo_information_request(bearer);
                    break;
                }
            }

            break;
        }
        case MME_EVT_ESM_BEARER_MSG:
        {
            index_t index = event_get_param1(e);
            mme_bearer_t *bearer = NULL;
            mme_ue_t *ue = NULL;
            nas_message_t *message = NULL;

            d_assert(index, return, "Null param");
            bearer = mme_bearer_find(index);
            d_assert(bearer, return, "Null param");
            ue = bearer->ue;
            d_assert(ue, return, "Null param");
            message = (nas_message_t *)event_get_param3(e);
            d_assert(message, break, "Null param");

            switch(message->esm.h.message_type)
            {
                case NAS_PDN_CONNECTIVITY_REQUEST:
                {
                    esm_handle_pdn_connectivity_request(
                            bearer, &message->esm.pdn_connectivity_request);
                    d_info("[NAS] PDN connectivity request : "
                            "UE[%s] --> ESM[%d]", ue->imsi_bcd, bearer->pti);
                    break;
                }
                case NAS_ESM_INFORMATION_RESPONSE:
                {
                    /* FIXME : SGW Selection */
                    bearer->sgw = mme_sgw_first();

                    d_info("[NAS] ESM information response : "
                            "UE[%s] --> ESM[%d]", ue->imsi_bcd, bearer->pti);
                    esm_handle_information_response(
                            bearer, &message->esm.esm_information_response);
                    break;
                }
                default:
                {
                    d_warn("Not implemented(type:%d)", 
                            message->esm.h.message_type);
                    break;
                }
            }
            break;
        }

        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void esm_state_exception(fsm_t *s, event_t *e)
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
