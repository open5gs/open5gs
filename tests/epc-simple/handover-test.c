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

static void handover_test1(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap1, *s1ap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 9;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    const char *_nh1 = "10"
        "3715a966536b75b4 d46e99774dcdb344 5ce5e893fbbf28f4 9f58508c36f827cc";
    const char *_nh2 = "18"
        "a29ed36339514717 481992f77f47a9af 934a7b763afcec39 edf5071461db6ae8";

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
    "{"
      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2b\" }," 
      "\"imsi\" : \"001010123456801\","
      "\"pdn\" : ["
      "{ \"apn\" : \"internet.ng2.mnet\","
        "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd32\" },"
        "\"qos\" : {"
          "\"qci\" : 9,"
          "\"arp\" : {"
            "\"priority_level\" : 8,"
            "\"pre_emption_vulnerability\" : 1,"
            "\"pre_emption_capability\" : 1 } },"
        "\"type\" : 2 },"
      "{ \"apn\" : \"internet\","
        "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2c\" },"
        "\"pcc_rule\" : ["
        "{ \"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2d\" },"
          "\"qos\" : {"
            "\"qci\" : 1,"
            "\"gbr\" : {"
              "\"downlink\" : { \"$numberLong\" : \"64\" },"
              "\"uplink\" : { \"$numberLong\" : \"44\" } },"
            "\"mbr\" : {"
              "\"downlink\" : { \"$numberLong\" : \"64\" },"
              "\"uplink\" : { \"$numberLong\" : \"44\" } },"
            "\"arp\" : {"
              "\"priority_level\" : 3,"
              "\"pre_emption_vulnerability\" : 0,"
              "\"pre_emption_capability\" : 0 } },"
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
            "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2e\" } } ]"
        "} ],"
        "\"ambr\" : {"
          "\"downlink\" : { \"$numberLong\" : \"35840\" },"
          "\"uplink\" : { \"$numberLong\" : \"15360\" } },"
        "\"qos\" : {"
          "\"qci\" : 7,"
          "\"arp\" : {"
            "\"priority_level\" : 1,"
            "\"pre_emption_vulnerability\" : 0,"
            "\"pre_emption_capability\" : 1 } },"
        "\"type\" : 2 }"
      "],"
      "\"ambr\" : {"
        "\"downlink\" : { \"$numberLong\" : \"1024000\" },"
        "\"uplink\" : { \"$numberLong\" : \"1024000\" } },"
      "\"subscribed_rau_tau_timer\" : 12,"
      "\"network_access_mode\" : 2,"
      "\"subscriber_status\" : 0,"
      "\"access_restriction_data\" : 32,"
      "\"security\" : {"
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\","
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\","
          "\"amf\" : \"8000\","
          "\"sqn\" : { \"$numberLong\" : \"32\" }, "
          "\"rand\" : \"0a303a1e 63603f61 404c1241 30320f39\" }, "
      "\"__v\" : 0"
    "}";

    mme_self()->mme_ue_s1ap_id = 16777689;

    /* Two eNB connects to MME */
    s1ap1 = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap1);

    s1ap2 = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap2);

    /* eNB connects to SGW */
    gtpu1 = testenb_gtpu_server("127.0.0.5");
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = testenb_gtpu_server("127.0.0.4");
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* S1-Setup Reqeust/Response for Source eNB */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64, 12345, 1, 1, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* S1-Setup Reqeust/Response for Target eNB */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f65, 12345, 1, 1, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456801"));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /***********************************************************************
     * Attach Request : Known IMSI, Integrity Protected, No Security Context
     * Send Initial-UE Message + Attach Request + PDN Connectivity        */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            16777690, 1, 5, 1, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive E-RAB Setup Request + 
     * Activate dedicated EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send E-RAB Setup Response */
    rv = tests1ap_build_e_rab_setup_response(&sendbuf,
            33554492, 1, 6, 2, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate dedicated EPS bearer context accept */
    rv = tests1ap_build_activate_dedicated_bearer_accept(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(50);

    /* Send Path Switch Request */
    rv = tests1ap_build_path_switch_request(&sendbuf,
            1, 16777690, 1, 2, 5, 1, "127.0.0.5", "127.0.0.4");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Path Switch Ack */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_TRUE(tc, memcmp(recvbuf->data + 26,
                OGS_HEX(_nh1, strlen(_nh1), tmp), 33) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Path Switch Request */
    rv = tests1ap_build_path_switch_request(&sendbuf, 0, 16777690, 2, 2, 5, 1,
            "127.0.0.5", "127.0.0.4");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive Path Switch Ack */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_TRUE(tc, memcmp(recvbuf->data + 26,
                OGS_HEX(_nh2, strlen(_nh2), tmp), 33) == 0);
    ogs_pkbuf_free(recvbuf);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("001010123456801"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from SGW */
    testenb_gtpu_close(gtpu2);
    testenb_gtpu_close(gtpu1);

    /* Two eNB disonncect from MME */
    testenb_s1ap_close(s1ap1);
    testenb_s1ap_close(s1ap2);
}

static void handover_test2(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap1, *s1ap2;
    ogs_socknode_t *gtpu1, *gtpu2;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 10;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
    "{"
      "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2b\" }," 
      "\"imsi\" : \"001010123456815\","
      "\"pdn\" : ["
      "{ \"apn\" : \"internet.ng2.mnet\","
        "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd32\" },"
        "\"qos\" : {"
          "\"qci\" : 9,"
          "\"arp\" : {"
            "\"priority_level\" : 8,"
            "\"pre_emption_vulnerability\" : 1,"
            "\"pre_emption_capability\" : 1 } },"
        "\"type\" : 2 },"
      "{ \"apn\" : \"internet\","
        "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2c\" },"
        "\"pcc_rule\" : ["
        "{ \"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2d\" },"
          "\"qos\" : {"
            "\"qci\" : 1,"
            "\"gbr\" : {"
              "\"downlink\" : { \"$numberLong\" : \"976\" },"
              "\"uplink\" : { \"$numberLong\" : \"976\" } },"
            "\"mbr\" : {"
              "\"downlink\" : { \"$numberLong\" : \"976\" },"
              "\"uplink\" : { \"$numberLong\" : \"976\" } },"
            "\"arp\" : {"
              "\"priority_level\" : 1,"
              "\"pre_emption_vulnerability\" : 0,"
              "\"pre_emption_capability\" : 0 } },"
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
            "\"_id\" : { \"$oid\" : \"599eb929c850caabcbfdcd2e\" } } ]"
        "} ],"
        "\"ambr\" : {"
          "\"downlink\" : { \"$numberLong\" : \"35840\" },"
          "\"uplink\" : { \"$numberLong\" : \"15360\" } },"
        "\"qos\" : {"
          "\"qci\" : 7,"
          "\"arp\" : {"
            "\"priority_level\" : 1,"
            "\"pre_emption_vulnerability\" : 1,"
            "\"pre_emption_capability\" : 0 } },"
        "\"type\" : 2 }"
      "],"
      "\"ambr\" : {"
        "\"downlink\" : { \"$numberLong\" : \"202400\" },"
        "\"uplink\" : { \"$numberLong\" : \"202400\" } },"
      "\"subscribed_rau_tau_timer\" : 12,"
      "\"network_access_mode\" : 2,"
      "\"subscriber_status\" : 0,"
      "\"access_restriction_data\" : 32,"
      "\"security\" : {"
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\","
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\","
          "\"amf\" : \"8000\","
          "\"sqn\" : { \"$numberLong\" : \"161\" }, "
          "\"rand\" : \"160b4726 39115e14 075f4731 50355216\" }, "
      "\"__v\" : 0"
    "}";

    mme_self()->mme_ue_s1ap_id = 33554627;

    /* Two eNB connects to MME */
    s1ap1 = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap1);

    s1ap2 = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap2);

    /* eNB connects to SGW */
    gtpu1 = testenb_gtpu_server("127.0.0.5");
    ABTS_PTR_NOTNULL(tc, gtpu1);

    gtpu2 = testenb_gtpu_server("127.0.0.4");
    ABTS_PTR_NOTNULL(tc, gtpu2);

    /* S1-Setup Reqeust/Response for Source eNB */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x001f2, 12345, 1, 1, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* S1-Setup Reqeust/Response for Target eNB */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x00043, 12345, 1, 1, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456815"));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /***********************************************************************
     * Attach Request : Known IMSI, No Security Context
     * Send Initial-UE Message + Attach Request + PDN Connectivity        */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Identity Response */
    rv = tests1ap_build_identity_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            33554628, 12, 5, 1, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Retreive M-TMSI */
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(33554628);
    ogs_assert(enb_ue);
    mme_ue = enb_ue->mme_ue;
    ogs_assert(mme_ue);
    m_tmsi = mme_ue->guti.m_tmsi;

    /* Receive E-RAB Setup Request + 
     * Activate dedicated EPS bearer context request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send E-RAB Setup Response */
    rv = tests1ap_build_e_rab_setup_response(&sendbuf,
            33554628, 12, 6, 2, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Activate dedicated EPS bearer context accept */
    rv = tests1ap_build_activate_dedicated_bearer_accept(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send ENB configuration transfer */
    rv = tests1ap_build_enb_configuration_transfer(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send ENB configuration transfer */
    rv = tests1ap_build_enb_configuration_transfer(&sendbuf, 1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME configuration transfer */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Required */
    rv = tests1ap_build_handover_required(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

#if TEST_HANDOVER_FAILURE
    /* Send Handover Failure */
    rv = tests1ap_build_handover_failure(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

    /* Send Handover Request Ack */
    rv = tests1ap_build_handover_request_ack(&sendbuf,
            1, 33554629, 8, 2, 5, 1, "127.0.0.5", "127.0.0.4");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send eNB Status Transfer */
    rv = tests1ap_build_enb_status_transfer(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME Status Transfer */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Notify */
    rv = tests1ap_build_handover_notify(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Tracking Area Update Request */
    rv = tests1ap_build_tau_request(&sendbuf, 1,
            0x000300, 0x000800, 0, m_tmsi, 4, 0, mme_ue->knas_int);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Tracking Area Update Accept */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex-1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Handover Required */
    rv = tests1ap_build_handover_required(&sendbuf, 1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Request Ack */
    rv = tests1ap_build_handover_request_ack(&sendbuf,
            0, 33554630, 13, 2, 5, 1, "127.0.0.5", "127.0.0.4");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send eNB Status Transfer */
    rv = tests1ap_build_enb_status_transfer(&sendbuf, 1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive MME Status Transfer */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Notify */
    rv = tests1ap_build_handover_notify(&sendbuf, 1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive End Mark */
    recvbuf = testenb_gtpu_read(gtpu2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Handover Required */
    rv = tests1ap_build_handover_required(&sendbuf, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Request */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Request Ack */
    rv = tests1ap_build_handover_request_ack(&sendbuf,
            0, 33554631, 9, 2, 5, 1, "127.0.0.5", "127.0.0.4");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Handover Command */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Handover Cancel */
    rv = tests1ap_build_handover_cancel(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap1, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Recv Handover Cancel Ack */
    recvbuf = testenb_s1ap_read(s1ap1);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Recv UE Context Relase Command */
    recvbuf = testenb_s1ap_read(s1ap2);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap2, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("001010123456815"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* Two eNB disonncect from MME */
    testenb_s1ap_close(s1ap1);
    testenb_s1ap_close(s1ap2);

    /* eNB disonncect from SGW */
    testenb_gtpu_close(gtpu1);
    testenb_gtpu_close(gtpu2);
}

abts_suite *test_handover(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, handover_test1, NULL);
    abts_run_test(suite, handover_test2, NULL);

    return suite;
}
