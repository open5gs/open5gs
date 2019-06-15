
#include <mongoc.h>

#include "core/abts.h"

#include "app/context.h"
#include "mme/mme-context.h"
#include "mme/s1ap-build.h"
#include "asn1c/s1ap-message.h"

#include "test-packet.h"

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_socknode_t *gtpu;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    s1ap_message_t message;
    int i;
    int msgindex = 18;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    uint32_t m_tmsi = 0;

    uint8_t tmp[MAX_SDU_LEN];
    char *_identity_request = 
        "000b401700000300 0000020001000800 020001001a000403 075501";
    char *_authentication_request = 
        "000b403800000300 0000020001000800 020001001a002524 075200aa266700bc"
        "2887354e9f87368d 5d0ae710ab857af5 5f1a8000d71e5537 4ee176e9";
    char *_security_mode_command = 
        "000b402400000300 0000020001000800 020001001a001110 378ccbca6000075d"
        "010005f0f0c04070";
    char *_esm_information_request =
        "000b401d00000300 0000020001000800 020001001a000a09 27d1237969010234"
        "d9";
    char *_initial_context_setup_request = 
        "00090080c8000006 0000000200010008 000200010042000a 183d090000603d09"
        "00000018007a0000 340075450009230f 807f000002000000 01662775a81d1902"
        "07420249062009f1 07000700415234c1 01090908696e7465 726e657405012d2d"
        "00025e06fefee2e2 0303272280802110 0200001081060808 0808830608080404"
        "000d040808080800 0d0408080404500b f609f107000201de 0008885949640101"
        "006b00051c000e00 0000490020f9f4f8 0b206c33ae286c6d aff4c253585174c3"
        "a0a12a661967f5e1 ba0a686c8c";
    char *_emm_information = 
        "000b403800000300 0000020001000800 020001001a002524 2751034124030761"
        "430f10004e006500 7800740045005000 4347916051216124 63490100";

    mongoc_collection_t *collection = NULL;
    bson_t *doc = NULL;
    int64_t count = 0;
    bson_error_t error;
    const char *json =
      "{"
        "\"_id\" : { \"$oid\" : \"310014158b8861d7605378c6\" }, "
        "\"imsi\" : \"901700000021777\", "
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
                "\"priority_level\" : 8,"
                "\"pre_emption_vulnerability\" : 0, "
                "\"pre_emption_capability\" : 0"
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
          "\"k\" : \"70D49A71DD1A2B806A25ABE0EF749F1E\", "
          "\"opc\" : \"6F1BF53D624B3A43AF6592854E2444C7\", "
          "\"amf\" : \"8000\", "
          "\"sqn\" : { \"$numberLong\" : \"2374\" }, "
          "\"rand\" : \"aa266700bc2887354e9f87368d5d0ae7\" "
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
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x0019b0, 7, 901, 70, 2);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S1-Setup Response */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    s1ap_free_pdu(&message);
    ogs_pkbuf_free(recvbuf);

    collection = mongoc_client_get_collection(
        context_self()->db.client,
        context_self()->db.name, "subscribers");
    ABTS_PTR_NOTNULL(tc, collection);

    /********** Insert Subscriber in Database */
    doc = bson_new_from_json((const uint8_t *)json, -1, &error);;
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_TRUE(tc, mongoc_collection_insert(collection, 
                MONGOC_INSERT_NONE, doc, NULL, &error));
    bson_destroy(doc);

    doc = BCON_NEW("imsi", BCON_UTF8("901700000021777"));
    ABTS_PTR_NOTNULL(tc, doc);
    do
    {
        count = mongoc_collection_count (
            collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
    } while (count == 0);
    bson_destroy(doc);

    /* Send Attach Request */
    rv = tests1ap_build_initial_ue_msg(&sendbuf, msgindex);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity-Request */
    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_TRUE(tc, memcmp(recvbuf->data, 
        OGS_HEX(_identity_request, strlen(_identity_request), tmp),
        recvbuf->len) == 0);
    ogs_pkbuf_free(recvbuf);

    /* Send Identity Response */
    rv = tests1ap_build_identity_response(&sendbuf, msgindex);
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
#if 0
    OGS_HEX(_initial_context_setup_request,
            strlen(_initial_context_setup_request), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->data, tmp, 62) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+66, tmp+66, 78) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+148, tmp+148, 50) == 0);
#endif
    ogs_pkbuf_free(recvbuf);

    /* Send Initial Context Setup Response */
    rv = tests1ap_build_initial_context_setup_response(&sendbuf,
            1, 1, 5, 0x00460003, "127.0.0.5");
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
#if 0
    OGS_HEX(_emm_information, strlen(_emm_information), tmp);
    ABTS_TRUE(tc, memcmp(recvbuf->data, tmp, 28) == 0);
    ABTS_TRUE(tc, memcmp(recvbuf->data+32, tmp+32, 20) == 0);
#endif
    ogs_pkbuf_free(recvbuf);

#if 0
    /* Send GTP-U ICMP Packet */
    rv = testgtpu_build_ping(&sendbuf, "45.45.0.2", "45.45.0.1");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = testenb_gtpu_send(gtpu, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = testenb_gtpu_read(gtpu);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);
#endif

    /********** Remove Subscriber in Database */
    doc = BCON_NEW("imsi", BCON_UTF8("901700000021777"));
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

abts_suite *test_csfb(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);

    return suite;
}
