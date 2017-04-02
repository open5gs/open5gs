#define TRACE_MODULE _pgw_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "3gpp_defs.h"
#include "gtp_path.h"

#include "context.h"
#include "pgw_path.h"

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

    d_trace(1, "S5-C PDU received from PGW\n");
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set(&e, EVT_MSG_PGW_S5C);
    event_set_param1(&e, (c_uintptr_t)sock);
    event_set_param2(&e, (c_uintptr_t)gnode);
    event_set_param3(&e, (c_uintptr_t)pkbuf);
    rv = pgw_event_send(&e);
    if (rv != CORE_OK)
    {
        d_error("pgw_event_send error");
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

status_t pgw_path_open()
{
    status_t rv;

    rv = gtp_listen(&pgw_self()->s5c_sock, _gtpv2_c_recv_cb, 
            pgw_self()->s5c_addr, pgw_self()->s5c_port, &pgw_self()->s5c_node);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S5-C Path for PGW");
        return rv;
    }

    rv = gtp_listen(&pgw_self()->s5u_sock, _gtpv1_u_recv_cb, 
            pgw_self()->s5u_addr, pgw_self()->s5u_port, &pgw_self()->s5u_node);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S5-U Path for PGW");
        return rv;
    }

    return CORE_OK;
}

status_t pgw_path_close()
{
    status_t rv;

    rv = gtp_close(pgw_self()->s5c_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close S5-C Path for MME");
        return rv;
    }

    rv = gtp_close(pgw_self()->s5u_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close S5-U Path for MME");
        return rv;
    }

    return CORE_OK;
}

gtp_xact_t *pgw_s5c_send_to_sgw(gtp_message_t *gtp_message)
{
    gtp_xact_t *xact;

    d_assert(gtp_message, return NULL, "Null param");

    xact = gtp_xact_send(&pgw_self()->gtp_xact_ctx, pgw_self()->s5c_sock, 
            &pgw_self()->s5c_node, gtp_message);
    d_assert(xact, return NULL, "Null param");

    return xact;
}

status_t pgw_s5u_send_to_sgw(pkbuf_t *pkbuf)
{
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    return gtp_send(pgw_self()->s5u_sock, &pgw_self()->s5u_node, pkbuf);
}
