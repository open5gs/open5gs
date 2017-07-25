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

#include "mme_s11_build.h"
#include "mme_s11_path.h"

void emm_handle_identity_request(mme_ue_t *mme_ue);

void emm_handle_esm_message_container(
        mme_ue_t *mme_ue, nas_esm_message_container_t *esm_message_container)
{
    pkbuf_t *esmbuf = NULL;
    event_t e;

    nas_esm_header_t *h = NULL;
    c_uint8_t pti = NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    c_uint8_t ebi = NAS_EPS_BEARER_IDENTITY_UNASSIGNED;
    mme_bearer_t *bearer = NULL;

    d_assert(mme_ue, return, "Null param");
    d_assert(esm_message_container, return, "Null param");
    d_assert(esm_message_container->len, return, "Null param");

    h = (nas_esm_header_t *)esm_message_container->data;
    d_assert(h, return, "Null param");

    pti = h->procedure_transaction_identity;
    ebi = h->eps_bearer_identity;
    if (pti == NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED && ebi)
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, ebi);
    else if (ebi == NAS_EPS_BEARER_IDENTITY_UNASSIGNED && pti)
        bearer = mme_bearer_find_by_ue_pti(mme_ue, pti);

    if (!bearer)
        bearer = mme_sess_add(mme_ue, pti);
    d_assert(bearer, return, "No Bearer Context");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    esmbuf = pkbuf_alloc(NAS_HEADROOM, esm_message_container->len);
    d_assert(esmbuf, return, "Null param");
    memcpy(esmbuf->payload, 
            esm_message_container->data, esm_message_container->len);

    event_set(&e, MME_EVT_ESM_BEARER_MSG);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    event_set_param2(&e, (c_uintptr_t)esmbuf);
    mme_event_send(&e);
}

void emm_handle_attach_request(
        mme_ue_t *mme_ue, nas_attach_request_t *attach_request)
{
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;

    emm_handle_esm_message_container(
            mme_ue, &attach_request->esm_message_container);

    /* Store UE specific information */
    if (attach_request->presencemask &
        NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
    {
        nas_tracking_area_identity_t *last_visited_registered_tai = 
            &attach_request->last_visited_registered_tai;

        memcpy(&mme_ue->visited_plmn_id, 
                &last_visited_registered_tai->plmn_id,
                PLMN_ID_LEN);
    }
    else
    {
        /* FIXME : what will do if we don't know last visited plmn_id */
        memcpy(&mme_ue->visited_plmn_id,
                &mme_self()->served_tai[0].plmn_id, PLMN_ID_LEN);
    }

    memcpy(&mme_ue->ue_network_capability, 
            &attach_request->ue_network_capability,
            sizeof(attach_request->ue_network_capability));
    memcpy(&mme_ue->ms_network_capability, 
            &attach_request->ms_network_capability,
            sizeof(attach_request->ms_network_capability));

    switch(eps_mobile_identity->imsi.type)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

            nas_imsi_to_bcd(
                &eps_mobile_identity->imsi, eps_mobile_identity->length,
                imsi_bcd);
            mme_ue_set_imsi(mme_ue, imsi_bcd);

            d_info("[NAS] Attach request : UE_IMSI[%s] --> EMM", imsi_bcd);

            /* TODO : check if authorization process is needed or not */
            mme_s6a_send_air(mme_ue);
            break;
        }
        case NAS_EPS_MOBILE_IDENTITY_GUTI:
        {
            nas_eps_mobile_identity_guti_t *nas_guti = NULL;
            nas_guti = &eps_mobile_identity->guti;
            guti_t guti;

            guti.plmn_id = nas_guti->plmn_id;
            guti.mme_gid = nas_guti->mme_gid;
            guti.mme_code = nas_guti->mme_code;
            guti.m_tmsi = nas_guti->m_tmsi;

            d_info("[NAS] Attach request : UE_GUTI[G:%d,C:%d,M_TMSI:0x%x] --> EMM", 
                    guti.mme_gid,
                    guti.mme_code,
                    guti.m_tmsi);


            /* FIXME :Check if GUTI was assigend from us */
            
            /* FIXME :If not, forward the message to other MME */

            /* FIXME : Find UE based on GUTI.
             *         The record with GUTI,IMSI should be 
             *         stored in permanent DB
             */
            if (memcmp(&guti, &mme_ue->guti, sizeof(guti_t)) == 0)
            {
                /* TODO : check if authorization process is needed or not */
                mme_s6a_send_air(mme_ue);
            }
            else
            {
                emm_handle_identity_request(mme_ue);
            }
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

void emm_handle_identity_request(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;

    nas_message_t message;
    nas_identity_request_t *identity_request = 
        &message.emm.identity_request;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_IDENTITY_REQUEST;

    /* Request IMSI */
    identity_request->identity_type.type = NAS_IDENTITY_TYPE_2_IMSI;

    d_assert(nas_plain_encode(&emmbuf, &message) == CORE_OK && emmbuf,,);

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_identity_response(
        mme_ue_t *mme_ue, nas_identity_response_t *identity_response)
{
    nas_mobile_identity_t *mobile_identity = NULL;

    d_assert(mme_ue, return, "Null param");
    d_assert(identity_response, return, "Null param");

    mobile_identity = &identity_response->mobile_identity;

    if (mobile_identity->imsi.type == NAS_IDENTITY_TYPE_2_IMSI)
    {
        c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

        nas_imsi_to_bcd(
            &mobile_identity->imsi, mobile_identity->length,
            imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        d_assert(mme_ue->imsi_len, return,
                "Can't get IMSI(len:%d\n", mme_ue->imsi_len);
    }
    else
    {
        d_warn("Not supported Identity type(%d)", mobile_identity->imsi.type);
        return;
    }

    /* TODO : check if authorization process is needed or not */
    /* Send Authentication Information Request to HSS */
    mme_s6a_send_air(mme_ue);

}

void emm_handle_authentication_request(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;

    nas_message_t message;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REQUEST;

    memcpy(authentication_request->authentication_parameter_rand.rand,
            mme_ue->rand, RAND_LEN);
    memcpy(authentication_request->authentication_parameter_autn.autn,
            mme_ue->autn, AUTN_LEN);
    authentication_request->authentication_parameter_autn.length = 
            AUTN_LEN;

    d_assert(nas_plain_encode(&emmbuf, &message) == CORE_OK && emmbuf,,);

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_authentication_response(mme_ue_t *mme_ue, 
        nas_authentication_response_t *authentication_response)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;
    int i;

    nas_authentication_response_parameter_t *authentication_response_parameter =
        &authentication_response->authentication_response_parameter;

    nas_message_t message;
    nas_security_mode_command_t *security_mode_command = 
        &message.emm.security_mode_command;
    nas_security_algorithms_t *selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    nas_key_set_identifier_t *nas_key_set_identifier =
        &security_mode_command->nas_key_set_identifier;
    nas_ue_security_capability_t *replayed_ue_security_capabilities = 
        &security_mode_command->replayed_ue_security_capabilities;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    if (authentication_response_parameter->length != mme_ue->xres_len ||
        memcmp(authentication_response_parameter->res,
            mme_ue->xres, mme_ue->xres_len) != 0)
    {
        d_error("authentication failed");
        return;
    }

    d_info("[NAS] Authentication response : UE[%s] --> EMM", 
            mme_ue->imsi_bcd);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_SECURITY_MODE_COMMAND;

    for (i = 0; i < mme_self()->num_of_integrity_order; i++)
    {
        if (mme_ue->ue_network_capability.eia & 
                (0x80 >> mme_self()->integrity_order[i]))
        {
            mme_ue->selected_int_algorithm = mme_self()->integrity_order[i];
            break;
        }
    }
    for (i = 0; i < mme_self()->num_of_ciphering_order; i++)
    {
        if (mme_ue->ue_network_capability.eea & 
                (0x80 >> mme_self()->ciphering_order[i]))
        {
            mme_ue->selected_enc_algorithm = mme_self()->ciphering_order[i];
            break;
        }
    }

    selected_nas_security_algorithms->type_of_integrity_protection_algorithm =
        mme_ue->selected_int_algorithm;
    selected_nas_security_algorithms->type_of_ciphering_algorithm =
        mme_ue->selected_enc_algorithm;

    nas_key_set_identifier->tsc = 0;
    nas_key_set_identifier->nas_key_set_identifier = 0;

    replayed_ue_security_capabilities->length =
        sizeof(replayed_ue_security_capabilities->eea) +
        sizeof(replayed_ue_security_capabilities->eia) +
        sizeof(replayed_ue_security_capabilities->uea) +
        sizeof(replayed_ue_security_capabilities->uia) +
        sizeof(replayed_ue_security_capabilities->gea);
    replayed_ue_security_capabilities->eea = mme_ue->ue_network_capability.eea;
    replayed_ue_security_capabilities->eia = mme_ue->ue_network_capability.eia;
    replayed_ue_security_capabilities->uea = mme_ue->ue_network_capability.uea;
    replayed_ue_security_capabilities->uia = mme_ue->ue_network_capability.uia;
    replayed_ue_security_capabilities->gea = 
        (mme_ue->ms_network_capability.gea1 << 6) | 
        mme_ue->ms_network_capability.extended_gea;

    mme_kdf_nas(MME_KDF_NAS_INT_ALG, mme_ue->selected_int_algorithm,
            mme_ue->kasme, mme_ue->knas_int);
    mme_kdf_nas(MME_KDF_NAS_ENC_ALG, mme_ue->selected_enc_algorithm,
            mme_ue->kasme, mme_ue->knas_enc);
    mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);

    d_info("[NAS] Security mode command : UE[%s] <-- EMM", 
            mme_ue->imsi_bcd);

    rv = nas_security_encode(&emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && emmbuf, return, "emm build error");

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_create_session_response(mme_bearer_t *bearer)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_enb_t *enb = NULL;
    pkbuf_t *esmbuf = NULL, *emmbuf = NULL, *s1apbuf = NULL;

    d_assert(bearer, return, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    rv = esm_build_activate_default_bearer_context(&esmbuf, bearer);
    d_assert(rv == CORE_OK && esmbuf, 
            return, "bearer build error");

    d_info("[NAS] Activate default bearer context request : EMM <-- ESM[%d]",
            bearer->ebi);

    rv = emm_build_attach_accept(&emmbuf, mme_ue, esmbuf);
    d_assert(rv == CORE_OK && emmbuf, 
            pkbuf_free(esmbuf); return, "emm build error");

    d_info("[NAS] Attach accept : UE[%s] <-- EMM", mme_ue->imsi_bcd);

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, bearer, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_attach_complete(
    mme_ue_t *mme_ue, nas_attach_complete_t *attach_complete)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;

    nas_message_t message;
    nas_emm_information_t *emm_information = &message.emm.emm_information;
    nas_time_zone_and_time_t *universal_time_and_local_time_zone =
        &emm_information->universal_time_and_local_time_zone;
    nas_daylight_saving_time_t *network_daylight_saving_time = 
        &emm_information->network_daylight_saving_time;

    time_exp_t time_exp;
    time_exp_lt(&time_exp, time_now());

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    emm_handle_esm_message_container(
            mme_ue, &attach_complete->esm_message_container);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_EMM_INFORMATION;

    emm_information->presencemask |=
        NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
    universal_time_and_local_time_zone->year = 
                NAS_TIME_TO_BCD(time_exp.tm_year % 100);
    universal_time_and_local_time_zone->mon = NAS_TIME_TO_BCD(time_exp.tm_mon);
    universal_time_and_local_time_zone->mday = 
                NAS_TIME_TO_BCD(time_exp.tm_mday);
    universal_time_and_local_time_zone->hour = 
                NAS_TIME_TO_BCD(time_exp.tm_hour);
    universal_time_and_local_time_zone->min = NAS_TIME_TO_BCD(time_exp.tm_min);
    universal_time_and_local_time_zone->sec = NAS_TIME_TO_BCD(time_exp.tm_sec);
    if (time_exp.tm_gmtoff > 0)
        universal_time_and_local_time_zone->sign = 0;
    else
        universal_time_and_local_time_zone->sign = 1;
    /* quarters of an hour */
    universal_time_and_local_time_zone->gmtoff = 
                NAS_TIME_TO_BCD(time_exp.tm_gmtoff / 900);

    emm_information->presencemask |=
        NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
    network_daylight_saving_time->length = 1;

    d_info("[NAS] EMM information : UE[%s] <-- EMM", 
            mme_ue->imsi_bcd);

    rv = nas_security_encode(&emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && emmbuf, return, "emm build error");

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_emm_status(mme_ue_t *mme_ue, nas_emm_status_t *emm_status)
{
    d_assert(mme_ue, return, "Null param");

    d_warn("[NAS] EMM status(%d) : UE[%s] --> EMM", 
            emm_status->emm_cause, mme_ue->imsi_bcd);
}

void emm_handle_detach_request(
        mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s11buf = NULL, *s1apbuf = NULL;
    mme_sess_t *sess;

    nas_message_t message;
    nas_detach_type_t *detach_type = &detach_request->detach_type;

    /* FIXME: nas_key_set_identifier is ignored
     * detach_type->tsc
     * detach_type->nas_key_set_identifier 
     */

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    /* Encode ue->s1ap.cause for later use */
    enb_ue->s1ap.cause.present = S1ap_Cause_PR_nas;
    enb_ue->s1ap.cause.choice.nas = S1ap_CauseNas_detach;

    switch (detach_type->detach_type)
    {
        /* 0 0 1 : EPS detach */
        case NAS_DETACH_TYPE_FROM_UE_EPS_DETACH: 
            d_info("[NAS] (EPS) Detach request : UE_IMSI[%s] --> EMM", 
                    mme_ue->imsi_bcd);
            break;
        /* 0 1 0 : IMSI detach */
        case NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
            d_info("[NAS] (IMSI) Detach request : UE_IMSI[%s] --> EMM", 
                    mme_ue->imsi_bcd);
            break;
        case 6: /* 1 1 0 : reserved */
        case 7: /* 1 1 1 : reserved */
            d_info("[NAS] (Unknown) Detach request : UE_IMSI[%s] --> EMM", 
                    mme_ue->imsi_bcd);
            break;
        /* 0 1 1 : combined EPS/IMSI detach */
        case NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH: 
        default: /* all other values */
            d_info("[NAS] (EPS+IMSI) Detach request : UE_IMSI[%s] --> EMM", 
                    mme_ue->imsi_bcd);
            break;
    }
    
    sess = mme_sess_first(mme_ue);
    while (sess != NULL)
    {
        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);

        if (bearer != NULL)
        {
            rv = mme_s11_build_delete_session_request(&s11buf, sess);
            d_assert(rv == CORE_OK, return, "S11 build error");

            rv = mme_s11_send_to_sgw(bearer->sgw, 
                    GTP_DELETE_SESSION_REQUEST_TYPE, sess->sgw_s11_teid, 
                    s11buf);
            d_assert(rv == CORE_OK, return, "S11 send error");
        }
        sess = mme_sess_next(sess);
    }

    if ((detach_type->switch_off & 0x1) == 0)
    {
        /* reply with detach accept */
        memset(&message, 0, sizeof(message));
        message.h.security_header_type = 
            NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

        message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
        message.emm.h.message_type = NAS_DETACH_ACCEPT;

        d_info("[NAS] Detach accept : UE[%s] <-- EMM", 
            mme_ue->imsi_bcd);

        rv = nas_security_encode(&emmbuf, mme_ue, &message);
        d_assert(rv == CORE_OK && emmbuf, return, "emm build error");

        rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, emmbuf);
        d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return, "s1ap build error");

        d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
    }

    /* TODO: initiate s1 ue context release with a timeout value */
}

void emm_handle_delete_session_response(mme_bearer_t *bearer)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    mme_enb_t *enb = NULL;
    pkbuf_t *s1apbuf = NULL;
    mme_sess_t *sess;
    int b_wait = 0;
    enb_ue_t *enb_ue = NULL;

    d_assert(bearer, return, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    sess = mme_sess_find_by_ebi(mme_ue, bearer->ebi);
    mme_sess_remove(sess);

    /* sess and bearer are not valid from here */

    sess = mme_sess_first(mme_ue);
    while (sess != NULL)
    {
        mme_bearer_t *temp_bearer = mme_default_bearer_in_sess(sess);

        if (temp_bearer != NULL)
        {
            b_wait = 1;
            break;
        }
        sess = mme_sess_next(sess);
    }

    if (!b_wait)
    {
        d_info("[NAS] Detach done : UE[%s] <-- EMM", mme_ue->imsi_bcd);

        rv = s1ap_build_ue_context_release_commmand(
                &s1apbuf, enb_ue, &enb_ue->s1ap.cause);
        d_assert(rv == CORE_OK && s1apbuf, return, "s1ap build error");

        d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");

        /* TODO: launch a timer */
    }
}

void emm_handle_service_request(
        mme_ue_t *mme_ue, nas_service_request_t *service_request)
{
    /* TODO */
}

mme_ue_t *emm_find_ue_by_message(enb_ue_t *enb_ue, nas_message_t *message)
{
    mme_ue_t *mme_ue = NULL;

    switch(message->emm.h.message_type)
    {
        case NAS_ATTACH_REQUEST:
        {
            nas_attach_request_t *attach_request = 
                &message->emm.attach_request;

            nas_eps_mobile_identity_t *eps_mobile_identity =
                            &attach_request->eps_mobile_identity;

            switch(eps_mobile_identity->imsi.type)
            {
                case NAS_EPS_MOBILE_IDENTITY_IMSI:
                {
                    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

                    nas_imsi_to_bcd(
                        &eps_mobile_identity->imsi, eps_mobile_identity->length,
                        imsi_bcd);

                    d_trace(3,"Search mme_ue by UE_IMSI[%s]\n", imsi_bcd);

                    mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);

                    break;
                }
                case NAS_EPS_MOBILE_IDENTITY_GUTI:
                {
                    nas_eps_mobile_identity_guti_t *nas_guti = NULL;
                    nas_guti = &eps_mobile_identity->guti;
                    guti_t guti;

                    guti.plmn_id = nas_guti->plmn_id;
                    guti.mme_gid = nas_guti->mme_gid;
                    guti.mme_code = nas_guti->mme_code;
                    guti.m_tmsi = nas_guti->m_tmsi;

                    d_trace(3,"Search mme_ue by GUTI[G:%d,C:%d,M_TMSI:0x%x]\n",
                            guti.mme_gid,
                            guti.mme_code,
                            guti.m_tmsi);

                    mme_ue = mme_ue_find_by_guti(&guti);
                    break;
                }
                default:
                {
                    printf("Uknown message imsi type =%d\n",
                        eps_mobile_identity->imsi.type);
                    break;
                }
            }
            break;
        }
        case NAS_DETACH_REQUEST:
        {
            /* TODO */
            break;
        }
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
        {
            /* TODO */
            break;
        }
        default:
        {
            break;
        }
    }

    return mme_ue;
}
