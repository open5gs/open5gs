#include <mongoc.h>

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "context.h"
#include "mme_context.h"
#include "s1ap_message.h"

#include "testutil.h"
#include "tests1ap.h"

/**************************************************************
 * eNB : MACRO
 * UE : IMSI 
 * Protocol Configuration Options in ESM information response */
static void nas_sm_test1(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int rc;
    int i;
    int msgindex = 0;

    c_uint8_t tmp[MAX_SDU_LEN];
    char *_authentication_request = 
        "000b403b00000300 000005c00100009d 000800020001001a 0025240752002008"
        "0c3818183b522614 162c07601d0d10f1 1b89a2a8de8000ad 0ccf7f55e8b20d";
    char *_security_mode_command = 
        "000b402700000300 000005c00100009d 000800020001001a 00111037f933b5d5"
        "00075d010005e060 c04070";
    char *_esm_information_request =
        "000b402000000300 000005c00100009d 000800020001001a 000a092779012320"
        "010221d9";
    char *_initial_context_setup_request = 
        "00090080e4000006 00000005c0010000 9d00080002000100 42000a183e800000"
        "603e800000001800 809200003400808c 4540092000000000 000000000f800a01"
        "23d8000000017527 e07f582902074202 49064000f1105ba0 004c5221c10509ff"
        "ffffff0908696e74 65726e657405012d 2d2d015e06000000 000404272980c223"
        "0403000004802110 0200001081060808 0808830604040404 000d040808080800"
        "0d0404040404500b f600f11000020100 0000015312172c59 49640125006b0005"
        "1c000c0000004900 203311c603c6a6d6 7f695e5ac02bb75b 381b693c3893a6d9"
        "32fd91823544e3e7 9b";
    char *_emm_information = 
        "000b402a00000300 000005c00100009d 000800020001001a 001413279fcc7266"
        "0307614771304112 527563490100";

    mongoc_collection_t *collection = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456819\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c7\" }, "
            "\"pdn_ambr\" : {"
              "\"max_bandwidth_ul\" : 1024000, "
              "\"max_bandwidth_dl\" : 1024000  "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 8,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 0"
          "}"
        "],"
        "\"ue_ambr\" : { "
          "\"max_bandwidth_ul\" : 1024000, "
          "\"max_bandwidth_dl\" : 1024000 "
        "},"
        "\"subscribed_rau_tau_timer\" : 12,"
        "\"network_access_mode\" : 2, "
        "\"subscriber_status\" : 0, "
        "\"access_restriction_data\" : 32, "
        "\"security\" : { "
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\", "
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" }, "
          "\"rand\" : \"20080C38 18183B52 2614162C 07601D0D\" "
        "}, "
        "\"__v\" : 0 "
      "}";
    bson_t *doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_ERROR);

    /* eNB connects to MME */
    sock = tests1ap_enb_connect();
    ABTS_PTR_NOTNULL(tc, sock);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(&sendbuf, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    /* Send Initial-UE Message */
    mme_self()->mme_ue_s1ap_id = 16777372;
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    recvbuf->len = 63;
    ABTS_TRUE(tc, memcmp(recvbuf->payload, 
        CORE_HEX(_authentication_request, strlen(_authentication_request), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 43;
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_security_mode_command, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 36;
    ABTS_TRUE(tc, memcmp(recvbuf->payload, 
        CORE_HEX(_esm_information_request, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 233;
    ABTS_TRUE(tc, memcmp(recvbuf->payload, 
        CORE_HEX(_initial_context_setup_request, 
            strlen(_initial_context_setup_request), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive EMM information */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 46;
    CORE_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->payload, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->payload+43, tmp+43, 3) == 0);
    pkbuf_free(recvbuf);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456819"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);
}

/**************************************************************
 * eNB : HOME
 * UE : IMSI 
 * Protocol Configuration Options in PDN Connectivity Request */
static void nas_sm_test2(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int rc;
    int i;
    int msgindex = 1;

    mongoc_collection_t *collection = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456826\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c7\" }, "
            "\"pdn_ambr\" : {"
              "\"max_bandwidth_ul\" : 1024000, "
              "\"max_bandwidth_dl\" : 1024000  "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 8,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 0"
          "}"
        "],"
        "\"ue_ambr\" : { "
          "\"max_bandwidth_ul\" : 1024000, "
          "\"max_bandwidth_dl\" : 1024000 "
        "},"
        "\"subscribed_rau_tau_timer\" : 12,"
        "\"network_access_mode\" : 2, "
        "\"subscriber_status\" : 0, "
        "\"access_restriction_data\" : 32, "
        "\"security\" : { "
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\", "
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" }, "
          "\"rand\" : \"2AE4FC02 1DD4D1C2 E0A277C2 317C2E67\" "
        "}, "
        "\"__v\" : 0 "
      "}";
    bson_t *doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_ERROR);

    c_uint8_t tmp[MAX_SDU_LEN];
    /* eNB connects to MME */
    sock = tests1ap_enb_connect();
    ABTS_PTR_NOTNULL(tc, sock);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(&sendbuf, 0x002343d);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    /* Send Initial-UE Message */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    recvbuf->len = 60;
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 40;
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);
    
    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 33;
    pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 208;
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send EMM Status */
    rv = tests1ap_build_emm_status(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456826"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);
}

/**************************************************************
 * Attach -> Release -> Service Request */
static void nas_sm_test3(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int rc;
    int i;
    int msgindex = 2;

    c_uint8_t tmp[MAX_SDU_LEN];
    char *_authentication_request =
        "000b"
        "403b000003000000 05c0020000c80008 00020002001a0025 2407520042200639"
        "1c0021554d444928 4a1a062e10e543cb 257f1f800021f4f9 2d522a5b87";
    char *_security_mode_command =
        "000b"
        "4027000003000000 05c0020000c80008 00020002001a0011 103744bcbbd20007"
        "5d010005f0f00000 00";

    char *_esm_information_request =
        "000b"
        "4020000003000000 05c0020000c80008 00020002001a000a 09277330e06c0102"
        "01d9";

    char *_initial_context_setup_request =
        "00090080c8000006 00000005c0020000 c800080002000200 42000a183e800000"
        "603e800000001800 7700003400724540 0920000000000000 00000f800a0123d8"
        "000000035b27a23a 27b4020742024906 4000f1102b670032 5201c10509ffffff"
        "ff0908696e746572 6e657405012d2d2d 035e060000000004 04270f80000d0408"
        "080808000d040404 0404500bf600f110 0002010000000353 12172c5949640125"
        "006b00051e000e00 000049002040964d eb63a0afb5d0d374 c1da505f6252d1f9"
        "05ff9c6791b8503a 032c6effa7";

    char *_emm_information =
        "000b"
        "402a000003000000 05c0020000c80008 00020002001a0014 1327dc833e850307"
        "6147717042911120 8a490100";

    char *_ue_context_release_command = 
        "0017"
        "0013000002006300 070c020000c80002 0002400120";

    mongoc_collection_t *collection = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456797\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c7\" }, "
            "\"pdn_ambr\" : {"
              "\"max_bandwidth_ul\" : 1024000, "
              "\"max_bandwidth_dl\" : 1024000  "
            "},"
            "\"qos\" : { "
              "\"qci\" : 9, "
              "\"arp\" : { "
                "\"priority_level\" : 8,"
                "\"pre_emption_vulnerability\" : 1, "
                "\"pre_emption_capability\" : 1"
              "} "
            "}, "
            "\"type\" : 0"
          "}"
        "],"
        "\"ue_ambr\" : { "
          "\"max_bandwidth_ul\" : 1024000, "
          "\"max_bandwidth_dl\" : 1024000 "
        "},"
        "\"subscribed_rau_tau_timer\" : 12,"
        "\"network_access_mode\" : 2, "
        "\"subscriber_status\" : 0, "
        "\"access_restriction_data\" : 32, "
        "\"security\" : { "
          "\"k\" : \"465B5CE8 B199B49F AA5F0A2E E238A6BC\", "
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" }, "
          "\"rand\" : \"42200639 1c002155 4d444928 4a1a062e\" "
        "}, "
        "\"__v\" : 0 "
      "}";
    bson_t *doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    ABTS_TRUE(tc, mongoc_collection_insert(collection,
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_ERROR);

    /* eNB connects to MME */
    sock = tests1ap_enb_connect();
    ABTS_PTR_NOTNULL(tc, sock);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(&sendbuf, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    /* Send Initial-UE Message */
    mme_self()->mme_ue_s1ap_id = 33554631;
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    recvbuf->len = 63;
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_authentication_request, strlen(_authentication_request), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 43;
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_security_mode_command, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 36;
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_esm_information_request, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request +
     * Attach Accept +
     * Activate Default Bearer Context Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 205;
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_initial_context_setup_request,
            strlen(_initial_context_setup_request), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send Attach Complete + Activate default EPS bearer cotext accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive EMM information */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 46;
    CORE_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->payload, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->payload+43, tmp+43, 3) == 0);
    pkbuf_free(recvbuf);

    /* Send UE Release Request */
    rv = tests1ap_build_ue_context_release_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive UE Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    recvbuf->len = 23;
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_ue_context_release_command, 
        strlen(_ue_context_release_command), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send UE Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Service Request */
    rv = tests1ap_build_service_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456797"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);
}

abts_suite *test_nas_sm(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, nas_sm_test1, NULL);
    abts_run_test(suite, nas_sm_test2, NULL);
    abts_run_test(suite, nas_sm_test3, NULL);

    return suite;
}
