#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

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
        "00090080d6000006 00000005c0010000 9d00080002000100 42000a1806400000"
        "6006400000001800 8084000034007f45 4009200000000000 0000000f807f0000"
        "0100000001682733 2c24090207420249 064000f1105ba000 4c5221c10509ffff"
        "ffff0908696e7465 726e657405012d2d 2d715e0600000000 0404272980c22304"
        "0300000480211002 0000108106080808 0883060404040400 0d0408080808000d"
        "0404040404531217 2c5949640125006b 00051c000c000000 4900203311c603c6"
        "a6d67f695e5ac02b b75b381b693c3893 a6d932fd91823544 e3e79b0000000000"
        "0000000000000000 00";
    char *_emm_information = 
        "000b402a00000300 000005c00100009d 000800020001001a 001413279fcc7266"
        "0307614771304112 527563490100";

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
#if 0
    d_print_hex(recvbuf->payload, recvbuf->len);
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
}

abts_suite *test_nas_sm(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, nas_sm_test1, NULL);
#if 0
    abts_run_test(suite, nas_sm_test2, NULL);
#endif

    return suite;
}
