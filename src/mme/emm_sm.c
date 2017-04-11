#define TRACE_MODULE _emm_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "emm_handler.h"
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
        case EVT_MSG_MME_EMM:
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
                case NAS_AUTHENTICATION_REQUEST:
                {
                    pkbuf_t *recvbuf = (pkbuf_t *)event_get_param2(e);
                    d_assert(recvbuf, break, "Null param");

                    mme_event_nas_to_s1ap(ue, pkbuf_copy(recvbuf));

                    d_info("[NAS] Authentication request : UE[%s] <-- EMM",
                            ue->imsi_bcd);
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
