#include "core_pkbuf.h"
#include "core_lib.h"

#include "testutil.h"

#include "nas_message.h"
#include "nas_security.h"

static void nas_message_test1(abts_case *tc, void *data)
{
    /* Attach Request */
    char *payload = 
        "0741020bf600f110000201030003e605"
        "f07000001000050215d011d15200f110"
        "30395c0a003103e5e0349011035758a6"
        "5d0100e0c1";

    nas_message_t message;
    pkbuf_t *pkbuf;
    status_t rv;

    pkbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 53;
    core_ascii_to_hex(payload, strlen(payload), pkbuf->payload, pkbuf->len);

    rv = nas_plain_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(pkbuf);
}

static void nas_message_test2(abts_case *tc, void *data)
{
    /* Attach Accept */
    char *payload = 
        "07420223060014f799303900325201c1"
        "01090908696e7465726e657405010ae1"
        "000a271b80802110020200108106c0a8"
        "a8018306c0a8a801000d04c0a8a80150"
        "0bf614f7992345e1000004561300f120"
        "fffd2305f400e102d4640123";
    char buffer[92];

    char *esm_payload = 
        "5201c101090908696e7465726e657405"
        "010ae1000a271b808021100202001081"
        "06c0a8a8018306c0a8a801000d04c0a8"
        "a801";
    char esm_buffer[50];

    nas_message_t message;
    nas_attach_accept_t *attach_accept = &message.attach_accept;

    pkbuf_t *pkbuf = NULL;
    status_t rv;

    memset(&message, 0, sizeof(message));
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_type = NAS_ATTACH_ACCEPT;
    attach_accept->eps_attach_result.result = 
        NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH;
    attach_accept->t3412_value.unit = 
        NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    attach_accept->t3412_value.timer_value = 3;
    attach_accept->tai_list.length = 6;
    attach_accept->tai_list.type0.plmn.mcc_digit1 = 4;
    attach_accept->tai_list.type0.plmn.mcc_digit2 = 1;
    attach_accept->tai_list.type0.plmn.mcc_digit3 = 7;
    attach_accept->tai_list.type0.plmn.mnc_digit1 = 9;
    attach_accept->tai_list.type0.plmn.mnc_digit2 = 9;
    attach_accept->tai_list.type0.plmn.mnc_digit3 = 0xf;
    attach_accept->tai_list.type0.tac[0] = 12345;
    attach_accept->esm_message_container.length = sizeof(esm_buffer);
    attach_accept->esm_message_container.buffer = 
        core_ascii_to_hex(esm_payload, strlen(esm_payload), 
                esm_buffer, sizeof(esm_buffer));

    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_GUTI_PRESENT;
    attach_accept->guti.length = 11;
    attach_accept->guti.guti.type_of_identity = NAS_EPS_MOBILE_IDENTITY_GUTI;
    attach_accept->guti.guti.plmn.mcc_digit1 = 4;
    attach_accept->guti.guti.plmn.mcc_digit2 = 1;
    attach_accept->guti.guti.plmn.mcc_digit3 = 7;
    attach_accept->guti.guti.plmn.mnc_digit1 = 9;
    attach_accept->guti.guti.plmn.mnc_digit2 = 9;
    attach_accept->guti.guti.plmn.mnc_digit3 = 0xf;
    attach_accept->guti.guti.mme_group_id = 9029;
    attach_accept->guti.guti.mme_code = 225;
    attach_accept->guti.guti.m_tmsi = 0x00000456;

    attach_accept->presencemask |= 
        NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
    attach_accept->location_area_identification.plmn.mcc_digit1 = 0;
    attach_accept->location_area_identification.plmn.mcc_digit2 = 0;
    attach_accept->location_area_identification.plmn.mcc_digit3 = 1;
    attach_accept->location_area_identification.plmn.mnc_digit1 = 0;
    attach_accept->location_area_identification.plmn.mnc_digit2 = 2;
    attach_accept->location_area_identification.plmn.mnc_digit3 = 0xf;
    attach_accept->location_area_identification.lac = 0xfffd;

    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT;
    attach_accept->ms_identity.length = 5;
    attach_accept->ms_identity.tmsi.type_of_identity = 
        NAS_MOBILE_IDENTITY_TMSI;
    attach_accept->ms_identity.tmsi.tmsi = 0x00e102d4;

    attach_accept->presencemask |= 
        NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    attach_accept->eps_network_feature_support.length = 1;
    attach_accept->eps_network_feature_support.esr_ps = 1;
    attach_accept->eps_network_feature_support.emc_bs = 1;
    attach_accept->eps_network_feature_support.ims_vops = 1;

    rv = nas_plain_encode(&pkbuf, &message);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(payload, strlen(payload), buffer, sizeof(buffer)),
            pkbuf->payload, pkbuf->len) == 0);

    pkbuf_free(pkbuf);
}

static void nas_message_test3(abts_case *tc, void *data)
{
    char *payload = "074300035200c2";

    nas_message_t message;
    pkbuf_t *pkbuf;
    status_t rv;

    pkbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 7;
    core_ascii_to_hex(payload, strlen(payload), pkbuf->payload, pkbuf->len);

    rv = nas_plain_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(pkbuf);
}

static void nas_message_test4(abts_case *tc, void *data)
{
    /* Attach Reject */
    char *payload = "074411";
    char buffer[3];

    nas_message_t message;
    nas_attach_reject_t *attach_reject = &message.attach_reject;

    pkbuf_t *pkbuf = NULL;
    status_t rv;

    memset(&message, 0, sizeof(message));
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_type = NAS_ATTACH_REJECT;
    attach_reject->emm_cause = NAS_EMM_CAUSE_NETWORK_FAILURE; 

    rv = nas_plain_encode(&pkbuf, &message);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(payload, strlen(payload), buffer, sizeof(buffer)),
            pkbuf->payload, pkbuf->len) == 0);

    pkbuf_free(pkbuf);
}

static void nas_message_test5(abts_case *tc, void *data)
{
    ue_ctx_t ue;

    ue.ul_count.i32 = 0x123456;
    ABTS_INT_EQUAL(tc, 0x1234, ue.ul_count.overflow);
    ABTS_INT_EQUAL(tc, 0x56, ue.ul_count.sqn);
    ABTS_INT_EQUAL(tc, 0, ue.ul_count.spare);

    ue.ul_count.overflow = 0xabcd;
    ue.ul_count.sqn = 0xef;
    ABTS_INT_EQUAL(tc, 0xabcdef, ue.ul_count.i32);
}

static void nas_message_test6(abts_case *tc, void *data)
{
    /* Identity Request */
    char *payload = "075501";

    nas_message_t message;
    nas_identity_request_t *identity_request = &message.identity_request;
    pkbuf_t *pkbuf;
    status_t rv;

    pkbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 3;
    core_ascii_to_hex(payload, strlen(payload), pkbuf->payload, pkbuf->len);

    rv = nas_plain_decode(&message, pkbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    ABTS_INT_EQUAL(tc, NAS_PROTOCOL_DISCRIMINATOR_EMM, 
            message.h.protocol_discriminator);
    ABTS_INT_EQUAL(tc, NAS_IDENTITY_REQUEST, message.h.message_type);
    ABTS_INT_EQUAL(tc, NAS_IDENTITY_IMSI_TYPE, 
            identity_request->identity_type.type_of_identity);

    pkbuf_free(pkbuf);
}

static void nas_message_test7(abts_case *tc, void *data)
{
    /* Identity Response */
    char *payload = "0756080910101032548651";
    char buffer[11];

    nas_message_t message;
    nas_identity_response_t *identity_response = &message.identity_response;

    pkbuf_t *pkbuf = NULL;
    status_t rv;

    memset(&message, 0, sizeof(message));
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.h.message_type = NAS_IDENTITY_RESPONSE;

    identity_response->mobile_identity.length = 8;
    identity_response->mobile_identity.imsi.digit1 = 0;
    identity_response->mobile_identity.imsi.type_of_identity = NAS_MOBILE_IDENTITY_IMSI;
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

    rv = nas_plain_encode(&pkbuf, &message);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, sizeof(buffer), pkbuf->len);
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(payload, strlen(payload), buffer, sizeof(buffer)),
            pkbuf->payload, pkbuf->len) == 0);

    pkbuf_free(pkbuf);
}

abts_suite *test_nas_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, nas_message_test1, NULL);
    abts_run_test(suite, nas_message_test2, NULL);
    abts_run_test(suite, nas_message_test3, NULL);
    abts_run_test(suite, nas_message_test4, NULL);
    abts_run_test(suite, nas_message_test5, NULL);
    abts_run_test(suite, nas_message_test6, NULL);
    abts_run_test(suite, nas_message_test7, NULL);

    return suite;
}
