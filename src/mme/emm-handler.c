/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "mme-event.h"
#include "mme-sm.h"

#include "nas-security.h"

#include "s1ap-path.h"
#include "nas-path.h"
#include "mme-fd-path.h"
#include "mme-gtp-path.h"

#include "emm-handler.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __emm_log_domain

static uint8_t emm_cause_from_access_control(mme_ue_t *mme_ue);

int emm_handle_attach_request(enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_attach_request_t *attach_request, ogs_pkbuf_t *pkbuf)
{
    int r;
    uint8_t emm_cause;
    int served_tai_index = 0;

    ogs_nas_eps_mobile_identity_guti_t *eps_mobile_identity_guti = NULL;
    ogs_nas_eps_guti_t nas_guti;

    ogs_nas_eps_attach_type_t *eps_attach_type =
                    &attach_request->eps_attach_type;
    ogs_nas_eps_mobile_identity_t *eps_mobile_identity =
                    &attach_request->eps_mobile_identity;
    ogs_nas_esm_message_container_t *esm_message_container =
                    &attach_request->esm_message_container;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    ogs_assert(esm_message_container);
    if (!esm_message_container->length) {
        ogs_error("No ESM Message Container");
        r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                OGS_NAS_EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    /* HashMME */
    ogs_kdf_hash_mme(pkbuf->data, pkbuf->len, mme_ue->hash_mme);

    /* Set EPS Attach */
    memcpy(&mme_ue->nas_eps.attach, eps_attach_type,
            sizeof(ogs_nas_eps_attach_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_ATTACH_REQUEST;
    mme_ue->nas_eps.ksi = eps_attach_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_EPS TYPE[%d] KSI[%d]",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi);
    ogs_debug("    ATTACH TSC[%d] KSI[%d] VALUE[%d]",
            mme_ue->nas_eps.attach.tsc,
            mme_ue->nas_eps.attach.nas_key_set_identifier,
            mme_ue->nas_eps.attach.value);
    switch(mme_ue->nas_eps.attach.value){
        case OGS_NAS_ATTACH_TYPE_EPS_ATTACH:
            ogs_debug("    Requested EPS_ATTACH_TYPE[1, EPS_ATTACH]");
            break;
        case OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH:
            ogs_debug("    Requested EPS_ATTACH_TYPE[2, COMBINED_EPS_IMSI_ATTACH]");
            break;
        case OGS_NAS_ATTACH_TYPE_EPS_EMERGENCY_ATTACH:
            ogs_debug("    Requested EPS_ATTACH_TYPE[3, EPS_EMERGENCY_ATTACH]");
            break;
        default:
            ogs_error("    Invalid Requested EPS_ATTACH_TYPE[%d]",
                mme_ue->nas_eps.attach.value);
    }
    /*
     * ATTACH_REQUEST
     * TAU_REQUEST
     * SERVICE_REQUEST
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_MME_UE_ALL_TIMERS(mme_ue);

    CLEAR_EPS_BEARER_ID(mme_ue);
    CLEAR_SERVICE_INDICATOR(mme_ue);
    if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        ogs_kdf_kenb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id), mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&enb_ue->saved.tai.plmn_id),
            enb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from enb_ue */
    mme_ue->enb_ostream_id = enb_ue->enb_ostream_id;
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(ogs_eps_tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));
    mme_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0) {
        /* Send Attach Reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
        r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                OGS_NAS_EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED,
                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    /* Store UE specific information */
    if (attach_request->presencemask &
        OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        ogs_nas_eps_tai_t *last_visited_registered_tai =
            &attach_request->last_visited_registered_tai;

        ogs_nas_to_plmn_id(&mme_ue->last_visited_plmn_id,
            &last_visited_registered_tai->nas_plmn_id);
        ogs_debug("    Visited_PLMN_ID:%06x",
                ogs_plmn_id_hexdump(&mme_ue->last_visited_plmn_id));
    }

    memcpy(&mme_ue->ue_network_capability,
            &attach_request->ue_network_capability,
            sizeof(attach_request->ue_network_capability));

    if (attach_request->presencemask &
            OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ms_network_capability,
                &attach_request->ms_network_capability,
                sizeof(attach_request->ms_network_capability));
    }
    if (attach_request->presencemask &
        OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ue_additional_security_capability,
                &attach_request->ue_additional_security_capability,
                sizeof(attach_request->ue_additional_security_capability));
    }

    if (mme_selected_int_algorithm(mme_ue) ==
            OGS_NAS_SECURITY_ALGORITHMS_EIA0) {
        ogs_warn("Encrypt[0x%x] can be skipped with EEA0, "
            "but Integrity[0x%x] cannot be bypassed with EIA0",
            mme_selected_enc_algorithm(mme_ue),
            mme_selected_int_algorithm(mme_ue));
        r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                OGS_NAS_EMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH,
                OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    switch (eps_mobile_identity->imsi.type) {
    case OGS_NAS_EPS_MOBILE_IDENTITY_IMSI:
        if (sizeof(ogs_nas_mobile_identity_imsi_t) !=
                eps_mobile_identity->length) {
            ogs_error("mobile_identity length (%d != %d)",
                    (int)sizeof(ogs_nas_mobile_identity_imsi_t),
                    eps_mobile_identity->length);
            return OGS_ERROR;
        }
        memcpy(&mme_ue->nas_mobile_identity_imsi,
            &eps_mobile_identity->imsi, eps_mobile_identity->length);

        emm_cause = emm_cause_from_access_control(mme_ue);
        if (emm_cause != OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED) {
            ogs_error("Rejected by PLMN-ID access control");
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    emm_cause, OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        ogs_nas_eps_imsi_to_bcd(
            &eps_mobile_identity->imsi, eps_mobile_identity->length,
            imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        ogs_info("    IMSI[%s]", imsi_bcd);

        break;
    case OGS_NAS_EPS_MOBILE_IDENTITY_GUTI:
        eps_mobile_identity_guti = &eps_mobile_identity->guti;

        nas_guti.nas_plmn_id = eps_mobile_identity_guti->nas_plmn_id;
        nas_guti.mme_gid = eps_mobile_identity_guti->mme_gid;
        nas_guti.mme_code = eps_mobile_identity_guti->mme_code;
        nas_guti.m_tmsi = eps_mobile_identity_guti->m_tmsi;

        ogs_info("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]",
                nas_guti.mme_gid,
                nas_guti.mme_code,
                nas_guti.m_tmsi,
                MME_UE_HAVE_IMSI(mme_ue)
                    ? mme_ue->imsi_bcd : "Unknown IMSI");
        break;
    default:
        ogs_warn("Not implemented[%d]", eps_mobile_identity->imsi.type);
        break;
    }

    OGS_NAS_STORE_DATA(
            &mme_ue->pdn_connectivity_request, esm_message_container);

    return OGS_OK;
}

int emm_handle_attach_complete(
    enb_ue_t *enb_ue, mme_ue_t *mme_ue,
    ogs_nas_eps_attach_complete_t *attach_complete)
{
    int r, rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_nas_eps_message_t message;
    ogs_nas_eps_emm_information_t *emm_information =
        &message.emm.emm_information;
    ogs_nas_time_zone_t *local_time_zone = &emm_information->local_time_zone;
    ogs_nas_time_zone_and_time_t *universal_time_and_local_time_zone =
        &emm_information->universal_time_and_local_time_zone;
    ogs_nas_daylight_saving_time_t *network_daylight_saving_time =
        &emm_information->network_daylight_saving_time;

    struct timeval tv;
    struct tm gmt, local;

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    ogs_info("    IMSI[%s]", mme_ue->imsi_bcd);

    ogs_gettimeofday(&tv);
    ogs_gmtime(tv.tv_sec, &gmt);
    ogs_localtime(tv.tv_sec, &local);

    ogs_info("    UTC [%04d-%02d-%02dT%02d:%02d:%02d] Timezone[%d]/DST[%d]",
        gmt.tm_year+1900, gmt.tm_mon+1, gmt.tm_mday,
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec,
        (int)gmt.tm_gmtoff, gmt.tm_isdst);
    ogs_info("    LOCAL [%04d-%02d-%02dT%02d:%02d:%02d] Timezone[%d]/DST[%d]",
        local.tm_year+1900, local.tm_mon+1, local.tm_mday,
        local.tm_hour, local.tm_min, local.tm_sec,
        (int)local.tm_gmtoff, local.tm_isdst);

    rv = nas_eps_send_emm_to_esm(
            mme_ue, &attach_complete->esm_message_container);
    if (rv != OGS_OK) {
        ogs_error("nas_eps_send_emm_to_esm() failed");
        return OGS_ERROR;
    }

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_EMM_INFORMATION;

    if (mme_self()->full_name.length) {
        emm_information->presencemask |=
            OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT;
        memcpy(&emm_information->full_name_for_network,
            &mme_self()->full_name, sizeof(ogs_nas_network_name_t));
    }

    if (mme_self()->short_name.length) {
        emm_information->presencemask |=
            OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT;
        memcpy(&emm_information->short_name_for_network,
            &mme_self()->short_name, sizeof(ogs_nas_network_name_t));
    }

    if (!ogs_global_conf()->parameter.no_time_zone_information) {
        emm_information->presencemask |=
            OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT;

        if (local.tm_gmtoff >= 0) {
            *local_time_zone = OGS_NAS_TIME_TO_BCD(local.tm_gmtoff / 900);
        } else {
            *local_time_zone = OGS_NAS_TIME_TO_BCD((-local.tm_gmtoff) / 900);
            *local_time_zone |= 0x08;
        }
        ogs_debug("    Timezone:0x%x", *local_time_zone);

        emm_information->presencemask |=
            OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
        universal_time_and_local_time_zone->year =
                    OGS_NAS_TIME_TO_BCD(gmt.tm_year % 100);
        universal_time_and_local_time_zone->mon =
                    OGS_NAS_TIME_TO_BCD(gmt.tm_mon+1);
        universal_time_and_local_time_zone->mday =
                    OGS_NAS_TIME_TO_BCD(gmt.tm_mday);
        universal_time_and_local_time_zone->hour =
                    OGS_NAS_TIME_TO_BCD(gmt.tm_hour);
        universal_time_and_local_time_zone->min =
                    OGS_NAS_TIME_TO_BCD(gmt.tm_min);
        universal_time_and_local_time_zone->sec =
                    OGS_NAS_TIME_TO_BCD(gmt.tm_sec);
        universal_time_and_local_time_zone->timezone = *local_time_zone;

        emm_information->presencemask |=
            OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
        network_daylight_saving_time->length = 1;
        if (local.tm_isdst > 0) {
            network_daylight_saving_time->value = 1;
        }
    }

    emmbuf = nas_eps_security_encode(mme_ue, &message);
    if (!emmbuf) {
        ogs_error("nas_eps_security_encode() failed");
        return OGS_ERROR;
    }

    r = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    ogs_debug("EMM information");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    return r;
}

int emm_handle_identity_response(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_identity_response_t *identity_response)
{
    int r;
    uint8_t emm_cause;
    ogs_nas_mobile_identity_t *mobile_identity = NULL;

    ogs_assert(identity_response);

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    mobile_identity = &identity_response->mobile_identity;

    if (mobile_identity->imsi.type == OGS_NAS_IDENTITY_TYPE_2_IMSI) {
        char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

        if (sizeof(ogs_nas_mobile_identity_imsi_t) != mobile_identity->length) {
            ogs_error("mobile_identity length (%d != %d)",
                    (int)sizeof(ogs_nas_mobile_identity_imsi_t),
                    mobile_identity->length);
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                    OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }
        memcpy(&mme_ue->nas_mobile_identity_imsi,
            &mobile_identity->imsi, mobile_identity->length);

        emm_cause = emm_cause_from_access_control(mme_ue);
        if (emm_cause != OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED) {
            ogs_error("Rejected by PLMN-ID access control");
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    emm_cause, OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        ogs_nas_eps_imsi_to_bcd(
            &mobile_identity->imsi, mobile_identity->length, imsi_bcd);
        mme_ue_set_imsi(mme_ue, imsi_bcd);

        if (mme_ue->imsi_len != OGS_MAX_IMSI_LEN) {
            ogs_error("Invalid IMSI LEN[%d]", mme_ue->imsi_len);
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                    OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        ogs_info("    IMSI[%s]", mme_ue->imsi_bcd);
    } else {
        ogs_warn("Not supported Identity type[%d]", mobile_identity->imsi.type);
    }

    return OGS_OK;
}

int emm_handle_detach_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_detach_request_from_ue_t *detach_request)
{
    ogs_nas_detach_type_t *detach_type = NULL;

    ogs_assert(detach_request);
    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    detach_type = &detach_request->detach_type;

    /* Set EPS Detach */
    memcpy(&mme_ue->nas_eps.detach, detach_type, sizeof(ogs_nas_detach_type_t));

    /* 1. MME initiated detach request to the UE.
     *    (nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_TO_UE)
     * 2. If UE is IDLE, Paging sent to the UE
     * 3. If UE is wake-up, UE will send Server Request.
     *    (nas_eps.type = MME_EPS_TYPE_SERVICE_REQUEST)
     *
     * So, we will lose the MME_EPS_TYPE_DETACH_REQUEST_TO_UE.
     *
     * We need more variable(detach_type)
     * to keep Detach-Type whether UE-initiated or MME-initiaed.  */
    mme_ue->nas_eps.type = MME_EPS_TYPE_DETACH_REQUEST_FROM_UE;
    mme_ue->detach_type = MME_DETACH_TYPE_REQUEST_FROM_UE;

    mme_ue->nas_eps.ksi = detach_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_EPS TYPE[%d] KSI[%d]",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi);
    ogs_debug("    DETACH TSC[%d] KSI[%d] SWITCH_OFF[%d] VALUE[%d]",
            mme_ue->nas_eps.attach.tsc,
            mme_ue->nas_eps.detach.nas_key_set_identifier,
            mme_ue->nas_eps.detach.switch_off,
            mme_ue->nas_eps.attach.value);

    switch (detach_request->detach_type.value) {
    /* 0 0 1 : EPS detach */
    case OGS_NAS_DETACH_TYPE_FROM_UE_EPS_DETACH:
        ogs_debug("    EPS Detach");
        break;
    /* 0 1 0 : IMSI detach */
    case OGS_NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH:
        ogs_debug("    IMSI Detach");
        break;
    case 6: /* 1 1 0 : reserved */
    case 7: /* 1 1 1 : reserved */
        ogs_warn("Unknown Detach type[%d]",
            detach_request->detach_type.value);
        break;
    /* 0 1 1 : combined EPS/IMSI detach */
    case OGS_NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH:
        ogs_debug("    Combined EPS/IMSI Detach");
    default: /* all other values */
        break;
    }
    if (detach_request->detach_type.switch_off) {
        ogs_debug("    Switch-Off");

        /*
         * Issue #1917
         *
         * When the UE sends a Detach Request with Switch-Off,
         * MME should remove the the stored UE Radio Capability.
         *
         * Otherwise, the Radio Capability will not match
         * because the eNB will not query the Radio Capability
         * when the UE changes USIM.
         */
        OGS_ASN_CLEAR_DATA(&mme_ue->ueRadioCapability);
    }

    return OGS_OK;
}

int emm_handle_service_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_service_request_t *service_request)
{
    ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
                    &service_request->ksi_and_sequence_number;

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    /* Set EPS Service */
    mme_ue->nas_eps.type = MME_EPS_TYPE_SERVICE_REQUEST;
    mme_ue->nas_eps.ksi = ksi_and_sequence_number->ksi;
    ogs_debug("    OGS_NAS_EPS TYPE[%d] KSI[%d]",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi);
    ogs_debug("    SERVICE TSC[%d] KSI[%d] VALUE[%d]",
            mme_ue->nas_eps.service.tsc,
            mme_ue->nas_eps.service.nas_key_set_identifier,
            mme_ue->nas_eps.service.value);

    /*
     * ATTACH_REQUEST
     * TAU_REQUEST
     * SERVICE_REQUEST
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_MME_UE_ALL_TIMERS(mme_ue);

    if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        ogs_kdf_kenb(mme_ue->kasme, mme_ue->ul_count.i32, mme_ue->kenb);
        ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
        mme_ue->nhcc = 1;
    }

    ogs_info("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI[%s]",
            mme_ue->current.guti.mme_gid,
            mme_ue->current.guti.mme_code,
            mme_ue->current.guti.m_tmsi,
            MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown");

    return OGS_OK;
}

bool emm_tau_request_ue_comes_from_gb_or_iu(const ogs_nas_eps_tracking_area_update_request_t *tau_request)
{
    /* "When the tracking area updating procedure is initiated in EMM-IDLE mode
     * to perform an inter-system change from A/Gb mode or Iu mode to S1 mode
     * and the TIN is set to "P-TMSI", the UE shall include the GPRS ciphering
     * key sequence number applicable for A/Gb mode or Iu mode and a nonce UE in
     * the TRACKING AREA UPDATE REQUEST message."
     */
    if (!(tau_request->presencemask &
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT))
            return false;

    if (tau_request->presencemask &
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT) {
            /* 0 = Native, 1 = Mapped */
            return tau_request->old_guti_type.guti_type;
    } else {
        /* TS 23.003 2.8.2.2.2:
            * "The most significant bit of the <LAC> shall be set to zero;
            * and the most significant bit of <MME group id> shall be set to
            * one. Based on this definition, the most significant bit of the
            * <MME group id> can be used to distinguish the node type, i.e.
            * whether it is an MME or SGSN */
        const ogs_nas_eps_mobile_identity_t *eps_mobile_identity = &tau_request->old_guti;
        if (eps_mobile_identity->imsi.type != OGS_NAS_EPS_MOBILE_IDENTITY_GUTI)
            return false;
        return !(eps_mobile_identity->guti.mme_gid & 0x8000);
    }
}

int emm_handle_tau_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_tracking_area_update_request_t *tau_request,
        ogs_pkbuf_t *pkbuf)
{
    int r;
    int served_tai_index = 0;

    ogs_nas_eps_mobile_identity_guti_t *eps_mobile_identity_guti = NULL;
    ogs_nas_eps_guti_t nas_guti;

    ogs_nas_eps_update_type_t *eps_update_type =
                    &tau_request->eps_update_type;
    ogs_nas_eps_mobile_identity_t *eps_mobile_identity =
                    &tau_request->old_guti;

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    /* HashMME */
    ogs_kdf_hash_mme(pkbuf->data, pkbuf->len, mme_ue->hash_mme);

    /* Set EPS Update */
    memcpy(&mme_ue->nas_eps.update, eps_update_type,
            sizeof(ogs_nas_eps_update_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_TAU_REQUEST;
    mme_ue->nas_eps.ksi = eps_update_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_EPS TYPE[%d] KSI[%d]",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi);
    ogs_debug("    UPDATE TSC[%d] KSI[%d] Active-flag[%d] VALUE[%d]",
            mme_ue->nas_eps.update.tsc,
            mme_ue->nas_eps.update.nas_key_set_identifier,
            mme_ue->nas_eps.update.active_flag,
            mme_ue->nas_eps.update.value);

    /*
     * ATTACH_REQUEST
     * TAU_REQUEST
     * SERVICE_REQUEST
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_MME_UE_ALL_TIMERS(mme_ue);

    CLEAR_SERVICE_INDICATOR(mme_ue);
    if (mme_ue->nas_eps.update.active_flag)
        ogs_debug("    Active flag");
    else
        ogs_debug("    No Active flag");

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id), mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&enb_ue->saved.tai.plmn_id),
            enb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from enb_ue */
    mme_ue->enb_ostream_id = enb_ue->enb_ostream_id;
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(ogs_eps_tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));
    mme_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0) {
        /* Send TAU reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
        r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                OGS_NAS_EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    /* Store UE specific information */
    if (tau_request->presencemask &
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        ogs_nas_eps_tai_t *last_visited_registered_tai =
            &tau_request->last_visited_registered_tai;

        ogs_nas_to_plmn_id(&mme_ue->last_visited_plmn_id,
                &last_visited_registered_tai->nas_plmn_id);
        ogs_debug("    Visited_PLMN_ID:%06x",
                ogs_plmn_id_hexdump(&mme_ue->last_visited_plmn_id));
    }

    if (tau_request->presencemask &
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ue_network_capability,
                &tau_request->ue_network_capability,
                sizeof(tau_request->ue_network_capability));
    }

    if (tau_request->presencemask &
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&mme_ue->ms_network_capability,
                &tau_request->ms_network_capability,
                sizeof(tau_request->ms_network_capability));
    }

    if (tau_request->presencemask &
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT) {
        mme_ue->gprs_ciphering_key_sequence_number = tau_request->gprs_ciphering_key_sequence_number.key_sequence;
    } else {
        /* Mark as unavailable, Table 10.5.2/3GPP TS 24.008 */
        mme_ue->gprs_ciphering_key_sequence_number = OGS_NAS_CIPHERING_KEY_SEQUENCE_NUMBER_NO_KEY_FROM_MS;
    }

    if (tau_request->presencemask &
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT) {
        mme_ue->nonceue = tau_request->nonceue;
    }

    /* TODO:
     *   1) Consider if MME is changed or not.
     *   2) Consider if SGW is changed or not.
     */
    switch (eps_mobile_identity->imsi.type) {
    case OGS_NAS_EPS_MOBILE_IDENTITY_GUTI:
        eps_mobile_identity_guti = &eps_mobile_identity->guti;

        nas_guti.nas_plmn_id = eps_mobile_identity_guti->nas_plmn_id;
        nas_guti.mme_gid = eps_mobile_identity_guti->mme_gid;
        nas_guti.mme_code = eps_mobile_identity_guti->mme_code;
        nas_guti.m_tmsi = eps_mobile_identity_guti->m_tmsi;

        ogs_info("    GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
                nas_guti.mme_gid,
                nas_guti.mme_code,
                nas_guti.m_tmsi,
                MME_UE_HAVE_IMSI(mme_ue)
                    ? mme_ue->imsi_bcd : "Unknown");

        memcpy(&mme_ue->next.guti,
           &nas_guti, sizeof(ogs_nas_eps_guti_t));

        break;
    default:
        ogs_error("Not implemented[%d]", eps_mobile_identity->imsi.type);

        return OGS_OK;
    }

    return OGS_OK;
}

int emm_handle_extended_service_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_extended_service_request_t *extended_service_request)
{
    int r;
    int served_tai_index = 0;

    ogs_nas_service_type_t *service_type =
        &extended_service_request->service_type;
    ogs_nas_mobile_identity_t *mobile_identity =
        &extended_service_request->m_tmsi;
    ogs_nas_mobile_identity_tmsi_t *mobile_identity_tmsi = NULL;

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    /* Set Service Type */
    memcpy(&mme_ue->nas_eps.service, service_type,
            sizeof(ogs_nas_service_type_t));
    mme_ue->nas_eps.type = MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST;
    mme_ue->nas_eps.ksi = service_type->nas_key_set_identifier;
    ogs_debug("    OGS_NAS_EPS TYPE[%d] KSI[%d]",
            mme_ue->nas_eps.type, mme_ue->nas_eps.ksi);

    /*
     * ATTACH_REQUEST
     * TAU_REQUEST
     * SERVICE_REQUEST
     * EXTENDED_SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_MME_UE_ALL_TIMERS(mme_ue);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id), mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&enb_ue->saved.tai.plmn_id),
            enb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from enb_ue */
    mme_ue->enb_ostream_id = enb_ue->enb_ostream_id;
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(ogs_eps_tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));
    mme_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    if (served_tai_index < 0) {
        /* Send TAU reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id), mme_ue->tai.tac);
        r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                OGS_NAS_EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    switch(mobile_identity->tmsi.type) {
    case OGS_NAS_MOBILE_IDENTITY_TMSI:
        mobile_identity_tmsi = &mobile_identity->tmsi;

        ogs_info("    M-TMSI:[0x%x] IMSI:[%s]",
                mobile_identity_tmsi->tmsi,
                MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown");
        break;
    default:
        ogs_error("Unknown TMSI type [%d]", mobile_identity->tmsi.type);
        break;
    }

    return OGS_OK;
}

int emm_handle_security_mode_complete(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        ogs_nas_eps_security_mode_complete_t *security_mode_complete)
{
    ogs_nas_mobile_identity_t *imeisv = &security_mode_complete->imeisv;

    ogs_assert(mme_ue);
    ogs_assert(enb_ue);

    if (security_mode_complete->presencemask &
        OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT) {
        switch (imeisv->imeisv.type) {
        case OGS_NAS_MOBILE_IDENTITY_IMEISV:
            /* TS23.003 6.2.2 Composition of IMEISV
             *
             * The International Mobile station Equipment Identity and
             * Software Version Number (IMEISV) is composed.
             *
             * TAC(8 digits) - SNR(6 digits) - SVN(2 digits)
             * IMEISV(16 digits) ==> 8bytes
             */
            if (imeisv->length == sizeof(ogs_nas_mobile_identity_imeisv_t)) {
                memcpy(&mme_ue->nas_mobile_identity_imeisv,
                    &imeisv->imeisv, imeisv->length);
                ogs_nas_imeisv_to_bcd(&imeisv->imeisv, imeisv->length,
                        mme_ue->imeisv_bcd);
                ogs_bcd_to_buffer(mme_ue->imeisv_bcd,
                        mme_ue->imeisv, &mme_ue->imeisv_len);
                ogs_nas_imeisv_bcd_to_buffer(mme_ue->imeisv_bcd,
                        mme_ue->masked_imeisv, &mme_ue->masked_imeisv_len);
                mme_ue->masked_imeisv[5] = 0xff;
                mme_ue->masked_imeisv[6] = 0xff;
            } else {
                ogs_error("[%s] Unknown IMEISV Length [%d]",
                        mme_ue->imsi_bcd, imeisv->length);
                ogs_log_hexdump(OGS_LOG_ERROR,
                        (unsigned char *)&imeisv->imeisv, imeisv->length);
            }
            break;
        default:
            ogs_warn("Invalid IMEISV Type[%d]", imeisv->imeisv.type);
            break;

        }

    }

    return OGS_OK;
}

static uint8_t emm_cause_from_access_control(mme_ue_t *mme_ue)
{
    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int i;

    ogs_assert(mme_ue);
    nas_mobile_identity_imsi = &mme_ue->nas_mobile_identity_imsi;

    /* No Access Control */
    if (mme_self()->num_of_access_control == 0)
        return OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;

    for (i = 0; i < mme_self()->num_of_access_control; i++) {
        if ((nas_mobile_identity_imsi->digit1 ==
                mme_self()->access_control[i].plmn_id.mcc1 &&
             nas_mobile_identity_imsi->digit2 ==
                mme_self()->access_control[i].plmn_id.mcc2 &&
             nas_mobile_identity_imsi->digit3 ==
                mme_self()->access_control[i].plmn_id.mcc3) &&
           ((nas_mobile_identity_imsi->digit4 ==
                mme_self()->access_control[i].plmn_id.mnc2 &&
             nas_mobile_identity_imsi->digit5 ==
                 mme_self()->access_control[i].plmn_id.mnc3) ||
            (nas_mobile_identity_imsi->digit4 ==
                 mme_self()->access_control[i].plmn_id.mnc1 &&
             nas_mobile_identity_imsi->digit5 ==
                 mme_self()->access_control[i].plmn_id.mnc2 &&
             nas_mobile_identity_imsi->digit6 ==
                 mme_self()->access_control[i].plmn_id.mnc3))) {
            if (mme_self()->access_control[i].reject_cause)
                return mme_self()->access_control[i].reject_cause;
            else
                return OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED;
        }
    }

    if (mme_self()->default_reject_cause)
        return mme_self()->default_reject_cause;

    return OGS_NAS_EMM_CAUSE_PLMN_NOT_ALLOWED;
}
