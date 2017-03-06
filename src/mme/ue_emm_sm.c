#define TRACE_MODULE _ue_emm_sm

#include "core_debug.h"

#include "nas_message.h"
#include "nas_conv.h"
#include "s6a_message.h"

#include "sm.h"
#include "context.h"
#include "event.h"

static void ue_emm_handle_attachrequest(ue_ctx_t *ue, nas_message_t *message);

void ue_emm_state_initial(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);

    FSM_TRAN(s, &ue_emm_state_operational);
}

void ue_emm_state_final(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);
}

void ue_emm_state_operational(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    ue_ctx_t *ue = s->ctx;
    d_assert(ue, return, "Null param");

    sm_trace(1, e);

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

            rv = nas_decode_pdu(&message, recvbuf);
            if (rv != CORE_OK) 
            {
                d_error("Can't parse NAS_PDU");
                break;
            }

            switch(message.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    ue_emm_handle_attachrequest(ue, &message);
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
            d_error("Unknown event %s", event_get_name(e));
            break;
        }
    }
}

void ue_emm_state_exception(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    sm_trace(1, e);

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
            d_error("Unknown event %s", event_get_name(e));
            break;
        }
    }
}

static void ue_emm_handle_attachrequest(ue_ctx_t *ue, nas_message_t *message)
{
    nas_attach_request_t *attach_request = &message->emm.attach_request;
    nas_eps_mobile_identity_t *eps_mobile_identity =
        &attach_request->eps_mobile_identity;

    switch(eps_mobile_identity->imsi.type_of_identity)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            c_uint8_t plmn_id[PLMN_ID_LEN];

            plmn_id_to_buffer(&mme_self()->plmn_id, plmn_id);
            if (attach_request->presencemask &
                NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
            {
                nas_tracking_area_identity_t *last_visited_registered_tai = 
                    &attach_request->last_visited_registered_tai;

                nas_plmn_bcd_to_buffer(
                    &last_visited_registered_tai->plmn, plmn_id);
            }

            nas_imsi_bcd_to_buffer(
                &eps_mobile_identity->imsi, eps_mobile_identity->length, 
                ue->imsi, &ue->imsi_len);

            s6a_send_auth_info_req(ue, plmn_id);
            break;
        }
        default:
        {
            d_warn("Not implemented(type:%d)", 
                    eps_mobile_identity->imsi.type_of_identity);
            
            break;
        }
    }
}
