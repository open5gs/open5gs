#define TRACE_MODULE _emm_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "nas_security.h"
#include "emm_handler.h"

void emm_state_initial(emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &emm_state_operational);
}

void emm_state_final(emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void emm_state_operational(emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_ue_t *ue = s->ctx;
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
        case EVT_MSG_UE_EMM:
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

            switch(message.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    emm_handle_attach_request(
                            ue, &message.attach_request);
                    break;
                }
                case NAS_AUTHENTICATION_REQUEST:
                {
                    pkbuf_t *pkbuf = pkbuf_copy(recvbuf);
                    mme_event_nas_to_s1ap(ue, pkbuf);

                    d_assert(ue->imsi, return,);
                    d_info("[NAS] Authentication request : UE[%s] <-- EMM",
                            ue->imsi);
                    break;
                }
                case NAS_AUTHENTICATION_RESPONSE:
                {
                    emm_handle_authentication_response(
                            ue, &message.authentication_response);
                    break;
                }
                case NAS_SECURITY_MODE_COMPLETE:
                {
                    d_assert(ue->imsi, return,);
                    d_info("[NAS] Security mode complete : UE[%s] --> EMM",
                            ue->imsi);
                    emm_handle_security_mode_complete(ue);
                    break;
                }
                default:
                {
                    d_warn("Not implemented(type:%d)", message.h.message_type);
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

void emm_state_exception(emm_sm_t *s, event_t *e)
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
