/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of OpenEPS.
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

#include "test-common.h"

ogs_pkbuf_t *testemm_build_attach_request(
        test_ue_t *test_ue, ogs_pkbuf_t *esmbuf,
        bool integrity_protected, bool ciphered)
{
    int i;
    uint16_t psimask = 0;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_eps_message_t message;
    ogs_nas_eps_attach_request_t *attach_request = &message.emm.attach_request;

    ogs_nas_eps_attach_type_t *eps_attach_type =
        &attach_request->eps_attach_type;
    ogs_nas_eps_mobile_identity_t *eps_mobile_identity =
        &attach_request->eps_mobile_identity;
    ogs_nas_ue_network_capability_t *ue_network_capability =
        &attach_request->ue_network_capability;
    ogs_nas_esm_message_container_t *esm_message_container =
        &attach_request->esm_message_container;

    ogs_nas_tracking_area_identity_t *last_visited_registered_tai =
        &attach_request->last_visited_registered_tai;
    ogs_nas_drx_parameter_t *drx_parameter = &attach_request->drx_parameter;
    ogs_nas_ms_network_capability_t *ms_network_capability =
        &attach_request->ms_network_capability;
    ogs_nas_location_area_identification_t *old_location_area_identification =
        &attach_request->old_location_area_identification;
    ogs_nas_tmsi_status_t *tmsi_status = &attach_request->tmsi_status;
    ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2 =
        &attach_request->mobile_station_classmark_2;
    ogs_nas_mobile_station_classmark_3_t *mobile_station_classmark_3 =
        &attach_request->mobile_station_classmark_3;
    ogs_nas_supported_codec_list_t *supported_codecs =
        &attach_request->supported_codecs;
    ogs_nas_additional_update_type_t *additional_update_type =
        &attach_request->additional_update_type;
    ogs_nas_voice_domain_preference_and_ue_usage_setting_t
        *voice_domain_preference_and_ue_usage_setting =
            &attach_request->voice_domain_preference_and_ue_usage_setting;
    ogs_nas_guti_type_t *old_guti_type = &attach_request->old_guti_type;
    ogs_nas_ms_network_feature_support_t *ms_network_feature_support =
            &attach_request->ms_network_feature_support;
    ogs_nas_network_resource_identifier_container_t *tmsi_based_nri_container =
        &attach_request->tmsi_based_nri_container;
    ogs_nas_ue_additional_security_capability_t
        *ue_additional_security_capability =
            &attach_request->ue_additional_security_capability;

    uint8_t classmark_3[11] = "\x60\x14\x04\xef\x65\x23\x3b\x88\x78\xd2\x90";

    ogs_assert(test_ue);
    ogs_assert(esmbuf);

    memset(&message, 0, sizeof(message));
    if (integrity_protected) {
        if (ciphered)
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        else
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
        message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_ATTACH_REQUEST;

    esm_message_container->length = esmbuf->len;
    esm_message_container->buffer = esmbuf->data;

    memcpy(eps_attach_type, &test_ue->nas.data, sizeof(*eps_attach_type));

    if (test_ue->attach_request_param.guti) {
        eps_mobile_identity->length =
            sizeof(ogs_nas_eps_mobile_identity_guti_t);
        eps_mobile_identity->guti.odd_even = OGS_NAS_MOBILE_IDENTITY_EVEN;
        eps_mobile_identity->guti.type = OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
        eps_mobile_identity->guti.nas_plmn_id =
            test_ue->nas_eps_guti.nas_plmn_id;
        eps_mobile_identity->guti.mme_gid = test_ue->nas_eps_guti.mme_gid;
        eps_mobile_identity->guti.mme_code = test_ue->nas_eps_guti.mme_code;
        eps_mobile_identity->guti.m_tmsi = test_ue->nas_eps_guti.m_tmsi;
    } else {
        eps_mobile_identity->length =
            sizeof(ogs_nas_mobile_identity_imsi_t);
        memcpy(&eps_mobile_identity->imsi,
                &test_ue->mobile_identity_imsi, eps_mobile_identity->length);
    }

    ue_network_capability->length = 7;
    ue_network_capability->eea = 0xf0;
    ue_network_capability->eia = 0xf0;
    ue_network_capability->uea = 0xc0;
    ue_network_capability->uia = 0x40;
    ue_network_capability->notification_procedure = 1;
    ue_network_capability->dual_connectivity_with_nr = 1;

    if (test_ue->attach_request_param.last_visited_registered_tai) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
        ogs_nas_from_plmn_id(&last_visited_registered_tai->nas_plmn_id,
            &test_ue->e_tai.plmn_id);
        last_visited_registered_tai->tac = test_ue->e_tai.tac;
    }

    if (test_ue->attach_request_param.drx_parameter) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT;
        drx_parameter->split_pg_cycle_code = 32;
    }

    if (test_ue->attach_request_param.ms_network_capability) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT;
        ms_network_capability->length = 3;
        ms_network_capability->gea1 = 1;
        ms_network_capability->sm_capabilities_via_dedicated_channels = 1;
        ms_network_capability->sm_capabilities_via_gprs_channels = 1;
        ms_network_capability->ss_screening_indicator = 1;
        ms_network_capability->revision_level_indicator = 1;
        ms_network_capability->pfc_feature_mode = 1;
        ms_network_capability->extended_gea = 0x30;
        ms_network_capability->emm_combined_procedures_capability = 1;
        ms_network_capability->isr_support = 1;
        ms_network_capability->epc_capability = 1;
    }

    if (test_ue->attach_request_param.location_area_identication) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT;
        ogs_nas_from_plmn_id(&old_location_area_identification->nas_plmn_id,
            &test_ue->e_tai.plmn_id);
        old_location_area_identification->lac = 1;
    }

    if (test_ue->attach_request_param.tmsi_status) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_PRESENT;
    }

    if (test_ue->attach_request_param.mobile_station_classmark_2) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
        mobile_station_classmark_2->length = 3;
        mobile_station_classmark_2->revision_level = 2;
        mobile_station_classmark_2->es_ind = 1;
        mobile_station_classmark_2->a5_1 = 1;
        mobile_station_classmark_2->rf_power_capability = 7;
        mobile_station_classmark_2->ps_capa = 1;
        mobile_station_classmark_2->ss_screen_indicator = 1;
        mobile_station_classmark_2->sm_capabi = 1;
        mobile_station_classmark_2->cm3 = 1;
        mobile_station_classmark_2->lcsva_cap = 1;
        mobile_station_classmark_2->cmsp = 1;
        mobile_station_classmark_2->a5_3 = 1;
    }

    if (test_ue->attach_request_param.mobile_station_classmark_3) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT;
        mobile_station_classmark_3->length = 11;
        memcpy(mobile_station_classmark_3->buffer,
                classmark_3, mobile_station_classmark_3->length);
    }

    if (test_ue->attach_request_param.supported_codecs) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT;
        supported_codecs->length = 8;
        supported_codecs->item[0].system_identification = 0x04;
        supported_codecs->item[0].length_of_bitmap = 2;
        supported_codecs->item[0].codec_bitmap = htobe16(0x6004);
        supported_codecs->item[1].system_identification = 0;
        supported_codecs->item[1].length_of_bitmap = 2;
        supported_codecs->item[1].codec_bitmap = htobe16(0x1f02);
    }

    if (test_ue->attach_request_param.additional_update_type) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT;
        additional_update_type->autv = 1;
    }

    if (test_ue->attach_request_param.ue_usage_setting) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT;
        voice_domain_preference_and_ue_usage_setting->length = 1;
        voice_domain_preference_and_ue_usage_setting->ue_usage_setting = 1;
        voice_domain_preference_and_ue_usage_setting->
            voice_domain_preference_for_e_utran = 1;
    }

    if (test_ue->attach_request_param.old_guti_type) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT;
    }

    if (test_ue->attach_request_param.ms_network_feature_support) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT;
        ms_network_feature_support->extended_periodic_timers = 1;
    }

    if (test_ue->attach_request_param.nri_container) {
        attach_request->presencemask |=
            OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT;
        tmsi_based_nri_container->length = 2;
        tmsi_based_nri_container->nri_container_value1 = 0x8d;
        tmsi_based_nri_container->nri_container_value2 = 0x3;
    }

    if (test_ue->attach_request_param.ue_additional_security_capability) {
        attach_request->presencemask |=
        OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT;
        ue_additional_security_capability->length = 4;
        ue_additional_security_capability->nea = 0xf0;
        ue_additional_security_capability->nia = 0xf0;
    }

    if (integrity_protected)
        pkbuf = test_nas_eps_security_encode(test_ue, &message);
    else
        pkbuf = ogs_nas_eps_plain_encode(&message);

    ogs_pkbuf_free(esmbuf);

    return pkbuf;
}

ogs_pkbuf_t *testemm_build_identity_response(test_ue_t *test_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_identity_response_t *identity_response =
            &message.emm.identity_response;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator =
            OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_IDENTITY_RESPONSE;

    identity_response->mobile_identity.length =
            sizeof(ogs_nas_mobile_identity_imsi_t);
    memcpy(&identity_response->mobile_identity.imsi,
            &test_ue->mobile_identity_imsi,
            identity_response->mobile_identity.length);

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testemm_build_authentication_response(test_ue_t *test_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_authentication_response_t *authentication_response =
            &message.emm.authentication_response;
    ogs_nas_authentication_response_parameter_t
        *authentication_response_parameter =
            &authentication_response->authentication_response_parameter;

    uint8_t ik[OGS_KEY_LEN];
    uint8_t ck[OGS_KEY_LEN];
    uint8_t ak[OGS_AK_LEN];
    uint8_t sqn[OGS_SQN_LEN];
    uint8_t res[OGS_MAX_RES_LEN];

    ogs_nas_plmn_id_t nas_plmn_id;
    uint8_t kasme[32];

    int i;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_AUTHENTICATION_RESPONSE;

    OGS_HEX(test_ue->k_string, strlen(test_ue->k_string), test_ue->k);
    OGS_HEX(test_ue->opc_string, strlen(test_ue->opc_string), test_ue->opc);

    milenage_f2345(test_ue->opc, test_ue->k, test_ue->rand,
            res, ck, ik, ak, NULL);

	for (i = 0; i < 6; i++)
		sqn[i] = test_ue->autn[i] ^ ak[i];

    ogs_nas_from_plmn_id(&nas_plmn_id, &test_ue->e_tai.plmn_id);
    ogs_auc_kasme(ck, ik, &nas_plmn_id, sqn, ak, test_ue->kasme);

    authentication_response_parameter->length = 8;
    memcpy(authentication_response_parameter->res, res, 8);

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testemm_build_authentication_failure(
        test_ue_t *test_ue, ogs_nas_emm_cause_t emm_cause, uint64_t sqn_ms)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_authentication_failure_t *authentication_failure =
            &message.emm.authentication_failure;
    ogs_nas_authentication_failure_parameter_t
        *authentication_failure_parameter =
            &authentication_failure->authentication_failure_parameter;

    uint8_t ak[OGS_AK_LEN];
#if 0
    uint8_t sqn_ms[OGS_SQN_LEN] = "\x00\x00\x11\x22\x33\x44";
    uint8_t sqn_ms[OGS_SQN_LEN] = "\x00\x00\x00\x00\x1f\x60"; /* Issues 482 */
#endif
    uint8_t sqn[OGS_SQN_LEN];
    uint8_t mac_s[OGS_MAC_S_LEN];
    uint8_t amf[2] = { 0, 0 };
    uint8_t auts[OGS_AUTS_LEN];
    int i = 0;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_AUTHENTICATION_FAILURE;

    authentication_failure->emm_cause = emm_cause;

    if (emm_cause == EMM_CAUSE_SYNCH_FAILURE) {
        authentication_failure->presencemask |=
            OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT;

        OGS_HEX(test_ue->k_string, strlen(test_ue->k_string), test_ue->k);
        OGS_HEX(test_ue->opc_string, strlen(test_ue->opc_string), test_ue->opc);

        milenage_f2345(test_ue->opc, test_ue->k, test_ue->rand,
                NULL, NULL, NULL, NULL, ak);

        ogs_uint64_to_buffer(sqn_ms, 6, sqn);
        milenage_f1(test_ue->opc, test_ue->k, test_ue->rand,
                sqn, amf, NULL, auts + OGS_SQN_LEN);
        for (i = 0; i < OGS_SQN_LEN; i++)
            auts[i] = sqn[i] ^ ak[i];

        authentication_failure_parameter->length = OGS_AUTS_LEN;
        memcpy(authentication_failure_parameter->auts, auts,
                        authentication_failure_parameter->length);
    }

    return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testemm_build_security_mode_complete(test_ue_t *test_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_security_mode_complete_t *security_mode_complete =
            &message.emm.security_mode_complete;
    ogs_nas_mobile_identity_t *imeisv = &security_mode_complete->imeisv;

    ogs_nas_mobile_identity_imeisv_t mobile_identity_imeisv;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_SECURITY_MODE_COMPLETE;

    if (test_ue->mobile_identity_imeisv_presence) {
        security_mode_complete->presencemask |=
            OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT;
        memset(&mobile_identity_imeisv, 0, sizeof(mobile_identity_imeisv));
        imeisv->length = sizeof(test_ue->mobile_identity_imeisv);
        memcpy(&imeisv->imeisv,
                &test_ue->mobile_identity_imeisv, imeisv->length);
        imeisv->imeisv.type = OGS_NAS_MOBILE_IDENTITY_IMEISV;
    }

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testemm_build_attach_complete(
        test_ue_t *test_ue, ogs_pkbuf_t *esmbuf)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_eps_message_t message;
    ogs_nas_eps_attach_complete_t *attach_complete =
        &message.emm.attach_complete;

    ogs_nas_esm_message_container_t *esm_message_container =
        &attach_complete->esm_message_container;

    ogs_assert(test_ue);
    ogs_assert(esmbuf);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_ATTACH_COMPLETE;

    esm_message_container->length = esmbuf->len;
    esm_message_container->buffer = esmbuf->data;

    pkbuf = test_nas_eps_security_encode(test_ue, &message);

    ogs_pkbuf_free(esmbuf);

    return pkbuf;
}

ogs_pkbuf_t *testemm_build_tau_complete(test_ue_t *test_ue)
{
    ogs_nas_eps_message_t message;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testemm_build_detach_request(
        test_ue_t *test_ue, bool switch_off,
        bool integrity_protected, bool ciphered)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_detach_request_from_ue_t *detach_request =
        &message.emm.detach_request_from_ue;

    ogs_nas_detach_type_t *detach_type = &detach_request->detach_type;
    ogs_nas_eps_mobile_identity_t *eps_mobile_identity =
        &detach_request->eps_mobile_identity;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    if (integrity_protected) {
        if (ciphered)
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        else
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
        message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_DETACH_REQUEST;

    detach_type->nas_key_set_identifier = test_ue->nas.ksi;
    detach_type->switch_off = switch_off;
    detach_type->value = OGS_NAS_DETACH_TYPE_FROM_UE_EPS_DETACH;

    if (test_ue->nas_eps_guti.m_tmsi) {
        eps_mobile_identity->length =
            sizeof(ogs_nas_eps_mobile_identity_guti_t);
        eps_mobile_identity->guti.odd_even = OGS_NAS_MOBILE_IDENTITY_EVEN;
        eps_mobile_identity->guti.type = OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
        eps_mobile_identity->guti.nas_plmn_id =
            test_ue->nas_eps_guti.nas_plmn_id;
        eps_mobile_identity->guti.mme_gid = test_ue->nas_eps_guti.mme_gid;
        eps_mobile_identity->guti.mme_code = test_ue->nas_eps_guti.mme_code;
        eps_mobile_identity->guti.m_tmsi = test_ue->nas_eps_guti.m_tmsi;
    } else {
        eps_mobile_identity->length =
            sizeof(ogs_nas_mobile_identity_imsi_t);
        memcpy(&eps_mobile_identity->imsi,
                &test_ue->mobile_identity_imsi, eps_mobile_identity->length);
    }

    if (integrity_protected)
        return test_nas_eps_security_encode(test_ue, &message);
    else
        return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testemm_build_service_request(test_ue_t *test_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_service_request_t *service_request =
        &message.emm.service_request;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pkbuf_t *new = NULL;
    uint8_t mac[4];

    ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
        &service_request->ksi_and_sequence_number;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE;
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    ksi_and_sequence_number->sequence_number = test_ue->ul_count;

    pkbuf = ogs_nas_eps_plain_encode(&message);
    ogs_assert(pkbuf);

    new = ogs_pkbuf_copy(pkbuf);
    ogs_assert(new);
    ogs_pkbuf_trim(new, 2);

    /* To prevent error message in ogs_nas_mac_calculate() */
    if (test_ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_EIA0)
        test_ue->selected_int_algorithm = OGS_NAS_SECURITY_ALGORITHMS_128_EIA1;

    ogs_nas_mac_calculate(test_ue->selected_int_algorithm,
        test_ue->knas_int, test_ue->ul_count, 0, 0, new, mac);
    test_ue->ul_count = (test_ue->ul_count + 1) & 0xffffff; /* Use 24bit */

    memcpy(pkbuf->data + 2, mac + 2, 2);
    ogs_pkbuf_free(new);

    return pkbuf;
}

ogs_pkbuf_t *testemm_build_tau_request(
        test_ue_t *test_ue, bool active_flag, uint8_t update_type,
        bool integrity_protected, bool ciphered)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_tracking_area_update_request_t *tau_request =
        &message.emm.tracking_area_update_request;

    ogs_nas_eps_update_type_t *eps_update_type = &tau_request->eps_update_type;
    ogs_nas_eps_mobile_identity_t *old_guti = &tau_request->old_guti;

    ogs_nas_ue_network_capability_t *ue_network_capability =
        &tau_request->ue_network_capability;
    ogs_nas_tracking_area_identity_t *last_visited_registered_tai =
        &tau_request->last_visited_registered_tai;
    ogs_nas_drx_parameter_t *drx_parameter = &tau_request->drx_parameter;
    ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status =
        &tau_request->eps_bearer_context_status;
    ogs_nas_ms_network_capability_t *ms_network_capability =
        &tau_request->ms_network_capability;
    ogs_nas_location_area_identification_t *old_location_area_identification =
        &tau_request->old_location_area_identification;
    ogs_nas_tmsi_status_t *tmsi_status = &tau_request->tmsi_status;
    ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2 =
        &tau_request->mobile_station_classmark_2;
    ogs_nas_mobile_station_classmark_3_t *mobile_station_classmark_3 =
        &tau_request->mobile_station_classmark_3;
    ogs_nas_supported_codec_list_t *supported_codecs =
        &tau_request->supported_codecs;
    ogs_nas_additional_update_type_t *additional_update_type =
        &tau_request->additional_update_type;
    ogs_nas_voice_domain_preference_and_ue_usage_setting_t
        *voice_domain_preference_and_ue_usage_setting =
            &tau_request->voice_domain_preference_and_ue_usage_setting;
    ogs_nas_guti_type_t *old_guti_type = &tau_request->old_guti_type;
    ogs_nas_ms_network_feature_support_t *ms_network_feature_support =
            &tau_request->ms_network_feature_support;
    ogs_nas_network_resource_identifier_container_t *tmsi_based_nri_container =
        &tau_request->tmsi_based_nri_container;
    ogs_nas_ue_additional_security_capability_t
        *ue_additional_security_capability =
            &tau_request->ue_additional_security_capability;

    uint8_t classmark_3[11] = "\x60\x14\x04\xef\x65\x23\x3b\x88\x78\xd2\x90";

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    if (integrity_protected) {
        if (ciphered) {
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        } else {
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
        }
    }
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST;

    eps_update_type->nas_key_set_identifier = test_ue->nas.ksi;
    eps_update_type->active_flag = active_flag;
    eps_update_type->value = update_type;

    old_guti->length = sizeof(ogs_nas_eps_mobile_identity_guti_t);
    old_guti->guti.odd_even = OGS_NAS_MOBILE_IDENTITY_EVEN;
    old_guti->guti.type = OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
    old_guti->guti.nas_plmn_id = test_ue->nas_eps_guti.nas_plmn_id;
    old_guti->guti.mme_gid = test_ue->nas_eps_guti.mme_gid;
    old_guti->guti.mme_code = test_ue->nas_eps_guti.mme_code;
    old_guti->guti.m_tmsi = test_ue->nas_eps_guti.m_tmsi;

    if (test_ue->tau_request_param.ue_network_capability) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT;
        ue_network_capability->length = 7;
        ue_network_capability->eea = 0xf0;
        ue_network_capability->eia = 0xf0;
        ue_network_capability->uea = 0xc0;
        ue_network_capability->uia = 0x40;
        ue_network_capability->notification_procedure = 1;
        ue_network_capability->dual_connectivity_with_nr = 1;
    }

    if (test_ue->tau_request_param.last_visited_registered_tai) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
        ogs_nas_from_plmn_id(&last_visited_registered_tai->nas_plmn_id,
            &test_ue->e_tai.plmn_id);
        last_visited_registered_tai->tac = test_ue->e_tai.tac;
    }

    if (test_ue->tau_request_param.drx_parameter) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT;
        drx_parameter->split_pg_cycle_code = 32;
    }

    if (test_ue->tau_request_param.eps_bearer_context_status) {
        test_sess_t *sess = NULL;
        test_bearer_t *bearer = NULL;

        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
        eps_bearer_context_status->length = 2;

        ogs_list_for_each(&test_ue->sess_list, sess) {
            ogs_list_for_each(&sess->bearer_list, bearer) {
                if (bearer->ebi == 5) eps_bearer_context_status->ebi5 = 1;
                else if (bearer->ebi == 6) eps_bearer_context_status->ebi6 = 1;
                else if (bearer->ebi == 7) eps_bearer_context_status->ebi7 = 1;
            }
        }
    }

    if (test_ue->tau_request_param.ms_network_capability) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT;
        ms_network_capability->length = 3;
        ms_network_capability->gea1 = 1;
        ms_network_capability->sm_capabilities_via_dedicated_channels = 1;
        ms_network_capability->sm_capabilities_via_gprs_channels = 1;
        ms_network_capability->ss_screening_indicator = 1;
        ms_network_capability->revision_level_indicator = 1;
        ms_network_capability->pfc_feature_mode = 1;
        ms_network_capability->extended_gea = 0x30;
        ms_network_capability->emm_combined_procedures_capability = 1;
        ms_network_capability->isr_support = 1;
        ms_network_capability->epc_capability = 1;
    }

    if (test_ue->tau_request_param.location_area_identication) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT;
        ogs_nas_from_plmn_id(&old_location_area_identification->nas_plmn_id,
            &test_ue->e_tai.plmn_id);
        old_location_area_identification->lac = 1;
    }

    if (test_ue->tau_request_param.tmsi_status) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT;
    }

    if (test_ue->tau_request_param.mobile_station_classmark_2) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
        mobile_station_classmark_2->length = 3;
        mobile_station_classmark_2->revision_level = 2;
        mobile_station_classmark_2->es_ind = 1;
        mobile_station_classmark_2->a5_1 = 1;
        mobile_station_classmark_2->rf_power_capability = 7;
        mobile_station_classmark_2->ps_capa = 1;
        mobile_station_classmark_2->ss_screen_indicator = 1;
        mobile_station_classmark_2->sm_capabi = 1;
        mobile_station_classmark_2->cm3 = 1;
        mobile_station_classmark_2->lcsva_cap = 1;
        mobile_station_classmark_2->cmsp = 1;
        mobile_station_classmark_2->a5_3 = 1;
    }

    if (test_ue->tau_request_param.mobile_station_classmark_3) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT;
        mobile_station_classmark_3->length = 11;
        memcpy(mobile_station_classmark_3->buffer,
                classmark_3, mobile_station_classmark_3->length);
    }

    if (test_ue->tau_request_param.supported_codecs) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT;
        supported_codecs->length = 8;
        supported_codecs->item[0].system_identification = 0x04;
        supported_codecs->item[0].length_of_bitmap = 2;
        supported_codecs->item[0].codec_bitmap = htobe16(0x6004);
        supported_codecs->item[1].system_identification = 0;
        supported_codecs->item[1].length_of_bitmap = 2;
        supported_codecs->item[1].codec_bitmap = htobe16(0x1f02);
    }

    if (test_ue->tau_request_param.additional_update_type) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT;
        additional_update_type->autv = 1;
    }

    if (test_ue->tau_request_param.ue_usage_setting) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT;
        voice_domain_preference_and_ue_usage_setting->length = 1;
        voice_domain_preference_and_ue_usage_setting->ue_usage_setting = 1;
        voice_domain_preference_and_ue_usage_setting->
            voice_domain_preference_for_e_utran = 1;
    }

    if (test_ue->tau_request_param.old_guti_type) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT;
    }

    if (test_ue->tau_request_param.ms_network_feature_support) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT;
        ms_network_feature_support->extended_periodic_timers = 1;
    }

    if (test_ue->tau_request_param.nri_container) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT;
        tmsi_based_nri_container->length = 2;
        tmsi_based_nri_container->nri_container_value1 = 0x8d;
        tmsi_based_nri_container->nri_container_value2 = 0x3;
    }

    if (test_ue->tau_request_param.ue_additional_security_capability) {
        tau_request->presencemask |=
        OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT;
        ue_additional_security_capability->length = 4;
        ue_additional_security_capability->nea = 0xf0;
        ue_additional_security_capability->nia = 0xf0;
    }

    if (test_ue->tau_request_param.device_properties) {
        tau_request->presencemask |=
            OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT;
    }

    if (integrity_protected)
        return test_nas_eps_security_encode(test_ue, &message);
    else
        return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testemm_build_emm_status(
        test_ue_t *test_ue, ogs_nas_emm_cause_t emm_cause)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_emm_status_t *emm_status = &message.emm.emm_status;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_EMM_STATUS;

    emm_status->emm_cause = emm_cause;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testemm_build_extended_service_request(
        test_ue_t *test_ue, uint8_t type,
        bool integrity_protected, bool ciphered)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_extended_service_request_t *extended_service_request =
        &message.emm.extended_service_request;

    ogs_nas_service_type_t *service_type =
        &extended_service_request->service_type;
    ogs_nas_mobile_identity_t *mobile_identity =
        &extended_service_request->m_tmsi;
    ogs_nas_csfb_response_t *csfb_response =
        &extended_service_request->csfb_response;
    ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status =
        &extended_service_request->eps_bearer_context_status;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    if (integrity_protected) {
        if (ciphered)
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        else
            message.h.security_header_type =
                OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
        message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST;

    service_type->value = type;

    mobile_identity->length = 5;
    mobile_identity->tmsi.type = OGS_NAS_MOBILE_IDENTITY_TMSI;
    mobile_identity->tmsi.tmsi = test_ue->nas_eps_guti.m_tmsi;

    if (test_ue->extended_service_request_param.csfb_response) {
        extended_service_request->presencemask |=
            OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT;
        csfb_response->response =
            test_ue->extended_service_request_param.csfb.response;
    }

    if (test_ue->extended_service_request_param.eps_bearer_context_status) {
        test_sess_t *sess = NULL;
        test_bearer_t *bearer = NULL;

        extended_service_request->presencemask |= OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
        eps_bearer_context_status->length = 2;

        ogs_list_for_each(&test_ue->sess_list, sess) {
            ogs_list_for_each(&sess->bearer_list, bearer) {
                if (bearer->ebi == 5) eps_bearer_context_status->ebi5 = 1;
                else if (bearer->ebi == 6) eps_bearer_context_status->ebi6 = 1;
                else if (bearer->ebi == 7) eps_bearer_context_status->ebi7 = 1;
            }
        }
    }

    if (integrity_protected)
        return test_nas_eps_security_encode(test_ue, &message);
    else
        return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testemm_build_uplink_nas_transport(test_ue_t *test_ue)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_uplink_nas_transport_t *uplink_nas_transport =
        &message.emm.uplink_nas_transport;

    ogs_nas_eps_message_container_t *nas_message_container =
        &uplink_nas_transport->nas_message_container;

    char hexbuf[OGS_MAX_SDU_LEN];
    const char *payload =
        "390167000300"
        "0581005155f55d11 030c914477680205 490000055ad2e2b1 252d467ff6de6c47"
        "efd568375b303613 166fb51c6d160cc2 8ab462b006a3d98a 31da90060b0673c5"
        "9c512684158bb119 2c88b3058b37e1ad 081bca84c1582d07 93ede4bddc6d2693"
        "e566371b";

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_UPLINK_NAS_TRANSPORT;

    OGS_HEX(payload, strlen(payload), hexbuf);
    nas_message_container->length = 106;
    memcpy(nas_message_container->buffer,
            hexbuf, nas_message_container->length);

    return test_nas_eps_security_encode(test_ue, &message);
}
