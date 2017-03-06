#include "core_debug.h"
#include "core_pkbuf.h"

#include "s1ap_message.h"

#include "testutil.h"
#include "tests1ap.h"

static void nas_sm_test1(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock;
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf;
    s1ap_message_t message;
    int rc;
    int i;

    /* eNB connects to MME */
    sock = tests1ap_enb_connect();
    ABTS_PTR_NOTNULL(tc, sock);

    /* Send S1-Setup Reqeust */
    rv = tests1ap_build_setup_req(&sendbuf, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(sendbuf);

    /* Receive S1-Setup Response */
    recvbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    /* Send Initial-UE Message */
    rv = tests1ap_build_initial_ue_msg(&sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(sendbuf);

    /* Receive Authentication-Request */
    recvbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    rc = tests1ap_enb_read(sock, recvbuf);
    ABTS_INT_NEQUAL(tc, 0, rc);
    pkbuf_free(recvbuf);

    /* Send Authentication-Response */
    rv = tests1ap_build_authentication_response(&sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tests1ap_enb_send(sock, sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    pkbuf_free(sendbuf);

    /* eNB disonncect from MME */
    rv = tests1ap_enb_close(sock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_sleep(time_from_msec(300));
}

abts_suite *test_nas_sm(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);

    abts_run_test(suite, nas_sm_test1, NULL);

    return suite;
}
