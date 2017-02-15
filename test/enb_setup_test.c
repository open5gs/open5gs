#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "s1ap_contents.h"
#include "s1ap_enb_message.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"

net_sock_t *enb_net_open(void)
{
    status_t rv;
    mme_ctx_t *mme = mme_self();
    net_sock_t *sock = NULL;

    if (!mme) return NULL;

    rv = net_open_with_addr(&sock, mme->enb_local_addr, "127.0.0.1", 0, 
            mme->enb_s1_port, SOCK_SEQPACKET, IPPROTO_SCTP, 0);
    if (rv != CORE_OK) return NULL;

    return sock;
}

status_t enb_net_close(net_sock_t *sock)
{
    return net_close(sock);
}

int enb_net_send(net_sock_t *sock, pkbuf_t *sendbuf)
{
    return s1ap_send(sock, sendbuf);
}

int enb_net_read(net_sock_t *sock, pkbuf_t *recvbuf)
{
    int rc = 0;

    while(1)
    {
        rc = net_read(sock, recvbuf->payload, recvbuf->len, 0);
        if (rc == -2) 
        {
            continue;
        }
        else if (rc <= 0)
        {
            if (sock->sndrcv_errno == EAGAIN)
            {
                continue;
            }
            break;
        }
        else
        {
            break;
        }
    }

    return rc;
}

#define NUM_OF_TEST_DUPLICATED_ENB 4

#define TEST_S1_SETUP_RESPONSE_SIZE 27 
#define TEST_S1_SETUP_FAILURE_SIZE 12 
static void enb_setup_test1(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[NUM_OF_TEST_DUPLICATED_ENB];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, S1AP_SDU_SIZE);
    s1ap_message message;
    int rc;
    int i;

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        sock[i] = enb_net_open();
        ABTS_PTR_NOTNULL(tc, sock[i]);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = s1ap_build_setup_req(&sendbuf, 0x54f64);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rv = enb_net_send(sock[i], sendbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        pkbuf_free(sendbuf);

        rc = enb_net_read(sock[i], recvbuf);
        ABTS_INT_NEQUAL(tc, 0, rc);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        s1ap_free_pdu(&message);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = enb_net_close(sock[i]);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
    }

    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));
}

#define NUM_OF_TEST_ENB 32

static void enb_setup_test2(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[NUM_OF_TEST_ENB];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, S1AP_SDU_SIZE);
    s1ap_message message;
    int rc;
    int i;

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        sock[i] = enb_net_open();
        ABTS_PTR_NOTNULL(tc, sock[i]);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = s1ap_build_setup_req(&sendbuf, 0x54f64+i);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rv = enb_net_send(sock[i], sendbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        pkbuf_free(sendbuf);

        rc = enb_net_read(sock[i], recvbuf);
        ABTS_INT_NEQUAL(tc, 0, rc);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        s1ap_free_pdu(&message);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = enb_net_close(sock[i]);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
    }

    pkbuf_free(recvbuf);

    core_sleep(time_from_sec(1));
}

abts_suite *test_enb_setup(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, enb_setup_test1, NULL);
    abts_run_test(suite, enb_setup_test2, NULL);

    return suite;
}
