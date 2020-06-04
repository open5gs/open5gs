/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "nas-security.h"

#include "ngap-path.h"
#include "nas-path.h"

#include "gmm-handler.h"
#include "sbi-path.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gmm_log_domain

int gmm_handle_registration_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_registration_request_t *registration_request)
{
    int served_tai_index = 0;

    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_registration_type_t *registration_type = NULL;
    ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
    ogs_nas_5gs_mobile_identity_header_t *mobile_identity_header = NULL;
    ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti = NULL;
    ogs_nas_5gs_guti_t nas_guti;

    ogs_assert(registration_request);
    registration_type = &registration_request->registration_type;
    ogs_assert(registration_type);
    mobile_identity = &registration_request->mobile_identity;
    ogs_assert(mobile_identity);
    mobile_identity_header =
            (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;
    ogs_assert(mobile_identity_header);

    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    switch (mobile_identity_header->type) {
    case OGS_NAS_5GS_MOBILE_IDENTITY_SUCI:
        amf_ue_set_suci(amf_ue, mobile_identity);
        ogs_debug("[%s]    SUCI", amf_ue->suci);
        break;
    case OGS_NAS_5GS_MOBILE_IDENTITY_GUTI:
        mobile_identity_guti =
            (ogs_nas_5gs_mobile_identity_guti_t *)mobile_identity->buffer;
        ogs_assert(mobile_identity_guti);

        memcpy(&nas_guti.nas_plmn_id,
                &mobile_identity_guti->nas_plmn_id, OGS_PLMN_ID_LEN);
        memcpy(&nas_guti.amf_id,
                &mobile_identity_guti->amf_id, sizeof(ogs_amf_id_t));
        nas_guti.m_tmsi = be32toh(mobile_identity_guti->m_tmsi);

        ogs_debug("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]",
            AMF_UE_HAVE_SUCI(amf_ue) ? amf_ue->suci : "Unknown ID",
            ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
        break;
    default:
        ogs_error("Unknown SUCI type [%d]", mobile_identity_header->type);
        break;
    }


    /* Set 5GS Registration Type */
    memcpy(&amf_ue->nas.registration, registration_type,
            sizeof(ogs_nas_5gs_registration_type_t));
    amf_ue->nas.type = OGS_NAS_5GS_REGISTRATION_REQUEST;
    amf_ue->nas.ksi = registration_type->type;
    ogs_debug("[%s]    OGS_NAS_5GS TYPE[%d] KSI[%d] REGISTRATION[0x%x]",
            amf_ue->suci, amf_ue->nas.type, amf_ue->nas.ksi, amf_ue->nas.data);
    /*
     * REGISTRATION_REQUEST
     *   Clear EBI generator
     *   Clear Timer and Message
     *
     * TAU_REQUEST
     *   Clear Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Timer and Message
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    CLEAR_5GS_BEARER_ID(amf_ue);
    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_kdf_kgnb_and_kn3iwf(
                amf_ue->kamf, amf_ue->ul_count.i32,
                OGS_KDF_ACCESS_TYPE_3GPP, amf_ue->kgnb);
        ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
        amf_ue->nhcc = 1;
    }

    ogs_debug("[%s]    OLD TAI[PLMN_ID:%06x,TAC:%d]", amf_ue->suci,
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac.v);
    ogs_debug("[%s]    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]", amf_ue->suci,
            ogs_plmn_id_hexdump(&amf_ue->cgi.plmn_id),
            (long long)amf_ue->cgi.cell_id);
    ogs_debug("[%s]    TAI[PLMN_ID:%06x,TAC:%d]", amf_ue->suci,
            ogs_plmn_id_hexdump(&ran_ue->saved.tai.plmn_id),
            ran_ue->saved.tai.tac.v);
    ogs_debug("[%s]    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]", amf_ue->suci,
            ogs_plmn_id_hexdump(&ran_ue->saved.cgi.plmn_id),
            (long long)ran_ue->saved.cgi.cell_id);

    /* Copy TAI and ECGI from ran_ue */
    memcpy(&amf_ue->tai, &ran_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->cgi, &ran_ue->saved.cgi, sizeof(ogs_nr_cgi_t));

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->tai);
    if (served_tai_index < 0) {
        /* Send Registration Reject */
        ogs_warn("[%s] Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            amf_ue->suci,
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac.v);
        nas_5gs_send_registration_reject(amf_ue,
            OGS_5GMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("[%s]    SERVED_TAI_INDEX[%d]", amf_ue->suci, served_tai_index);

    /* Store UE specific information */
    if (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        ogs_nas_5gs_tai_t *last_visited_registered_tai = 
            &registration_request->last_visited_registered_tai;

        ogs_nas_to_plmn_id(&amf_ue->last_visited_plmn_id,
            &last_visited_registered_tai->nas_plmn_id);
        ogs_debug("[%s]    Visited_PLMN_ID:%06x", amf_ue->suci,
                ogs_plmn_id_hexdump(&amf_ue->last_visited_plmn_id));
    }

    if (registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) {
        ogs_nas_5gmm_capability_t *gmm_capability =
            &registration_request->gmm_capability;

        amf_ue->gmm_capability.lte_positioning_protocol_capability
            = gmm_capability->lte_positioning_protocol_capability;
        amf_ue->gmm_capability.ho_attach = gmm_capability->ho_attach;
        amf_ue->gmm_capability.s1_mode = gmm_capability->s1_mode;
            
        ogs_debug("[%s]    5GMM Capability:[LPP:%d, HO_ATTACH:%d, S1_MODE:%d]",
            amf_ue->suci,
            amf_ue->gmm_capability.lte_positioning_protocol_capability,
            amf_ue->gmm_capability.ho_attach,
            amf_ue->gmm_capability.s1_mode);
    }

    if (registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ue_security_capability, 
                &registration_request->ue_security_capability,
                ogs_min(sizeof(amf_ue->ue_security_capability),
                    registration_request->ue_security_capability.length));
    }

    if (amf_selected_int_algorithm(amf_ue) ==
            OGS_NAS_SECURITY_ALGORITHMS_NIA0) {
        ogs_warn("[%s] NEA0 can be used in Encrypt[0x%x], "
            "but Integrity[0x%x] cannot be bypassed with NIA0", amf_ue->suci,
            amf_selected_enc_algorithm(amf_ue), 
            amf_selected_int_algorithm(amf_ue));
        nas_5gs_send_registration_reject(amf_ue,
            OGS_5GMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int gmm_handle_authentication_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_authentication_response_t *authentication_response)
{
    int rv;
    ogs_nas_authentication_response_parameter_t
        *authentication_response_parameter = NULL;
    uint8_t hxres_star[OGS_MAX_RES_LEN];

    ogs_assert(amf_ue);
    ogs_assert(authentication_response);

    authentication_response_parameter = &authentication_response->
                authentication_response_parameter;

    ogs_debug("[%s] Authentication response", amf_ue->suci);

    CLEAR_AMF_UE_TIMER(amf_ue->t3560);

    if (authentication_response_parameter->length != OGS_MAX_RES_LEN) {
        ogs_error("[%s] Invalid length [%d]",
                amf_ue->suci, authentication_response_parameter->length);
        return OGS_ERROR;
    }

    ogs_kdf_hxres_star(
            amf_ue->rand, authentication_response_parameter->res, hxres_star);

    if (memcmp(hxres_star, amf_ue->hxres_star, OGS_MAX_RES_LEN) != 0) {
        ogs_log_hexdump(OGS_LOG_WARN,
                authentication_response_parameter->res,
                authentication_response_parameter->length);
        ogs_log_hexdump(OGS_LOG_WARN, hxres_star, OGS_MAX_RES_LEN);
        ogs_log_hexdump(OGS_LOG_WARN,
                amf_ue->hxres_star, OGS_MAX_RES_LEN);
        return OGS_ERROR;
    }

    memcpy(amf_ue->xres_star, authentication_response_parameter->res,
            authentication_response_parameter->length);

    rv = amf_nausf_auth_discover_and_send_authenticate(amf_ue);
    if (rv == OGS_ERROR) {
        ogs_error("[%s] Cannot send SBI message", amf_ue->suci);
        return OGS_ERROR;
    }

    return OGS_OK;
}

#if 0
int gmm_handle_registration_complete(amf_ue_t *amf_ue,
        ogs_nas_5gs_registration_complete_t *registration_complete)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_gmm_information_t *gmm_information =
        &message.gmm.gmm_information;
    ogs_nas_time_zone_and_time_t *universal_time_and_local_time_zone =
        &gmm_information->universal_time_and_local_time_zone;
    ogs_nas_daylight_saving_time_t *network_daylight_saving_time = 
        &gmm_information->network_daylight_saving_time;

    struct timeval tv;
    struct tm gmt, local;

    ogs_gettimeofday(&tv);
    ogs_gmtime(tv.tv_sec, &gmt);
    ogs_localtime(tv.tv_sec, &local);

    ogs_assert(amf_ue);

    ogs_debug("    GMT Time[Y:M:D H:M:S GMT] - %d:%d:%d, %d:%d:%d, %d",
        gmt.tm_year, gmt.tm_mon, gmt.tm_mday,
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec,
        (int)gmt.tm_gmtoff);
    ogs_debug("    LOCAL Time[Y:M:D H:M:S GMT] - %d:%d:%d, %d:%d:%d, %d",
        local.tm_year, local.tm_mon, local.tm_mday,
        local.tm_hour, local.tm_min, local.tm_sec,
        (int)local.tm_gmtoff);

    rv = nas_5gs_send_gmm_to_esm(
            amf_ue, &registration_complete->esm_message_container);
    if (rv != OGS_OK) {
        ogs_error("nas_5gs_send_gmm_to_esm() failed");
        return OGS_ERROR;
    }

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.gmm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.gmm.h.message_type = OGS_NAS_5GS_EMM_INFORMATION;

    gmm_information->presencemask |=
        OGS_NAS_5GS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
    universal_time_and_local_time_zone->year = 
                OGS_OGS_NAS_TIME_TO_BCD(gmt.tm_year % 100);
    universal_time_and_local_time_zone->mon =
                OGS_OGS_NAS_TIME_TO_BCD(gmt.tm_mon+1);
    universal_time_and_local_time_zone->mday = 
                OGS_OGS_NAS_TIME_TO_BCD(gmt.tm_mday);
    universal_time_and_local_time_zone->hour = 
                OGS_OGS_NAS_TIME_TO_BCD(gmt.tm_hour);
    universal_time_and_local_time_zone->min =
                OGS_OGS_NAS_TIME_TO_BCD(gmt.tm_min);
    universal_time_and_local_time_zone->sec =
                OGS_OGS_NAS_TIME_TO_BCD(gmt.tm_sec);
    if (local.tm_gmtoff >= 0) {
        universal_time_and_local_time_zone->timezone = 
                    OGS_OGS_NAS_TIME_TO_BCD(local.tm_gmtoff / 900);
    } else {
        universal_time_and_local_time_zone->timezone = 
                    OGS_OGS_NAS_TIME_TO_BCD((-local.tm_gmtoff) / 900);
        universal_time_and_local_time_zone->timezone |= 0x08;
    }
    ogs_debug("    Timezone:0x%x", 
        universal_time_and_local_time_zone->timezone);

    gmm_information->presencemask |=
        OGS_NAS_5GS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
    network_daylight_saving_time->length = 1;

    if (amf_self()->full_name.length) {
        gmm_information->presencemask |=
            OGS_NAS_5GS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT;
        memcpy(&gmm_information->full_name_for_network,
            &amf_self()->full_name, sizeof(ogs_nas_network_name_t));
    }
    
    if (amf_self()->short_name.length) {
        gmm_information->presencemask |=
            OGS_NAS_5GS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT;
        memcpy(&gmm_information->short_name_for_network,
            &amf_self()->short_name, sizeof(ogs_nas_network_name_t));
    }                

    gmmbuf = nas_5gs_security_encode(amf_ue, &message);
    if (gmmbuf) 
        nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);

    ogs_debug("[EMM] EMM information");
    ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

    return OGS_OK;
}

int gmm_handle_identity_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_identity_response_t *identity_response)
{
    ogs_nas_mobile_identity_t *mobile_identity = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(identity_response);

    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    mobile_identity = &identity_response->mobile_identity;

    if (mobile_identity->imsi.type == OGS_NAS_IDENTITY_TYPE_2_IMSI) {
        char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

        if (sizeof(ogs_nas_mobile_identity_imsi_t) != mobile_identity->length) {
            ogs_error("mobile_identity length (%d != %d)",
                    (int)sizeof(ogs_nas_mobile_identity_imsi_t),
                    mobile_identity->length);
            return OGS_ERROR;
        }
        memcpy(&amf_ue->nas_mobile_identity_imsi, 
            &mobile_identity->imsi, mobile_identity->length);
        ogs_nas_imsi_to_bcd(
            &mobile_identity->imsi, mobile_identity->length, imsi_bcd);
        amf_ue_set_imsi(amf_ue, imsi_bcd);

        if (amf_ue->imsi_len != OGS_MAX_IMSI_LEN) {
            ogs_error("Invalid IMSI LEN[%d]", amf_ue->imsi_len);
            return OGS_ERROR;
        }
    } else {
        ogs_warn("Not supported Identity type[%d]", mobile_identity->imsi.type);
    }

    return OGS_OK;
}

int gmm_handle_deregistration_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request)
{
    ogs_nas_deregistration_type_t *deregistration_type = NULL;

    ogs_assert(deregistration_request);
    ogs_assert(amf_ue);

    deregistration_type = &deregistration_request->deregistration_type;

    /* Set 5GS Attach Type */
    memcpy(&amf_ue->nas.deregistration, deregistration_type, sizeof(ogs_nas_deregistration_type_t));
    amf_ue->nas.type = AMF_5GS_TYPE_DETACH_REQUEST_FROM_UE;
    amf_ue->nas.ksi = deregistration_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_5GS TYPE[%d] KSI[%d] DETACH[0x%x]",
            amf_ue->nas.type, amf_ue->nas.ksi, amf_ue->nas.data);

    switch (deregistration_request->deregistration_type.value) {
    /* 0 0 1 : 5GS deregistration */
    case OGS_NAS_DETACH_TYPE_FROM_UE_5GS_DETACH: 
        ogs_debug("    5GS Detach");
        break;
    /* 0 1 0 : IMSI deregistration */
    case OGS_NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
        ogs_debug("    IMSI Detach");
        break;
    case 6: /* 1 1 0 : reserved */
    case 7: /* 1 1 1 : reserved */
        ogs_warn("Unknown Detach type[%d]",
            deregistration_request->deregistration_type.value);
        break;
    /* 0 1 1 : combined 5GS/IMSI deregistration */
    case OGS_NAS_DETACH_TYPE_FROM_UE_COMBINED_5GS_IMSI_DETACH: 
        ogs_debug("    Combined 5GS/IMSI Detach");
    default: /* all other values */
        break;
    }
    if (deregistration_request->deregistration_type.switch_off)
        ogs_debug("    Switch-Off");

    return OGS_OK;
}

int gmm_handle_service_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_service_request_t *service_request)
{
    ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
                    &service_request->ksi_and_sequence_number;

    ogs_assert(amf_ue);

    /* Set 5GS Update Type */
    amf_ue->nas.type = AMF_5GS_TYPE_SERVICE_REQUEST;
    amf_ue->nas.ksi = ksi_and_sequence_number->ksi;
    ogs_debug("    OGS_NAS_5GS TYPE[%d] KSI[%d]",
            amf_ue->nas.type, amf_ue->nas.ksi);

    /*
     * REGISTRATION_REQUEST
     *   Clear EBI generator
     *   Clear Timer and Message
     *
     * TAU_REQUEST
     *   Clear Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Timer and Message
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_kdf_kgnb_and_kn3iwf(
                amf_ue->kamf, amf_ue->ul_count.i32,
                OGS_KDF_ACCESS_TYPE_3GPP, amf_ue->kgnb);
        ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
        amf_ue->nhcc = 1;
    }

    ogs_debug("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]",
            amf_ue->guti.amf_gid,
            amf_ue->guti.amf_code,
            amf_ue->guti.m_tmsi,
            AMF_UE_HAVE_IMSI(amf_ue) ? amf_ue->imsi_bcd : "Unknown");

    return OGS_OK;
}

int gmm_handle_tau_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_tracking_area_update_request_t *tau_request)
{
    int served_tai_index = 0;

    ogs_nas_5gs_mobile_identity_guti_t *5gs_mobile_identity_guti = NULL;
    ogs_nas_5gs_guti_t nas_guti;

    ogs_nas_5gs_update_type_t *5gs_update_type =
                    &tau_request->5gs_update_type;
    ogs_nas_5gs_mobile_identity_t *5gs_mobile_identity =
                    &tau_request->old_guti;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    /* Set 5GS Update Type */
    memcpy(&amf_ue->nas.update, 5gs_update_type,
            sizeof(ogs_nas_5gs_update_type_t));
    amf_ue->nas.type = AMF_5GS_TYPE_TAU_REQUEST;
    amf_ue->nas.ksi = 5gs_update_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_5GS TYPE[%d] KSI[%d] UPDATE[0x%x]",
            amf_ue->nas.type, amf_ue->nas.ksi,
            amf_ue->nas.data);
    
    /*
     * REGISTRATION_REQUEST
     *   Clear EBI generator
     *   Clear Timer and Message
     *
     * TAU_REQUEST
     *   Clear Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Timer and Message
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    CLEAR_SERVICE_INDICATOR(amf_ue);
    if (BEARER_CONTEXT_IS_ACTIVE(amf_ue))
        ogs_debug("    Bearer-Active");
    else
        ogs_debug("    Bearer-Inactive");

    if (amf_ue->nas.update.active_flag)
        ogs_debug("    Active flag");
    else
        ogs_debug("    No Active flag");

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac);
    ogs_debug("    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&amf_ue->e_cgi.plmn_id), amf_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.tai.plmn_id),
            ran_ue->saved.tai.tac);
    ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.e_cgi.plmn_id),
            ran_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from ran_ue */
    memcpy(&amf_ue->tai, &ran_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->e_cgi, &ran_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->tai);
    if (served_tai_index < 0) {
        /* Send TAU reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac);
        nas_5gs_send_tau_reject(amf_ue, EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    /* Store UE specific information */
    if (tau_request->presencemask &
        OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        ogs_nas_5gs_tai_t *last_visited_registered_tai = 
            &tau_request->last_visited_registered_tai;

        ogs_nas_to_plmn_id(&amf_ue->last_visited_plmn_id,
                &last_visited_registered_tai->nas_plmn_id);
        ogs_debug("    Visited_PLMN_ID:%06x",
                ogs_plmn_id_hexdump(&amf_ue->last_visited_plmn_id));
    } 

    if (tau_request->presencemask &
            OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ue_network_capability, 
                &tau_request->ue_network_capability,
                sizeof(tau_request->ue_network_capability));
    }

    if (tau_request->presencemask &
            OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ms_network_capability, 
                &tau_request->ms_network_capability,
                sizeof(tau_request->ms_network_capability));
    }

    /* TODO: 
     *   1) Consider if AMF is changed or not.
     *   2) Consider if SGW is changed or not.
     */
    switch (5gs_mobile_identity->imsi.type) {
    case OGS_NAS_5GS_MOBILE_IDENTITY_GUTI:
        5gs_mobile_identity_guti = &5gs_mobile_identity->guti;

        nas_guti.nas_plmn_id = 5gs_mobile_identity_guti->nas_plmn_id;
        nas_guti.amf_gid = 5gs_mobile_identity_guti->amf_gid;
        nas_guti.amf_code = 5gs_mobile_identity_guti->amf_code;
        nas_guti.m_tmsi = 5gs_mobile_identity_guti->m_tmsi;

        ogs_debug("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
                nas_guti.amf_gid,
                nas_guti.amf_code,
                nas_guti.m_tmsi,
                AMF_UE_HAVE_IMSI(amf_ue) 
                    ? amf_ue->imsi_bcd : "Unknown");
        break;
    default:
        ogs_warn("Not implemented[%d]", 
                5gs_mobile_identity->imsi.type);
        
        return OGS_OK;
    }

    return OGS_OK;
}

int gmm_handle_extended_service_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_extended_service_request_t *extended_service_request)
{
    int served_tai_index = 0;

    ogs_nas_service_type_t *service_type =
        &extended_service_request->service_type;
    ogs_nas_mobile_identity_t *mobile_identity =
        &extended_service_request->m_tmsi;
    ogs_nas_mobile_identity_tmsi_t *mobile_identity_tmsi = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    /* Set Service Type */
    memcpy(&amf_ue->nas.service, service_type,
            sizeof(ogs_nas_service_type_t));
    amf_ue->nas.type = AMF_5GS_TYPE_EXTENDED_SERVICE_REQUEST;
    amf_ue->nas.ksi = service_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_5GS TYPE[%d] KSI[%d] SERVICE[0x%x]",
            amf_ue->nas.type, amf_ue->nas.ksi, amf_ue->nas.value);
    
    /*
     * REGISTRATION_REQUEST
     *   Clear EBI generator
     *   Clear Timer and Message
     *
     * TAU_REQUEST
     *   Clear Timer and Message
     *
     * SERVICE_REQUEST
     *   Clear Timer and Message
     *
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac);
    ogs_debug("    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&amf_ue->e_cgi.plmn_id), amf_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.tai.plmn_id),
            ran_ue->saved.tai.tac);
    ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.e_cgi.plmn_id),
            ran_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from ran_ue */
    memcpy(&amf_ue->tai, &ran_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->e_cgi, &ran_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->tai);
    if (served_tai_index < 0) {
        /* Send TAU reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac);
        nas_5gs_send_tau_reject(amf_ue, EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    switch(mobile_identity->tmsi.type) {
    case OGS_NAS_MOBILE_IDENTITY_TMSI:
        mobile_identity_tmsi = &mobile_identity->tmsi;

        ogs_debug("    M-TMSI:[0x%x] IMSI:[%s]",
                mobile_identity_tmsi->tmsi,
                AMF_UE_HAVE_IMSI(amf_ue) ? amf_ue->imsi_bcd : "Unknown");
        break;
    default:
        ogs_error("Unknown TMSI type [%d]", mobile_identity->tmsi.type);
        break;
    }

    return OGS_OK;
}
#endif

int gmm_handle_security_mode_complete(amf_ue_t *amf_ue,
    ogs_nas_5gs_security_mode_complete_t *security_mode_complete)
{
    ogs_nas_5gs_mobile_identity_t *imeisv = NULL;
    ogs_nas_mobile_identity_imeisv_t *mobile_identity_imeisv = NULL;

    ogs_assert(amf_ue);
    ogs_assert(security_mode_complete);

    if (security_mode_complete->presencemask &
        OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT) {

        imeisv = &security_mode_complete->imeisv;
        ogs_assert(imeisv);
        mobile_identity_imeisv =
            (ogs_nas_mobile_identity_imeisv_t *)imeisv->buffer;
        ogs_assert(mobile_identity_imeisv);

        switch (mobile_identity_imeisv->type) {
        case OGS_NAS_5GS_MOBILE_IDENTITY_IMEISV:
            memcpy(&amf_ue->nas_mobile_identity_imeisv,
                mobile_identity_imeisv, imeisv->length);
            ogs_nas_imeisv_to_bcd(mobile_identity_imeisv, imeisv->length,
                    amf_ue->imeisv_bcd);
            ogs_bcd_to_buffer(amf_ue->imeisv_bcd,
                    amf_ue->imeisv, &amf_ue->imeisv_len);
            amf_ue->imeisv_presence = true;
            break;
        default:
            ogs_warn("[%s] Invalid IMEISV Type [%d]",
                    amf_ue->supi, mobile_identity_imeisv->type);
            break;

        }
    }

    return OGS_OK;
}
