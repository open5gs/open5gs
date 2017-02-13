#define TRACE_MODULE _s1ap_path

#include "core.h"
#include "core_debug.h"
#include "core_net.h"

#include "s1ap_codecs.h"

#include "event.h"
#include "s1ap_path.h"

static int _s1ap_accept_cb(net_sock_t *net_sock, void *data);

status_t s1ap_open(msgq_id queue_id)
{
    char buf[INET_ADDRSTRLEN];
    int rc;

    rc = net_listen_with_addr(&mme_self()->enb_s1_sock, 
            SOCK_STREAM, IPPROTO_SCTP, mme_self()->enb_s1_port,
            mme_self()->enb_local_addr);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB(port:%d) path(%d:%s)",
            mme_self()->enb_s1_port, errno, strerror(errno));
        mme_self()->enb_s1_sock = NULL;
        return CORE_ERROR;
    }

    rc = net_register_sock(
            mme_self()->enb_s1_sock, _s1ap_accept_cb, (void *)queue_id);
    if (rc != 0)
    {
        d_error("Can't establish S1-ENB path(%d:%s)",
            errno, strerror(errno));
        net_close(mme_self()->enb_s1_sock);
        mme_self()->enb_s1_sock = NULL;
        return CORE_ERROR;
    }

    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->enb_local_addr, buf), mme_self()->enb_s1_port);

    return CORE_OK;
}

status_t s1ap_close()
{
    d_assert(mme_self(), return CORE_ERROR, "Null param");
    d_assert(mme_self()->enb_s1_sock != NULL, return CORE_ERROR,
            "S1-ENB path already opened");
    net_unregister_sock(mme_self()->enb_s1_sock);
    net_close(mme_self()->enb_s1_sock);
    mme_self()->enb_s1_sock = NULL;

    return CORE_OK;
}

static int _s1ap_accept_cb(net_sock_t *net_sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t r;
    net_sock_t *remote_sock;
    msgq_id queue_id = (msgq_id)data;

    d_assert(net_sock, return -1, "Null param");
    d_assert(queue_id, return -1, "Null param");

    r = net_accept(&remote_sock, net_sock, 0);
    if (r > 0)
    {
        d_trace(1, "eNB-S1 accepted[%s] in s1_path module\n", 
            INET_NTOP(&remote_sock->remote.sin_addr.s_addr, buf));

        event_t e;
        event_set(&e, EVT_LO_ENB_S1_ACCEPT, (c_uintptr_t)remote_sock);
        event_send(queue_id, &e);
    }
    else
    {
        d_error("net_accept failed(r = %d, errno = %d)", r, errno);
    }

    return r;
}

static status_t s1ap_recv(net_sock_t *net_sock, pkbuf_t *pkb, msgq_id queue_id)
{
    event_t e;

    d_assert(net_sock, return CORE_ERROR, "Null param");
    d_assert(pkb, return CORE_ERROR, "Null param");
    d_assert(queue_id, return -1, "Null param");

    d_trace(1, "S1AP_PDU is received from eNB-Inf\n");
    d_trace_hex(1, pkb->payload, pkb->len);

    event_set(&e, EVT_S1_ENB_INF, (c_uintptr_t)net_sock);
    event_set_msg(&e, pkb);

    return event_send(queue_id, &e);
}

int _s1ap_recv_cb(net_sock_t *net_sock, void *data)
{
    status_t rv;
    pkbuf_t *pkb;
    ssize_t r;
    msgq_id queue_id = (msgq_id)data;

    d_assert(net_sock, return -1, "Null param");
    d_assert(queue_id, return -1, "Null param");

    pkb = pkbuf_alloc(0, S1AP_SDU_SIZE);
    d_assert(pkb, return -1, "Can't allocate pkbuf");

    r = net_read(net_sock, pkb->payload, pkb->len, 0);
    if (r == -2)
    {
        pkbuf_free(pkb);
    }
    else if (r <= 0)
    {
        pkbuf_free(pkb);

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

        event_set(&e, EVT_LO_ENB_S1_CONNREFUSED, (c_uintptr_t)net_sock);
        event_send(queue_id, &e);

        return -1;
    }
    else
    {
        pkb->len = r;

        rv = s1ap_recv(net_sock, pkb, queue_id);
        if (rv == CORE_ERROR)
        {
            pkbuf_free(pkb);
            d_error("s1_recv() failed");
            return -1;
        }
    }

    return 0;
}

status_t s1ap_send(net_sock_t *s, pkbuf_t *pkb)
{
    char buf[INET_ADDRSTRLEN];

    ssize_t sent;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkb, return CORE_ERROR, "Null param");

    sent = net_send(s, pkb->payload, pkb->len);
    d_trace(1,"Sent %d->%d bytes to [%s:%d]\n", 
            pkb->len, sent, INET_NTOP(&s->remote.sin_addr.s_addr, buf), 
            ntohs(s->remote.sin_port));
    d_trace_hex(1, pkb->payload, pkb->len);
    if (sent < 0 || sent != pkb->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t s1ap_send_to_enb(enb_ctx_t *enb, pkbuf_t *pkb)
{
    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(pkb, return CORE_ERROR, "Null param");
    d_assert(enb->s1_sock, return CORE_ERROR, "No S1 path with ENB");

    return s1ap_send(enb->s1_sock, pkb);
}
