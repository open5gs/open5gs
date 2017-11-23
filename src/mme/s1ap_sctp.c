#define TRACE_MODULE _s1ap_sctp

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "s1ap_path.h"

static int s1ap_accept_cb(sock_id sock, void *data);

status_t s1ap_open(void)
{
#if 0 /* ADDR */
    char buf[CORE_ADDRSTRLEN];
#endif
    status_t rv;
    c_sockaddr_t addr;

    memset(&addr, 0, sizeof(c_sockaddr_t));
    addr.sin.sin_addr.s_addr = mme_self()->s1ap_addr;
    addr.c_sa_family = AF_INET;
    addr.c_sa_port = htons(mme_self()->s1ap_port);

    rv = sctp_socket(&mme_self()->s1ap_sock, AF_INET, SOCK_STREAM);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = sock_bind(mme_self()->s1ap_sock, &addr);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = sock_listen(mme_self()->s1ap_sock);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = sock_register(mme_self()->s1ap_sock, s1ap_accept_cb, NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

#if 0 /* ADDR */
    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);
#endif

    return CORE_OK;
}

status_t s1ap_close()
{
    sock_delete(mme_self()->s1ap_sock);
    mme_self()->s1ap_sock = 0;

    return CORE_OK;
}

status_t s1ap_final()
{
    return CORE_OK;
}

static int s1ap_accept_cb(sock_id id, void *data)
{
    char buf[CORE_ADDRSTRLEN];
    status_t rv;
    sock_id new;

    d_assert(id, return -1, "Null param");

    rv = sock_accept(&new, id);
    if (rv == CORE_OK)
    {
        c_sockaddr_t *addr = sock_remote_addr_get(new);
        event_t e;

        d_trace(1, "eNB-S1 accepted[%s] in s1_path module\n", 
            CORE_NTOP(addr, buf));

        event_set(&e, MME_EVT_S1AP_LO_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)new);
        event_set_param2(&e, (c_uintptr_t)addr);
        mme_event_send(&e);

        return 0;
    }
    else
    {
        d_error("sock accept failed(%d:%s)", errno, strerror(errno));

        return -1;
    }

}

static status_t s1ap_recv(sock_id sock, pkbuf_t *pkbuf)
{
    event_t e;

    d_assert(sock, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_trace(10, "S1AP_PDU is received from eNB-Inf\n");
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    event_set(&e, MME_EVT_S1AP_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sock);
    event_set_param2(&e, (c_uintptr_t)pkbuf);
    return mme_event_send(&e);
}

int s1ap_recv_cb(sock_id sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf;
    int rc;
    event_t e;

    d_assert(sock, return -1, "Null param");

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (pkbuf == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        core_recv(sock, tmp_buf, MAX_SDU_LEN, 0);

        return -1;
    }

    rc = core_sctp_recvmsg(sock, pkbuf->payload, pkbuf->len,
            NULL, NULL, NULL);
    if (rc <= 0)
    {
        pkbuf_free(pkbuf);

        if (errno == 0 || errno == EAGAIN)
            return 0;

        if (rc == CORE_SCTP_REMOTE_CLOSED)
        {
            event_set(&e, MME_EVT_S1AP_LO_CONNREFUSED);
            event_set_param1(&e, (c_uintptr_t)sock);
            mme_event_send(&e);

            return 0;
        }

        d_error("core_sctp_recvmsg(%d) failed(%d:%s)",
                rc, errno, strerror(errno));
        return -1;
    }

    pkbuf->len = rc;

    rv = s1ap_recv(sock, pkbuf);
    if (rv != CORE_OK)
    {
        pkbuf_free(pkbuf);
        d_error("s1_recv() failed");
        return -1;
    }

    return 0;
}

status_t s1ap_send(sock_id sock, pkbuf_t *pkbuf)
{
    int sent;

    d_assert(sock, return CORE_ERROR,);
    d_assert(pkbuf, return CORE_ERROR,);

    sent = core_sctp_sendmsg(sock, pkbuf->payload, pkbuf->len,
            NULL, SCTP_S1AP_PPID, 0);
    d_trace(10,"Sent %d->%d bytes\n", pkbuf->len, sent);
    d_trace_hex(10, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("core_sctp_sendmsg error (%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}
