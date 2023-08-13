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

#include "ogs-nas-eps.h"
#include "core/abts.h"

static void ogs_nas_eps_message_test1(abts_case *tc, void *data)
{
    /* Attach Request */
    const char *payload = 
        "0741020bf600f110000201030003e605"
        "f07000001000050215d011d15200f110"
        "30395c0a003103e5e0349011035758a6"
        "5d0100e0c1";

    ogs_nas_eps_message_t message;
    ogs_pkbuf_t *pkbuf;
    int rv;
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    ogs_pkbuf_trim(pkbuf, 53);
    memcpy(pkbuf->data, 
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), pkbuf->len);

    rv = ogs_nas_emm_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_pkbuf_free(pkbuf);
}

static void ogs_nas_eps_message_test2(abts_case *tc, void *data)
{
    /* Attach Accept */
    const char *payload = 
        "07420223060014f799303900325201c1"
        "01090908696e7465726e657405010ae1"
        "000a271b80802110020200108106c0a8"
        "a8018306c0a8a801000d04c0a8a80150"
        "0bf614f7992345e1000004561300f120"
        "fffd2305f400e102d4640123";
    char buffer[92];

    const char *esm_payload = 
        "5201c101090908696e7465726e657405"
        "010ae1000a271b808021100202001081"
        "06c0a8a8018306c0a8a801000d04c0a8"
        "a801";
    char esm_buffer[50];

    ogs_nas_eps_message_t message;
    ogs_nas_eps_attach_accept_t *attach_accept = &message.emm.attach_accept;
    ogs_eps_tai0_list_t tai0_list;
    ogs_eps_tai2_list_t tai2_list;
    ogs_plmn_id_t plmn_id;

    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_ATTACH_ACCEPT;
    attach_accept->eps_attach_result.result = 
        OGS_NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH;
    attach_accept->t3412_value.unit = 
        OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    attach_accept->t3412_value.value = 3;

    memset(&tai0_list, 0, sizeof(ogs_eps_tai0_list_t));
    memset(&tai2_list, 0, sizeof(ogs_eps_tai2_list_t));
    tai0_list.tai[0].type = OGS_TAI0_TYPE;
    tai0_list.tai[0].num = 1;
    ogs_plmn_id_build(&tai0_list.tai[0].plmn_id, 417, 99, 2);
    tai0_list.tai[0].tac[0] = 12345;
    ogs_nas_tai_list_build(&attach_accept->tai_list, &tai0_list, NULL, NULL);

    attach_accept->esm_message_container.length = sizeof(esm_buffer);
    attach_accept->esm_message_container.buffer = 
        ogs_hex_from_string(esm_payload, esm_buffer, sizeof(esm_buffer));

    attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_PRESENT;
    attach_accept->guti.length = 11;
    attach_accept->guti.guti.type = OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
    ogs_nas_from_plmn_id(&attach_accept->guti.guti.nas_plmn_id,
        ogs_plmn_id_build(&plmn_id, 417, 99, 2));
    attach_accept->guti.guti.mme_gid = 9029;
    attach_accept->guti.guti.mme_code = 225;
    attach_accept->guti.guti.m_tmsi = 0x00000456;

    attach_accept->presencemask |= 
        OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
    ogs_nas_from_plmn_id(
            &attach_accept->location_area_identification.nas_plmn_id,
            ogs_plmn_id_build(&plmn_id, 1, 2, 2));
    attach_accept->location_area_identification.lac = 0xfffd;

    attach_accept->presencemask |=
        OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT;
    attach_accept->ms_identity.length = 5;
    attach_accept->ms_identity.tmsi.type = 
        OGS_NAS_MOBILE_IDENTITY_TMSI;
    attach_accept->ms_identity.tmsi.tmsi = 0x00e102d4;

    attach_accept->presencemask |= 
        OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    attach_accept->eps_network_feature_support.length = 1;
    attach_accept->eps_network_feature_support.
        support_of_extended_service_request_for_packet_services = 1;
    attach_accept->eps_network_feature_support.
        emergency_bearer_services_in_s1_mode = 1;
    attach_accept->eps_network_feature_support.
        ims_voice_over_ps_session_in_s1_mode = 1;

    pkbuf = ogs_nas_eps_plain_encode(&message);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ogs_log_hexdump(OGS_LOG_DEBUG, pkbuf->data, pkbuf->len);
    ABTS_TRUE(tc, memcmp(ogs_hex_from_string(payload, buffer, sizeof(buffer)),
            pkbuf->data, pkbuf->len) == 0);

    ogs_pkbuf_free(pkbuf);
}

static void ogs_nas_eps_message_test3(abts_case *tc, void *data)
{
    const char *payload = "074300035200c2";

    ogs_nas_eps_message_t message;
    ogs_pkbuf_t *pkbuf;
    int rv;
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    ogs_pkbuf_trim(pkbuf, 7);
    memcpy(pkbuf->data, 
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), pkbuf->len);

    rv = ogs_nas_emm_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_pkbuf_free(pkbuf);
}

static void ogs_nas_eps_message_test4(abts_case *tc, void *data)
{
    /* Attach Reject */
    const char *payload = "074411";
    char buffer[3];

    ogs_nas_eps_message_t message;
    ogs_nas_eps_attach_reject_t *attach_reject = &message.emm.attach_reject;

    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_ATTACH_REJECT;
    attach_reject->emm_cause = OGS_NAS_EMM_CAUSE_NETWORK_FAILURE; 

    pkbuf = ogs_nas_eps_plain_encode(&message);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ABTS_TRUE(tc, memcmp(ogs_hex_from_string(payload, buffer, sizeof(buffer)),
            pkbuf->data, pkbuf->len) == 0);

    ogs_pkbuf_free(pkbuf);
}

#if 0
static void ogs_nas_eps_message_test5(abts_case *tc, void *data)
{
    mme_ue_t ue;

    ue.ul_count.i32 = 0x123456;
    ABTS_INT_EQUAL(tc, 0x1234, ue.ul_count.overflow);
    ABTS_INT_EQUAL(tc, 0x56, ue.ul_count.sqn);
    ABTS_INT_EQUAL(tc, 0, ue.ul_count.spare);

    ue.ul_count.overflow = 0xabcd;
    ue.ul_count.sqn = 0xef;
    ABTS_INT_EQUAL(tc, 0xabcdef, ue.ul_count.i32);
}
#endif

static void ogs_nas_eps_message_test6(abts_case *tc, void *data)
{
    /* Identity Request */
    const char *payload = "075501";
    char hexbuf[OGS_HUGE_LEN];

    ogs_nas_eps_message_t message;
    ogs_nas_eps_identity_request_t *identity_request =
        &message.emm.identity_request;
    ogs_pkbuf_t *pkbuf;
    int rv;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    ogs_pkbuf_trim(pkbuf, 3);
    memcpy(pkbuf->data, 
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), pkbuf->len);

    rv = ogs_nas_emm_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM, 
            message.emm.h.protocol_discriminator);
    ABTS_INT_EQUAL(tc, OGS_NAS_EPS_IDENTITY_REQUEST,
            message.emm.h.message_type);
    ABTS_INT_EQUAL(tc, OGS_NAS_IDENTITY_TYPE_2_IMSI, 
            identity_request->identity_type.type);

    ogs_pkbuf_free(pkbuf);
}

static void ogs_nas_eps_message_test7(abts_case *tc, void *data)
{
    /* Identity Response */
    const char *payload = "0756080910101032548651";
    char buffer[11];

    ogs_nas_eps_message_t message;
    ogs_nas_eps_identity_response_t *identity_response =
        &message.emm.identity_response;

    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = OGS_NAS_EPS_IDENTITY_RESPONSE;

    identity_response->mobile_identity.length = 8;
    identity_response->mobile_identity.imsi.digit1 = 0;
    identity_response->mobile_identity.imsi.type = 
        OGS_NAS_IDENTITY_TYPE_2_IMSI;
    identity_response->mobile_identity.imsi.odd_even = 1;
    identity_response->mobile_identity.imsi.digit2 = 0;
    identity_response->mobile_identity.imsi.digit3 = 1;
    identity_response->mobile_identity.imsi.digit4 = 0;
    identity_response->mobile_identity.imsi.digit5 = 1;
    identity_response->mobile_identity.imsi.digit6 = 0;
    identity_response->mobile_identity.imsi.digit7 = 1;
    identity_response->mobile_identity.imsi.digit8 = 2;
    identity_response->mobile_identity.imsi.digit9 = 3;
    identity_response->mobile_identity.imsi.digit10 = 4;
    identity_response->mobile_identity.imsi.digit11 = 5;
    identity_response->mobile_identity.imsi.digit12 = 6;
    identity_response->mobile_identity.imsi.digit13 = 8;
    identity_response->mobile_identity.imsi.digit14 = 1;
    identity_response->mobile_identity.imsi.digit15 = 5;

    pkbuf = ogs_nas_eps_plain_encode(&message);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ABTS_TRUE(tc, memcmp(ogs_hex_from_string(payload, buffer, sizeof(buffer)),
            pkbuf->data, pkbuf->len) == 0);

    ogs_pkbuf_free(pkbuf);
}

static void ogs_nas_eps_message_test8(abts_case *tc, void *data)
{
    /* Security Request */
    const char *payload = "c7a8640c";
    char buffer[4];

    ogs_nas_eps_message_t message;
    ogs_pkbuf_t *pkbuf;
    int rv;
    char hexbuf[OGS_HUGE_LEN];
    ogs_nas_eps_service_request_t *service_request =
        &message.emm.service_request;
    ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number = 
        &service_request->ksi_and_sequence_number;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    ogs_pkbuf_trim(pkbuf, 4);
    memcpy(pkbuf->data, 
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), pkbuf->len);

    /* Decode service request */
    rv = ogs_nas_emm_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, 5, ksi_and_sequence_number->ksi);
    ABTS_INT_EQUAL(tc, 8, ksi_and_sequence_number->sequence_number);
    ABTS_INT_EQUAL(tc, 0x640c, service_request->message_authentication_code);

    ogs_pkbuf_free(pkbuf);

    /* Encode service request */
    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.security_header_type = 
        OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE;

    ksi_and_sequence_number->ksi = 5;
    ksi_and_sequence_number->sequence_number = 8;
    service_request->message_authentication_code = 0x640c;

    pkbuf = ogs_nas_eps_plain_encode(&message);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ABTS_TRUE(tc, memcmp(ogs_hex_from_string(payload, buffer, sizeof(buffer)),
            pkbuf->data, pkbuf->len) == 0);

    ogs_pkbuf_free(pkbuf);
}

static void ogs_nas_eps_message_test9(abts_case *tc, void *data)
{
    ogs_nas_gprs_timer_t gprs_timer;
    int rv;

    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_FATAL);

    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_2_SS,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 1, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 3);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_2_SS,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 2, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 63);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 64);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*2-1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_1_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 2, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*2+1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*30);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_1_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 30, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_1_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*32);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*6*8);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_DECI_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 8, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*6*8+1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*6*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_DECI_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_from_sec(&gprs_timer, 60*6*32);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);

    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_2_SS,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 1, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 3);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_2_SS,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 2, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 63);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 64);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*2-1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 2, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*2+1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*30);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 30, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*32);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*10*4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_10_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 4, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*10*4+1);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*10*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_10_MM,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*10*32);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*10*36);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 6, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*32);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*40);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_10_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 4, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*10*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_10_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*10*32);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_320_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 1, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*10*32*31);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_320_HH,
            gprs_timer.unit);
    ABTS_INT_EQUAL(tc, 31, gprs_timer.value);
    rv = ogs_nas_gprs_timer_3_from_sec(&gprs_timer, 60*60*10*32*32);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);

    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_ERROR);
}

abts_suite *test_nas_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_ERROR);

    abts_run_test(suite, ogs_nas_eps_message_test1, NULL);
    abts_run_test(suite, ogs_nas_eps_message_test2, NULL);
    abts_run_test(suite, ogs_nas_eps_message_test3, NULL);
    abts_run_test(suite, ogs_nas_eps_message_test4, NULL);
#if 0 /* Will remove it in order not to use mme-context.h */
    abts_run_test(suite, ogs_nas_eps_message_test5, NULL);
#endif
    abts_run_test(suite, ogs_nas_eps_message_test6, NULL);
    abts_run_test(suite, ogs_nas_eps_message_test7, NULL);
    abts_run_test(suite, ogs_nas_eps_message_test8, NULL);
    abts_run_test(suite, ogs_nas_eps_message_test9, NULL);

    return suite;
}
