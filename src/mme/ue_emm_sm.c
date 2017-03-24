#define TRACE_MODULE _ue_emm_sm

#include "core_debug.h"

#include "s1ap_message.h"
#include "nas_message.h"

#include "sm.h"
#include "context.h"
#include "event.h"

#include "kdf.h"
#include "nas_security.h"
#include "s1ap_path.h"
#include "s1ap_conv.h"
#include "nas_conv.h"
#include "s6a_sm.h"

static void ue_emm_handle_attach_request(
        ue_ctx_t *ue, nas_attach_request_t *attach_request);
static void ue_emm_handle_authentication_response(
        ue_ctx_t *ue, nas_authentication_response_t *authentication_response);

static void ue_emm_send_to_enb(ue_ctx_t *ue, pkbuf_t *pkbuf);

void ue_emm_state_initial(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &ue_emm_state_operational);
}

void ue_emm_state_final(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void ue_emm_state_operational(ue_emm_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    ue_ctx_t *ue = s->ctx;
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
                    ue_emm_handle_attach_request(
                            ue, &message.attach_request);
                    break;
                }
                case NAS_AUTHENTICATION_REQUEST:
                {
                    pkbuf_t *pkbuf = pkbuf_copy(recvbuf);
                    ue_emm_send_to_enb(ue, pkbuf);

                    d_assert(ue->imsi, return,);
                    d_info("[NAS] Authentication request : UE[%s] <-- EMM",
                            ue->imsi);
                    break;
                }
                case NAS_AUTHENTICATION_RESPONSE:
                {
                    ue_emm_handle_authentication_response(
                            ue, &message.authentication_response);
                    break;
                }
                case NAS_SECURITY_MODE_COMPLETE:
                {
                    d_assert(ue->imsi, return,);
                    d_info("[NAS] Security mode complete : UE[%s] --> EMM",
                            ue->imsi);
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

void ue_emm_state_exception(ue_emm_sm_t *s, event_t *e)
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

static void ue_emm_handle_attach_request(
        ue_ctx_t *ue, nas_attach_request_t *attach_request)
{
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

            memcpy(&ue->ue_network_capability, 
                    &attach_request->ue_network_capability,
                    sizeof(attach_request->ue_network_capability));
            memcpy(&ue->ms_network_capability, 
                    &attach_request->ms_network_capability,
                    sizeof(attach_request->ms_network_capability));

            d_assert(ue->imsi, return,);
            d_info("[NAS] Attach request : UE[%s] --> EMM", ue->imsi);

            s6a_send_auth_info_req(ue, plmn_id);
            break;
        }
        default:
        {
            d_warn("Not implemented(type:%d)", 
                    eps_mobile_identity->imsi.type_of_identity);
            
            return;
        }
    }
}

static void ue_emm_handle_authentication_response(
        ue_ctx_t *ue, nas_authentication_response_t *authentication_response)
{
    nas_authentication_response_parameter_t *authentication_response_parameter =
        &authentication_response->authentication_response_parameter;

    nas_message_t message;
    pkbuf_t *sendbuf = NULL;
    nas_security_mode_command_t *security_mode_command = 
        &message.security_mode_command;
    nas_security_algorithms_t *selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    nas_key_set_identifier_t *nas_key_set_identifier =
        &security_mode_command->nas_key_set_identifier;
    nas_ue_security_capability_t *replayed_ue_security_capabilities = 
        &security_mode_command->replayed_ue_security_capabilities;

    d_assert(ue, return, "Null param");

    if (authentication_response_parameter->length != ue->xres_len ||
        memcmp(authentication_response_parameter->res,
            ue->xres, ue->xres_len) != 0)
    {
        d_error("authentication failed");
        return;
    }

    d_assert(ue->imsi, return, );
    d_info("[NAS] Authentication response : UE[%s] --> EMM", ue->imsi);

    memset(&message, 0, sizeof(message));
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_type = NAS_SECURITY_MODE_COMMAND;

    selected_nas_security_algorithms->type_of_ciphering_algorithm =
        mme_self()->selected_enc_algorithm;
    selected_nas_security_algorithms->type_of_integrity_protection_algorithm =
        mme_self()->selected_int_algorithm;

    nas_key_set_identifier->tsc = 0;
    nas_key_set_identifier->nas_key_set_identifier = 0;

    replayed_ue_security_capabilities->length =
        sizeof(replayed_ue_security_capabilities->eea) +
        sizeof(replayed_ue_security_capabilities->eia) +
        sizeof(replayed_ue_security_capabilities->uea) +
        sizeof(replayed_ue_security_capabilities->uia) +
        sizeof(replayed_ue_security_capabilities->gea);
    replayed_ue_security_capabilities->eea = ue->ue_network_capability.eea;
    replayed_ue_security_capabilities->eia = ue->ue_network_capability.eia;
    replayed_ue_security_capabilities->uea = ue->ue_network_capability.uea;
    replayed_ue_security_capabilities->uia = ue->ue_network_capability.uia;
    replayed_ue_security_capabilities->gea = 
        (ue->ms_network_capability.gea1 << 6) | 
        ue->ms_network_capability.extended_gea;

    mme_kdf_nas(MME_KDF_NAS_INT_ALG, mme_self()->selected_int_algorithm,
            ue->kasme, ue->knas_int);
    mme_kdf_nas(MME_KDF_NAS_ENC_ALG, mme_self()->selected_enc_algorithm,
            ue->kasme, ue->knas_enc);

    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT;
    d_assert(nas_security_encode(&sendbuf, ue, &message) == CORE_OK && 
            sendbuf,,);
    ue_emm_send_to_enb(ue, sendbuf);

    d_assert(ue->imsi, return,);
    d_info("[NAS] Security mode command : UE[%s] <-- EMM", ue->imsi);
}

static void ue_emm_send_to_enb(ue_ctx_t *ue, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;
    pkbuf_t *sendbuf = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = ue->enb_ue_s1ap_id;

    nasPdu->size = pkbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, pkbuf->payload, nasPdu->size);

    message.procedureCode = S1ap_ProcedureCode_id_downlinkNASTransport;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(&sendbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(sendbuf && encoded >= 0,,);
    d_assert(s1ap_send_to_enb(ue->enb, sendbuf) == CORE_OK,,);
    pkbuf_free(pkbuf);

    d_assert(ue->enb,,);
    d_info("[S1AP] downlinkNASTransport : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        ue->enb->enb_id);
}
