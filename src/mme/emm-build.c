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

#include "nas-security.h"
#include "emm-build.h"
#include "mme-sm.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __emm_log_domain

ogs_pkbuf_t *emm_build_attach_accept(
        mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf)
{
    ogs_nas_eps_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_eps_attach_accept_t *attach_accept = &message.emm.attach_accept;
    ogs_nas_eps_attach_result_t *eps_attach_result = 
        &attach_accept->eps_attach_result;
    ogs_nas_gprs_timer_t *t3412_value = &attach_accept->t3412_value;
    int served_tai_index = 0;
    ogs_nas_eps_mobile_identity_t *nas_guti = &attach_accept->guti;
    ogs_nas_eps_network_feature_support_t *eps_network_feature_support =
        &attach_accept->eps_network_feature_support;
    ogs_nas_location_area_identification_t *lai =
        &attach_accept->location_area_identification;
    ogs_nas_mobile_identity_t *ms_identity = &attach_accept->ms_identity;
    ogs_nas_mobile_identity_tmsi_t *tmsi = &ms_identity->tmsi;;

    ogs_assert(mme_ue);
    ogs_assert(esmbuf);

    ogs_debug("Attach accept");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_ATTACH_ACCEPT;

    /* Set T3412 */
    eps_attach_result->result = mme_ue->nas_eps.attach.value;
    t3412_value->unit = OGS_NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    t3412_value->value = 9;

    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id),
            mme_ue->tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id),
            mme_ue->e_cgi.cell_id);
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
    ogs_assert(served_tai_index >= 0 &&
            served_tai_index < OGS_MAX_NUM_OF_SERVED_TAI);
    ogs_nas_tai_list_build(&attach_accept->tai_list,
            &mme_self()->served_tai[served_tai_index].list0,
            &mme_self()->served_tai[served_tai_index].list2);

    attach_accept->esm_message_container.buffer = esmbuf->data;
    attach_accept->esm_message_container.length = esmbuf->len;

    ogs_debug("    %s GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
            mme_ue->guti_present ? "[V]" : "[N]",
            mme_ue->guti.mme_gid, mme_ue->guti.mme_code,
            mme_ue->guti.m_tmsi, mme_ue->imsi_bcd);
    if (mme_ue->guti_present) {
        attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_PRESENT;
        nas_guti->length = sizeof(ogs_nas_eps_mobile_identity_guti_t);
        nas_guti->guti.odd_even = OGS_NAS_MOBILE_IDENTITY_EVEN;
        nas_guti->guti.type = OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
        nas_guti->guti.nas_plmn_id = mme_ue->guti.nas_plmn_id;
        nas_guti->guti.mme_gid = mme_ue->guti.mme_gid;
        nas_guti->guti.mme_code = mme_ue->guti.mme_code;
        nas_guti->guti.m_tmsi = mme_ue->guti.m_tmsi;
    }
    mme_ue->guti_present = 0;

#if 0 /* Need not to include T3402 */
    /* Set T3402 */
    attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_PRESENT;
    attach_accept->t3402_value.unit = OGS_NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    attach_accept->t3402_value.value = 12;
#endif

    /* Set T3423 */
    attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_PRESENT;
    attach_accept->t3423_value.unit = OGS_NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    attach_accept->t3423_value.value = 9;

    attach_accept->presencemask |= 
        OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    if (ogs_app()->parameter.use_openair == false) {
        eps_network_feature_support->length = 2;
    } else {
        eps_network_feature_support->length = 1;
    }
    eps_network_feature_support->ims_voice_over_ps_session_in_s1_mode = 1;
    eps_network_feature_support->extended_protocol_configuration_options = 1;

    if (MME_P_TMSI_IS_AVAILABLE(mme_ue)) {
        ogs_assert(mme_ue->csmap);
        ogs_assert(mme_ue->p_tmsi);

        attach_accept->presencemask |=
            OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
        lai->nas_plmn_id = mme_ue->csmap->lai.nas_plmn_id;
        lai->lac = mme_ue->csmap->lai.lac;
        ogs_debug("    LAI[PLMN_ID:%06x,LAC:%d]",
                ogs_plmn_id_hexdump(&lai->nas_plmn_id), lai->lac);

        attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT;
        ms_identity->length = 5;
        tmsi->spare = 0xf;
        tmsi->odd_even = 0;
        tmsi->type = OGS_NAS_MOBILE_IDENTITY_TMSI;
        tmsi->tmsi = mme_ue->p_tmsi;
        ogs_debug("    P-TMSI: 0x%08x", tmsi->tmsi);
    }

    pkbuf = nas_eps_security_encode(mme_ue, &message);
    ogs_pkbuf_free(esmbuf);

    return pkbuf;
}

ogs_pkbuf_t *emm_build_attach_reject(
        ogs_nas_emm_cause_t emm_cause, ogs_pkbuf_t *esmbuf)
{
    ogs_nas_eps_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_eps_attach_reject_t *attach_reject = &message.emm.attach_reject;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_ATTACH_REJECT;

    attach_reject->emm_cause = emm_cause;

    if (esmbuf) {
        attach_reject->presencemask |=
            OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT;
        attach_reject->esm_message_container.buffer = esmbuf->data;
        attach_reject->esm_message_container.length = esmbuf->len;
    }

    pkbuf = ogs_nas_eps_plain_encode(&message);
    if (esmbuf)
        ogs_pkbuf_free(esmbuf);

    return pkbuf;
}

ogs_pkbuf_t *emm_build_identity_request(mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_identity_request_t *identity_request = 
        &message.emm.identity_request;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_IDENTITY_REQUEST;

    /* Request IMSI */
    ogs_debug("    Identity Type 2 : IMSI");
    identity_request->identity_type.type = OGS_NAS_IDENTITY_TYPE_2_IMSI;

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *emm_build_authentication_request(mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_AUTHENTICATION_REQUEST;

    authentication_request->nas_key_set_identifierasme.tsc = 0;
    authentication_request->nas_key_set_identifierasme.value =
        mme_ue->nas_eps.ksi;
    memcpy(authentication_request->authentication_parameter_rand.rand,
            mme_ue->rand, OGS_RAND_LEN);
    memcpy(authentication_request->authentication_parameter_autn.autn,
            mme_ue->autn, OGS_AUTN_LEN);
    authentication_request->authentication_parameter_autn.length = 
            OGS_AUTN_LEN;

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *emm_build_authentication_reject(void)
{
    ogs_nas_eps_message_t message;

    memset(&message, 0, sizeof(message));

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_AUTHENTICATION_REJECT;

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *emm_build_security_mode_command(mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_security_mode_command_t *security_mode_command = 
        &message.emm.security_mode_command;
    ogs_nas_security_algorithms_t *selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    ogs_nas_key_set_identifier_t *nas_key_set_identifier =
        &security_mode_command->nas_key_set_identifier;
    ogs_nas_ue_security_capability_t *replayed_ue_security_capabilities = 
        &security_mode_command->replayed_ue_security_capabilities;
    ogs_nas_imeisv_request_t *imeisv_request =
        &security_mode_command->imeisv_request;
    ogs_nas_hashmme_t *hashmme = &security_mode_command->hashmme;
    ogs_nas_ue_additional_security_capability_t
        *replayed_ue_additional_security_capability =
            &security_mode_command->replayed_ue_additional_security_capability;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_SECURITY_MODE_COMMAND;

    mme_ue->selected_int_algorithm = mme_selected_int_algorithm(mme_ue);
    mme_ue->selected_enc_algorithm = mme_selected_enc_algorithm(mme_ue);

    selected_nas_security_algorithms->type_of_integrity_protection_algorithm =
        mme_ue->selected_int_algorithm;
    selected_nas_security_algorithms->type_of_ciphering_algorithm =
        mme_ue->selected_enc_algorithm;

    nas_key_set_identifier->tsc = 0;
    nas_key_set_identifier->value = 0;

    replayed_ue_security_capabilities->eea = mme_ue->ue_network_capability.eea;
    replayed_ue_security_capabilities->eia = mme_ue->ue_network_capability.eia;
    replayed_ue_security_capabilities->uea = mme_ue->ue_network_capability.uea;
    replayed_ue_security_capabilities->uia = 
        mme_ue->ue_network_capability.uia & 0x7f;
    replayed_ue_security_capabilities->gea = 
        (mme_ue->ms_network_capability.gea1 << 6) | 
        mme_ue->ms_network_capability.extended_gea;

    replayed_ue_security_capabilities->length =
        sizeof(replayed_ue_security_capabilities->eea) +
        sizeof(replayed_ue_security_capabilities->eia);
    if (replayed_ue_security_capabilities->uea ||
        replayed_ue_security_capabilities->uia)
        replayed_ue_security_capabilities->length =
            sizeof(replayed_ue_security_capabilities->eea) +
            sizeof(replayed_ue_security_capabilities->eia) +
            sizeof(replayed_ue_security_capabilities->uea) +
            sizeof(replayed_ue_security_capabilities->uia);
    if (replayed_ue_security_capabilities->gea)
        replayed_ue_security_capabilities->length =
            sizeof(replayed_ue_security_capabilities->eea) +
            sizeof(replayed_ue_security_capabilities->eia) +
            sizeof(replayed_ue_security_capabilities->uea) +
            sizeof(replayed_ue_security_capabilities->uia) +
            sizeof(replayed_ue_security_capabilities->gea);
    ogs_debug("    Replayed UE SEC[LEN:%d EEA:0x%x EIA:0x%x UEA:0x%x "
            "UIA:0x%x GEA:0x%x]",
            replayed_ue_security_capabilities->length,
            replayed_ue_security_capabilities->eea,
            replayed_ue_security_capabilities->eia,
            replayed_ue_security_capabilities->uea,
            replayed_ue_security_capabilities->uia,
            replayed_ue_security_capabilities->gea);
    ogs_debug("    Selected[Integrity:0x%x Encrypt:0x%x]",
            mme_ue->selected_int_algorithm, mme_ue->selected_enc_algorithm);

    security_mode_command->presencemask |=
        OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT;
    imeisv_request->type = OGS_NAS_IMEISV_TYPE;
    imeisv_request->value = OGS_NAS_IMEISV_REQUESTED;

    /*
     * TS24.301
     * 5.4.3.2 NAS security mode control initiation by the network
     *
     * If, during an ongoing attach or tracking area updating procedure,
     * the MME is initiating a SECURITY MODE COMMAND (i.e. after receiving
     * the ATTACH REQUEST or TRACKING AREA UPDATE REQUEST message,
     * but before sending a response to that message) and the ATTACH REQUEST
     * or TRACKING AREA UPDATE REQUEST message is received without integrity
     * protection or does not successfully pass the integrity check at the MME,
     * the MME shall calculate the HASH MME of the entire plain ATTACH REQUEST
     * or TRACKING AREA UPDATE REQUEST message as described
     * in 3GPP TS 33.401 [19] and shall include the HASH MME
     * in the SECURITY MODE COMMAND message
     *
     * However, Openair UE does not support HashMME. For user convenience,
     * we added a way not to include HashMME through the configuration file.
     */
    if (ogs_app()->parameter.use_openair == false) {
        security_mode_command->presencemask |=
            OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_PRESENT;
        hashmme->length = OGS_HASH_MME_LEN;
        memcpy(hashmme->value, mme_ue->hash_mme, hashmme->length);
    }

    if (mme_ue->ue_additional_security_capability.length) {
        security_mode_command->presencemask |=
            OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT;
        memcpy(replayed_ue_additional_security_capability,
                &mme_ue->ue_additional_security_capability,
                sizeof(mme_ue->ue_additional_security_capability));
    }

    if (mme_ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_EIA0) {
        ogs_error("Encrypt[0x%x] can be skipped with EEA0, "
            "but Integrity[0x%x] cannot be bypassed with EIA0",
            mme_ue->selected_enc_algorithm, mme_ue->selected_int_algorithm);
        return NULL;
    }

    ogs_kdf_nas_eps(OGS_KDF_NAS_INT_ALG, mme_ue->selected_int_algorithm,
            mme_ue->kasme, mme_ue->knas_int);
    ogs_kdf_nas_eps(OGS_KDF_NAS_ENC_ALG, mme_ue->selected_enc_algorithm,
            mme_ue->kasme, mme_ue->knas_enc);

    return nas_eps_security_encode(mme_ue, &message);
}

ogs_pkbuf_t *emm_build_detach_accept(mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    ogs_debug("Detach accept");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_DETACH_ACCEPT;

    return nas_eps_security_encode(mme_ue, &message);
}

ogs_pkbuf_t *emm_build_tau_accept(mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_tracking_area_update_accept_t *tau_accept = 
        &message.emm.tracking_area_update_accept;
    int served_tai_index = 0;

    mme_sess_t *sess = NULL;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT;

    if (mme_ue->nas_eps.update.value ==
            OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING ||
        mme_ue->nas_eps.update.value ==
            OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING_WITH_IMSI_ATTACH) {
        tau_accept->eps_update_result.result =
            OGS_NAS_EPS_UPDATE_RESULT_COMBINED_TA_LA_UPDATED;
    } else {
        tau_accept->eps_update_result.result =
            OGS_NAS_EPS_UPDATE_RESULT_TA_UPDATED;
    }

    /* Set T3412 */
    tau_accept->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT ;
    tau_accept->t3412_value.unit = OGS_NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    tau_accept->t3412_value.value = 9;

    /* Set TAI */
    tau_accept->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT;

    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id),
            mme_ue->tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id),
            mme_ue->e_cgi.cell_id);
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
    ogs_assert(served_tai_index >= 0 &&
            served_tai_index < OGS_MAX_NUM_OF_SERVED_TAI);
    ogs_nas_tai_list_build(&tau_accept->tai_list,
            &mme_self()->served_tai[served_tai_index].list0,
            &mme_self()->served_tai[served_tai_index].list2);

    /* Set EPS bearer context status */
    tau_accept->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;
    tau_accept->eps_bearer_context_status.length = 2;
    sess = mme_sess_first(mme_ue);
    while (sess) {
        mme_bearer_t *bearer = mme_bearer_first(sess);
        while (bearer) {
            switch (bearer->ebi) {
            case 5: tau_accept->eps_bearer_context_status.ebi5 = 1; break;
            case 6: tau_accept->eps_bearer_context_status.ebi6 = 1; break;
            case 7: tau_accept->eps_bearer_context_status.ebi7 = 1; break;
            case 8: tau_accept->eps_bearer_context_status.ebi8 = 1; break;
            case 9: tau_accept->eps_bearer_context_status.ebi9 = 1; break;
            case 10: tau_accept->eps_bearer_context_status.ebi10 = 1; break;
            case 11: tau_accept->eps_bearer_context_status.ebi11 = 1; break;
            case 12: tau_accept->eps_bearer_context_status.ebi12 = 1; break;
            case 13: tau_accept->eps_bearer_context_status.ebi13 = 1; break;
            case 14: tau_accept->eps_bearer_context_status.ebi14 = 1; break;
            case 15: tau_accept->eps_bearer_context_status.ebi15 = 1; break;
            default: break;
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

#if 0 /* Need not to include T3402 */
    /* Set T3402 */
    tau_accept->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT;
    tau_accept->t3402_value.unit = OGS_NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    tau_accept->t3402_value.value = 12;
#endif

    /* Set T3423 */
    tau_accept->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT;
    tau_accept->t3423_value.unit = OGS_NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    tau_accept->t3423_value.value = 9;

    /* Set EPS network feature support */
    tau_accept->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    if (ogs_app()->parameter.use_openair == false) {
        tau_accept->eps_network_feature_support.length = 2;
    } else {
        tau_accept->eps_network_feature_support.length = 1;
    }
    tau_accept->eps_network_feature_support.
        ims_voice_over_ps_session_in_s1_mode = 1;
    tau_accept->eps_network_feature_support.
        extended_protocol_configuration_options = 1;

    return nas_eps_security_encode(mme_ue, &message);
}

ogs_pkbuf_t *emm_build_tau_reject(
        ogs_nas_emm_cause_t emm_cause, mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_tracking_area_update_reject_t *tau_reject = 
        &message.emm.tracking_area_update_reject;

    ogs_assert(mme_ue);

    ogs_debug("Tracking area update reject");
    ogs_debug("    IMSI[%s] Cause[%d]",
            MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown", emm_cause);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT;

    tau_reject->emm_cause = emm_cause;

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *emm_build_service_reject(
        ogs_nas_emm_cause_t emm_cause, mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_service_reject_t *service_reject = &message.emm.service_reject;

    ogs_assert(mme_ue);

    ogs_debug("Service reject");
    ogs_debug("    Cause[%d]", emm_cause);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_SERVICE_REJECT;

    service_reject->emm_cause = emm_cause;

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *emm_build_cs_service_notification(mme_ue_t *mme_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_cs_service_notification_t *cs_service_notification = 
        &message.emm.cs_service_notification;
    ogs_nas_paging_identity_t *paging_identity =
        &cs_service_notification->paging_identity;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_CS_SERVICE_NOTIFICATION;

    /* FIXME : Does it right to use TMSI */
    paging_identity->identity = OGS_NAS_PAGING_IDENTITY_TMSI;
    ogs_debug("    Paging Identity[%d]", paging_identity->identity);

    /* FIXME : What optional filed should be included in this message? */  

    return nas_eps_security_encode(mme_ue, &message);
}

ogs_pkbuf_t *emm_build_downlink_nas_transport(
        mme_ue_t *mme_ue, uint8_t *buffer, uint8_t length)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_downlink_nas_transport_t *downlink_nas_transport = 
        &message.emm.downlink_nas_transport;
    ogs_nas_eps_message_container_t *nas_message_container =
        &downlink_nas_transport->nas_message_container;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_DOWNLINK_NAS_TRANSPORT;

    nas_message_container->length = length;
    memcpy(nas_message_container->buffer, buffer, length);

    return nas_eps_security_encode(mme_ue, &message);
}
