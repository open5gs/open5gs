#define TRACE_MODULE _emm_handler

#include "core_debug.h"
#include "core_lib.h"

#include "nas_message.h"

#include "mme_event.h"

#include "mme_kdf.h"
#include "nas_security.h"
#include "nas_conv.h"

#include "s1ap_path.h"
#include "nas_path.h"
#include "mme_fd_path.h"
#include "mme_gtp_path.h"

#include "emm_handler.h"

void emm_handle_attach_request(
        mme_ue_t *mme_ue, nas_attach_request_t *attach_request)
{
    status_t rv;

    enb_ue_t *enb_ue = NULL;
    nas_eps_attach_type_t *eps_attach_type =
                    &attach_request->eps_attach_type;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;
    nas_esm_message_container_t *esm_message_container =
                    &attach_request->esm_message_container;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    d_assert(esm_message_container, return, "Null param");
    d_assert(esm_message_container->length, return, "Null param");

    /*
     * ATTACH_REQUEST
     *   Clear EBI generator
     *   Clear Paging Timer and Message
     *   Update KeNB
     *
     * TAU_REQUEST
     *   Clear Paging Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_EPS_BEARER_ID(mme_ue);
    CLEAR_PAGING_INFO(mme_ue);
    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);

    /* Set EPS Attach Type */
    memcpy(&mme_ue->nas_eps.attach, eps_attach_type,
            sizeof(nas_eps_attach_type_t));
    mme_ue->nas_eps.type = MME_UE_EPS_ATTACH_TYPE;

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

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->e_cgi, sizeof(e_cgi_t));

    switch(eps_mobile_identity->imsi.type)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

            nas_imsi_to_bcd(
                &eps_mobile_identity->imsi, eps_mobile_identity->length,
                imsi_bcd);
            mme_ue_set_imsi(mme_ue, imsi_bcd);

            d_trace(3, "[NAS] Attach request : IMSI[%s] --> EMM\n", imsi_bcd);

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

            d_trace(3, "[NAS] Attach request : GUTI[G:%d,C:%d,M_TMSI:0x%x]-"
                    "IMSI:[%s] --> EMM\n", 
                    guti.mme_gid,
                    guti.mme_code,
                    guti.m_tmsi,
                    MME_UE_HAVE_IMSI(mme_ue) 
                        ? mme_ue->imsi_bcd : "Unknown");

            break;
        }
        default:
        {
            d_warn("Not implemented(type:%d)", 
                    eps_mobile_identity->imsi.type);
            
            break;
        }
    }

    NAS_STORE_DATA(&mme_ue->pdn_connectivity_request, esm_message_container);

    if (!MME_UE_HAVE_IMSI(mme_ue))
    {
        FSM_TRAN(&mme_ue->sm, &emm_state_identity);
    }
    else
    {
        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        {
            rv = nas_send_emm_to_esm(mme_ue, &mme_ue->pdn_connectivity_request);
            d_assert(rv == CORE_OK,, "nas_send_emm_to_esm failed");
            FSM_TRAN(&mme_ue->sm, &emm_state_default_esm);
        }
        else
        {
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                rv = mme_gtp_send_delete_all_sessions(mme_ue);
                d_assert(rv == CORE_OK, return, "gtp send failed");
            }
            else
            {
                mme_s6a_send_air(mme_ue);
            }
            FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
        }
    }
}

void emm_handle_attach_complete(
    mme_ue_t *mme_ue, nas_attach_complete_t *attach_complete)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    nas_message_t message;
    nas_emm_information_t *emm_information = &message.emm.emm_information;
    nas_time_zone_and_time_t *universal_time_and_local_time_zone =
        &emm_information->universal_time_and_local_time_zone;
    nas_daylight_saving_time_t *network_daylight_saving_time = 
        &emm_information->network_daylight_saving_time;

    time_exp_t time_exp;
    time_exp_lt(&time_exp, time_now());

    d_assert(mme_ue, return, "Null param");

    rv = nas_send_emm_to_esm(mme_ue, &attach_complete->esm_message_container);
    d_assert(rv == CORE_OK, return, "nas_send_emm_to_esm failed");

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

    d_trace(3, "[NAS] EMM information : UE[%s] <-- EMM\n", 
            mme_ue->imsi_bcd);

    rv = nas_security_encode(&emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && emmbuf, return, "emm build error");
    d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);
}

void emm_handle_identity_response(
        mme_ue_t *mme_ue, nas_identity_response_t *identity_response)
{
    status_t rv;

    nas_mobile_identity_t *mobile_identity = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(identity_response, return, "Null param");

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    mobile_identity = &identity_response->mobile_identity;

    if (mobile_identity->imsi.type == NAS_IDENTITY_TYPE_2_IMSI)
    {
        c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

        nas_imsi_to_bcd(
            &mobile_identity->imsi, mobile_identity->length, imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        d_assert(mme_ue->imsi_len, return,
                "Can't get IMSI(len:%d\n", mme_ue->imsi_len);
    }
    else
    {
        d_warn("Not supported Identity type(%d)", mobile_identity->imsi.type);
    }

    d_assert(MME_UE_HAVE_IMSI(mme_ue), return, "No IMSI in IDENTITY_RESPONSE");

    if (mme_ue->nas_eps.type == MME_UE_EPS_ATTACH_TYPE) /* ATTACH_REQUEST */
    {
        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        {
            rv = nas_send_emm_to_esm(mme_ue, &mme_ue->pdn_connectivity_request);
            d_assert(rv == CORE_OK, return, "nas_send_emm_to_esm failed");
            FSM_TRAN(&mme_ue->sm, &emm_state_default_esm);
        }
        else
        {
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                rv = mme_gtp_send_delete_all_sessions(mme_ue);
                d_assert(rv == CORE_OK, return, "gtp send failed");
            }
            else
            {
                mme_s6a_send_air(mme_ue);
            }
            FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
        }
    }
    else if (mme_ue->nas_eps.type == MME_UE_EPS_UPDATE_TYPE) /* TAU_REQUEST */
    {
        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        {
            /* Send TAU accept */
            rv = nas_send_tau_accept(mme_ue);
            d_assert(rv == CORE_OK, return, "nas_send_tau_accept failed");
        }
        else
        {
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                mme_s6a_send_air(mme_ue);
                FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
            }
            else
            {
                /* Send TAU reject */
                nas_send_tau_reject(mme_ue, 
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                FSM_TRAN(&mme_ue->sm, &emm_state_detached);
            }
        }
    }
}

void emm_handle_authentication_response(mme_ue_t *mme_ue, 
        nas_authentication_response_t *authentication_response)
{
    nas_authentication_response_parameter_t *authentication_response_parameter =
        &authentication_response->authentication_response_parameter;

    d_assert(mme_ue, return, "Null param");

    if (authentication_response_parameter->length != mme_ue->xres_len ||
        memcmp(authentication_response_parameter->res,
            mme_ue->xres, mme_ue->xres_len) != 0)
    {
        status_t rv;

        d_error("authentication failed");

        rv = nas_send_authentication_reject(mme_ue);
        d_assert(rv == CORE_OK,, "nas send error");
        FSM_TRAN(&mme_ue->sm, &emm_state_detached);
    }
    else
    {
        FSM_TRAN(&mme_ue->sm, &emm_state_security_mode);
    }
}

void emm_handle_detach_request(
        mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request)
{
    status_t rv;
    enb_ue_t *enb_ue = NULL;

    d_assert(detach_request, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    d_trace(3, "[NAS] Detach request : UE_IMSI[%s] --> EMM\n", 
            mme_ue->imsi_bcd);

    switch (detach_request->detach_type.detach_type)
    {
        /* 0 0 1 : EPS detach */
        case NAS_DETACH_TYPE_FROM_UE_EPS_DETACH: 
            break;
        /* 0 1 0 : IMSI detach */
        case NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
            break;
        case 6: /* 1 1 0 : reserved */
        case 7: /* 1 1 1 : reserved */
            d_warn("[NAS] (Unknown) Detach request : UE_IMSI[%s] --> EMM", 
                    mme_ue->imsi_bcd);
            break;
        /* 0 1 1 : combined EPS/IMSI detach */
        case NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH: 
        default: /* all other values */
            break;
    }

    /* Save detach type */
    mme_ue->detach_type = detach_request->detach_type;

    if (MME_HAVE_SGW_S11_PATH(mme_ue))
    {
        rv = mme_gtp_send_delete_all_sessions(mme_ue);
        d_assert(rv == CORE_OK, return,
            "mme_gtp_send_delete_all_sessions failed");
    }
    else
    {
        rv = nas_send_detach_accept(mme_ue);
        d_assert(rv == CORE_OK, return,
            "nas_send_detach_accept failed");
    }
}

void emm_handle_service_request(
        mme_ue_t *mme_ue, nas_service_request_t *service_request)
{
    status_t rv;

    d_assert(mme_ue, return, "Null param");

    /*
     * ATTACH_REQUEST
     *   Clear EBI generator
     *   Clear Paging Timer and Message
     *   Update KeNB
     *
     * TAU_REQUEST
     *   Clear Paging Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_PAGING_INFO(mme_ue);
    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);

    /* Set EPS Update Type */
    mme_ue->nas_eps.type = MME_UE_EPS_UPDATE_TYPE;

    if (!MME_UE_HAVE_IMSI(mme_ue))
    {
        FSM_TRAN(&mme_ue->sm, &emm_state_identity);
    }
    else
    {
        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        {
            /* Send Initial Context Setup Request */
            rv = s1ap_send_initial_context_setup_request(mme_ue);
            d_assert(rv == CORE_OK, return, "s1ap send error");
        }
        else
        {
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                /* Re-authentication */
                mme_s6a_send_air(mme_ue);
                FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
            }
            else
            {
                /* Send Service Reject */
                nas_send_service_reject(mme_ue, 
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                FSM_TRAN(&mme_ue->sm, &emm_state_detached);
            }
        }
    }
}

void emm_handle_emm_status(mme_ue_t *mme_ue, nas_emm_status_t *emm_status)
{
    d_assert(mme_ue, return, "Null param");

    d_warn("[NAS] EMM status(%d) : UE[%s] --> EMM", 
            emm_status->emm_cause, mme_ue->imsi_bcd);
}

void emm_handle_tau_request(
        mme_ue_t *mme_ue, nas_tracking_area_update_request_t *tau_request)
{
    status_t rv;

    nas_eps_update_type_t *eps_update_type =
                    &tau_request->eps_update_type;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &tau_request->old_guti;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    /*
     * ATTACH_REQUEST
     *   Clear EBI generator
     *   Clear Paging Timer and Message
     *   Update KeNB
     *
     * TAU_REQUEST
     *   Clear Paging Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_PAGING_INFO(mme_ue);

    /* Set EPS Update Type */
    memcpy(&mme_ue->nas_eps.update, eps_update_type,
            sizeof(nas_eps_update_type_t));
    mme_ue->nas_eps.type = MME_UE_EPS_UPDATE_TYPE;

    /* Store UE specific information */
    if (tau_request->presencemask &
        NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
    {
        nas_tracking_area_identity_t *last_visited_registered_tai = 
            &tau_request->last_visited_registered_tai;

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

    if (tau_request->presencemask &
            NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT)
    {
        memcpy(&mme_ue->ue_network_capability, 
                &tau_request->ue_network_capability,
                sizeof(tau_request->ue_network_capability));
    }

    if (tau_request->presencemask &
            NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT)
    {
        memcpy(&mme_ue->ms_network_capability, 
                &tau_request->ms_network_capability,
                sizeof(tau_request->ms_network_capability));
    }

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->e_cgi, sizeof(e_cgi_t));

    /* TODO: 
     *   1) Consider if MME is changed or not.
     *   2) Consider if SGW is changed or not.
     */
    switch(eps_mobile_identity->imsi.type)
    {
        case NAS_EPS_MOBILE_IDENTITY_GUTI:
        {
            nas_eps_mobile_identity_guti_t *nas_guti = NULL;
            nas_guti = &eps_mobile_identity->guti;
            guti_t guti;

            guti.plmn_id = nas_guti->plmn_id;
            guti.mme_gid = nas_guti->mme_gid;
            guti.mme_code = nas_guti->mme_code;
            guti.m_tmsi = nas_guti->m_tmsi;

            d_trace(3, "[NAS] TAU request : GUTI[G:%d,C:%d,M_TMSI:0x%x]-"
                    "IMSI:[%s] --> EMM\n", 
                    guti.mme_gid,
                    guti.mme_code,
                    guti.m_tmsi,
                    MME_UE_HAVE_IMSI(mme_ue) 
                        ? mme_ue->imsi_bcd : "Unknown");
            break;
        }
        default:
        {
            d_warn("Not implemented(type:%d)", 
                    eps_mobile_identity->imsi.type);
            
            return;
        }
    }

    if (!MME_UE_HAVE_IMSI(mme_ue))
    {
        FSM_TRAN(&mme_ue->sm, &emm_state_identity);
    }
    else
    {
        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
        {
            /* Send TAU Accept */
            rv = nas_send_tau_accept(mme_ue);
            d_assert(rv == CORE_OK, return, "nas_send_tau_accept failed");
        }
        else
        {
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                /* Re-authentication */
                mme_s6a_send_air(mme_ue);
                FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
            }
            else
            {
                /* Send TAU reject */
                nas_send_tau_reject(mme_ue, 
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                FSM_TRAN(&mme_ue->sm, &emm_state_detached);
            }
        }
    }
}
