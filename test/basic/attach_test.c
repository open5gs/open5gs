
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"
#include <mongoc.h>

#include "common/context.h"
#include "mme/mme_context.h"
#include "mme/s1ap_build.h"
#include "s1ap/s1ap_message.h"

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
    int i;
    int msgindex = 0;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    c_uint32_t m_tmsi = 0;

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
        "603e800000001800 8086000034008080 450009200f807f00 0002000000017127"
        "4db5d98302074202 49064000f1105ba0 00485221c1010909 08696e7465726e65"
        "7405012d2d00025e 06fefeeeee030327 2980c22304030000 0480211002000010"
        "8106080808088306 08080404000d0408 080808000d040808 0404500bf600f110"
        "0002010000000153 12172c5949640125 006b000518000c00 00004900203311c6"
        "03c6a6d67f695e5a c02bb75b381b693c 3893a6d932fd9182 3544e3e79b";
    char *_emm_information = 
        "000b403b00000300 000005c00100009d 000800020001001a 002524271f9b491e"
        "030761430f10004e 0065007800740045 0050004347812072 11240563490100";

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
          "\"rand\" : \"20080C38 18183B52 2614162C 07601D0D\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    core_sleep(time_from_msec(300));

    /* eNB connects to MME */
    rv = tests1ap_enb_connect(&sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB connects to SGW */
    rv = testgtpu_enb_connect(&gtpu);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /********** Insert Subscriber in Database */
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_security_mode_command, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    /* 
     * We cannot check it since SGW S1U ADDR is changed
     * from configuration file
     */ 
#if 0
    ABTS_TRUE(tc, memcmp(recvbuf->payload, 
        CORE_HEX(_initial_context_setup_request, 
            strlen(_initial_context_setup_request), tmp),
        recvbuf->len) == 0);
#endif
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            16777373, 1, 5, 1);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    CORE_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->payload, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->payload+32, tmp+32, 20) == 0);
    pkbuf_free(recvbuf);

#if TEST_INITIAL_CONTEXT_SETUP_FAILURE
    /* Send Initial Context Setup Failure */
    rv = tests1ap_build_initial_context_setup_failure(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
#endif
    core_sleep(time_from_msec(300));

    rv = testgtpu_build_slacc_rs(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = testgtpu_enb_send(sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = testgtpu_enb_read(gtpu, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send GTP-U ICMP Packet */
    rv = testgtpu_build_ping(&sendbuf, "45.45.0.2", "45.45.0.1");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = testgtpu_enb_send(sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = testgtpu_enb_read(gtpu, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

#if LINUX == 1
    rv = testgtpu_build_ping(&sendbuf, "cafe::2", "cafe::1");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = testgtpu_enb_send(sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = testgtpu_enb_read(gtpu, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);
#endif

    /* Retreive M-TMSI */
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(16777373);
    d_assert(enb_ue, goto out,);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, goto out,);
    m_tmsi = mme_ue->guti.m_tmsi;

    /*****************************************************************
     * Attach Request : Known GUTI, Integrity Protected, MAC Matched
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex+1);
    /* Update M-TMSI */
    m_tmsi = htonl(m_tmsi);
    memcpy(sendbuf->payload + 36, &m_tmsi, 4);
    /* Update NAS MAC */
    void snow_3g_f9(c_uint8_t* key, c_uint32_t count, c_uint32_t fresh,
            c_uint32_t dir, c_uint8_t *data, c_uint64_t length, c_uint8_t *out);
    snow_3g_f9(mme_ue->knas_int, 7, 0, 0,
            sendbuf->payload + 24, (109 << 3), sendbuf->payload+20);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

#if 1 /* IMPLICIT_S1_RELEASE */
    /* Send UE Context Release Request */
    rv = tests1ap_build_ue_context_release_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Error Indicaation */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

#else /* S1_HOLDING_TIMER */
    /* Send UE Context Release Request */
    rv = tests1ap_build_ue_context_release_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+3);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
#endif

    /* Send ESM Information Response */
    rv = tests1ap_build_esm_information_response(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request + 
     * Attach Accept + 
     * Activate Default Bearer Context Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Attach Complete + 
     * Activate Default EPS Bearer Context Accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive EMM information */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));

    /* Send Detach Request */
    rv = tests1ap_build_detach_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("001010123456819"));
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Receive UE Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB disonncect from SGW */
    rv = testgtpu_enb_close(gtpu);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    return;

out:
    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("001010123456819"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    core_sleep(time_from_msec(300));

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
    const char *json2 =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c7\" }, "
        "\"imsi\" : \"001010000000003\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c8\" }, "
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
          "\"k\" : \"00112233 44556677 8899AABB CCDDEEFF\", "
          "\"opc\" : \"00010203 04050607 08090A0B 0C0D0E0F\", "
          "\"amf\" : \"9001\", "
          "\"sqn\" : { \"$numberLong\" : \"96\" }, "
          "\"rand\" : \"1c92dd6e dcd676e8 7b590ba2 20c1874e\" "
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
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x002343d);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /********** Insert Subscriber in Database */
    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

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

    doc = bson_new_from_json((const uint8_t *)json2, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);

    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010000000003"));
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

    mme_self()->mme_ue_s1ap_id = 0;
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);
    
    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            1, 31, 5, 1);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));

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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Attach Complete + 
     * Activate Default EPS Bearer Context Accept */
    rv = tests1ap_build_attach_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive EMM information */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /*****************************************************************
     * Attach Request : Unknown IMSI, Integrity Protected
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Authentication Authentication Failure */
    rv = tests1ap_build_authentication_failure(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Authentication Authentication Failure */
    rv = tests1ap_build_authentication_failure(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Reject */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456826"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010000000003"));
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
 * Attach -> Release -> Service Request -> TAU Request */
static void attach_test3(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int i;
    int msgindex = 6;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    c_uint32_t m_tmsi = 0;

    c_uint8_t tmp[MAX_SDU_LEN];
    char *_authentication_request =
        "000b"
        "403b000003000000 05c0020000c80008 00020002001a0025 2407520042200639"
        "1c0021554d444928 4a1a062e10e543cb 257f1f800021f4f9 2d522a5b87";
    char *_security_mode_command =
        "000b402400000300 000005c0020000c8 000800020002001a 000e0d37a3761a13"
        "00075d010002f0f0";

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
        "000b403b00000300 000005c0020000c8 000800020002001a 002524276782702a"
        "030761430f10004e 0065007800740045 0050004347812072 11941563490100";

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
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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

    /********** Insert Subscriber in Database */
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

    /*****************************************************************
     * Attach Request : Known IMSI, Plain NAS message
     * Send Initial-UE Message + Attach Request + PDN Connectivity  */
    core_sleep(time_from_msec(300));

    mme_self()->mme_ue_s1ap_id = 33554631;
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_security_mode_command, strlen(_security_mode_command), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            33554632, 2, 5, 1);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    CORE_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->payload, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->payload+32, tmp+32, 20) == 0);
    pkbuf_free(recvbuf);

    /* Retreive M-TMSI */
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(33554632);
    d_assert(enb_ue, goto out,);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, goto out,);
    m_tmsi = mme_ue->guti.m_tmsi;

    /* Send UE Context Release Request */
    rv = tests1ap_build_ue_context_release_request(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_TRUE(tc, memcmp(recvbuf->payload,
        CORE_HEX(_ue_context_release_command, 
        strlen(_ue_context_release_command), tmp),
        recvbuf->len) == 0);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send Service Request */
    core_sleep(time_from_msec(300));

    rv = tests1ap_build_service_request(&sendbuf, 0x000400, 4, 0xd4b8, m_tmsi);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            33554633, 4, 5, 1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send TAU Request */
    rv = tests1ap_build_tau_request(&sendbuf, 0,
            0, 0x003600, 1, m_tmsi, 7, 0xe73ce7c, mme_ue->knas_int);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive TAU Accept */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            33554634, 54, 5, 1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send TAU Request */
    rv = tests1ap_build_tau_request(&sendbuf, 0,
            0, 0x002600, 0, m_tmsi, 8, 0x972dc6f8, mme_ue->knas_int);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive TAU Accept */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
out:
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

/**************************************************************
 * eNB : MACRO
 * UE : IMSI 
 * Protocol Configuration Options without default APN */
static void attach_test4(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sock;
    sock_id gtpu;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int i;
    int msgindex = 8;

    c_uint8_t tmp[MAX_SDU_LEN];

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    c_int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223258b8861d7605378c7\" }, "
        "\"imsi\" : \"001010000000002\", "
        "\"pdn\" : ["
          "{"
            "\"apn\" : \"internet\", "
            "\"_id\" : { \"$oid\" : \"598223158b8861d7605378c8\" }, "
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
          "\"k\" : \"00112233 44556677 8899AABB CCDDEEFF\", "
          "\"opc\" : \"00010203 04050607 08090A0B 0C0D0E0F\", "
          "\"amf\" : \"9001\", "
          "\"sqn\" : { \"$numberLong\" : \"96\" }, "
          "\"rand\" : \"9bdbfb93 16be4d52 80153094 38326671\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    core_sleep(time_from_msec(300));

    /* eNB connects to MME */
    rv = tests1ap_enb_connect(&sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB connects to SGW */
    rv = testgtpu_enb_connect(&gtpu);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /********** Insert Subscriber in Database */
    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010000000002"));
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

    mme_self()->mme_ue_s1ap_id = 0;
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Initial Context Setup Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

#if 0
    rv = testgtpu_build_slacc_rs(&sendbuf, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = testgtpu_enb_send(sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = testgtpu_enb_read(gtpu, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);
#endif

    /* Send EMM Status */
    rv = tests1ap_build_emm_status(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    doc = BCON_NEW("imsi", BCON_UTF8("001010000000002"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection,
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error))
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* eNB disonncect from SGW */
    rv = testgtpu_enb_close(gtpu);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void attach_test5(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sock;
    sock_id gtpu;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int i;
    int msgindex = 12;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    c_uint32_t m_tmsi = 0;

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    c_int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"597223158b8861d7605378c6\" }, "
        "\"imsi\" : \"001010123456937\", "
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
          "\"sqn\" : { \"$numberLong\" : \"768\" }, "
          "\"rand\" : \"2e815f03 cc54b55f 00933008 5cab5ca3\" "
        "}, "
        "\"__v\" : 0 "
      "}";

    core_sleep(time_from_msec(300));

    /* eNB connects to MME */
    rv = tests1ap_enb_connect(&sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x787b0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        context_self()->db_client,
        context_self()->db_name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /********** Insert Subscriber in Database */
    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("001010123456937"));
    ABTS_PTR_NOTNULL(tc, doc);
    do
    {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    core_sleep(time_from_msec(300));

    /* Send Service request */
    mme_self()->mme_ue_s1ap_id = 0;
    rv = tests1ap_build_service_request(&sendbuf,
            0x40072c, 17, 0x9551, 0x12345678);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Service reject */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Receive Initial Context Setup Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /***********************************************************************
     * Attach Request : Known IMSI, Integrity Protected, No Security Context
     * Send Initial-UE Message + Attach Request + PDN Connectivity        */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Authentication Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Authentication Response */
    rv = tests1ap_build_authentication_response(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Security mode Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Security mode Complete */
    rv = tests1ap_build_security_mode_complete(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive ESM Information Request */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Capability Info Indication */
    rv = tests1ap_build_ue_capability_info_indication(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            2, 1837, 5, 0x1000908);
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
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Retreive M-TMSI */
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(2);
    d_assert(enb_ue, goto out,);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, goto out,);
    m_tmsi = mme_ue->guti.m_tmsi;

#if 0
    rv = tests1ap_build_s1_reset(&sendbuf, 0);
#elif 0
    rv = s1ap_build_s1_reset(&sendbuf,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_release_due_to_eutran_generated_reason,
            NULL);
#elif 0
    {
        S1AP_UE_associatedLogicalS1_ConnectionListRes_t
            *partOfS1_Interface = NULL;
        S1AP_UE_associatedLogicalS1_ConnectionItemRes_t *ie = NULL;
        S1AP_UE_associatedLogicalS1_ConnectionItem_t *item = NULL;

        partOfS1_Interface = core_calloc(1,
                sizeof(S1AP_UE_associatedLogicalS1_ConnectionListRes_t));
        d_assert(partOfS1_Interface, goto out,);

        ie = core_calloc(1,
                sizeof(S1AP_UE_associatedLogicalS1_ConnectionItemRes_t));
        ASN_SEQUENCE_ADD(&partOfS1_Interface->list, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_UE_associatedLogicalS1_ConnectionItemRes__value_PR_UE_associatedLogicalS1_ConnectionItem;

        item = &ie->value.choice.UE_associatedLogicalS1_ConnectionItem;

        item->mME_UE_S1AP_ID = core_calloc(1, sizeof(S1AP_MME_UE_S1AP_ID_t));
        d_assert(item->mME_UE_S1AP_ID, goto out,);
        *item->mME_UE_S1AP_ID = 100;
        item->eNB_UE_S1AP_ID = core_calloc(1, sizeof(S1AP_ENB_UE_S1AP_ID_t));
        d_assert(item->eNB_UE_S1AP_ID, goto out,);
        *item->eNB_UE_S1AP_ID = 4;

        ie = core_calloc(1,
                sizeof(S1AP_UE_associatedLogicalS1_ConnectionItemRes_t));
        ASN_SEQUENCE_ADD(&partOfS1_Interface->list, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_UE_associatedLogicalS1_ConnectionItemRes__value_PR_UE_associatedLogicalS1_ConnectionItem;

        item = &ie->value.choice.UE_associatedLogicalS1_ConnectionItem;

        item->mME_UE_S1AP_ID = core_calloc(1, sizeof(S1AP_MME_UE_S1AP_ID_t));
        d_assert(item->mME_UE_S1AP_ID, goto out,);
        *item->mME_UE_S1AP_ID = 2;
        item->eNB_UE_S1AP_ID = core_calloc(1, sizeof(S1AP_ENB_UE_S1AP_ID_t));
        d_assert(item->eNB_UE_S1AP_ID, goto out,);
        *item->eNB_UE_S1AP_ID = 3;

        ie = core_calloc(1,
                sizeof(S1AP_UE_associatedLogicalS1_ConnectionItemRes_t));
        ASN_SEQUENCE_ADD(&partOfS1_Interface->list, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_UE_associatedLogicalS1_ConnectionItemRes__value_PR_UE_associatedLogicalS1_ConnectionItem;

        item = &ie->value.choice.UE_associatedLogicalS1_ConnectionItem;

        item->mME_UE_S1AP_ID = core_calloc(1, sizeof(S1AP_MME_UE_S1AP_ID_t));
        d_assert(item->mME_UE_S1AP_ID, goto out,);
        *item->mME_UE_S1AP_ID = 44;

        rv = s1ap_build_s1_reset(&sendbuf,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_release_due_to_eutran_generated_reason,
                partOfS1_Interface);
    }
#else
    {
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id = NULL;

        mme_ue_s1ap_id = core_calloc(1, sizeof(S1AP_MME_UE_S1AP_ID_t));
        d_assert(mme_ue_s1ap_id, goto out,);
        *mme_ue_s1ap_id = 2;
        rv = s1ap_build_s1_reset_partial(&sendbuf,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_release_due_to_eutran_generated_reason,
                mme_ue_s1ap_id, NULL);
    }
#endif
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive S1 Reset Acknowledge */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send Service request */
    rv = tests1ap_build_service_request(&sendbuf,
            0x40072e, 4, 0xda67, m_tmsi);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    /* Receive Service Reject */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Receive UE Context Release Command */
    recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    rv = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(recvbuf);

    /* Send UE Context Release Complete */
    rv = tests1ap_build_ue_context_release_complete(&sendbuf, msgindex+1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));

out:
    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("001010123456937"));
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_remove(collection, 
            MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) 
    bson_destroy(doc);

    mongoc_collection_destroy(collection);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *test_attach(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, attach_test1, NULL);
    abts_run_test(suite, attach_test2, NULL);
    abts_run_test(suite, attach_test3, NULL);
    abts_run_test(suite, attach_test4, NULL);
    abts_run_test(suite, attach_test5, NULL);

    return suite;
}
