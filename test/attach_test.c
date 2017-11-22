
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"
#include <mongoc.h>

#include "context.h"
#include "mme_context.h"
#include "s1ap_message.h"

#include "testutil.h"
#include "testpacket.h"

/**************************************************************
 * eNB : MACRO
 * UE : IMSI 
 * Protocol Configuration Options in ESM information response */
static void attach_test1(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sock;
    sock_id gtpu;
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
        "00090080d8000006 00000005c0010000 9d00080002000100 42000a183e800000"
        "603e800000001800 8086000034008080 450009200f807f4c 0002000000017127"
        "45c2015402074202 49064000f1105ba0 00485221c1010909 08696e7465726e65"
        "7405012d2d00025e 06fefeeeee030327 2980c22304030000 0480211002000010"
        "8106080808088306 04040404000d0408 080808000d040404 0404500bf600f110"
        "0002010000000153 12172c5949640125 006b000518000c00 00004900203311c6"
        "03c6a6d67f695e5a c02bb75b381b693c 3893a6d932fd9182 3544e3e79b000000"
        "0000000000000000 00";
    char *_emm_information = 
        "000b402a00000300 000005c00100009d 000800020001001a 001413279fcc7266"
        "0307614771304112 527563490100";

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    c_int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456819\", "
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
            "\"type\" : 0"
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
          "\"rand\" : \"20080C38 18183B52 2614162C 07601D0D\" "
        "}, "
        "\"__v\" : 0 "
      "}";
    const char *json2 =
      "{"
        "\"_id\" : { \"$oid\" : \"697223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456815\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"697223158b8861d7605378c7\" }, "
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
            "\"type\" : 0"
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
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" }, "
          "\"rand\" : \"20080C38 18183B52 2614162C 07601D0D\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    core_sleep(time_from_msec(300));

    /* eNB connects to MME */
    rv = tests1ap_enb_connect(&sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB connects to SGW */
    gtpu = testgtpu_enb_connect();
    ABTS_INT_NEQUAL(tc, 0, gtpu);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1ap_ENB_ID_PR_macroENB_ID, 0x54f64);
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

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456819"));
    ABTS_PTR_NOTNULL(tc, doc);
    do
    {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    doc = bson_new_from_json((const uint8_t *)json2, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456815"));
    ABTS_PTR_NOTNULL(tc, doc);
    do
    {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /***********************************************************************
     * Attach Request : Known IMSI, Integrity Protected, No Security Context
     * Send Initial-UE Message + Attach Request + PDN Connectivity        */
    core_sleep(time_from_msec(300));

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
    /* 
     * We cannot check it since SGW S1U ADDR is changed
     * from configuration file
     */ 
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
    CORE_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->payload, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->payload+43, tmp+43, 3) == 0);
    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));

    /* Send GTP-U ICMP Packet */
    rv = testgtpu_enb_send(gtpu,
            inet_addr("45.45.0.2"), inet_addr("45.45.0.1"));
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = testgtpu_enb_read(gtpu, recvbuf);
    pkbuf_free(recvbuf);

    /*****************************************************************
     * Attach Request : Known GUTI, Integrity Protected, MAC Matched
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send Attach Complete + 
     * Activate Default EPS Bearer Context Accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive EMM information */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));

    /* Send Detach Request */
    rv = tests1ap_build_detach_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /*****************************************************************
     * Attach Request : Unknown GUTI, Integrity Protected
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Identity Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    pkbuf_free(recvbuf);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("001010123456819"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456815"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* Send Identity Response */
    rv = tests1ap_build_identity_response(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Attach Reject */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Receive UE Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send UE Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB disonncect from SGW */
    rv = testgtpu_enb_close(gtpu);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

/**************************************************************
 * eNB : HOME
 * UE : IMSI 
 * Protocol Configuration Options in PDN Connectivity Request */
static void attach_test2(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int rc;
    int i;
    int msgindex = 3;

    mongoc_collection_t *collection = NULL;
    c_int64_t count = 0;
    bson_t *doc = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456826\", "
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
            "\"type\" : 0"
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

    c_uint8_t tmp[MAX_SDU_LEN];

    core_sleep(time_from_msec(300));

    /* eNB connects to MME */
    rv = tests1ap_enb_connect(&sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1ap_ENB_ID_PR_macroENB_ID, 0x002343d);
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

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456826"));
    ABTS_PTR_NOTNULL(tc, doc);
    do
    {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /*****************************************************************
     * Attach Request : Known IMSI, Plain NAS message
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
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

    /*****************************************************************
     * Attach Request : IMSI, Integrity Protected, MAC Matched
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    pkbuf_free(recvbuf);

    /* Send Attach Complete + 
     * Activate Default EPS Bearer Context Accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive EMM information */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    pkbuf_free(recvbuf);

    /*****************************************************************
     * Attach Request : IMSI, Integrity Protected, MAC Failed
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456826"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

/**************************************************************
 * Attach -> Release -> Service Request */
static void attach_test3(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int rc;
    int i;
    int msgindex = 6;

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
    c_int64_t count = 0;
    bson_t *doc = NULL;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456797\", "
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
            "\"type\" : 0"
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
          "\"op\" : \"5F1D289C 5D354D0A 140C2548 F5F3E3BA\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"64\" }, "
          "\"rand\" : \"42200639 1c002155 4d444928 4a1a062e\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    core_sleep(time_from_msec(300));

    /* eNB connects to MME */
    rv = tests1ap_enb_connect(&sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1ap_ENB_ID_PR_macroENB_ID, 0x54f64);
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

    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    ABTS_TRUE(tc, mongoc_collection_insert(collection,
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456797"));
    ABTS_PTR_NOTNULL(tc, doc);
    do
    {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    mme_self()->mme_ue_s1ap_id = 33554631;
    mme_self()->m_tmsi = 2;

    /*****************************************************************
     * Attach Request : Known IMSI, Plain NAS message
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

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
    CORE_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->payload, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->payload+43, tmp+43, 3) == 0);
    pkbuf_free(recvbuf);

    /* Send UE Release Request */
    core_sleep(time_from_msec(300));

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

    /* Send Service Request */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_service_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rc = tests1ap_enb_read(sock, recvbuf);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456797"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
}

abts_suite *test_attach(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, attach_test1, NULL);
    abts_run_test(suite, attach_test2, NULL);
    abts_run_test(suite, attach_test3, NULL);

    return suite;
}
