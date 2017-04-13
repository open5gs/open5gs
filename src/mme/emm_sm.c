#define TRACE_MODULE _emm_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "emm_handler.h"
#include "emm_build.h"
#include "mme_s6a_handler.h"

void emm_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &emm_state_operational);
}

void emm_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void emm_state_operational(fsm_t *s, event_t *e)
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
        case MME_EVT_EMM_BEARER_LO_CREATE_SESSION:
        {
            index_t index = event_get_param1(e);
            mme_bearer_t *bearer = NULL;

            d_assert(index, break, "Null param");
            bearer = mme_bearer_find(index);
            d_assert(bearer, break, "No Bearer context");

            emm_handle_lo_create_session(bearer);
            break;
        }
        case MME_EVT_EMM_UE_LO_AUTH_REQ:
        case MME_EVT_EMM_UE_LO_LOCATION_UPDATE:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *ue = NULL;

            d_assert(index, return, "Null param");
            ue = mme_ue_find(index);
            d_assert(ue, return, "Null param");

            switch(event_get(e))
            {
                case MME_EVT_EMM_UE_LO_AUTH_REQ:
                {
                     d_info("[NAS] Authentication request : UE[%s] <-- EMM",
                             ue->imsi_bcd);
                    emm_handle_authentication_request(ue);
                    break;
                }
                case MME_EVT_EMM_UE_LO_LOCATION_UPDATE:
                {
                    mme_bearer_t *bearer = mme_bearer_first(ue);

                    while(bearer)
                    {
                        event_t e;
                        event_set(&e, MME_EVT_ESM_BEARER_LO_INFO_REQ);
                        event_set_param1(&e, (c_uintptr_t)bearer->index);
                        mme_event_send(&e);

                        bearer = mme_bearer_next(bearer);
                    }
                    break;
                }
            }
            break;
        }
        case MME_EVT_EMM_UE_MSG:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *ue = NULL;
            nas_message_t *message = NULL;

            d_assert(index, return, "Null param");
            ue = mme_ue_find(index);
            d_assert(ue, return, "Null param");

            message = (nas_message_t *)event_get_param3(e);
            d_assert(message, break, "Null param");

            switch(message->emm.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    emm_handle_attach_request(
                            ue, &message->emm.attach_request);
                    break;
                }
                case NAS_AUTHENTICATION_RESPONSE:
                {
                    emm_handle_authentication_response(
                            ue, &message->emm.authentication_response);
                    break;
                }
                case NAS_SECURITY_MODE_COMPLETE:
                {
                    d_info("[NAS] Security mode complete : UE[%s] --> EMM",
                            ue->imsi_bcd);
                    mme_s6a_send_ulr(ue);
                    break;
                }
                case NAS_ATTACH_COMPLETE:
                {
                    d_info("[NAS] Attach complete : UE[%s] --> EMM",
                            ue->imsi_bcd);
                    emm_handle_attach_complete(
                            ue, &message->emm.attach_complete);
                    break;
                }
                default:
                {
                    d_warn("Not implemented(type:%d)", 
                            message->emm.h.message_type);
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

void emm_state_exception(fsm_t *s, event_t *e)
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
