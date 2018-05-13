#define TRACE_MODULE _emm_handler

#include "core_debug.h"
#include "core_lib.h"

#include "nas/nas_message.h"

#include "mme_event.h"

#include "mme_kdf.h"
#include "nas_security.h"
#include "nas_conv.h"

#include "s1ap_path.h"
#include "nas_path.h"
#include "mme_fd_path.h"
#include "mme_gtp_path.h"

#include "emm_handler.h"

status_t emm_handle_attach_request(
        mme_ue_t *mme_ue, nas_attach_request_t *attach_request)
{
    int served_tai_index = 0;

    enb_ue_t *enb_ue = NULL;
    nas_eps_attach_type_t *eps_attach_type =
                    &attach_request->eps_attach_type;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;
    nas_esm_message_container_t *esm_message_container =
                    &attach_request->esm_message_container;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    d_assert(esm_message_container, return CORE_ERROR, "Null param");
    d_assert(esm_message_container->length, return CORE_ERROR, "Null param");

    /* Set EPS Attach Type */
    memcpy(&mme_ue->nas_eps.attach, eps_attach_type,
            sizeof(nas_eps_attach_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_ue->nas_eps.ksi = eps_attach_type->nas_key_set_identifier;
    d_trace(5, "    NAS_EPS TYPE[%d] KSI[%d] ATTACH[0x%x]\n",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi, mme_ue->nas_eps.data);
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
    {
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    d_trace(5, "    OLD TAI[PLMN_ID:0x%x,TAC:%d]\n",
            mme_ue->tai.plmn_id, mme_ue->tai.tac);
    d_trace(5, "    OLD E_CGI[PLMN_ID:0x%x,CELL_ID:%d]\n",
            mme_ue->e_cgi.plmn_id, mme_ue->e_cgi.cell_id);
    d_trace(5, "    TAI[PLMN_ID:0x%x,TAC:%d]\n",
            enb_ue->nas.tai.plmn_id, enb_ue->nas.tai.tac);
    d_trace(5, "    E_CGI[PLMN_ID:0x%x,CELL_ID:%d]\n",
            enb_ue->nas.e_cgi.plmn_id, enb_ue->nas.e_cgi.cell_id);

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->nas.tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->nas.e_cgi, sizeof(e_cgi_t));

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0)
    {
        /* Send Attach Reject */
        d_warn("Cannot find Served TAI[PLMN_ID:0x%x,TAC:%d]",
            mme_ue->tai.plmn_id, mme_ue->tai.tac);
        nas_send_attach_reject(mme_ue,
            EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED,
            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        return CORE_ERROR;
    }
    d_trace(5, "    SERVED_TAI_INDEX[%d]\n", served_tai_index);

    /* Store UE specific information */
    if (attach_request->presencemask &
        NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
    {
        nas_tracking_area_identity_t *last_visited_registered_tai = 
            &attach_request->last_visited_registered_tai;

        memcpy(&mme_ue->visited_plmn_id, 
                &last_visited_registered_tai->plmn_id, PLMN_ID_LEN);
    }
    else
    {
        memcpy(&mme_ue->visited_plmn_id, &mme_ue->tai.plmn_id, PLMN_ID_LEN);
    }

    memcpy(&mme_ue->ue_network_capability, 
            &attach_request->ue_network_capability,
            sizeof(attach_request->ue_network_capability));

    if (attach_request->presencemask &
            NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT)
    {
        memcpy(&mme_ue->ms_network_capability, 
                &attach_request->ms_network_capability,
                sizeof(attach_request->ms_network_capability));
    }

    switch(eps_mobile_identity->imsi.type)
    {
        case NAS_EPS_MOBILE_IDENTITY_IMSI:
        {
            c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

            nas_imsi_to_bcd(
                &eps_mobile_identity->imsi, eps_mobile_identity->length,
                imsi_bcd);
            mme_ue_set_imsi(mme_ue, imsi_bcd);

            d_trace(5, "    IMSI[%s]\n", imsi_bcd);

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

            d_trace(5, "    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]\n",
                    guti.mme_gid,
                    guti.mme_code,
                    guti.m_tmsi,
                    MME_UE_HAVE_IMSI(mme_ue) 
                        ? mme_ue->imsi_bcd : "Unknown");
            break;
        }
        default:
        {
            d_warn("Not implemented[%d]", eps_mobile_identity->imsi.type);
            break;
        }
    }

    NAS_STORE_DATA(&mme_ue->pdn_connectivity_request, esm_message_container);

    return CORE_OK;
}

status_t emm_handle_attach_complete(
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

    time_exp_t xt_gmt, xt_local;

    rv = time_exp_gmt(&xt_gmt, time_now());
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = time_exp_lt(&xt_local, time_now());
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(5, "    GMT Time[Y:M:D H:M:S GMT] - %d:%d:%d, %d:%d:%d, %d\n",
        xt_gmt.tm_year, xt_gmt.tm_mon, xt_gmt.tm_mday,
        xt_gmt.tm_hour, xt_gmt.tm_min, xt_gmt.tm_sec,
        xt_gmt.tm_gmtoff);
    d_trace(5, "    LOCAL Time[Y:M:D H:M:S GMT] - %d:%d:%d, %d:%d:%d, %d\n",
        xt_local.tm_year, xt_local.tm_mon, xt_local.tm_mday,
        xt_local.tm_hour, xt_local.tm_min, xt_local.tm_sec,
        xt_local.tm_gmtoff);

    rv = nas_send_emm_to_esm(mme_ue, &attach_complete->esm_message_container);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas_send_emm_to_esm failed");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_EMM_INFORMATION;

    emm_information->presencemask |=
        NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
    universal_time_and_local_time_zone->year = 
                NAS_TIME_TO_BCD(xt_gmt.tm_year % 100);
    universal_time_and_local_time_zone->mon =
                NAS_TIME_TO_BCD(xt_gmt.tm_mon+1);
    universal_time_and_local_time_zone->mday = 
                NAS_TIME_TO_BCD(xt_gmt.tm_mday);
    universal_time_and_local_time_zone->hour = 
                NAS_TIME_TO_BCD(xt_gmt.tm_hour);
    universal_time_and_local_time_zone->min = NAS_TIME_TO_BCD(xt_gmt.tm_min);
    universal_time_and_local_time_zone->sec = NAS_TIME_TO_BCD(xt_gmt.tm_sec);
    if (xt_local.tm_gmtoff >= 0)
        universal_time_and_local_time_zone->sign = 0;
    else
        universal_time_and_local_time_zone->sign = 1;
    /* quarters of an hour */
    universal_time_and_local_time_zone->gmtoff = 
                NAS_TIME_TO_BCD(xt_local.tm_gmtoff / 900);

    emm_information->presencemask |=
        NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
    network_daylight_saving_time->length = 1;

    if(mme_self()->full_name.length) 
    {
        emm_information->presencemask |=
            NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT;
        memcpy(&emm_information->full_name_for_network,
            &mme_self()->full_name, sizeof(nas_network_name_t));
    }
    
    if(mme_self()->short_name.length)
    {
        emm_information->presencemask |=
            NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT;
        memcpy(&emm_information->short_name_for_network,
            &mme_self()->short_name, sizeof(nas_network_name_t));
    }                

    rv = nas_security_encode(&emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && emmbuf, return CORE_ERROR, "emm build error");
    d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);

    d_trace(3, "[EMM] EMM information\n");
    d_trace(5, "    IMSI[%s]\n", mme_ue->imsi_bcd);

    return CORE_OK;
}

status_t emm_handle_identity_response(
        mme_ue_t *mme_ue, nas_identity_response_t *identity_response)
{
    nas_mobile_identity_t *mobile_identity = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(identity_response, return CORE_ERROR, "Null param");

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    mobile_identity = &identity_response->mobile_identity;

    if (mobile_identity->imsi.type == NAS_IDENTITY_TYPE_2_IMSI)
    {
        c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

        nas_imsi_to_bcd(
            &mobile_identity->imsi, mobile_identity->length, imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        d_assert(mme_ue->imsi_len, return CORE_ERROR,
                "Can't get IMSI : [LEN:%d]\n", mme_ue->imsi_len);
    }
    else
    {
        d_warn("Not supported Identity type[%d]", mobile_identity->imsi.type);
    }

    return CORE_OK;
}

status_t emm_handle_detach_request(
        mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request)
{
    nas_detach_type_t *detach_type = NULL;

    d_assert(detach_request, return CORE_ERROR, "Null param");
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    detach_type = &detach_request->detach_type;

    /* Set EPS Attach Type */
    memcpy(&mme_ue->nas_eps.detach, detach_type, sizeof(nas_detach_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_FROM_UE;
    mme_ue->nas_eps.ksi = detach_type->nas_key_set_identifier;
    d_trace(5, "    NAS_EPS TYPE[%d] KSI[%d] DETACH[0x%x]\n",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi, mme_ue->nas_eps.data);

    switch (detach_request->detach_type.detach_type)
    {
        /* 0 0 1 : EPS detach */
        case NAS_DETACH_TYPE_FROM_UE_EPS_DETACH: 
            d_trace(5, "    EPS Detach\n");
            break;
        /* 0 1 0 : IMSI detach */
        case NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
            d_trace(5, "    IMSI Detach\n");
            break;
        case 6: /* 1 1 0 : reserved */
        case 7: /* 1 1 1 : reserved */
            d_warn("Unknown Detach type[%d]",
                detach_request->detach_type.detach_type);
            break;
        /* 0 1 1 : combined EPS/IMSI detach */
        case NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH: 
            d_trace(5, "    Combined EPS/IMSI Detach\n");
        default: /* all other values */
            break;
    }
    if (detach_request->detach_type.switch_off)
        d_trace(5, "    Switch-Off\n");

    return CORE_OK;
}

status_t emm_handle_service_request(
        mme_ue_t *mme_ue, nas_service_request_t *service_request)
{
    nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
                    &service_request->ksi_and_sequence_number;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    /* Set EPS Update Type */
    mme_ue->nas_eps.type = MME_EPS_TYPE_SERVICE_REQUEST;
    mme_ue->nas_eps.ksi = ksi_and_sequence_number->ksi;
    d_trace(5, "    NAS_EPS TYPE[%d] KSI[%d]\n",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi);

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
    {
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    d_trace(5, "    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]\n",
            mme_ue->guti.mme_gid,
            mme_ue->guti.mme_code,
            mme_ue->guti.m_tmsi,
            MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown");

    return CORE_OK;
}

status_t emm_handle_tau_request(
        mme_ue_t *mme_ue, nas_tracking_area_update_request_t *tau_request)
{
    int served_tai_index = 0;

    nas_eps_update_type_t *eps_update_type =
                    &tau_request->eps_update_type;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &tau_request->old_guti;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    /* Set EPS Update Type */
    memcpy(&mme_ue->nas_eps.update, eps_update_type,
            sizeof(nas_eps_update_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_TAU_REQUEST;
    mme_ue->nas_eps.ksi = eps_update_type->nas_key_set_identifier;
    d_trace(5, "    NAS_EPS TYPE[%d] KSI[%d] UPDATE[0x%x]\n",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi,
            mme_ue->nas_eps.data);
    
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

    if (BEARER_CONTEXT_IS_ACTIVE(mme_ue))
        d_trace(5, "    Bearer-Active\n");
    else
        d_trace(5, "    Bearer-Inactive\n");

    if (mme_ue->nas_eps.update.active_flag)
        d_trace(5, "    Active flag\n");
    else
        d_trace(5, "    No Active flag\n");

    d_trace(5, "    OLD TAI[PLMN_ID:0x%x,TAC:%d]\n",
            mme_ue->tai.plmn_id, mme_ue->tai.tac);
    d_trace(5, "    OLD E_CGI[PLMN_ID:0x%x,CELL_ID:%d]\n",
            mme_ue->e_cgi.plmn_id, mme_ue->e_cgi.cell_id);
    d_trace(5, "    TAI[PLMN_ID:0x%x,TAC:%d]\n",
            enb_ue->nas.tai.plmn_id, enb_ue->nas.tai.tac);
    d_trace(5, "    E_CGI[PLMN_ID:0x%x,CELL_ID:%d]\n",
            enb_ue->nas.e_cgi.plmn_id, enb_ue->nas.e_cgi.cell_id);

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->nas.tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->nas.e_cgi, sizeof(e_cgi_t));

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0)
    {
        /* Send TAU reject */
        d_warn("Cannot find Served TAI[PLMN_ID:0x%x,TAC:%d]",
            mme_ue->tai.plmn_id, mme_ue->tai.tac);
        nas_send_tau_reject(mme_ue, EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return CORE_ERROR;
    }
    d_trace(5, "    SERVED_TAI_INDEX[%d]\n", served_tai_index);

    /* Store UE specific information */
    if (tau_request->presencemask &
        NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
    {
        nas_tracking_area_identity_t *last_visited_registered_tai = 
            &tau_request->last_visited_registered_tai;

        memcpy(&mme_ue->visited_plmn_id, 
                &last_visited_registered_tai->plmn_id, PLMN_ID_LEN);
    }
    else
    {
        memcpy(&mme_ue->visited_plmn_id, &mme_ue->tai.plmn_id, PLMN_ID_LEN);
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

            d_trace(5, "    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]\n",
                    guti.mme_gid,
                    guti.mme_code,
                    guti.m_tmsi,
                    MME_UE_HAVE_IMSI(mme_ue) 
                        ? mme_ue->imsi_bcd : "Unknown");
            break;
        }
        default:
        {
            d_warn("Not implemented[%d]", 
                    eps_mobile_identity->imsi.type);
            
            return CORE_OK;
        }
    }

    return CORE_OK;
}
