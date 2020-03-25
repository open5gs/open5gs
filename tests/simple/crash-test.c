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

static void crash_test1(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 21;

    mongoc_collection_t *collection = NULL;
    int64_t count = 0;
    bson_t *doc = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"311980000000725\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c7\" }, "
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1024000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1024000\" } "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 8,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 2"
          "}"
        "],"
        "\"ambr\" : { "
          "\"uplink\" : { \"$numberLong\" : \"1024000\" }, "
          "\"downlink\" : { \"$numberLong\" : \"1024000\" } "
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
          "\"rand\" : \"2AE4FC02 1DD4D1C2 E0A277C2 317C2E67\" "
        "}, "
        "\"__v\" : 0 "
      "}";
    uint8_t tmp[OGS_MAX_SDU_LEN];

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x002343d, 12345, 1, 1, 2);
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

    doc = BCON_NEW("imsi", BCON_UTF8("311980000000725"));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /* Send Attach Request */
    mme_self()->mme_ue_s1ap_id = 207;
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

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    msgindex = 22;

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

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            209, 209, 5, 0x01001908, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    msgindex = 21;

    /* Send Initial Context Setup Failure */
    rv = tests1ap_build_initial_context_setup_failure(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    doc = BCON_NEW("imsi", BCON_UTF8("311980000000725"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);
}

static void crash_test2(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;
    int msgindex = 21;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    mongoc_collection_t *collection = NULL;
    int64_t count = 0;
    bson_t *doc = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"311980000000725\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c7\" }, "
            "\"ambr\" : {"
              "\"uplink\" : { \"$numberLong\" : \"1024000\" }, "
              "\"downlink\" : { \"$numberLong\" : \"1024000\" } "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 8,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 2"
          "}"
        "],"
        "\"ambr\" : { "
          "\"uplink\" : { \"$numberLong\" : \"1024000\" }, "
          "\"downlink\" : { \"$numberLong\" : \"1024000\" } "
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
          "\"rand\" : \"2AE4FC02 1DD4D1C2 E0A277C2 317C2E67\" "
        "}, "
        "\"__v\" : 0 "
      "}";
    uint8_t tmp[OGS_MAX_SDU_LEN];

    /* eNB connects to MME */
    s1ap = testenb_s1ap_client("127.0.0.1");
    ABTS_PTR_NOTNULL(tc, s1ap);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x002343d, 12345, 1, 1, 2);
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

    doc = BCON_NEW("imsi", BCON_UTF8("311980000000725"));
    ABTS_PTR_NOTNULL(tc, doc);
    do {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /* Send Attach Request */
    mme_self()->mme_ue_s1ap_id = 207;
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

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    msgindex = 22;

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

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            209, 209, 5, 0x01001908, "127.0.0.5");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive EMM information */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Retreive M-TMSI */
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(209);
    ogs_assert(enb_ue);
    mme_ue = enb_ue->mme_ue;
    ogs_assert(mme_ue);
    m_tmsi = mme_ue->guti.m_tmsi;

    /* Send UE Context Release Request */
    rv = tests1ap_build_ue_context_release_request(&sendbuf, msgindex-1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Service Request */
    rv = tests1ap_build_service_request(&sendbuf, 0x00d200, 4, 0xef95, m_tmsi);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Initial Context Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Initial Context Setup Failure */
    rv = tests1ap_build_initial_context_setup_failure(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UE Context Release Request */
    rv = tests1ap_build_ue_context_release_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send Service Request */
    rv = tests1ap_build_service_request(&sendbuf, 0x00d300, 4, 0xef95, m_tmsi);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Service Reject */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    doc = BCON_NEW("imsi", BCON_UTF8("311980000000725"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    testenb_s1ap_close(s1ap);
}

abts_suite *test_crash(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, crash_test1, NULL);
    abts_run_test(suite, crash_test2, NULL);

    return suite;
}
