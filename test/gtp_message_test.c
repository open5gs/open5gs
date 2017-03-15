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

    pkbuf = pkbuf_alloc(0, 240);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    core_ascii_to_hex(_payload, strlen(_payload), pkbuf->payload, 
            pkbuf->len);

    memset(&req, 0, sizeof(req));
    rv = tlv_parse_msg(&req, &gtpv2c_desc_create_session_request, pkbuf,
            TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    ABTS_INT_EQUAL(tc, 1, req.imsi.presence);
    ABTS_INT_EQUAL(tc, 8, req.imsi.len);
    _value = "55153011 340010f4";
    ABTS_TRUE(tc, memcmp(core_ascii_to_hex(_value, strlen(_value), 
            tmp, req.imsi.len), req.imsi.data, req.imsi.len) == 0);

    ABTS_INT_EQUAL(tc, 1, req.msisdn.presence);
    ABTS_INT_EQUAL(tc, 6, req.msisdn.len);
    _value = "94715276 0041";
    ABTS_TRUE(tc, memcmp(core_ascii_to_hex(_value, strlen(_value), 
            tmp, req.msisdn.len), req.msisdn.data, req.msisdn.len) == 0);

    ABTS_INT_EQUAL(tc, 1, req.me_identity.presence);
    ABTS_INT_EQUAL(tc, 8, req.me_identity.len);
    _value = "53612000 91788400";
    ABTS_TRUE(tc, memcmp(core_ascii_to_hex(_value, strlen(_value),
            tmp, req.me_identity.len),
        req.me_identity.data, req.me_identity.len) == 0);

    pkbuf_free(pkbuf);

    ABTS_INT_EQUAL(tc, 1, req.user_location_information.presence);
    ABTS_INT_EQUAL(tc, 1, req.serving_network.presence);
    ABTS_INT_EQUAL(tc, 1, req.rat_type.presence);
    ABTS_INT_EQUAL(tc, 0, req.indication_flags.presence);
    ABTS_INT_EQUAL(tc, 1, req.sender_f_teid_for_control_plane.presence);
    ABTS_INT_EQUAL(tc, 1, req.pgw_s5_s8_address_for_control_plane_or_pmip.
            presence);
    ABTS_INT_EQUAL(tc, 1, req.access_point_name.presence);
    ABTS_INT_EQUAL(tc, 1, req.selection_mode.presence);
    ABTS_INT_EQUAL(tc, 1, req.pdn_type.presence);
    ABTS_INT_EQUAL(tc, 1, req.pdn_address_allocation.presence);
    ABTS_INT_EQUAL(tc, 1, req.maximum_apn_restriction.presence);
    ABTS_INT_EQUAL(tc, 1, req.aggregate_maximum_bit_rate.presence);
    ABTS_INT_EQUAL(tc, 0, req.linked_eps_bearer_id.presence);
    ABTS_INT_EQUAL(tc, 0, req.trusted_wlan_mode_indication.presence);
    ABTS_INT_EQUAL(tc, 1, req.protocol_configuration_options.presence);
    ABTS_INT_EQUAL(tc, 1, req.bearer_contexts_to_be_created.presence);
    ABTS_INT_EQUAL(tc, 1, req.
                bearer_contexts_to_be_created.eps_bearer_id.presence);
    ABTS_INT_EQUAL(tc, 1, 
            req.bearer_contexts_to_be_created.eps_bearer_id.len);
    _value = "05";
    ABTS_TRUE(tc, memcmp(core_ascii_to_hex(_value, strlen(_value), tmp, 
            req.bearer_contexts_to_be_created.eps_bearer_id.len),
        req.bearer_contexts_to_be_created.eps_bearer_id.data,
        req.bearer_contexts_to_be_created.eps_bearer_id.len)
            == 0);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.tft.presence);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.s1_u_enodeb_f_teid.presence);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.s2b_u_epdg_f_teid_5.presence);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.s2a_u_twan_f_teid_6.presence);
    ABTS_INT_EQUAL(tc, 1, req.
                bearer_contexts_to_be_created.bearer_level_qos.presence);
    ABTS_INT_EQUAL(tc, 22,
            req.bearer_contexts_to_be_created.bearer_level_qos.len);
    _value = "45050000 00000000 00000000 00000000 00000000 0000";
    ABTS_TRUE(tc, memcmp(
        core_ascii_to_hex(_value, strlen(_value), tmp,
            req.bearer_contexts_to_be_created.bearer_level_qos.len),
        req.bearer_contexts_to_be_created.bearer_level_qos.data,
        req.bearer_contexts_to_be_created.bearer_level_qos.len) 
            == 0);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.s11_u_mme_f_teid.presence);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.cause.presence);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.transaction_identifier.presence);
    ABTS_INT_EQUAL(tc, 0, req.
                bearer_contexts_to_be_created.packet_flow_id.presence);
    ABTS_INT_EQUAL(tc, 0, req.bearer_contexts_to_be_removed.presence);
    ABTS_INT_EQUAL(tc, 0, req.recovery.presence);
    ABTS_INT_EQUAL(tc, 0, req.mme_fq_csid.presence);
    ABTS_INT_EQUAL(tc, 0, req.sgw_fq_csid.presence);
    ABTS_INT_EQUAL(tc, 0, req.epdg_fq_csid.presence);
    ABTS_INT_EQUAL(tc, 0, req.twan_fq_csid.presence);
    ABTS_INT_EQUAL(tc, 1, req.ue_time_zone.presence);
    ABTS_INT_EQUAL(tc, 0, req.user_csg_information.presence);
    ABTS_INT_EQUAL(tc, 1, req.charging_characteristics.presence);
    ABTS_INT_EQUAL(tc, 0, req.mme_s4_sgsn_ldn.presence);
    ABTS_INT_EQUAL(tc, 0, req.sgw_ldn.presence);
    ABTS_INT_EQUAL(tc, 0, req.epdg_ldn.presence);
    ABTS_INT_EQUAL(tc, 0, req.mo_exception_data_counter.presence);
    ABTS_INT_EQUAL(tc, 0, req.ue_tcp_port.presence);
}

abts_suite *test_gtp_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, gtp_message_test1, NULL);

    return suite;
}
