#include "core_pkbuf.h"
#include "core_lib.h"
#include "core_debug.h"
#include "core_network.h"

#include "3gpp_types.h"
#include "gtp/gtp_types.h"
#include "gtp/gtp_message.h"

#include "testutil.h"

static void gtp_message_test1(abts_case *tc, void *data)
{
    status_t rv;
    /* Create Session Request */
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
    char hexbuf[MAX_SDU_LEN];

    gtp_create_session_request_t req;
    gtp_uli_t uli;
    char ulibuf[GTP_MAX_ULI_LEN];
    plmn_id_t serving_network;
    char apnbuf[34];
    gtp_f_teid_t s11, s5;
    paa_t paa;
    gtp_ambr_t ambr;
    pco_t pco;
    char pcobuf[MAX_PCO_LEN];
    gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    gtp_ue_timezone_t ue_timezone;
    c_int16_t size = 0;

    pkbuf_t *pkbuf = NULL;
    c_sockaddr_t sa;

    memset(&req, 0, sizeof(gtp_create_session_request_t));

    req.imsi.presence = 1;
    req.imsi.data = (c_uint8_t *)"\x55\x15\x30\x11\x34\x00\x10\xf4";
    req.imsi.len = 8;

    req.msisdn.presence = 1;
    req.msisdn.data = (c_uint8_t *)"\x94\x71\x52\x76\x00\x41";
    req.msisdn.len = 6;

    req.me_identity.presence = 1;
    req.me_identity.data = (c_uint8_t *)"\x53\x61\x20\x00\x91\x78\x84\x00";
    req.me_identity.len = 8;

    memset(&uli, 0, sizeof(gtp_uli_t));
    uli.flags.e_cgi = 1;
    uli.flags.tai = 1;
    plmn_id_build(&uli.tai.plmn_id, 555, 10, 2);
    uli.tai.tac = 4130;
    plmn_id_build(&uli.e_cgi.plmn_id, 555, 10, 2);
    uli.e_cgi.cell_id = 105729;
    req.user_location_information.presence = 1;
    size = gtp_build_uli(&req.user_location_information, &uli, 
            ulibuf, GTP_MAX_ULI_LEN);
    ABTS_INT_EQUAL(tc, 13, req.user_location_information.len);

    req.serving_network.presence = 1;
    req.serving_network.data = plmn_id_build(&serving_network, 555, 10, 2);
    req.serving_network.len = sizeof(serving_network);

    req.rat_type.presence = 1;
    req.rat_type.u8 = GTP_RAT_TYPE_EUTRAN;

    memset(&s11, 0, sizeof(gtp_f_teid_t));
    s11.ipv4 = 1;
    s11.interface_type = GTP_F_TEID_S11_MME_GTP_C;
    s11.teid = htonl(0x80000084);
    core_inet_pton(AF_INET, "10.50.54.10", &sa);
    s11.addr = sa.sin.sin_addr.s_addr;
    req.sender_f_teid_for_control_plane.presence = 1;
    req.sender_f_teid_for_control_plane.data = &s11;
    req.sender_f_teid_for_control_plane.len = GTP_F_TEID_IPV4_LEN;

    memset(&s5, 0, sizeof(gtp_f_teid_t));
    s5.ipv4 = 1;
    s5.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    core_inet_pton(AF_INET, "10.50.54.37", &sa);
    s5.addr = sa.sin.sin_addr.s_addr;
    req.pgw_s5_s8_address_for_control_plane_or_pmip.presence = 1;
    req.pgw_s5_s8_address_for_control_plane_or_pmip.data = &s5;
    req.pgw_s5_s8_address_for_control_plane_or_pmip.len = GTP_F_TEID_IPV4_LEN;

    _value = "05766f6c7465036e 6732046d6e657406 6d6e63303130066d 6363353535046770 7273";
    req.access_point_name.presence = 1;
    req.access_point_name.data = CORE_HEX(_value, strlen(_value), apnbuf);
    req.access_point_name.len = sizeof(apnbuf);

    req.selection_mode.presence = 1;
    req.selection_mode.u8 = GTP_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN | 0xfc;

    req.pdn_type.presence = 1;
    req.pdn_type.u8 = GTP_PDN_TYPE_IPV4;

    memset(&paa, 0, sizeof(paa_t));
    paa.pdn_type = GTP_PDN_TYPE_IPV4;
    req.pdn_address_allocation.presence = 1;
    req.pdn_address_allocation.data = &paa;
    req.pdn_address_allocation.len = PAA_IPV4_LEN;

    req.maximum_apn_restriction.presence = 1;
    req.maximum_apn_restriction.u8 = GTP_APN_NO_RESTRICTION;

    memset(&ambr, 0, sizeof(gtp_ambr_t));
    ambr.uplink = htonl(1000);
    ambr.downlink = htonl(2000);
    req.aggregate_maximum_bit_rate.presence = 1;
    req.aggregate_maximum_bit_rate.data = &ambr;
    req.aggregate_maximum_bit_rate.len = sizeof(ambr);

    memset(&pco, 0, sizeof(pco_t));
    pco.ext = 1;
    pco.configuration_protocol = 
        PCO_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE;
    pco.num_of_id = 3;
    pco.ids[0].id = PCO_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL;
    pco.ids[0].data = (c_uint8_t *)"\x01\x00\x00\x10\x81\x06\x00\x00\x00\x00\x83\x06\x00\x00\x00\x00";
    pco.ids[0].len = 16;
    pco.ids[1].id = PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST;
    pco.ids[1].len = 0;
    pco.ids[2].id = PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING;
    pco.ids[2].len = 0;

    req.protocol_configuration_options.presence = 1;
    req.protocol_configuration_options.data = &pcobuf;
    req.protocol_configuration_options.len = 
        pco_build(pcobuf, MAX_PCO_LEN, &pco);

    req.bearer_contexts_to_be_created.presence = 1;
    req.bearer_contexts_to_be_created.eps_bearer_id.presence = 1;
    req.bearer_contexts_to_be_created.eps_bearer_id.u8 = 5;

    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.pre_emption_vulnerability = 1;
    bearer_qos.priority_level = 1;
    bearer_qos.pre_emption_capability = 1;
    bearer_qos.qci = 5;
    req.bearer_contexts_to_be_created.bearer_level_qos.presence = 1;
    size = gtp_build_bearer_qos(
            &req.bearer_contexts_to_be_created.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);

    memset(&ue_timezone, 0, sizeof(ue_timezone));
    ue_timezone.gmtoff = 0x40;
    ue_timezone.daylight_saving_time = 
        GTP_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;
    req.ue_time_zone.presence = 1;
    req.ue_time_zone.data = &ue_timezone;
    req.ue_time_zone.len = sizeof(ue_timezone);

    req.charging_characteristics.presence = 1;
    req.charging_characteristics.data = (c_uint8_t *)"\x54\x00";
    req.charging_characteristics.len = 2;

    rv = tlv_build_msg(&pkbuf, &tlv_desc_create_session_request, &req,
            TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    ABTS_TRUE(tc, memcmp(pkbuf->payload, 
        CORE_HEX(_payload, strlen(_payload), hexbuf), pkbuf->len) == 0);

    memset(&req, 0, sizeof(req));
    rv = tlv_parse_msg(&req, &tlv_desc_create_session_request, pkbuf,
            TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(pkbuf);

    ABTS_INT_EQUAL(tc, 1, req.imsi.presence);
    ABTS_INT_EQUAL(tc, 8, req.imsi.len);
    _value = "55153011 340010f4";
    ABTS_TRUE(tc, memcmp(CORE_HEX(_value, strlen(_value), hexbuf), 
                req.imsi.data, req.imsi.len) == 0);

    ABTS_INT_EQUAL(tc, 1, req.msisdn.presence);
    ABTS_INT_EQUAL(tc, 6, req.msisdn.len);
    _value = "94715276 0041";
    ABTS_TRUE(tc, memcmp(CORE_HEX(_value, strlen(_value), hexbuf), 
                req.msisdn.data, req.msisdn.len) == 0);

    ABTS_INT_EQUAL(tc, 1, req.me_identity.presence);
    ABTS_INT_EQUAL(tc, 8, req.me_identity.len);
    _value = "53612000 91788400";
    ABTS_TRUE(tc, memcmp(CORE_HEX(_value, strlen(_value), hexbuf),
        req.me_identity.data, req.me_identity.len) == 0);

    ABTS_INT_EQUAL(tc, 1, req.user_location_information.presence);
    size = gtp_parse_uli(&uli, &req.user_location_information);
    ABTS_INT_EQUAL(tc, 13, size);
    ABTS_INT_EQUAL(tc, 0, uli.flags.lai);
    ABTS_INT_EQUAL(tc, 1, uli.flags.e_cgi);
    ABTS_INT_EQUAL(tc, 105729, uli.e_cgi.cell_id);
    ABTS_INT_EQUAL(tc, 1, uli.flags.tai);
    ABTS_INT_EQUAL(tc, 4130, uli.tai.tac);
    ABTS_INT_EQUAL(tc, 0, uli.flags.rai);
    ABTS_INT_EQUAL(tc, 0, uli.flags.sai);
    ABTS_INT_EQUAL(tc, 0, uli.flags.cgi);

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
    size = pco_parse(&pco, req.protocol_configuration_options.data,
                        req.protocol_configuration_options.len);
    ABTS_INT_EQUAL(tc, 26, size);
    ABTS_INT_EQUAL(tc, 1, pco.ext);
    ABTS_INT_EQUAL(tc, 0, pco.configuration_protocol);
    ABTS_INT_EQUAL(tc, 3, pco.num_of_id);
    ABTS_INT_EQUAL(tc, 
        PCO_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL, 
        pco.ids[0].id);
    ABTS_INT_EQUAL(tc, 16, pco.ids[0].len); 
    ABTS_TRUE(tc, memcmp(
        "\x01\x00\x00\x10\x81\x06\x00\x00\x00\x00\x83\x06\x00\x00\x00\x00",
        pco.ids[0].data, pco.ids[0].len) == 0);
    ABTS_INT_EQUAL(tc, 
        PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST, 
        pco.ids[1].id);
    ABTS_INT_EQUAL(tc, 0, pco.ids[1].len); 
    ABTS_INT_EQUAL(tc, 
        PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING, 
        pco.ids[2].id);
    ABTS_INT_EQUAL(tc, 0, pco.ids[2].len); 
    ABTS_INT_EQUAL(tc, 1, req.bearer_contexts_to_be_created.presence);
    ABTS_INT_EQUAL(tc, 1, req.
                bearer_contexts_to_be_created.eps_bearer_id.presence);
    ABTS_INT_EQUAL(tc, 0x05, 
            req.bearer_contexts_to_be_created.eps_bearer_id.u8);
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
    size = gtp_parse_bearer_qos(&bearer_qos, 
            &req.bearer_contexts_to_be_created.bearer_level_qos);
    ABTS_INT_EQUAL(tc, 22, size);
    ABTS_INT_EQUAL(tc, 1, bearer_qos.pre_emption_vulnerability);
    ABTS_INT_EQUAL(tc, 1, bearer_qos.priority_level);
    ABTS_INT_EQUAL(tc, 1, bearer_qos.pre_emption_capability);
    ABTS_INT_EQUAL(tc, 5, bearer_qos.qci);
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
