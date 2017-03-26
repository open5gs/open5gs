#define TRACE_MODULE _s1ap_path

#include "core_debug.h"

#include "event.h"

#include "s1ap_path.h"

static int _s1ap_accept_cb(net_sock_t *net_sock, void *data);

status_t s1ap_open(void)
{
    char buf[INET_ADDRSTRLEN];
    int rc;

    rc = net_listen_with_addr(&mme_self()->s1ap_sock, 
            SOCK_STREAM, IPPROTO_SCTP, mme_self()->s1ap_port,
            mme_self()->s1ap_addr);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB(port:%d) path(%d:%s)",
            mme_self()->s1ap_port, errno, strerror(errno));
        mme_self()->s1ap_sock = NULL;
        return CORE_ERROR;
    }

    rc = net_register_sock(
            mme_self()->s1ap_sock, _s1ap_accept_cb, NULL);
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
    d_assert(mme_self(), return CORE_ERROR, "Null param");
    d_assert(mme_self()->s1ap_sock != NULL, return CORE_ERROR,
            "S1-ENB path already opened");
    net_unregister_sock(mme_self()->s1ap_sock);
    net_close(mme_self()->s1ap_sock);
    mme_self()->s1ap_sock = NULL;

    return CORE_OK;
}

static int _s1ap_accept_cb(net_sock_t *net_sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t r;
    net_sock_t *remote_sock;

    d_assert(net_sock, return -1, "Null param");

    r = net_accept(&remote_sock, net_sock, 0);
    if (r > 0)
    {
        d_trace(1, "eNB-S1 accepted[%s] in s1_path module\n", 
            INET_NTOP(&remote_sock->remote.sin_addr.s_addr, buf));

        event_t e;
        event_set(&e, EVT_LO_ENB_S1AP_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)remote_sock);
        mme_event_send(&e);
    }
    else
    {
        d_error("net_accept failed(r = %d, errno = %d)", r, errno);
    }

    return r;
}

static status_t s1ap_recv(net_sock_t *net_sock, pkbuf_t *pkbuf)
{
    event_t e;

    d_assert(net_sock, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_trace(1, "S1AP_PDU is received from eNB-Inf\n");
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set(&e, EVT_MSG_ENB_S1AP);
    event_set_param1(&e, (c_uintptr_t)net_sock);
    event_set_param2(&e, (c_uintptr_t)pkbuf);
    return mme_event_send(&e);
}

int _s1ap_recv_cb(net_sock_t *net_sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf;
    ssize_t r;

    d_assert(net_sock, return -1, "Null param");

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    d_assert(pkbuf, return -1, "Can't allocate pkbufuf");

    r = net_read(net_sock, pkbuf->payload, pkbuf->len, 0);
    if (r == -2)
    {
        pkbuf_free(pkbuf);
    }
    else if (r <= 0)
    {
        pkbuf_free(pkbuf);

        if (net_sock->sndrcv_errno == EAGAIN)
        {
            d_warn("net_read failed(%d:%s)",
                    net_sock->sndrcv_errno, strerror(net_sock->sndrcv_errno));
            return 0;
        } 
        else if (net_sock->sndrcv_errno == ECONNREFUSED)
        {
            d_warn("net_read failed(%d:%s)",
                    net_sock->sndrcv_errno, strerror(net_sock->sndrcv_errno));
        }
        else
        {
            d_error("net_read failed(%d:%s)",
                    net_sock->sndrcv_errno, strerror(net_sock->sndrcv_errno));
        }

        event_t e;

        event_set(&e, EVT_LO_ENB_S1AP_CONNREFUSED);
        event_set_param1(&e, (c_uintptr_t)net_sock);
        mme_event_send(&e);

        return -1;
    }
    else
    {
        pkbuf->len = r;

        rv = s1ap_recv(net_sock, pkbuf);
        if (rv == CORE_ERROR)
        {
            pkbuf_free(pkbuf);
            d_error("s1_recv() failed");
            return -1;
        }
    }

    return 0;
}

status_t s1ap_send(net_sock_t *s, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];

    ssize_t sent;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sent = net_send(s, pkbuf->payload, pkbuf->len);
    d_trace(1,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&s->remote.sin_addr.s_addr, buf), 
            ntohs(s->remote.sin_port));
    d_trace_hex(1, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}

status_t s1ap_send_to_enb(enb_ctx_t *enb, pkbuf_t *pkbuf)
{
    status_t rv = CORE_ERROR;
    d_assert(enb,,);
    d_assert(pkbuf,,);
    d_assert(enb->s1ap_sock,,);

    rv = s1ap_send(enb->s1ap_sock, pkbuf);
    if (rv != CORE_OK)
    {
        d_error("s1_send error");
        pkbuf_free(pkbuf);
    }

    return rv;
}
