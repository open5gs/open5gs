#define TRACE_MODULE _emm_handler

#include "core_debug.h"
#include "core_lib.h"

#include "nas_message.h"

#include "mme_event.h"

#include "mme_kdf.h"
#include "nas_security.h"
#include "nas_conv.h"
#include "esm_build.h"
#include "emm_build.h"
#include "s1ap_build.h"
#include "s1ap_path.h"
#include "nas_path.h"

#include "mme_s11_build.h"
#include "mme_gtp_path.h"

#include "emm_handler.h"

void emm_handle_attach_request(
        mme_ue_t *mme_ue, nas_attach_request_t *attach_request)
{
    enb_ue_t *enb_ue = NULL;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;
    nas_esm_message_container_t *esm_message_container =
                    &attach_request->esm_message_container;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    d_assert(esm_message_container, return, "Null param");
    d_assert(esm_message_container->length, return, "Null param");

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
            &mobile_identity->imsi, mobile_identity->length,
            imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        d_assert(mme_ue->imsi_len, return,
                "Can't get IMSI(len:%d\n", mme_ue->imsi_len);
    }
    else
    {
        d_warn("Not supported Identity type(%d)", mobile_identity->imsi.type);
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
        d_error("authentication failed");
        return;
    }

    d_trace(3, "[NAS] Authentication response : UE[%s] --> EMM\n", 
            mme_ue->imsi_bcd);
}

void emm_handle_detach_request(
        mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request)
{
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
}

void emm_handle_detach_accept(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    nas_message_t message;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;
    S1ap_Cause_t cause;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    /* reply with detach accept */
    if ((mme_ue->detach_type.switch_off & 0x1) == 0)
    {
        memset(&message, 0, sizeof(message));
        message.h.security_header_type = 
            NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

        message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
        message.emm.h.message_type = NAS_DETACH_ACCEPT;

        d_trace(3, "[NAS] Detach accept : UE[%s] <-- EMM\n", 
            mme_ue->imsi_bcd);

        rv = nas_security_encode(&emmbuf, mme_ue, &message);
        d_assert(rv == CORE_OK && emmbuf, return, "emm build error");
        d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);
    }

    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = S1ap_CauseNas_detach;

    rv = s1ap_build_ue_context_release_commmand(&s1apbuf, enb_ue, &cause);
    d_assert(rv == CORE_OK && s1apbuf, return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_service_request(
        mme_ue_t *mme_ue, nas_service_request_t *service_request)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    sess = mme_sess_first(mme_ue);
    d_assert(sess, return, "Null param");

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, sess, NULL);
    d_assert(rv == CORE_OK && s1apbuf, return, "s1ap build error");

    d_assert(nas_send_to_enb(enb_ue, s1apbuf) == CORE_OK,, "s1ap send error");
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
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &tau_request->old_guti;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

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

            if (!MME_UE_HAVE_IMSI(mme_ue))
            {
                /* Unknown GUTI */

                /* FIXME : Need to check if GUTI is allocated to old MME.
                 * if so , transmit context request to get the context of ue.
                 */

                /* Send TAU reject */
                emm_handle_tau_reject(mme_ue, 
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            }
            else if (!SECURITY_CONTEXT_IS_VALID(mme_ue))
            {
                /* Need Authencation */
                d_warn("Need Authenticatoin");
            }
            else
            {
                /* Send TAU accept */
                emm_handle_tau_accept(mme_ue);
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

void emm_handle_tau_accept(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;
    S1ap_Cause_t cause;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    /* Build TAU accept */
    if (emm_build_tau_accept(&emmbuf, mme_ue) != CORE_OK)
    {
        d_error("emm_build_tau_accept error");
        pkbuf_free(emmbuf);
        return;
    }

    /* Send Dl NAS to UE */
    d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);


    /* FIXME : delay required before sending UE context release to make sure 
     * that UE receive DL NAS ? */
    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = S1ap_CauseNas_normal_release;

    rv = s1ap_build_ue_context_release_commmand(&s1apbuf, enb_ue, &cause);
    d_assert(rv == CORE_OK && s1apbuf, return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void emm_handle_tau_reject(mme_ue_t *mme_ue, nas_emm_cause_t emm_cause)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL, *s1apbuf = NULL;
    S1ap_Cause_t cause;

    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    /* Build TAU reject */
    if (emm_build_tau_reject(&emmbuf, emm_cause, mme_ue) != CORE_OK)
    {
        d_error("emm_build_tau_accept error");
        pkbuf_free(emmbuf);
        return;
    }

    /* Send Dl NAS to UE */
    d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);


    /* FIXME : delay required before sending UE context release to make sure 
     * that UE receive DL NAS ? */
    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = S1ap_CauseNas_normal_release;

    rv = s1ap_build_ue_context_release_commmand(&s1apbuf, enb_ue, &cause);
    d_assert(rv == CORE_OK && s1apbuf, return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}
