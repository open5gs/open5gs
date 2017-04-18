#define TRACE_MODULE _pgw_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "3gpp_common.h"
#include "gtp_path.h"

#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_path.h"

static int _gtpv1_tun_recv_cb(net_link_t *net_link, void *data)
{
    pkbuf_t *recvbuf = NULL;
    int n;
    status_t rv;
    pgw_bearer_t *bearer = NULL;

    recvbuf = pkbuf_alloc(sizeof(gtp_header_t), MAX_SDU_LEN);
    d_assert(recvbuf, return -1, "pkbuf_alloc error");

    n = net_link_read(net_link, recvbuf->payload, recvbuf->len, 0);
    if (n <= 0)
    {
        pkbuf_free(recvbuf);
        return -1;
    }

    /* Find the bearer by packet filter */
    bearer = pgw_bearer_find_by_packet(recvbuf);
    if (bearer)
    {
        gtp_header_t *gtp_h = NULL;
        gtp_node_t gnode;

        /* Add GTP-U header */
        rv = pkbuf_header(recvbuf, sizeof(gtp_header_t));
        if (rv != CORE_OK)
        {
            d_error("pkbuf_header error");
            pkbuf_free(recvbuf);
            return -1;
        }
        
        /* Bits    8  7  6  5  4  3  2  1
         *        +--+--+--+--+--+--+--+--+
         *        |version |PT| 1| E| S|PN|
         *        +--+--+--+--+--+--+--+--+
         *         0  0  1   1  0  0  0  0
         */
        gtp_h->flags = 0x30;
        gtp_h->type = GTPU_MSGTYPE_GPDU;
        gtp_h->length = n;
        gtp_h->teid = bearer->sgw_s5u_teid;

        /* Send to SGW */
        gnode.addr = bearer->sgw_s5u_addr;
        gnode.port = GTPV1_U_UDP_PORT;

        rv =  gtp_send(pgw_self()->s5u_sock, &gnode, recvbuf);
    }
    else
    {
        d_error("Can not find bearer");
    }

    pkbuf_free(recvbuf);
    return 0;

}

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

    event_set(&e, PGW_EVT_S5C_SESSION_MSG);
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
    c_uint32_t size = sizeof(gtp_header_t);

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(1, "S5-U PDU received from GTP\n");
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    /* Remove GTP header and send packets to TUN interface */
    if (pkbuf_header(pkbuf, -size) != CORE_OK)
    {
        d_error("pkbuf_header error");

        pkbuf_free(pkbuf);
        return -1;
    }

    if (net_link_write(pgw_self()->tun_link, pkbuf->payload, pkbuf->len) <= 0)
    {
        d_error("Can not send packets to tuntap");
    }

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

    {
        int rc;

        /* FIXME : dev_name should be configured */
        char *tun_dev_name = "pgwtun";

        /* NOTE : tun device can be created via following command.
         *
         * $ sudo ip tuntap add name pgwtun mode tun
         *
         * Also, before running pgw, assign the one IP from IP pool of UE 
         * to pgwtun. The IP should not be assigned to UE
         *
         * $ sudo ifconfig pgwtun 45.45.0.1/16 up
         *
         */

        /* Open Tun interface */
        rc = net_tuntap_open(&pgw_self()->tun_link, tun_dev_name, 0);
        if (rc != 0)
        {
            d_error("Can not open tun(dev : %s)",tun_dev_name);
            return CORE_ERROR;
        }

        rc = net_register_link(pgw_self()->tun_link, _gtpv1_tun_recv_cb, NULL);
        if (rc != 0)
        {
            d_error("Can not register tun(dev : %s)",tun_dev_name);
            net_tuntap_close(pgw_self()->tun_link);
            return CORE_ERROR;
        }

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

    net_unregister_link(pgw_self()->tun_link);
    net_link_close(pgw_self()->tun_link);

    return CORE_OK;
}

status_t pgw_s5c_send_to_sgw(
        gtp_xact_t *xact, c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(xact, return CORE_ERROR, "Null param");

    d_assert(gtp_xact_commit(xact, type, teid, pkbuf) == CORE_OK,
            return CORE_ERROR, "xact commit error");

    return CORE_OK;
}

status_t pgw_s5u_send_to_sgw(pkbuf_t *pkbuf)
{
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    return gtp_send(pgw_self()->s5u_sock, &pgw_self()->s5u_node, pkbuf);
}
