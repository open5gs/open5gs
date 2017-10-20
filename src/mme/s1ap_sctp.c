#define TRACE_MODULE _s1ap_sctp

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "s1ap_path.h"

static int s1ap_accept_cb(net_sock_t *net_sock, void *data);

status_t s1ap_open(void)
{
    char buf[INET_ADDRSTRLEN];
    int rc;

    rc = net_listen_ext(&mme_self()->s1ap_sock, 
            SOCK_STREAM, IPPROTO_SCTP, SCTP_S1AP_PPID,
            mme_self()->s1ap_addr, mme_self()->s1ap_port);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB(port:%d) path(%d:%s)",
            mme_self()->s1ap_port, errno, strerror(errno));
        mme_self()->s1ap_sock = NULL;
        return CORE_ERROR;
    }

    rc = net_register_sock(
            mme_self()->s1ap_sock, s1ap_accept_cb, NULL);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB path(%d:%s)",
            errno, strerror(errno));
        net_close(mme_self()->s1ap_sock);
        mme_self()->s1ap_sock = NULL;
        return CORE_ERROR;
    }

    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);

    return CORE_OK;
}

status_t s1ap_close()
{
    s1ap_sctp_close(mme_self()->s1ap_sock);
    mme_self()->s1ap_sock = NULL;

    return CORE_OK;
}

status_t s1ap_sctp_close(net_sock_t *sock)
{
    net_unregister_sock(sock);
    net_close(sock);

    return CORE_OK;
}

status_t s1ap_final()
{
    return CORE_OK;
}

static int s1ap_accept_cb(net_sock_t *net_sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t r;
    net_sock_t *remote_sock;

    d_assert(net_sock, return -1, "Null param");

    r = net_accept(&remote_sock, net_sock, 0);
    if (r > 0)
    {
        event_t e;
        c_uint32_t addr = remote_sock->remote.sin_addr.s_addr;
        c_uint16_t port = ntohs(remote_sock->remote.sin_port);
        d_trace(1, "eNB-S1 accepted[%s] in s1_path module\n", 
            INET_NTOP(&addr, buf));

        event_set(&e, MME_EVT_S1AP_LO_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)remote_sock);
        event_set_param2(&e, (c_uintptr_t)addr);
        event_set_param3(&e, (c_uintptr_t)port);
        /* FIXME : how to close remote_sock */
        mme_event_send(&e);
    }
    else
    {
        d_error("net_accept failed(r = %d, errno = %d)", r, errno);
    }

    return r;
}

static status_t s1ap_recv(net_sock_t *sock, pkbuf_t *pkbuf)
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

int s1ap_recv_cb(net_sock_t *sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf;
    ssize_t r;

    d_assert(sock, return -1, "Null param");

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (pkbuf == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        net_read(sock, tmp_buf, MAX_SDU_LEN, 0);

        return -1;
    }

    r = net_read(sock, pkbuf->payload, pkbuf->len, 0);
    if (r == -2)
    {
        pkbuf_free(pkbuf);
    }
    else if (r <= 0)
    {
        pkbuf_free(pkbuf);

        if (sock->sndrcv_errno == EAGAIN)
        {
            d_warn("net_read failed(%d:%s)",
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
            return 0;
        } 
        else if (sock->sndrcv_errno == ECONNREFUSED)
        {
            d_warn("net_read failed(%d:%s)",
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
        }
        else
        {
            d_error("net_read failed(%d:%s)",
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
        }

        event_t e;

        event_set(&e, MME_EVT_S1AP_LO_CONNREFUSED);
        event_set_param1(&e, (c_uintptr_t)sock);
        mme_event_send(&e);

        return -1;
    }
    else
    {
        pkbuf->len = r;

        rv = s1ap_recv(sock, pkbuf);
        if (rv != CORE_OK)
        {
            pkbuf_free(pkbuf);
            d_error("s1_recv() failed");
            return -1;
        }
    }

    return 0;
}

#if 0 /* deprecated */
status_t s1ap_send(net_sock_t *s, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];

    ssize_t sent;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sent = net_send(s, pkbuf->payload, pkbuf->len);
    d_trace(10,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&s->remote.sin_addr.s_addr, buf), 
            ntohs(s->remote.sin_port));
    d_trace_hex(10, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}
#endif

status_t s1ap_sendto(net_sock_t *s, pkbuf_t *pkbuf,
        c_uint32_t addr, c_uint16_t port)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t sent;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sent = net_sendto(s, pkbuf->payload, pkbuf->len, addr, port);
    d_trace(10,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&addr, buf), port);
    d_trace_hex(10, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}
