/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "nas/nas-message.h"
#include "mme-event.h"
#include "mme-sm.h"

#include "mme-kdf.h"
#include "nas-security.h"
#include "nas-conv.h"

#include "s1ap-path.h"
#include "nas-path.h"
#include "mme-fd-path.h"
#include "mme-gtp-path.h"

#include "emm-handler.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __emm_log_domain

int emm_handle_attach_request(
        mme_ue_t *mme_ue, nas_attach_request_t *attach_request)
{
    int served_tai_index = 0;

    nas_eps_mobile_identity_guti_t *eps_mobile_identity_guti = NULL;
    nas_guti_t nas_guti;

    enb_ue_t *enb_ue = NULL;
    nas_eps_attach_type_t *eps_attach_type =
                    &attach_request->eps_attach_type;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;
    nas_esm_message_container_t *esm_message_container =
                    &attach_request->esm_message_container;

    char imsi_bcd[MAX_IMSI_BCD_LEN+1];

    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    ogs_assert(esm_message_container);
    ogs_assert(esm_message_container->length);

    /* Set EPS Attach Type */
    memcpy(&mme_ue->nas_eps.attach, eps_attach_type,
            sizeof(nas_eps_attach_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_ue->nas_eps.ksi = eps_attach_type->nas_key_set_identifier;
    ogs_debug("    NAS_EPS TYPE[%d] KSI[%d] ATTACH[0x%x]",
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
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_EPS_BEARER_ID(mme_ue);
    CLEAR_PAGING_INFO(mme_ue);
    CLEAR_SERVICE_INDICATOR(mme_ue);
    if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&mme_ue->e_cgi.plmn_id), mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&enb_ue->saved.tai.plmn_id), enb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(e_cgi_t));

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0) {
        /* Send Attach Reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
        nas_send_attach_reject(mme_ue,
            EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED,
            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    /* Store UE specific information */
    if (attach_request->presencemask &
        NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        nas_tracking_area_identity_t *last_visited_registered_tai = 
            &attach_request->last_visited_registered_tai;

        nas_to_plmn_id(&mme_ue->last_visited_plmn_id,
            &last_visited_registered_tai->nas_plmn_id);
        ogs_debug("    Visited_PLMN_ID:%06x",
                plmn_id_hexdump(&mme_ue->last_visited_plmn_id));
    }

    memcpy(&mme_ue->ue_network_capability, 
            &attach_request->ue_network_capability,
            sizeof(attach_request->ue_network_capability));

    if (attach_request->presencemask &
            NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ms_network_capability, 
                &attach_request->ms_network_capability,
                sizeof(attach_request->ms_network_capability));
    }

    switch (eps_mobile_identity->imsi.type) {
    case NAS_EPS_MOBILE_IDENTITY_IMSI:
        memcpy(&mme_ue->nas_mobile_identity_imsi, 
            &eps_mobile_identity->imsi, sizeof(nas_mobile_identity_imsi_t));
        nas_imsi_to_bcd(
            &eps_mobile_identity->imsi, eps_mobile_identity->length,
            imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        ogs_debug("    IMSI[%s]", imsi_bcd);

        break;
    case NAS_EPS_MOBILE_IDENTITY_GUTI:
        eps_mobile_identity_guti = &eps_mobile_identity->guti;

        nas_guti.nas_plmn_id = eps_mobile_identity_guti->nas_plmn_id;
        nas_guti.mme_gid = eps_mobile_identity_guti->mme_gid;
        nas_guti.mme_code = eps_mobile_identity_guti->mme_code;
        nas_guti.m_tmsi = eps_mobile_identity_guti->m_tmsi;

        ogs_debug("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]",
                nas_guti.mme_gid,
                nas_guti.mme_code,
                nas_guti.m_tmsi,
                MME_UE_HAVE_IMSI(mme_ue) 
                    ? mme_ue->imsi_bcd : "Unknown");
        break;
    default:
        ogs_warn("Not implemented[%d]", eps_mobile_identity->imsi.type);
        break;
    }

    NAS_STORE_DATA(&mme_ue->pdn_connectivity_request, esm_message_container);

    return OGS_OK;
}

int emm_handle_attach_complete(
    mme_ue_t *mme_ue, nas_attach_complete_t *attach_complete)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    nas_message_t message;
    nas_emm_information_t *emm_information = &message.emm.emm_information;
    nas_time_zone_and_time_t *universal_time_and_local_time_zone =
        &emm_information->universal_time_and_local_time_zone;
    nas_daylight_saving_time_t *network_daylight_saving_time = 
        &emm_information->network_daylight_saving_time;

    struct timeval tv;
    struct tm gmt, local;

    ogs_gettimeofday(&tv);
    ogs_gmtime(tv.tv_sec, &gmt);
    ogs_localtime(tv.tv_sec, &local);

    ogs_assert(mme_ue);

    ogs_debug("    GMT Time[Y:M:D H:M:S GMT] - %d:%d:%d, %d:%d:%d, %d",
        gmt.tm_year, gmt.tm_mon, gmt.tm_mday,
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec,
        (int)gmt.tm_gmtoff);
    ogs_debug("    LOCAL Time[Y:M:D H:M:S GMT] - %d:%d:%d, %d:%d:%d, %d",
        local.tm_year, local.tm_mon, local.tm_mday,
        local.tm_hour, local.tm_min, local.tm_sec,
        (int)local.tm_gmtoff);

    rv = nas_send_emm_to_esm(mme_ue, &attach_complete->esm_message_container);
    ogs_assert(rv == OGS_OK);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_EMM_INFORMATION;

    emm_information->presencemask |=
        NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
    universal_time_and_local_time_zone->year = 
                NAS_TIME_TO_BCD(gmt.tm_year % 100);
    universal_time_and_local_time_zone->mon =
                NAS_TIME_TO_BCD(gmt.tm_mon+1);
    universal_time_and_local_time_zone->mday = 
                NAS_TIME_TO_BCD(gmt.tm_mday);
    universal_time_and_local_time_zone->hour = 
                NAS_TIME_TO_BCD(gmt.tm_hour);
    universal_time_and_local_time_zone->min = NAS_TIME_TO_BCD(gmt.tm_min);
    universal_time_and_local_time_zone->sec = NAS_TIME_TO_BCD(gmt.tm_sec);
    if (local.tm_gmtoff >= 0) {
        universal_time_and_local_time_zone->timezone = 
                    NAS_TIME_TO_BCD(local.tm_gmtoff / 900);
    } else {
        universal_time_and_local_time_zone->timezone = 
                    NAS_TIME_TO_BCD((-local.tm_gmtoff) / 900);
        universal_time_and_local_time_zone->timezone |= 0x08;
    }
    ogs_debug("    Timezone:0x%x", 
        universal_time_and_local_time_zone->timezone);

    emm_information->presencemask |=
        NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
    network_daylight_saving_time->length = 1;

    if (mme_self()->full_name.length) {
        emm_information->presencemask |=
            NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT;
        memcpy(&emm_information->full_name_for_network,
            &mme_self()->full_name, sizeof(nas_network_name_t));
    }
    
    if (mme_self()->short_name.length) {
        emm_information->presencemask |=
            NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT;
        memcpy(&emm_information->short_name_for_network,
            &mme_self()->short_name, sizeof(nas_network_name_t));
    }                

    rv = nas_security_encode(&emmbuf, mme_ue, &message);
    ogs_assert(rv == OGS_OK && emmbuf);
    ogs_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == OGS_OK);

    ogs_debug("[EMM] EMM information");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    return OGS_OK;
}

int emm_handle_identity_response(
        mme_ue_t *mme_ue, nas_identity_response_t *identity_response)
{
    nas_mobile_identity_t *mobile_identity = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(identity_response);

    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    mobile_identity = &identity_response->mobile_identity;

    if (mobile_identity->imsi.type == NAS_IDENTITY_TYPE_2_IMSI) {
        char imsi_bcd[MAX_IMSI_BCD_LEN+1];

        memcpy(&mme_ue->nas_mobile_identity_imsi, 
            &mobile_identity->imsi, sizeof(nas_mobile_identity_imsi_t));
        nas_imsi_to_bcd(
            &mobile_identity->imsi, mobile_identity->length, imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        ogs_assert(mme_ue->imsi_len);
    } else {
        ogs_warn("Not supported Identity type[%d]", mobile_identity->imsi.type);
    }

    return OGS_OK;
}

int emm_handle_detach_request(
        mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request)
{
    nas_detach_type_t *detach_type = NULL;

    ogs_assert(detach_request);
    ogs_assert(mme_ue);

    detach_type = &detach_request->detach_type;

    /* Set EPS Attach Type */
    memcpy(&mme_ue->nas_eps.detach, detach_type, sizeof(nas_detach_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_FROM_UE;
    mme_ue->nas_eps.ksi = detach_type->nas_key_set_identifier;
    ogs_debug("    NAS_EPS TYPE[%d] KSI[%d] DETACH[0x%x]",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi, mme_ue->nas_eps.data);

    switch (detach_request->detach_type.detach_type) {
    /* 0 0 1 : EPS detach */
    case NAS_DETACH_TYPE_FROM_UE_EPS_DETACH: 
        ogs_debug("    EPS Detach");
        break;
    /* 0 1 0 : IMSI detach */
    case NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
        ogs_debug("    IMSI Detach");
        break;
    case 6: /* 1 1 0 : reserved */
    case 7: /* 1 1 1 : reserved */
        ogs_warn("Unknown Detach type[%d]",
            detach_request->detach_type.detach_type);
        break;
    /* 0 1 1 : combined EPS/IMSI detach */
    case NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH: 
        ogs_debug("    Combined EPS/IMSI Detach");
    default: /* all other values */
        break;
    }
    if (detach_request->detach_type.switch_off)
        ogs_debug("    Switch-Off");

    return OGS_OK;
}

int emm_handle_service_request(
        mme_ue_t *mme_ue, nas_service_request_t *service_request)
{
    nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
                    &service_request->ksi_and_sequence_number;

    ogs_assert(mme_ue);

    /* Set EPS Update Type */
    mme_ue->nas_eps.type = MME_EPS_TYPE_SERVICE_REQUEST;
    mme_ue->nas_eps.ksi = ksi_and_sequence_number->ksi;
    ogs_debug("    NAS_EPS TYPE[%d] KSI[%d]",
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
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_PAGING_INFO(mme_ue);
    if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    ogs_debug("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]",
            mme_ue->guti.mme_gid,
            mme_ue->guti.mme_code,
            mme_ue->guti.m_tmsi,
            MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown");

    return OGS_OK;
}

int emm_handle_tau_request(
        mme_ue_t *mme_ue, nas_tracking_area_update_request_t *tau_request)
{
    int served_tai_index = 0;

    nas_eps_mobile_identity_guti_t *eps_mobile_identity_guti = NULL;
    nas_guti_t nas_guti;

    nas_eps_update_type_t *eps_update_type =
                    &tau_request->eps_update_type;
    nas_eps_mobile_identity_t *eps_mobile_identity =
                    &tau_request->old_guti;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    /* Set EPS Update Type */
    memcpy(&mme_ue->nas_eps.update, eps_update_type,
            sizeof(nas_eps_update_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_TAU_REQUEST;
    mme_ue->nas_eps.ksi = eps_update_type->nas_key_set_identifier;
    ogs_debug("    NAS_EPS TYPE[%d] KSI[%d] UPDATE[0x%x]",
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
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_PAGING_INFO(mme_ue);
    CLEAR_SERVICE_INDICATOR(mme_ue);
    if (BEARER_CONTEXT_IS_ACTIVE(mme_ue))
        ogs_debug("    Bearer-Active");
    else
        ogs_debug("    Bearer-Inactive");

    if (mme_ue->nas_eps.update.active_flag)
        ogs_debug("    Active flag");
    else
        ogs_debug("    No Active flag");

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&mme_ue->e_cgi.plmn_id), mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&enb_ue->saved.tai.plmn_id), enb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(e_cgi_t));

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0) {
        /* Send TAU reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
        nas_send_tau_reject(mme_ue, EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    /* Store UE specific information */
    if (tau_request->presencemask &
        NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        nas_tracking_area_identity_t *last_visited_registered_tai = 
            &tau_request->last_visited_registered_tai;

        nas_to_plmn_id(&mme_ue->last_visited_plmn_id,
                &last_visited_registered_tai->nas_plmn_id);
        ogs_debug("    Visited_PLMN_ID:%06x",
                plmn_id_hexdump(&mme_ue->last_visited_plmn_id));
    } 

    if (tau_request->presencemask &
            NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ue_network_capability, 
                &tau_request->ue_network_capability,
                sizeof(tau_request->ue_network_capability));
    }

    if (tau_request->presencemask &
            NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ms_network_capability, 
                &tau_request->ms_network_capability,
                sizeof(tau_request->ms_network_capability));
    }

    /* TODO: 
     *   1) Consider if MME is changed or not.
     *   2) Consider if SGW is changed or not.
     */
    switch (eps_mobile_identity->imsi.type) {
    case NAS_EPS_MOBILE_IDENTITY_GUTI:
        eps_mobile_identity_guti = &eps_mobile_identity->guti;

        nas_guti.nas_plmn_id = eps_mobile_identity_guti->nas_plmn_id;
        nas_guti.mme_gid = eps_mobile_identity_guti->mme_gid;
        nas_guti.mme_code = eps_mobile_identity_guti->mme_code;
        nas_guti.m_tmsi = eps_mobile_identity_guti->m_tmsi;

        ogs_debug("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
                nas_guti.mme_gid,
                nas_guti.mme_code,
                nas_guti.m_tmsi,
                MME_UE_HAVE_IMSI(mme_ue) 
                    ? mme_ue->imsi_bcd : "Unknown");
        break;
    default:
        ogs_warn("Not implemented[%d]", 
                eps_mobile_identity->imsi.type);
        
        return OGS_OK;
    }

    return OGS_OK;
}

int emm_handle_extended_service_request(
    mme_ue_t *mme_ue, nas_extended_service_request_t *extended_service_request)
{
    int served_tai_index = 0;

    nas_service_type_t *service_type = &extended_service_request->service_type;
    nas_mobile_identity_t *mobile_identity = &extended_service_request->m_tmsi;
    nas_mobile_identity_tmsi_t *mobile_identity_tmsi = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    /* Set Service Type */
    memcpy(&mme_ue->nas_eps.service, service_type,
            sizeof(nas_service_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST;
    mme_ue->nas_eps.ksi = service_type->nas_key_set_identifier;
    ogs_debug("    NAS_EPS TYPE[%d] KSI[%d] SERVICE[0x%x]",
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
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Paging Timer and Message
     *   Update KeNB
     */
    CLEAR_PAGING_INFO(mme_ue);
    if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&mme_ue->e_cgi.plmn_id), mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&enb_ue->saved.tai.plmn_id), enb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from enb_ue */
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(e_cgi_t));

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0) {
        /* Send TAU reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
        nas_send_tau_reject(mme_ue, EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    switch(mobile_identity->tmsi.type) {
    case NAS_MOBILE_IDENTITY_TMSI:
        mobile_identity_tmsi = &mobile_identity->tmsi;

        ogs_debug("    M-TMSI:[0x%x] IMSI:[%s]",
                mobile_identity_tmsi->tmsi,
                MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown");
        break;
    default:
        ogs_error("Unknown TMSI type [%d]", mobile_identity->tmsi.type);
        break;
    }

    return OGS_OK;
}
