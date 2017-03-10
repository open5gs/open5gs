#include "core_debug.h"
#include "core_pkbuf.h"

#include "s1ap_build.h"
#include "s1ap_conv.h"

#include "testutil.h"
#include "tests1ap.h"

#define NUM_OF_TEST_DUPLICATED_ENB 4

static void s1ap_sm_test1(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[NUM_OF_TEST_DUPLICATED_ENB];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    s1ap_message_t message;
    int rc;
    int i;

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        sock[i] = tests1ap_enb_connect();
        ABTS_PTR_NOTNULL(tc, sock[i]);
    }

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_WARN);
    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = tests1ap_build_setup_req(&sendbuf, 0x54f64);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rv = tests1ap_enb_send(sock[i], sendbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        pkbuf_free(sendbuf);

        rc = tests1ap_enb_read(sock[i], recvbuf);
        ABTS_INT_NEQUAL(tc, 0, rc);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        s1ap_free_pdu(&message);
    }

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_ERROR);

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = tests1ap_enb_close(sock[i]);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
    }

    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);
}

#define NUM_OF_TEST_ENB 32

static void s1ap_sm_test2(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[NUM_OF_TEST_ENB];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    s1ap_message_t message;
    int rc;
    int i;

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        sock[i] = tests1ap_enb_connect();
        ABTS_PTR_NOTNULL(tc, sock[i]);
    }

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_WARN);
    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = tests1ap_build_setup_req(&sendbuf, 0x54f64+i);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rv = tests1ap_enb_send(sock[i], sendbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        pkbuf_free(sendbuf);

        rc = tests1ap_enb_read(sock[i], recvbuf);
        ABTS_INT_NEQUAL(tc, 0, rc);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        s1ap_free_pdu(&message);
    }

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_ERROR);

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = tests1ap_enb_close(sock[i]);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
    }

    pkbuf_free(recvbuf);

    core_sleep(time_from_sec(1));

    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);
}

abts_suite *test_s1ap_sm(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1ap_sm_test1, NULL);
    abts_run_test(suite, s1ap_sm_test2, NULL);

    return suite;
}
