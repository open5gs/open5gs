#define TRACE_MODULE _emm_handler

#include "core_debug.h"
#include "core_lib.h"

#include "nas_message.h"

#include "mme_event.h"

#include "mme_kdf.h"
#include "nas_security.h"
#include "nas_conv.h"
#include "mme_s6a_handler.h"
#include "esm_build.h"
#include "emm_build.h"
#include "s1ap_build.h"
#include "s1ap_path.h"

void emm_handle_esm_message_container(
        mme_ue_t *ue, nas_esm_message_container_t *esm_message_container)
{

    nas_esm_header_t *h = NULL;
    c_uint8_t pti = 0;
    mme_bearer_t *bearer = NULL;

    d_assert(ue, return, "Null param");
    d_assert(esm_message_container, return, "Null param");
    d_assert(esm_message_container->len, return, "Null param");

    h = (nas_esm_header_t *)esm_message_container->data;
    d_assert(h, return, "Null param");

    pti = h->procedure_transaction_identity;
    if (pti == NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED)
    {
        d_error("Procedure Trasaction is NOT triggered by UE");
        return;
    }

    bearer = mme_bearer_find_by_pti(ue, pti);
    if (!bearer)
    {
        bearer = mme_bearer_add(ue, pti);
        d_assert(bearer, return, "Null param");
    }
    else
    {
        d_warn("Duplicated: MME-UE-S1AP-ID[%d] sends "
            "PDN Connectivity Message[PTI(%d)]",
            ue->mme_ue_s1ap_id, bearer->pti);
    }

    mme_event_emm_to_esm(bearer, esm_message_container);
}

void emm_handle_attach_request(
        mme_ue_t *ue, nas_attach_request_t *attach_request)
{
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;

    emm_handle_esm_message_container(
            ue, &attach_request->esm_message_container);

    switch(eps_mobile_identity->imsi.type)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            nas_imsi_to_bcd(
                &eps_mobile_identity->imsi, eps_mobile_identity->length,
                ue->imsi_bcd);
            core_bcd_to_buffer(ue->imsi_bcd, ue->imsi, &ue->imsi_len);
            d_assert(ue->imsi_len, return,
                    "Can't get IMSI(len:%d\n", ue->imsi_len);

            memcpy(&ue->visited_plmn_id, &mme_self()->plmn_id, PLMN_ID_LEN);
            if (attach_request->presencemask &
                NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
            {
                nas_tracking_area_identity_t *last_visited_registered_tai = 
                    &attach_request->last_visited_registered_tai;

                memcpy(&ue->visited_plmn_id, 
                        &last_visited_registered_tai->plmn_id,
                        PLMN_ID_LEN);
            }

            memcpy(&ue->ue_network_capability, 
                    &attach_request->ue_network_capability,
                    sizeof(attach_request->ue_network_capability));
            memcpy(&ue->ms_network_capability, 
                    &attach_request->ms_network_capability,
                    sizeof(attach_request->ms_network_capability));

            mme_s6a_send_air(ue);
            break;
        }
        default:
        {
            d_warn("Not implemented(type:%d)", 
                    eps_mobile_identity->imsi.type);
            
            return;
        }
    }
}

void emm_handle_authentication_request(mme_ue_t *ue)
{
    nas_message_t message;
    pkbuf_t *sendbuf = NULL;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;

    d_assert(ue, return, "Null param");

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REQUEST;

    memcpy(authentication_request->authentication_parameter_rand.rand,
            ue->rand, RAND_LEN);
    memcpy(authentication_request->authentication_parameter_autn.autn,
            ue->autn, AUTN_LEN);
    authentication_request->authentication_parameter_autn.length = 
            AUTN_LEN;

    d_assert(nas_plain_encode(&sendbuf, &message) == CORE_OK && sendbuf,,);

    mme_event_nas_to_s1ap(ue, sendbuf);
}

void emm_handle_authentication_response(
        mme_ue_t *ue, nas_authentication_response_t *authentication_response)
{
    nas_authentication_response_parameter_t *authentication_response_parameter =
        &authentication_response->authentication_response_parameter;

    nas_message_t message;
    pkbuf_t *sendbuf = NULL;
    nas_security_mode_command_t *security_mode_command = 
        &message.emm.security_mode_command;
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

    d_info("[NAS] Authentication response : UE[%s] --> EMM", 
            ue->imsi_bcd);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_SECURITY_MODE_COMMAND;

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

    d_assert(nas_security_encode(&sendbuf, ue, &message) == CORE_OK && 
            sendbuf,,);
    mme_event_nas_to_s1ap(ue, sendbuf);
}

void emm_handle_lo_create_session(mme_bearer_t *bearer)
{
    pkbuf_t *bearerbuf = NULL, *emmbuf = NULL, *s1apbuf = NULL;
    mme_ue_t *ue = NULL;
    mme_enb_t *enb = NULL;
    status_t rv;

    d_assert(bearer, return, "Null param");
    ue = bearer->ue;
    d_assert(ue, return, "Null param");
    enb = ue->enb;
    d_assert(ue->enb, return, "Null param");

    rv = esm_build_activate_default_bearer_context(&bearerbuf, bearer);
    d_assert(rv == CORE_OK, return, "bearer build error");

    rv = emm_build_attach_accept(&emmbuf, ue, bearerbuf);
    d_assert(rv == CORE_OK, pkbuf_free(bearerbuf); return, "emm build error");

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, bearer, emmbuf);
    d_assert(rv == CORE_OK, pkbuf_free(emmbuf); return, "emm build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,,);
}

