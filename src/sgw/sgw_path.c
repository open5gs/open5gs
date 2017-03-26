#define TRACE_MODULE _sgw_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "3gpp_defs.h"
#include "gtp_path.h"

#include "context.h"
#include "sgw_path.h"

static int _gtpv2_c_recv_cb(net_sock_t *net_sock, void *data)
{
    event_t e;
    int rc;
    pkbuf_t *pkbuf = NULL;
    gtp_node_t *gnode = data;

    d_assert(net_sock, return -1, "Null param");
    d_assert(gnode, return -1, "Null param");

    pkbuf = gtp_read(gnode);
    if (pkbuf == NULL)
    {
        if (net_sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    if (GTP_COMPARE_REMOTE_NODE(gnode, &sgw_self()->s11_node))
    {
        d_trace(1, "S11 PDU received from MME\n");
        event_set(&e, EVT_MSG_SGW_S11);
    }
    else if (GTP_COMPARE_REMOTE_NODE(gnode, &sgw_self()->s5c_node))
    {
        d_trace(1, "S5-C PDU received from PGW\n");
        event_set(&e, EVT_MSG_SGW_S5C);
    }
    else
        d_assert(0, pkbuf_free(pkbuf); return -1, "Unknown GTP-Node");

    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set_param1(&e, (c_uintptr_t)gnode);
    event_set_param2(&e, (c_uintptr_t)pkbuf);

    rc = event_send(sgw_self()->queue_id, &e);
    if (rc <= 0)
    {
        pkbuf_free(pkbuf);
        return rc;
    }

    return 0;
}

static int _gtpv1_u_recv_cb(net_sock_t *net_sock, void *data)
{
    pkbuf_t *pkbuf = NULL;
    gtp_node_t *gnode = data;

    d_assert(net_sock, return -1, "Null param");
    d_assert(gnode, return -1, "Null param");

    pkbuf = gtp_read(gnode);
    if (pkbuf == NULL)
    {
        if (net_sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(1, "S5U PDU received from GTP\n");
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    /* TODO */

    pkbuf_free(pkbuf);
    return 0;
}

status_t sgw_path_open()
{
    status_t rv;

    rv = gtp_open(&sgw_self()->s11_node, _gtpv2_c_recv_cb);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S11 Path with MME");
        return rv;
    }

    rv = gtp_open(&sgw_self()->s5c_node, _gtpv2_c_recv_cb);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S5-C Path with PGW");
        return rv;
    }

    rv = gtp_open(&sgw_self()->s5u_node, _gtpv1_u_recv_cb);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S5-U Path with PGW");
        return rv;
    }

    return CORE_OK;
}

status_t sgw_path_close()
{
    status_t rv;

    rv = gtp_close(&sgw_self()->s11_node);
    if (rv != CORE_OK)
    {
        d_error("Can't close S11 Path with MME");
        return rv;
    }

    rv = gtp_close(&sgw_self()->s5c_node);
    if (rv != CORE_OK)
    {
        d_error("Can't close S5-C Path with MME");
        return rv;
    }

    rv = gtp_close(&sgw_self()->s5u_node);
    if (rv != CORE_OK)
    {
        d_error("Can't close S5-U Path with MME");
        return rv;
    }

    return CORE_OK;
}

status_t sgw_path_send_to_s11(pkbuf_t *pkbuf)
{
    return gtp_send(&sgw_self()->s11_node, pkbuf);
}

status_t sgw_path_send_to_s5c(pkbuf_t *pkbuf)
{
    return gtp_send(&sgw_self()->s5c_node, pkbuf);
}

status_t sgw_path_send_to_s5u(pkbuf_t *pkbuf)
{
    return gtp_send(&sgw_self()->s5u_node, pkbuf);
}

