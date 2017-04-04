#define TRACE_MODULE _emm_handler

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"

#include "kdf.h"
#include "nas_security.h"
#include "nas_conv.h"
#include "s6a_sm.h"

void emm_handle_attach_request(
        ue_ctx_t *ue, nas_attach_request_t *attach_request)
{
    nas_eps_mobile_identity_t *eps_mobile_identity =
        &attach_request->eps_mobile_identity;

    switch(eps_mobile_identity->imsi.type_of_identity)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            plmn_id_t *plmn_id = &mme_self()->plmn_id;

            if (attach_request->presencemask &
                NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
            {
                nas_tracking_area_identity_t *last_visited_registered_tai = 
                    &attach_request->last_visited_registered_tai;

                plmn_id = &last_visited_registered_tai->plmn_id;
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

void emm_handle_authentication_response(
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
    mme_event_nas_to_s1ap(ue, sendbuf);

    d_assert(ue->imsi, return,);
    d_info("[NAS] Security mode command : UE[%s] <-- EMM", ue->imsi);
}

