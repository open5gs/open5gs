#include "core/abts.h"

#include "mme/s1ap-build.h"
#include "mme/s1ap-conv.h"

#include "test-packet.h"

#define NUM_OF_TEST_DUPLICATED_ENB 4

static void s1setup_test1(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *node[NUM_OF_TEST_DUPLICATED_ENB];
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf = NULL;
    s1ap_message_t message;
    int i;

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        node[i] = testenb_s1ap_client("127.0.0.1");
        ABTS_PTR_NOTNULL(tc, node[i]);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = tests1ap_build_setup_req(
                &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64, 12345, 1, 1, 2);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        rv = testenb_s1ap_send(node[i], sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        recvbuf = testenb_s1ap_read(node[i]);
        ABTS_PTR_NOTNULL(tc, recvbuf);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        s1ap_free_pdu(&message);
        ogs_pkbuf_free(recvbuf);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        testenb_s1ap_close(node[i]);
    }

    ogs_pkbuf_free(recvbuf);

    ogs_msleep(300);
}

#define NUM_OF_TEST_ENB 4

static void s1setup_test2(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *node[NUM_OF_TEST_ENB];
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    s1ap_message_t message;
    int i;

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        node[i] = testenb_s1ap_client("127.0.0.1");
        ABTS_PTR_NOTNULL(tc, node[i]);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = tests1ap_build_setup_req(
            &sendbuf, S1AP_ENB_ID_PR_macroENB_ID, 0x54f64+i, 12345, 1, 1, 2);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        rv = testenb_s1ap_send(node[i], sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        recvbuf = testenb_s1ap_read(node[i]);
        ABTS_PTR_NOTNULL(tc, recvbuf);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        s1ap_free_pdu(&message);
        ogs_pkbuf_free(recvbuf);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        testenb_s1ap_close(node[i]);
    }

    ogs_pkbuf_free(recvbuf);

    ogs_msleep(1000);
}

abts_suite *test_s1setup(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1setup_test1, NULL);
    abts_run_test(suite, s1setup_test2, NULL);

    return suite;
}
