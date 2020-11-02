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

#include "test-common.h"

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap1, *s1ap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;
    uint64_t mme_ue_s1ap_id;

    const char *_k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    uint8_t k[OGS_KEY_LEN];
    const char *_opc_string = "e8ed289deba952e4283b54e88e6183ca";
    uint8_t opc[OGS_KEY_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"901707364000060\", "
        "\"pdn\" : ["
          "{"
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
            "\"apn\" : \"internet\", "
            "\"pcc_rule\" : ["
              "{"
                "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2d\" },"
                "\"qos\" : {"
                  "\"qci\" : 1,"
                  "\"gbr\" : {"
                    "\"downlink\" : { \"$numberLong\" : \"64\" },"
                    "\"uplink\" : { \"$numberLong\" : \"44\" }"
                  "},"
                  "\"mbr\" : {"
                    "\"downlink\" : { \"$numberLong\" : \"64\" },"
                    "\"uplink\" : { \"$numberLong\" : \"44\" }"
                  "},"
                  "\"arp\" : {"
                    "\"priority_level\" : 3,"
                    "\"pre_emption_vulnerability\" : 0,"
                    "\"pre_emption_capability\" : 0 }"
                  "},"
                  "\"flow\" : ["
                    "{ \"direction\" : 2,"
                      "\"description\" : \"permit out udp from any 1-65535 to 10.200.136.98/32 23454\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd31\" } },"
                    "{ \"direction\" : 1,"
                      "\"description\" : \"permit out udp from any 50020 to 10.200.136.98/32 1-65535\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd30\" } },"
                    "{ \"direction\" : 2,"
                      "\"description\" : \"permit out udp from any 1-65535 to 10.200.136.98/32 23455\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2f\" } },"
                    "{ \"direction\" : 1,"
                      "\"description\" : \"permit out udp from any 50021 to 10.200.136.98/32 1-65535\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2e\" } }"
                  "]"
              "}"
            "],"
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 15,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 2"
          "}"
        "],"
        "\"ambr\" : { "
          "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
          "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
        "},"
        "\"subscribed_rau_tau_timer\" : 12,"
        "\"network_access_mode\" : 2, "
        "\"subscriber_status\" : 0, "
        "\"access_restriction_data\" : 32, "
        "\"security\" : { "
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\", "
          "\"opc\" : \"E8ED289D EBA952E4 283B54E8 8E6183CA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" } "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

    mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    mobile_identity_suci.routing_indicator1 = 0;
    mobile_identity_suci.routing_indicator2 = 0xf;
    mobile_identity_suci.routing_indicator3 = 0xf;
    mobile_identity_suci.routing_indicator4 = 0xf;
    mobile_identity_suci.protection_scheme_id = OGS_NAS_5GS_NULL_SCHEME;
    mobile_identity_suci.home_network_pki_value = 0;
    mobile_identity_suci.scheme_output[0] = 0x37;
    mobile_identity_suci.scheme_output[1] = 0x46;
    mobile_identity_suci.scheme_output[2] = 0;
    mobile_identity_suci.scheme_output[3] = 0;
    mobile_identity_suci.scheme_output[4] = 0x06;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, 13);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x4615380;
    test_ue->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    OGS_HEX(_k_string, strlen(_k_string), test_ue->k);
    OGS_HEX(_opc_string, strlen(_opc_string), test_ue->opc);

    sess = test_sess_add_by_apn(test_ue, "internet");
    ogs_assert(sess);

    /* Two eNB connects to MME */
    s1ap1 = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap1);

    s1ap2 = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap2);

    /* eNB connects to SGW */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* S1-Setup Reqeust/Response for Source eNB */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x1f2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /* S1-Setup Reqeust/Response for Target eNB */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x43);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /********** Insert Subscriber in Database */
    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);

    count = mongoc_collection_count (
        collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    if (count) {
        ABTS_TRUE(tc, mongoc_collection_remove(collection,
                MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    }
    bson_destroy(doc);

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection,
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    esmbuf = testesm_build_pdn_connectivity_request(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.integrity_protected = 1;
    test_ue->attach_request_param.guti = 1;
    test_ue->attach_request_param.last_visited_registered_tai = 1;
    test_ue->attach_request_param.drx_parameter = 1;
    test_ue->attach_request_param.tmsi_status = 1;
    test_ue->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue->attach_request_param.additional_update_type = 1;
    test_ue->attach_request_param.ue_usage_setting = 1;
    test_ue->attach_request_param.old_guti_type = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue->initial_ue_param, 0, sizeof(test_ue->initial_ue_param));
    test_ue->initial_ue_param.gummei_id = 1;
    test_ue->initial_ue_param.gummei.mme_gid = 2;
    test_ue->initial_ue_param.gummei.mme_code = 1;
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Identity response */
    emmbuf = testemm_build_identity_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Security mode complete */
    test_ue->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive E-RAB Setup Request +
     * Activate dedicated EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send E-RAB Setup Response */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    sendbuf = test_s1ap_build_e_rab_setup_response(bearer);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate dedicated EPS bearer context accept */
    esmbuf = testesm_build_activate_dedicated_eps_bearer_context_accept(bearer);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send ENB configuration transfer */
    sendbuf = test_s1ap_build_enb_configuration_transfer(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ENB configuration transfer */
    sendbuf = test_s1ap_build_enb_configuration_transfer(1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* DELAY is needed in dedicated EPS bearer */
    ogs_msleep(100);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    rv = test_gtpu_send_ping(gtpu1, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    rv = test_gtpu_send_ping(gtpu1, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Required */
    sendbuf = test_s1ap_build_handover_required(
            test_ue, S1AP_HandoverType_intralte,
            S1AP_ENB_ID_PR_macroENB_ID, 0x43,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_time_critical_handover);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Request Ack */
    ogs_list_for_each(&sess->bearer_list, bearer) {
        bearer->enb_s1u_addr = test_self()->gnb2_addr;
        bearer->enb_s1u_addr6 = test_self()->gnb2_addr6;
    }
    sendbuf = test_s1ap_build_handover_request_ack(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send eNB Status Transfer */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id--;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id--;

    sendbuf = test_s1ap_build_enb_status_transfer(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;
    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

    /* Receive MME Status Transfer */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Notify */
    test_ue->e_cgi.cell_id = 0x43a00;
    sendbuf = test_s1ap_build_handover_notify(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive End Mark */
    recvbuf = test_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = test_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send TAU Request */
    memset(&test_ue->tau_request_param, 0, sizeof(test_ue->tau_request_param));
    test_ue->tau_request_param.ue_network_capability = 1;
    test_ue->tau_request_param.last_visited_registered_tai = 1;
    test_ue->tau_request_param.drx_parameter = 1;
    test_ue->tau_request_param.eps_bearer_context_status = 1;
    test_ue->tau_request_param.ms_network_capability = 1;
    test_ue->tau_request_param.tmsi_status = 1;
    test_ue->tau_request_param.mobile_station_classmark_2 = 1;
    test_ue->tau_request_param.ue_usage_setting = 1;
    emmbuf = testemm_build_tau_request(
            test_ue, false, OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive TAU Accept */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive UE Context Release Command */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;
    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    rv = test_gtpu_send_ping(gtpu2, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    rv = test_gtpu_send_ping(gtpu2, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Required */
    sendbuf = test_s1ap_build_handover_required(
            test_ue, S1AP_HandoverType_intralte,
            S1AP_ENB_ID_PR_macroENB_ID, 0x1f2,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_time_critical_handover);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Request Ack */
    ogs_list_for_each(&sess->bearer_list, bearer) {
        bearer->enb_s1u_addr = test_self()->gnb1_addr;
        bearer->enb_s1u_addr6 = test_self()->gnb1_addr6;
    }
    sendbuf = test_s1ap_build_handover_request_ack(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send eNB Status Transfer */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id--;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id--;

    sendbuf = test_s1ap_build_enb_status_transfer(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;
    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

    /* Receive MME Status Transfer */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Notify */
    test_ue->e_cgi.cell_id = 0x1f20a0;
    sendbuf = test_s1ap_build_handover_notify(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive End Mark */
    recvbuf = test_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = test_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;
    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    rv = test_gtpu_send_ping(gtpu1, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    rv = test_gtpu_send_ping(gtpu1, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Required */
    sendbuf = test_s1ap_build_handover_required(
            test_ue, S1AP_HandoverType_intralte,
            S1AP_ENB_ID_PR_macroENB_ID, 0x43,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_time_critical_handover);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Request Ack */
    ogs_list_for_each(&sess->bearer_list, bearer) {
        bearer->enb_s1u_addr = test_self()->gnb1_addr;
        bearer->enb_s1u_addr6 = test_self()->gnb1_addr6;
    }
    sendbuf = test_s1ap_build_handover_request_ack(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Cancel */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id--;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id--;

    sendbuf = test_s1ap_build_handover_cancel(test_ue,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_tS1relocprep_expiry);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Recv Handover Cancel Ack */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive UE Context Release Command */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;
    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from SGW */
    testgnb_gtpu_close(gtpu2);
    testgnb_gtpu_close(gtpu1);

    /* Two eNB disonncect from MME */
    testenb_s1ap_close(s1ap1);
    testenb_s1ap_close(s1ap2);

    test_ue_remove(test_ue);
}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap1, *s1ap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;
    uint64_t mme_ue_s1ap_id;

    const char *_k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    uint8_t k[OGS_KEY_LEN];
    const char *_opc_string = "e8ed289deba952e4283b54e88e6183ca";
    uint8_t opc[OGS_KEY_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"901707364000060\", "
        "\"pdn\" : ["
          "{"
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
            "\"apn\" : \"internet\", "
            "\"pcc_rule\" : ["
              "{"
                "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2d\" },"
                "\"qos\" : {"
                  "\"qci\" : 1,"
                  "\"gbr\" : {"
                    "\"downlink\" : { \"$numberLong\" : \"64\" },"
                    "\"uplink\" : { \"$numberLong\" : \"44\" }"
                  "},"
                  "\"mbr\" : {"
                    "\"downlink\" : { \"$numberLong\" : \"64\" },"
                    "\"uplink\" : { \"$numberLong\" : \"44\" }"
                  "},"
                  "\"arp\" : {"
                    "\"priority_level\" : 3,"
                    "\"pre_emption_vulnerability\" : 0,"
                    "\"pre_emption_capability\" : 0 }"
                  "},"
                  "\"flow\" : ["
                    "{ \"direction\" : 2,"
                      "\"description\" : \"permit out udp from any 1-65535 to 10.200.136.98/32 23454\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd31\" } },"
                    "{ \"direction\" : 1,"
                      "\"description\" : \"permit out udp from any 50020 to 10.200.136.98/32 1-65535\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd30\" } },"
                    "{ \"direction\" : 2,"
                      "\"description\" : \"permit out udp from any 1-65535 to 10.200.136.98/32 23455\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2f\" } },"
                    "{ \"direction\" : 1,"
                      "\"description\" : \"permit out udp from any 50021 to 10.200.136.98/32 1-65535\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2e\" } }"
                  "]"
              "}"
            "],"
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 15,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 2"
          "}"
        "],"
        "\"ambr\" : { "
          "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
          "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
        "},"
        "\"subscribed_rau_tau_timer\" : 12,"
        "\"network_access_mode\" : 2, "
        "\"subscriber_status\" : 0, "
        "\"access_restriction_data\" : 32, "
        "\"security\" : { "
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\", "
          "\"opc\" : \"E8ED289D EBA952E4 283B54E8 8E6183CA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" } "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

    mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    mobile_identity_suci.routing_indicator1 = 0;
    mobile_identity_suci.routing_indicator2 = 0xf;
    mobile_identity_suci.routing_indicator3 = 0xf;
    mobile_identity_suci.routing_indicator4 = 0xf;
    mobile_identity_suci.protection_scheme_id = OGS_NAS_5GS_NULL_SCHEME;
    mobile_identity_suci.home_network_pki_value = 0;
    mobile_identity_suci.scheme_output[0] = 0x37;
    mobile_identity_suci.scheme_output[1] = 0x46;
    mobile_identity_suci.scheme_output[2] = 0;
    mobile_identity_suci.scheme_output[3] = 0;
    mobile_identity_suci.scheme_output[4] = 0x06;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, 13);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x4615380;
    test_ue->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    OGS_HEX(_k_string, strlen(_k_string), test_ue->k);
    OGS_HEX(_opc_string, strlen(_opc_string), test_ue->opc);

    sess = test_sess_add_by_apn(test_ue, "internet");
    ogs_assert(sess);

    /* Two eNB connects to MME */
    s1ap1 = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap1);

    s1ap2 = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap2);

    /* eNB connects to SGW */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* S1-Setup Reqeust/Response for Source eNB */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x1f2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /* S1-Setup Reqeust/Response for Target eNB */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x43);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /********** Insert Subscriber in Database */
    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);

    count = mongoc_collection_count (
        collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    if (count) {
        ABTS_TRUE(tc, mongoc_collection_remove(collection,
                MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    }
    bson_destroy(doc);

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection,
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    esmbuf = testesm_build_pdn_connectivity_request(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.integrity_protected = 1;
    test_ue->attach_request_param.guti = 1;
    test_ue->attach_request_param.last_visited_registered_tai = 1;
    test_ue->attach_request_param.drx_parameter = 1;
    test_ue->attach_request_param.tmsi_status = 1;
    test_ue->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue->attach_request_param.additional_update_type = 1;
    test_ue->attach_request_param.ue_usage_setting = 1;
    test_ue->attach_request_param.old_guti_type = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue->initial_ue_param, 0, sizeof(test_ue->initial_ue_param));
    test_ue->initial_ue_param.gummei_id = 1;
    test_ue->initial_ue_param.gummei.mme_gid = 2;
    test_ue->initial_ue_param.gummei.mme_code = 1;
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Identity response */
    emmbuf = testemm_build_identity_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Security mode complete */
    test_ue->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive E-RAB Setup Request +
     * Activate dedicated EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send E-RAB Setup Response */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    sendbuf = test_s1ap_build_e_rab_setup_response(bearer);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate dedicated EPS bearer context accept */
    esmbuf = testesm_build_activate_dedicated_eps_bearer_context_accept(bearer);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send ENB configuration transfer */
    sendbuf = test_s1ap_build_enb_configuration_transfer(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ENB configuration transfer */
    sendbuf = test_s1ap_build_enb_configuration_transfer(1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Required */
    sendbuf = test_s1ap_build_handover_required(
            test_ue, S1AP_HandoverType_intralte,
            S1AP_ENB_ID_PR_macroENB_ID, 0x43,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_time_critical_handover);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Request Ack */
    ogs_list_for_each(&sess->bearer_list, bearer) {
        bearer->enb_s1u_addr = test_self()->gnb1_addr;
        bearer->enb_s1u_addr6 = test_self()->gnb1_addr6;
    }
    sendbuf = test_s1ap_build_handover_request_ack(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Cancel */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id--;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id--;

    sendbuf = test_s1ap_build_handover_cancel(test_ue,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_tS1relocprep_expiry);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Recv Handover Cancel Ack */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive UE Context Release Command */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id;
    enb_ue_s1ap_id = test_ue->enb_ue_s1ap_id;

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Complete */
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;
    test_ue->enb_ue_s1ap_id = enb_ue_s1ap_id;

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from SGW */
    testgnb_gtpu_close(gtpu2);
    testgnb_gtpu_close(gtpu1);

    /* Two eNB disonncect from MME */
    testenb_s1ap_close(s1ap1);
    testenb_s1ap_close(s1ap2);

    test_ue_remove(test_ue);
}

static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap1, *s1ap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *emmbuf;
    ogs_pkbuf_t *esmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;

    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    uint32_t enb_ue_s1ap_id;
    uint64_t mme_ue_s1ap_id;

    const char *_k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    uint8_t k[OGS_KEY_LEN];
    const char *_opc_string = "e8ed289deba952e4283b54e88e6183ca";
    uint8_t opc[OGS_KEY_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"901707364000060\", "
        "\"pdn\" : ["
          "{"
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
            "\"apn\" : \"internet\", "
            "\"pcc_rule\" : ["
              "{"
                "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2d\" },"
                "\"qos\" : {"
                  "\"qci\" : 1,"
                  "\"gbr\" : {"
                    "\"downlink\" : { \"$numberLong\" : \"64\" },"
                    "\"uplink\" : { \"$numberLong\" : \"44\" }"
                  "},"
                  "\"mbr\" : {"
                    "\"downlink\" : { \"$numberLong\" : \"64\" },"
                    "\"uplink\" : { \"$numberLong\" : \"44\" }"
                  "},"
                  "\"arp\" : {"
                    "\"priority_level\" : 3,"
                    "\"pre_emption_vulnerability\" : 0,"
                    "\"pre_emption_capability\" : 0 }"
                  "},"
                  "\"flow\" : ["
                    "{ \"direction\" : 2,"
                      "\"description\" : \"permit out udp from any 1-65535 to 10.200.136.98/32 23454\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd31\" } },"
                    "{ \"direction\" : 1,"
                      "\"description\" : \"permit out udp from any 50020 to 10.200.136.98/32 1-65535\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd30\" } },"
                    "{ \"direction\" : 2,"
                      "\"description\" : \"permit out udp from any 1-65535 to 10.200.136.98/32 23455\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2f\" } },"
                    "{ \"direction\" : 1,"
                      "\"description\" : \"permit out udp from any 50021 to 10.200.136.98/32 1-65535\","
                      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2e\" } }"
                  "]"
              "}"
            "],"
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 15,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 2"
          "}"
        "],"
        "\"ambr\" : { "
          "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
          "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
        "},"
        "\"subscribed_rau_tau_timer\" : 12,"
        "\"network_access_mode\" : 2, "
        "\"subscriber_status\" : 0, "
        "\"access_restriction_data\" : 32, "
        "\"security\" : { "
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\", "
          "\"opc\" : \"E8ED289D EBA952E4 283B54E8 8E6183CA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" } "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

    mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    mobile_identity_suci.routing_indicator1 = 0;
    mobile_identity_suci.routing_indicator2 = 0xf;
    mobile_identity_suci.routing_indicator3 = 0xf;
    mobile_identity_suci.routing_indicator4 = 0xf;
    mobile_identity_suci.protection_scheme_id = OGS_NAS_5GS_NULL_SCHEME;
    mobile_identity_suci.home_network_pki_value = 0;
    mobile_identity_suci.scheme_output[0] = 0x37;
    mobile_identity_suci.scheme_output[1] = 0x46;
    mobile_identity_suci.scheme_output[2] = 0;
    mobile_identity_suci.scheme_output[3] = 0;
    mobile_identity_suci.scheme_output[4] = 0x06;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, 13);
    ogs_assert(test_ue);

    test_ue->e_cgi.cell_id = 0x4615380;
    test_ue->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue->nas.value = OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH;

    OGS_HEX(_k_string, strlen(_k_string), test_ue->k);
    OGS_HEX(_opc_string, strlen(_opc_string), test_ue->opc);

    sess = test_sess_add_by_apn(test_ue, "internet");
    ogs_assert(sess);

    /* Two eNB connects to MME */
    s1ap1 = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap1);

    s1ap2 = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap2);

    /* eNB connects to SGW */
    gtpu1 = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = test_gtpu_server(2, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* S1-Setup Reqeust/Response for Source eNB */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x1f2);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /* S1-Setup Reqeust/Response for Target eNB */
    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x43);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(NULL, recvbuf);

    /********** Insert Subscriber in Database */
    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);

    count = mongoc_collection_count (
        collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    if (count) {
        ABTS_TRUE(tc, mongoc_collection_remove(collection,
                MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    }
    bson_destroy(doc);

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection,
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /* Send Attach Request */
    memset(&sess->pdn_connectivity_param,
            0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.pco = 1;
    esmbuf = testesm_build_pdn_connectivity_request(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);

    memset(&test_ue->attach_request_param,
            0, sizeof(test_ue->attach_request_param));
    test_ue->attach_request_param.integrity_protected = 1;
    test_ue->attach_request_param.guti = 1;
    test_ue->attach_request_param.last_visited_registered_tai = 1;
    test_ue->attach_request_param.drx_parameter = 1;
    test_ue->attach_request_param.tmsi_status = 1;
    test_ue->attach_request_param.mobile_station_classmark_2 = 1;
    test_ue->attach_request_param.additional_update_type = 1;
    test_ue->attach_request_param.ue_usage_setting = 1;
    test_ue->attach_request_param.old_guti_type = 1;
    emmbuf = testemm_build_attach_request(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);

    memset(&test_ue->initial_ue_param, 0, sizeof(test_ue->initial_ue_param));
    test_ue->initial_ue_param.gummei_id = 1;
    test_ue->initial_ue_param.gummei.mme_gid = 2;
    test_ue->initial_ue_param.gummei.mme_code = 1;
    sendbuf = test_s1ap_build_initial_ue_message(
            test_ue, emmbuf, S1AP_RRC_Establishment_Cause_mo_Signalling, false);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Identity response */
    emmbuf = testemm_build_identity_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Authentication response */
    emmbuf = testemm_build_authentication_response(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Security mode complete */
    test_ue->mobile_identity_imeisv_presence = true;
    emmbuf = testemm_build_security_mode_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ESM Information Response */
    esmbuf = testesm_build_esm_information_response(sess);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Capability Info Indication */
    sendbuf = tests1ap_build_ue_radio_capability_info_indication(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    sendbuf = test_s1ap_build_initial_context_setup_response(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    test_ue->nr_cgi.cell_id = 0x1234502;
    bearer = test_bearer_find_by_ue_ebi(test_ue, 5);
    ogs_assert(bearer);
    esmbuf = testesm_build_activate_default_eps_bearer_context_accept(
            bearer, false);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    emmbuf = testemm_build_attach_complete(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, emmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, emmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Receive E-RAB Setup Request +
     * Activate dedicated EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send E-RAB Setup Response */
    bearer = test_bearer_find_by_ue_ebi(test_ue, 6);
    ogs_assert(bearer);
    sendbuf = test_s1ap_build_e_rab_setup_response(bearer);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate dedicated EPS bearer context accept */
    esmbuf = testesm_build_activate_dedicated_eps_bearer_context_accept(bearer);
    ABTS_PTR_NOTNULL(tc, esmbuf);
    sendbuf = test_s1ap_build_uplink_nas_transport(test_ue, esmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send ENB configuration transfer */
    sendbuf = test_s1ap_build_enb_configuration_transfer(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send ENB configuration transfer */
    sendbuf = test_s1ap_build_enb_configuration_transfer(1);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Required */
    sendbuf = test_s1ap_build_handover_required(
            test_ue, S1AP_HandoverType_intralte,
            S1AP_ENB_ID_PR_macroENB_ID, 0x43,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_time_critical_handover);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send Handover Failure */
    sendbuf = test_s1ap_build_handover_failure(test_ue,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_unspecified);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Preparation Failure */
    mme_ue_s1ap_id = test_ue->mme_ue_s1ap_id--;

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    test_ue->mme_ue_s1ap_id = mme_ue_s1ap_id;

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    tests1ap_recv(test_ue, recvbuf);

    /* Send UE Context Release Complete */
    test_ue->enb_ue_s1ap_id++;
    sendbuf = test_s1ap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue->imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from SGW */
    testgnb_gtpu_close(gtpu2);
    testgnb_gtpu_close(gtpu1);

    /* Two eNB disonncect from MME */
    testenb_s1ap_close(s1ap1);
    testenb_s1ap_close(s1ap2);

    test_ue_remove(test_ue);
}

abts_suite *test_epc_s1(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}
