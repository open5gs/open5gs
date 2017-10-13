#define TRACE_MODULE _pgw_gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "types.h"
#include "gtp_path.h"

#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_gtp_path.h"

static int _gtpv1_tun_recv_cb(net_link_t *net_link, void *data)
{
    pkbuf_t *recvbuf = NULL;
    int n;
    status_t rv;
    pgw_bearer_t *bearer = NULL;

    recvbuf = pkbuf_alloc(GTPV1U_HEADER_LEN, MAX_SDU_LEN);
    d_assert(recvbuf, return -1, "pkbuf_alloc error");

    n = net_link_read(net_link, recvbuf->payload, recvbuf->len, 0);
    if (n <= 0)
    {
        pkbuf_free(recvbuf);
        return -1;
    }

    recvbuf->len = n;

    d_trace(50, "PDU received from TunTap\n");
    d_trace_hex(50, recvbuf->payload, recvbuf->len);

    /* Find the bearer by packet filter */
    bearer = pgw_bearer_find_by_packet(recvbuf);
    if (bearer)
    {
        gtp_header_t *gtp_h = NULL;
        gtp_node_t gnode;
        char buf[INET_ADDRSTRLEN];

        /* Add GTP-U header */
        rv = pkbuf_header(recvbuf, GTPV1U_HEADER_LEN);
        if (rv != CORE_OK)
        {
            d_error("pkbuf_header error");
            pkbuf_free(recvbuf);
            return -1;
        }
        
        gtp_h = (gtp_header_t *)recvbuf->payload;
        /* Bits    8  7  6  5  4  3  2  1
         *        +--+--+--+--+--+--+--+--+
         *        |version |PT| 1| E| S|PN|
         *        +--+--+--+--+--+--+--+--+
         *         0  0  1   1  0  0  0  0
         */
        gtp_h->flags = 0x30;
        gtp_h->type = GTPU_MSGTYPE_GPDU;
        gtp_h->length = htons(n);
        gtp_h->teid = htonl(bearer->sgw_s5u_teid);

        /* Send to SGW */
        gnode.addr = bearer->sgw_s5u_addr;
        gnode.port = GTPV1_U_UDP_PORT;
        gnode.sock = pgw_self()->gtpu_sock;
        d_trace(50, "Send S5U PDU (teid = 0x%x)to SGW(%s)\n",
                bearer->sgw_s5u_teid,
                INET_NTOP(&gnode.addr, buf));

        rv =  gtp_send(&gnode, recvbuf);
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
    c_uint32_t addr;
    c_uint16_t port;
    pgw_sgw_t *sgw = NULL;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(10, "S5-C PDU received from PGW\n");
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    addr = sock->remote.sin_addr.s_addr;
    port = ntohs(sock->remote.sin_port);

    sgw = pgw_sgw_find(addr, port);
    if (!sgw)
    {
        sgw = pgw_sgw_add();
        d_assert(sgw, return -1, "Can't add MME-GTP node");

        sgw->addr = addr;
        sgw->port = port;
        sgw->sock = sock;
    }

    event_set(&e, PGW_EVT_S5C_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sgw);
    event_set_param2(&e, (c_uintptr_t)pkbuf);
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
    c_uint32_t size = GTPV1U_HEADER_LEN;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(50, "S5-U PDU received from SGW\n");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

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

status_t pgw_gtp_open()
{
    status_t rv;

    rv = gtp_listen(&pgw_self()->gtpc_sock, _gtpv2_c_recv_cb, 
            pgw_self()->gtpc_addr, pgw_self()->gtpc_port, NULL);
    if (rv != CORE_OK)
    {
        d_error("Can't establish GTP-C Path for PGW");
        return rv;
    }

    rv = gtp_listen(&pgw_self()->gtpu_sock, _gtpv1_u_recv_cb, 
            pgw_self()->gtpu_addr, pgw_self()->gtpu_port, NULL);
    if (rv != CORE_OK)
    {
        d_error("Can't establish GTP-U Path for PGW");
        return rv;
    }

    {
        int rc;

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
        rc = net_tuntap_open(&pgw_self()->tun_link, pgw_self()->tun_dev_name, 0);
        if (rc != 0)
        {
            d_error("Can not open tun(dev : %s)", pgw_self()->tun_dev_name);
            return CORE_ERROR;
        }

        rc = net_register_link(pgw_self()->tun_link, _gtpv1_tun_recv_cb, NULL);
        if (rc != 0)
        {
            d_error("Can not register tun(dev : %s)", pgw_self()->tun_dev_name);
            net_tuntap_close(pgw_self()->tun_link);
            return CORE_ERROR;
        }

    }

    return CORE_OK;
}

status_t pgw_gtp_close()
{
    status_t rv;

    rv = gtp_close(pgw_self()->gtpc_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close GTP-C Path for MME");
        return rv;
    }

    rv = gtp_close(pgw_self()->gtpu_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close GTP-U Path for MME");
        return rv;
    }

    net_unregister_link(pgw_self()->tun_link);
    net_tuntap_close(pgw_self()->tun_link);

    return CORE_OK;
}
