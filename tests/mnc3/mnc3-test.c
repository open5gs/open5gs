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

#include "test-epc.h"

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 15;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    const char *_authentication_request = 
        "000b403800000300 0000020001000800 020018001a002524 075200906d231ff5"
        "7ef278c7191d1703 03deb610d07c4def a47480001f2b5350 926bdb3a";
    const char *_security_mode_command = 
        "000b402c00000300 0000020001000800 020018001a001918 37b4fe6e2700075d"
        "010002e0e0c14f08 d14b7a88d1d1d02e";
    const char *_esm_information_request =
        "000b401d00000300 0000020001000800 020018001a000a09 27846a01a8010201"
        "d9";
    const char *_initial_context_setup_request = 
        "00090080bf000006 0000000200010008 000200180042000a 183d090000603d09"
        "0000001800710000 34006c4500093d0f 807f000007000000 025d2722ab599e02"
        "074201490c031340 1000320033003400 3500315201c10109 0c0773746172656e"
        "7403636f6d05010a 2d00025e06fefee2 e20303270f80000d 0408080808000d04"
        "08080404500bf613 4010801e64e400ae b859496402010800 6b000518000c0000"
        "0049002046c789cb a93e9b97758335c0 97e6c386c872e4b8 2434a48037c30601"
        "590edd8e";
    const char *_emm_information = 
        "000b403800000300 0000020001000800 020018001a002524 27e211a94a030761"
        "430f10004f007000 65006e0035004700 5347028040325204 69490100";

    test_ue_t test_ue;
    test_sess_t test_sess;

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
        "\"imsi\" : \"310014987654004\", "
#if 0
        "\"msisdn\" : [\"821012345678\", \"82107654321\" ], "
        "\"msisdn\" : [\"82107654321\", \"821012345678\" ], "
#endif
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"starent.com\", "
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
            "},"
#if 0
            "\"ue\" : { \"addr\" : \"10.45.0.2\", \"addr6\" : \"cafe::2\" },"
#endif
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 15,"
                "\"pre_emption_vulnerability\" : 0, "
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
          "\"sqn\" : { \"$numberLong\" : \"64\" }, "
          "\"rand\" : \"906d231f f57ef278 c7191d17 0303deb6\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&test_ue, 0, sizeof(test_ue));
    memset(&test_sess, 0, sizeof(test_sess));
    test_sess.test_ue = &test_ue;
    test_ue.sess = &test_sess;

    test_ue.imsi = (char *)"310014987654004";

    test_sess.gnb_n3_ip.ipv4 = true;
    test_sess.gnb_n3_ip.addr = inet_addr(TEST_GNB_IPV4);
    test_sess.gnb_n3_teid = 0;

    test_sess.upf_n3_ip.ipv4 = true;
    test_sess.upf_n3_ip.addr = inet_addr(TEST_SGWU_IPV4);

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client(TEST_MME_IPV4);
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = testenb_gtpu_server(TEST_ENB_IPV4);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64, 51, 310, 14, 3);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    rv = ogs_s1ap_decode(&message, recvbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ogs_s1ap_free(&message);
    ogs_pkbuf_free(recvbuf);

    /********** Insert Subscriber in Database */
    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue.imsi));
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

    doc = BCON_NEW("imsi", BCON_UTF8(test_ue.imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /* Send Attach Request */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_TRUE(tc, memcmp(recvbuf->data, 
        OGS_HEX(_authentication_request, strlen(_authentication_request), tmp),
        recvbuf->len) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_TRUE(tc, memcmp(recvbuf->data,
        OGS_HEX(_security_mode_command, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_TRUE(tc, memcmp(recvbuf->data, 
        OGS_HEX(_esm_information_request,
            strlen(_esm_information_request), tmp),
        recvbuf->len) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    OGS_HEX(_initial_context_setup_request,
            strlen(_initial_context_setup_request), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->data, tmp, 59) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+63, tmp+63, 78) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+145, tmp+145, 50) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            1, 24, 5, 1, TEST_ENB_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    OGS_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->data, tmp, 25) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+29, tmp+29, 20) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send GTP-U ICMP Packet */
    test_sess.upf_n3_teid = 2;
    test_sess.ue_ip.addr = inet_addr("10.45.0.2");

    rv = test_gtpu_build_ping(&sendbuf, &test_sess, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testgnb_gtpu_sendto(gtpu, sendbuf, TEST_SGWU_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = testenb_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8(test_ue.imsi));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);

    /* eNB disonncect from SGW */
    testenb_gtpu_close(gtpu);
}

abts_suite *test_mnc3(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);

    return suite;
}
