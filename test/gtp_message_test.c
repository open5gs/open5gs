#include "core_pkbuf.h"
#include "core_lib.h"
#include "core_debug.h"

#include "gtpv2c_tlv.h"

#include "testutil.h"

static void gtp_message_test1(abts_case *tc, void *data)
{
    status_t rv;
    char *_payload = 
        "0100080055153011 340010f44c000600 9471527600414b00 0800536120009178"
        "840056000d001855 f501102255f50100 019d015300030055 f501520001000657"
        "0009008a80000084 0a32360a57000901 87000000000a3236 254700220005766f"
        "6c7465036e673204 6d6e6574066d6e63 303130066d636335 3535046770727380"
        "000100fc63000100 014f000500010000 00007f0001000048 000800000003e800"
        "0007d04e001a0080 8021100100001081 0600000000830600 000000000d00000a"
        "005d001f00490001 0005500016004505 0000000000000000 0000000000000000"
        "0000000072000200 40005f0002005400";
    char *_value = NULL;
    gtpv2c_create_session_request_t req;
    c_uint8_t tmp[256];
    pkbuf_t *pkbuf = NULL;
    c_uint8_t *buf = NULL;
    c_uint32_t buflen = 0;

    pkbuf = pkbuf_alloc(0, 240);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    core_ascii_to_hex(_payload, strlen(_payload), pkbuf->payload, 
            pkbuf->len);

    memset(&req, 0, sizeof(req));
    rv = tlv_parse_msg(&req, &gtpv2c_desc_create_session_request, pkbuf,
            TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.imsi));
    tlv_get_octet(buf, buflen, req.imsi);
    ABTS_INT_EQUAL(tc, 8, buflen);
    _value = "55153011 340010f4";
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(_value, strlen(_value), tmp, buflen),
            buf, buflen) == 0);

    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.msisdn));
    tlv_get_octet(buf, buflen, req.msisdn);
    ABTS_INT_EQUAL(tc, 6, buflen);
    _value = "94715276 0041";
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(_value, strlen(_value), tmp, buflen),
            buf, buflen) == 0);

    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.me_identity));
    tlv_get_octet(buf, buflen, req.me_identity);
    ABTS_INT_EQUAL(tc, 8, buflen);
    _value = "53612000 91788400";
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(_value, strlen(_value), tmp, buflen),
            buf, buflen) == 0);

    pkbuf_free(pkbuf);

    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.user_location_information));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.serving_network));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.rat_type));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.indication_flags));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.sender_f_teid_for_control_plane));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.pgw_s5_s8_address_for_control_plane_or_pmip));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.access_point_name));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.selection_mode));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.pdn_type));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.pdn_address_allocation));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.maximum_apn_restriction));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.aggregate_maximum_bit_rate));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.linked_eps_bearer_id));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.trusted_wlan_mode_indication));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.protocol_configuration_options));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.bearer_contexts_to_be_created));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.
                bearer_contexts_to_be_created.eps_bearer_id));
    tlv_get_octet(buf, buflen, req.
                bearer_contexts_to_be_created.eps_bearer_id);
    ABTS_INT_EQUAL(tc, 1, buflen);
    _value = "05";
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(_value, strlen(_value), tmp, buflen),
            buf, buflen) == 0);
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.tft));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.s1_u_enodeb_f_teid));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.s2b_u_epdg_f_teid_5));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.s2a_u_twan_f_teid_6));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.
                bearer_contexts_to_be_created.bearer_level_qos));
    tlv_get_octet(buf, buflen, req.
                bearer_contexts_to_be_created.bearer_level_qos);
    ABTS_INT_EQUAL(tc, 22, buflen);
    _value = "45050000 00000000 00000000 00000000 00000000 0000";
    ABTS_TRUE(tc, memcmp(
            core_ascii_to_hex(_value, strlen(_value), tmp, buflen),
            buf, buflen) == 0);
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.s11_u_mme_f_teid));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.cause));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.transaction_identifier));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.
                bearer_contexts_to_be_created.packet_flow_id));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.bearer_contexts_to_be_removed));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.recovery));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.mme_fq_csid));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.sgw_fq_csid));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.epdg_fq_csid));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.twan_fq_csid));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.ue_time_zone));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.user_csg_information));
    ABTS_INT_EQUAL(tc, 1, tlv_is_present(req.charging_characteristics));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.mme_s4_sgsn_ldn));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.sgw_ldn));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.epdg_ldn));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.mo_exception_data_counter));
    ABTS_INT_EQUAL(tc, 0, tlv_is_present(req.ue_tcp_port));
}

abts_suite *test_gtp_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, gtp_message_test1, NULL);

    return suite;
}
