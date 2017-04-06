#define TRACE_MODULE _sgw_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "3gpp_common.h"
#include "gtp_path.h"

#include "sgw_context.h"
#include "sgw_event.h"
#include "sgw_path.h"

static int _gtpv2_c_recv_cb(net_sock_t *sock, void *data)
{
    event_t e;
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    gtp_node_t *gnode = data;

    d_assert(sock, return -1, "Null param");
    d_assert(gnode, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    if (GTP_COMPARE_NODE(gnode, &sgw_self()->s11_node))
    {
        d_trace(1, "S11 PDU received from MME\n");
        event_set(&e, EVT_MSG_SGW_S11);
    }
    else if (GTP_COMPARE_NODE(gnode, &sgw_self()->s5c_node))
    {
        d_trace(1, "S5-C PDU received from PGW\n");
        event_set(&e, EVT_MSG_SGW_S5C);
    }
    else
        d_assert(0, pkbuf_free(pkbuf); return -1, "Unknown GTP-Node");

    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set_param1(&e, (c_uintptr_t)sock);
    event_set_param2(&e, (c_uintptr_t)gnode);
    event_set_param3(&e, (c_uintptr_t)pkbuf);
    rv = sgw_event_send(&e);
    if (rv != CORE_OK)
    {
        d_error("sgw_event_send error");
        pkbuf_free(pkbuf);
        return -1;
    }

    return 0;
}

static int _gtpv1_u_recv_cb(net_sock_t *sock, void *data)
{
    pkbuf_t *pkbuf = NULL;
    gtp_node_t *gnode = data;

    d_assert(sock, return -1, "Null param");
    d_assert(gnode, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(1, "S5-U PDU received from GTP\n");
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    /* TODO */

    pkbuf_free(pkbuf);
    return 0;
}

status_t sgw_path_open()
{
    status_t rv;

    rv = gtp_listen(&sgw_self()->s11_sock, _gtpv2_c_recv_cb, 
            sgw_self()->s11_addr, sgw_self()->s11_port, &sgw_self()->s11_node);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S11 Path for MME");
        return rv;
    }

    rv = gtp_listen(&sgw_self()->s5c_sock, _gtpv2_c_recv_cb, 
            sgw_self()->s5c_addr, sgw_self()->s5c_port, &sgw_self()->s5c_node);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S5-C Path for PGW");
        return rv;
    }

    rv = gtp_listen(&sgw_self()->s5u_sock, _gtpv1_u_recv_cb, 
            sgw_self()->s5u_addr, sgw_self()->s5u_port, &sgw_self()->s5u_node);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S5-U Path for PGW");
        return rv;
    }

    return CORE_OK;
}

status_t sgw_path_close()
{
    status_t rv;

    rv = gtp_close(sgw_self()->s11_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close S11 Path for MME");
        return rv;
    }

    rv = gtp_close(sgw_self()->s5c_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close S5-C Path for MME");
        return rv;
    }

    rv = gtp_close(sgw_self()->s5u_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close S5-U Path for MME");
        return rv;
    }

    return CORE_OK;
}

status_t sgw_s11_send_to_mme(
    gtp_xact_t *xact, c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(xact, return CORE_ERROR, "Null param");

    d_assert(gtp_xact_commit(xact, type, teid, pkbuf) == CORE_OK,
            return CORE_ERROR, "xact commit error");

    return CORE_OK;
}

status_t sgw_s5c_send_to_pgw(
    gtp_xact_t *assoc_xact, c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    gtp_xact_t *xact = NULL;
    d_assert(assoc_xact, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    xact = gtp_xact_local_create(&sgw_self()->gtp_xact_ctx, 
            sgw_self()->s5c_sock, &sgw_self()->s5c_node);
    d_assert(xact, return CORE_ERROR, "Null param");

    d_assert(gtp_xact_associated_commit(
                xact, assoc_xact, type, teid, pkbuf) == CORE_OK,
            return CORE_ERROR, "gtp_send error");

    return CORE_OK;
}

status_t sgw_s5u_send_to_pgw(pkbuf_t *pkbuf)
{
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    return gtp_send(sgw_self()->s5u_sock, &sgw_self()->s5u_node, pkbuf);
}

