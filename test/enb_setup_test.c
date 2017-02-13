#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "s1ap_message.h"
#include "s1ap_enb_message.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"

net_sock_t *enb_s1ap_connect(void)
{
    status_t rv;
    mme_ctx_t *mme = mme_self();
    net_sock_t *sock = NULL;

    if (!mme) return NULL;

    c_uint16_t rport = mme->enb_s1_port;
    c_uint16_t lport = mme->enb_s1_port+1;

    rv = net_open_with_addr(&sock, mme->enb_local_addr, "127.0.0.1", lport, 
            rport, SOCK_STREAM, IPPROTO_SCTP, 0);
    if (rv != CORE_OK) return NULL;

    return sock;
}

status_t enb_s1ap_disconnect(net_sock_t *sock)
{
    return net_close(sock);
}

int enb_net_read(net_sock_t *sock, pkbuf_t *recvbuf, int size)
{
    int rc;

    while(1)
    {
        rc = net_read(sock, recvbuf->payload, recvbuf->len, 1);
        if (rc < 0 && sock->sndrcv_errno == EAGAIN)
            continue;
        if (rc == size) 
            break;
    }

    return rc;
}

static void enb_setup_test1(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[2];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, S1AP_SDU_SIZE);
    s1ap_message message;
    int rc;

    sock[0] = enb_s1ap_connect();
    ABTS_PTR_NOTNULL(tc, sock[0]);

    rv = s1ap_build_setup_req(&sendbuf, 0x54f64);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = s1ap_send(sock[0], sendbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(sendbuf);

    rc = enb_net_read(sock[0], recvbuf, 27);
    ABTS_INT_EQUAL(tc, 27, rc);

    rv = s1ap_decode_pdu(&message, recvbuf);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    s1ap_free_pdu(&message);
    pkbuf_free(recvbuf);

    rv = enb_s1ap_disconnect(sock[0]);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *test_enb_setup(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, enb_setup_test1, NULL);

    return suite;
}
