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

#include "test-app.h"

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
        "000b403b00000300 000005c001a00102 000800020018001a 002524075200906d"
        "231ff57ef278c719 1d170303deb610d0 7c4defa47480001f 2b5350926bdb3a";
    const char *_security_mode_command = 
        "000b4025"
        "00000300000005c0 01a0010200080002 0018001a000f0e37 62522c0900075d01"
        "0002e0e0c1";
    const char *_esm_information_request =
        "000b402000000300 000005c001a00102 000800020018001a 000a0927846a01a8"
        "010201d9";
    const char *_initial_context_setup_request = 
        "00090080c1000006 00000005c001a001 0200080002001800 42000a183d090000"
        "603d090000001800 70000034006b4500 093d0f807f000002 000000015c279a3e"
        "783d02074201490c 0313401000320033 0034003500315201 c101090c07737461"
        "72656e7403636f6d 05010a2d00025e06 fefee2e20303270f 80000d0408080808"
        "000d040808040450 0bf6134010801e64 d90068e259496401 01006b000518000c"
        "00000049002046c7 89cba93e9b977583 35c097e6c386c872 e4b82434a48037c3"
        "0601590edd8e";

    const char *_emm_information = 
        "000b403b00000300 000005c001a00102 000800020018001a 0025242729f8b0bb"
        "030761430f10004f 00700065006e0035 0047005347914032 80113463490100";

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"310014987654004\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"starent.com\", "
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1000000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1000000\" } "
            "},"
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

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = testenb_gtpu_server("127.0.0.5");
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

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /********** Insert Subscriber in Database */
    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("310014987654004"));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    mme_self()->mme_ue_s1ap_id = 27263233;
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
        OGS_HEX(_esm_information_request, strlen(_security_mode_command), tmp),
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
    ABTS_TRUE(tc, memcmp(recvbuf->data, tmp, 62) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+66, tmp+66, 78) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+148, tmp+148, 50) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(50);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            27263233, 24, 5, 1, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(50);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    OGS_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->data, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+32, tmp+32, 20) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send GTP-U ICMP Packet */
    rv = testgtpu_build_ping(&sendbuf, 1, "10.45.0.2", "10.45.0.1");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_gtpu_send(gtpu, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = testenb_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("310014987654004"));
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
