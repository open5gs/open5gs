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
    int msgindex = 24;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    test_ue_t test_ue;

    const char *_k_string = "4d060eab0a7743bc8eede608a94b63de";
    uint8_t k[OGS_KEY_LEN];
    const char *_opc_string = "7c0f7d5a427048a86fb429cc003c117c";
    uint8_t opc[OGS_KEY_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"235047364000060\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
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
          "\"k\" : \"4d060eab0a7743bc8eede608a94b63de\", "
          "\"opc\" : \"7c0f7d5a427048a86fb429cc003c117c\", "
          "\"amf\" : \"9001\", "
          "\"sqn\" : { \"$numberLong\" : \"7085\" }, "
          "\"rand\" : \"198e06800ea7a4c61e9e424f2558a774\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&test_ue, 0, sizeof(test_ue));
    test_ue.imsi = (char *)"235047364000060";

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = testenb_gtpu_server("127.0.0.5");
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x2000, 1, 235, 4, 2);
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

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /* Send Attach Request */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Identity Response */
    rv = tests1ap_build_identity_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            16777373, 1, 5, 1, "127.0.0.5");
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

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 25;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    test_ue_t test_ue;

    const char *_k_string = "4d060eab0a7743bc8eede608a94b63de";
    uint8_t k[OGS_KEY_LEN];
    const char *_opc_string = "7c0f7d5a427048a86fb429cc003c117c";
    uint8_t opc[OGS_KEY_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"235047364000060\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
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
          "\"k\" : \"4d060eab0a7743bc8eede608a94b63de\", "
          "\"opc\" : \"7c0f7d5a427048a86fb429cc003c117c\", "
          "\"amf\" : \"9001\", "
          "\"sqn\" : { \"$numberLong\" : \"9034\" }, "
          "\"rand\" : \"bb17c5f251e2ff6a528a9e9f70248ed9\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&test_ue, 0, sizeof(test_ue));
    test_ue.imsi = (char *)"235047364000060";

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = testenb_gtpu_server("127.0.0.5");
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x2000, 1, 235, 4, 2);
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

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /* Send Attach Request */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(50);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            1, 1, 5, 0x10783d92, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

#if 0
    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
#endif

    ogs_msleep(50);

#if 0
    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);
#endif

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

static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 26;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    test_ue_t test_ue;

    const char *_k_string = "4d060eab0a7743bc8eede608a94b63de";
    uint8_t k[OGS_KEY_LEN];
    const char *_opc_string = "7c0f7d5a427048a86fb429cc003c117c";
    uint8_t opc[OGS_KEY_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"235047364000060\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c7\" }, "
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
          "\"k\" : \"4d060eab0a7743bc8eede608a94b63de\", "
          "\"opc\" : \"7c0f7d5a427048a86fb429cc003c117c\", "
          "\"amf\" : \"9001\", "
          "\"sqn\" : { \"$numberLong\" : \"8968\" }, "
          "\"rand\" : \"5616d4deb87e39be6259ef8e5c753c0b\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    /* Setup Test UE & Session Context */
    memset(&test_ue, 0, sizeof(test_ue));
    test_ue.imsi = (char *)"235047364000060";

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* eNB connects to SGW */
    gtpu = testenb_gtpu_server("127.0.0.5");
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x2000, 1, 235, 4, 2);
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

    collection = mongoc_client_get_collection(
        ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /* Send Attach Request */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            1, 1, 5, 0x1654c84e, "127.0.0.5");
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

abts_suite *test_r16(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}
