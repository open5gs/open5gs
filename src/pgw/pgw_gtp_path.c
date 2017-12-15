#define TRACE_MODULE _pgw_gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"

#include "3gpp_types.h"
#include "gtp_node.h"
#include "gtp_path.h"

#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_gtp_path.h"

static status_t pgw_gtp_send_to_bearer(pgw_bearer_t *bearer, pkbuf_t *sendbuf)
{
    status_t rv;
    gtp_header_t *gtp_h = NULL;

    /* Add GTP-U header */
    rv = pkbuf_header(sendbuf, GTPV1U_HEADER_LEN);
    if (rv != CORE_OK)
    {
        d_error("pkbuf_header error");
        pkbuf_free(sendbuf);
        return CORE_ERROR;
    }
    
    gtp_h = (gtp_header_t *)sendbuf->payload;
    /* Bits    8  7  6  5  4  3  2  1
     *        +--+--+--+--+--+--+--+--+
     *        |version |PT| 1| E| S|PN|
     *        +--+--+--+--+--+--+--+--+
     *         0  0  1   1  0  0  0  0
     */
    gtp_h->flags = 0x30;
    gtp_h->type = GTPU_MSGTYPE_GPDU;
    gtp_h->length = htons(sendbuf->len);
    gtp_h->teid = htonl(bearer->sgw_s5u_teid);

    /* Send to SGW */
    d_trace(50, "Send S5U PDU (teid = 0x%x) to SGW\n",
            bearer->sgw_s5u_teid);
    rv =  gtp_send(bearer->gnode, sendbuf);

    return rv;
}

static int _gtpv1_tun_recv_cb(sock_id sock, void *data)
{
    pkbuf_t *recvbuf = NULL;
    int n;
    status_t rv;
    pgw_bearer_t *bearer = NULL;

    recvbuf = pkbuf_alloc(GTPV1U_HEADER_LEN, MAX_SDU_LEN);
    d_assert(recvbuf, return -1, "pkbuf_alloc error");

    n = sock_read(sock, recvbuf->payload, recvbuf->len);
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
        /* Unicast */
        rv = pgw_gtp_send_to_bearer(bearer, recvbuf);
        d_assert(rv == CORE_OK,, "pgw_gtp_send_to_bearer failed");
    }
    else
    {
        struct ip *ip_h =  NULL;
        struct ip6_hdr *ip6_h =  NULL;

        ip_h = (struct ip *)recvbuf->payload;
        if (ip_h->ip_v == 6)
        {
            ip6_h = (struct ip6_hdr *)recvbuf->payload;
            if (IN6_IS_ADDR_MULTICAST(&ip6_h->ip6_dst))
            {
                hash_index_t *hi = NULL;

                /* IPv6 Multicast */
                for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
                {
                    pgw_sess_t *sess = pgw_sess_this(hi);
                    d_assert(sess, return 0,);
                    if (sess->ipv6)
                    {
                        /* PDN IPv6 is avaiable */
                        pgw_bearer_t *bearer = pgw_default_bearer_in_sess(sess);
                        d_assert(bearer, return 0,);

                        rv = pgw_gtp_send_to_bearer(bearer, recvbuf);
                        d_assert(rv == CORE_OK,,
                                "pgw_gtp_send_to_bearer failed");
                    }
                }
            }
        }
    }

    pkbuf_free(recvbuf);
    return 0;

}

static int _gtpv2_c_recv_cb(sock_id sock, void *data)
{
    event_t e;
    status_t rv;
    pkbuf_t *pkbuf = NULL;

    d_assert(sock, return -1, "Null param");

    rv = gtp_recv(sock, &pkbuf);
    if (rv != CORE_OK)
    {
        if (errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(10, "S5-C PDU received from PGW\n");
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    event_set(&e, PGW_EVT_S5C_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)pkbuf);
    rv = pgw_event_send(&e);
    if (rv != CORE_OK)
    {
        d_error("pgw_event_send error");
        pkbuf_free(pkbuf);
        return -1;
    }

    return 0;
}

static int _gtpv1_u_recv_cb(sock_id sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t size = GTPV1U_HEADER_LEN;

    d_assert(sock, return -1, "Null param");

    rv = gtp_recv(sock, &pkbuf);
    if (rv != CORE_OK)
    {
        if (errno == EAGAIN)
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

    if (sock_write(pgw_self()->tun_sock, pkbuf->payload, pkbuf->len) <= 0)
    {
        d_error("Can not send packets to tuntap");
    }

    pkbuf_free(pkbuf);

    return 0;
}

status_t pgw_gtp_open()
{
    status_t rv;
    int i;
    int rc;

    rv = gtp_server_list(&pgw_self()->gtpc_list, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&pgw_self()->gtpc_list6, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    pgw_self()->gtpc_addr = gtp_local_addr_first(&pgw_self()->gtpc_list);
    pgw_self()->gtpc_addr6 = gtp_local_addr_first(&pgw_self()->gtpc_list6);

    d_assert(pgw_self()->gtpc_addr || pgw_self()->gtpc_addr6,
            return CORE_ERROR, "No GTP Server");

    rv = gtp_server_list(&pgw_self()->gtpu_list, _gtpv1_u_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&pgw_self()->gtpu_list6, _gtpv1_u_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    pgw_self()->gtpu_addr = gtp_local_addr_first(&pgw_self()->gtpu_list);
    pgw_self()->gtpu_addr6 = gtp_local_addr_first(&pgw_self()->gtpu_list6);

    d_assert(pgw_self()->gtpu_addr || pgw_self()->gtpu_addr6,
            return CORE_ERROR, "No GTP Server");

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
    rc = tun_open(&pgw_self()->tun_sock, (char *)pgw_self()->tun_ifname, 0);
    if (rc != 0)
    {
        d_error("Can not open tun(dev : %s)", pgw_self()->tun_ifname);
        return CORE_ERROR;
    }

    /* 
     * On Linux, it is possible to create a persistent tun/tap 
     * interface which will continue to exist even if nextepc quit, 
     * although this is normally not required. 
     * It can be useful to set up a tun/tap interface owned 
     * by a non-root user, so nextepc can be started without 
     * needing any root privileges at all.
     */

    /* Set P-to-P IP address with Netmask
     * Note that Linux will skip this configuration */
    for (i = 0; i < pgw_self()->num_of_ue_pool; i++)
    {
        rc = tun_set_ip(pgw_self()->tun_sock, 
                pgw_self()->ue_pool[i].ipstr,
                pgw_self()->ue_pool[i].mask_or_numbits);
        if (rc != 0)
        {
            d_error("Can not configure tun(dev : %s for %s/%s)",
                    pgw_self()->tun_ifname,
                    pgw_self()->ue_pool[i].ipstr,
                    pgw_self()->ue_pool[i].mask_or_numbits);

            return CORE_ERROR;
        }
    }

    rc = sock_register(pgw_self()->tun_sock, _gtpv1_tun_recv_cb, NULL);
    if (rc != 0)
    {
        d_error("Can not register tun(dev : %s)",
                pgw_self()->tun_ifname);
        sock_delete(pgw_self()->tun_sock);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t pgw_gtp_close()
{
    sock_delete_list(&pgw_self()->gtpc_list);
    sock_delete_list(&pgw_self()->gtpc_list6);

    sock_delete_list(&pgw_self()->gtpu_list);
    sock_delete_list(&pgw_self()->gtpu_list6);

    sock_delete(pgw_self()->tun_sock);

    return CORE_OK;
}
