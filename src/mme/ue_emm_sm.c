#define TRACE_MODULE _ue_emm_sm

#include "core_debug.h"

#include "s1ap_message.h"
#include "nas_message.h"

#include "sm.h"
#include "context.h"
#include "event.h"

#include "s1ap_path.h"
#include "s1ap_conv.h"
#include "nas_conv.h"
#include "s6a_sm.h"

static void ue_emm_handle_attach_request(ue_ctx_t *ue, nas_message_t *message);
static void ue_emm_handle_authentication_request(
        ue_ctx_t *ue, pkbuf_t *recvbuf);
static void ue_emm_handle_authentication_response(
        ue_ctx_t *ue, nas_message_t *message);

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
                    ue_emm_handle_attach_request(ue, &message);
                    break;
                }
                case NAS_AUTHENTICATION_REQUEST:
                {
                    ue_emm_handle_authentication_request(ue, recvbuf);
                    break;
                }
                case NAS_AUTHENTICATION_RESPONSE:
                {
                    ue_emm_handle_authentication_response(ue, &message);
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

static void ue_emm_handle_attach_request(ue_ctx_t *ue, nas_message_t *message)
{
    nas_attach_request_t *attach_request = &message->emm.attach_request;
    nas_eps_mobile_identity_t *eps_mobile_identity =
        &attach_request->eps_mobile_identity;

    switch(eps_mobile_identity->imsi.type_of_identity)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            c_uint8_t plmn_id[PLMN_ID_LEN];

            s1ap_plmn_id_to_buffer(&mme_self()->plmn_id, plmn_id);
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

static void ue_emm_handle_authentication_request(ue_ctx_t *ue, pkbuf_t *recvbuf)
{
    int encoded;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;
    pkbuf_t *sendbuf = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = ue->enb_ue_s1ap_id;

    nasPdu->size = recvbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, recvbuf->payload, nasPdu->size);

    message.procedureCode = S1ap_ProcedureCode_id_downlinkNASTransport;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(&sendbuf, &message);
    s1ap_free_pdu(&message);
    d_assert(encoded >= 0, , "encode failed");

    d_assert(s1ap_send_to_enb(ue->enb, sendbuf) == CORE_OK, , "send error");
    pkbuf_free(recvbuf);
}

static void ue_emm_handle_authentication_response(
        ue_ctx_t *ue, nas_message_t *message)
{
    nas_authentication_response_t *authentication_response = 
        &message->emm.authentication_response;
    nas_authentication_response_parameter_t *authentication_response_parameter =
        &authentication_response->authentication_response_parameter;

    if (authentication_response_parameter->length != ue->xres_len ||
        memcmp(authentication_response_parameter->res,
            ue->xres, ue->xres_len) != 0)
    {
        d_error("authentication failed");
        return;
    }

    d_print_hex(authentication_response_parameter->res, 
            authentication_response_parameter->length);
}
