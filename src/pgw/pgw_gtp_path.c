#define TRACE_MODULE _pgw_gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"

#include "3gpp_types.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"

#include "common/context.h"
#include "pgw_context.h"
#include "pgw_event.h"
#include "pgw_gtp_path.h"
#include "pgw_ipfw.h"

#define PGW_GTP_HANDLED     1

c_uint16_t in_cksum(c_uint16_t *addr, int len);
static status_t pgw_gtp_handle_multicast(pkbuf_t *recvbuf);
static status_t pgw_gtp_handle_slaac(pgw_sess_t *sess, pkbuf_t *recvbuf);
static status_t pgw_gtp_send_to_bearer(pgw_bearer_t *bearer, pkbuf_t *sendbuf);
static status_t pgw_gtp_send_router_advertisement(
        pgw_sess_t *sess, c_uint8_t *ip6_dst);

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

    d_trace(50, "[TUN] RECV : ");
    d_trace_hex(50, recvbuf->payload, recvbuf->len);

    /* Find the bearer by packet filter */
    bearer = pgw_bearer_find_by_packet(recvbuf);
    if (bearer)
    {
        /* Unicast */
        rv = pgw_gtp_send_to_bearer(bearer, recvbuf);
        d_assert(rv == CORE_OK,, "pgw_gtp_send_to_bearer() failed");
    }
    else
    {
        if (context_self()->parameter.multicast)
        {
            rv = pgw_gtp_handle_multicast(recvbuf);
            d_assert(rv != CORE_ERROR,, "pgw_gtp_handle_multicast() failed");
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

    event_set(&e, PGW_EVT_S5C_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)pkbuf);
    rv = pgw_event_send(&e);
    if (rv != CORE_OK)
    {
        d_error("pgw_event_send error");
        pkbuf_free(pkbuf);
        return 0;
    }

    return 0;
}

static int _gtpv1_u_recv_cb(sock_id sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t size = GTPV1U_HEADER_LEN;
    gtp_header_t *gtp_h = NULL;
    struct ip *ip_h = NULL;

    c_uint32_t teid;
    pgw_bearer_t *bearer = NULL;
    pgw_sess_t *sess = NULL;
    pgw_subnet_t *subnet = NULL;
    pgw_dev_t *dev = NULL;

    d_assert(sock, return -1, "Null param");

    rv = gtp_recv(sock, &pkbuf);
    if (rv != CORE_OK)
    {
        if (errno == EAGAIN)
            return 0;

        return -1;
    }

    d_assert(pkbuf, return 0,);
    d_assert(pkbuf->payload, goto cleanup,);
    d_trace(50, "[PGW] RECV : ");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    gtp_h = pkbuf->payload;
    if (gtp_h->flags & GTPU_FLAGS_S) size += 4;
    teid = ntohl(gtp_h->teid);

    d_trace(3, "[PGW] RECV GPU-U from SGW : TEID[0x%x]\n", teid);

    /* Remove GTP header and send packets to TUN interface */
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, goto cleanup,);

    ip_h = pkbuf->payload;
    d_assert(ip_h, goto cleanup,);

    bearer = pgw_bearer_find_by_pgw_s5u_teid(teid);
    d_assert(bearer, goto cleanup,);
    sess = bearer->sess;
    d_assert(sess, goto cleanup,);

    if (ip_h->ip_v == 4 && sess->ipv4)
        subnet = sess->ipv4->subnet;
    else if (ip_h->ip_v == 6 && sess->ipv6)
        subnet = sess->ipv6->subnet;

    if (!subnet)
    {
        d_trace_hex(9, pkbuf->payload, pkbuf->len);
        d_trace(9, "[DROP] Cannot find subnet V:%d, IPv4:%p, IPv6:%p\n",
                ip_h->ip_v, sess->ipv4, sess->ipv6);
        goto cleanup;
    }

    /* Check IPv6 */
    if (context_self()->parameter.no_slaac == 0 && ip_h->ip_v == 6)
    {
        rv = pgw_gtp_handle_slaac(sess, pkbuf);
        if (rv == PGW_GTP_HANDLED)
        {
            pkbuf_free(pkbuf);
            return 0;
        }
        d_assert(rv == CORE_OK,, "pgw_gtp_handle_slaac() failed");
    }

    dev = subnet->dev;
    d_assert(dev, goto cleanup,);
    if (sock_write(dev->sock, pkbuf->payload, pkbuf->len) <= 0)
        d_error("sock_write() failed");

cleanup:
    pkbuf_free(pkbuf);
    return 0;
}

status_t pgw_gtp_open()
{
    status_t rv;
    pgw_dev_t *dev = NULL;
    pgw_subnet_t *subnet = NULL;
    int rc;

    rv = gtp_server_list(&pgw_self()->gtpc_list, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&pgw_self()->gtpc_list6, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    pgw_self()->gtpc_sock = gtp_local_sock_first(&pgw_self()->gtpc_list);
    pgw_self()->gtpc_sock6 = gtp_local_sock_first(&pgw_self()->gtpc_list6);
    pgw_self()->gtpc_addr = gtp_local_addr_first(&pgw_self()->gtpc_list);
    pgw_self()->gtpc_addr6 = gtp_local_addr_first(&pgw_self()->gtpc_list6);

    d_assert(pgw_self()->gtpc_addr || pgw_self()->gtpc_addr6,
            return CORE_ERROR, "No GTP Server");

    rv = gtp_server_list(&pgw_self()->gtpu_list, _gtpv1_u_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&pgw_self()->gtpu_list6, _gtpv1_u_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    pgw_self()->gtpu_sock = gtp_local_sock_first(&pgw_self()->gtpu_list);
    pgw_self()->gtpu_sock6 = gtp_local_sock_first(&pgw_self()->gtpu_list6);
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
    for (dev = pgw_dev_first(); dev; dev = pgw_dev_next(dev))
    {
        rc = tun_open(&dev->sock, (char *)dev->ifname, 0);
        if (rc != 0)
        {
            d_error("tun_open(dev:%s) failed", dev->ifname);
            return CORE_ERROR;
        }

        rc = sock_register(dev->sock, _gtpv1_tun_recv_cb, NULL);
        if (rc != 0)
        {
            d_error("sock_register(dev:%s) failed", dev->ifname);
            sock_delete(dev->sock);
            return CORE_ERROR;
        }
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
    for (subnet = pgw_subnet_first(); subnet; subnet = pgw_subnet_next(subnet))
    {
        d_assert(subnet->dev, return CORE_ERROR,);
        rc = tun_set_ip(subnet->dev->sock, &subnet->gw, &subnet->sub);
        if (rc != 0)
        {
            d_error("tun_set_ip(dev:%s) failed", subnet->dev->ifname);
            return CORE_ERROR;
        }
    }

    /* Link-Local Address for PGW_TUN */
    for (dev = pgw_dev_first(); dev; dev = pgw_dev_next(dev))
        dev->link_local_addr = core_link_local_addr_by_dev(dev->ifname);

    return CORE_OK;
}

status_t pgw_gtp_close()
{
    pgw_dev_t *dev = NULL;

    sock_delete_list(&pgw_self()->gtpc_list);
    sock_delete_list(&pgw_self()->gtpc_list6);

    sock_delete_list(&pgw_self()->gtpu_list);
    sock_delete_list(&pgw_self()->gtpu_list6);

    for (dev = pgw_dev_first(); dev; dev = pgw_dev_next(dev))
        sock_delete(dev->sock);

    return CORE_OK;
}

static status_t pgw_gtp_handle_multicast(pkbuf_t *recvbuf)
{
    status_t rv;
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h =  NULL;

    ip_h = (struct ip *)recvbuf->payload;
    if (ip_h->ip_v == 6)
    {
#if COMPILE_ERROR_IN_MAC_OS_X  /* Compiler error in Mac OS X platform */
        ip6_h = (struct ip6_hdr *)recvbuf->payload;
        if (IN6_IS_ADDR_MULTICAST(&ip6_h->ip6_dst))
#else
        struct in6_addr ip6_dst;
        ip6_h = (struct ip6_hdr *)recvbuf->payload;
        memcpy(&ip6_dst, &ip6_h->ip6_dst, sizeof(struct in6_addr));
        if (IN6_IS_ADDR_MULTICAST(&ip6_dst))
#endif
        {
            hash_index_t *hi = NULL;

            /* IPv6 Multicast */
            for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
            {
                pgw_sess_t *sess = pgw_sess_this(hi);
                d_assert(sess, return CORE_ERROR,);
                if (sess->ipv6)
                {
                    /* PDN IPv6 is avaiable */
                    pgw_bearer_t *bearer = pgw_default_bearer_in_sess(sess);
                    d_assert(bearer, return CORE_ERROR,);

                    rv = pgw_gtp_send_to_bearer(bearer, recvbuf);
                    d_assert(rv == CORE_OK,,
                            "pgw_gtp_send_to_bearer failed");

                    return PGW_GTP_HANDLED;
                }
            }
        }
    }

    return CORE_OK;
}

static status_t pgw_gtp_handle_slaac(pgw_sess_t *sess, pkbuf_t *recvbuf)
{
    status_t rv;
    struct ip *ip_h = NULL;

    d_assert(sess, return CORE_ERROR,);
    d_assert(recvbuf, return CORE_ERROR,);
    d_assert(recvbuf->payload, return CORE_ERROR,);
    ip_h = (struct ip *)recvbuf->payload;
    if (ip_h->ip_v == 6)
    {
        struct ip6_hdr *ip6_h = (struct ip6_hdr *)recvbuf->payload;
        if (ip6_h->ip6_nxt == IPPROTO_ICMPV6)
        {
            struct icmp6_hdr *icmp_h =
                (struct icmp6_hdr *)(recvbuf->payload + sizeof(struct ip6_hdr));
            if (icmp_h->icmp6_type == ND_ROUTER_SOLICIT)
            {
                d_trace(5, "[PGW]      Router Solict\n");
                if (sess->ipv6)
                {
                    rv = pgw_gtp_send_router_advertisement(
                            sess, ip6_h->ip6_src.s6_addr);
                    d_assert(rv == CORE_OK,,"send router advertisement failed");
                }
                return PGW_GTP_HANDLED;
            }
        }
    }

    return CORE_OK;
}

static status_t pgw_gtp_send_to_bearer(pgw_bearer_t *bearer, pkbuf_t *sendbuf)
{
    char buf[CORE_ADDRSTRLEN];
    status_t rv;
    gtp_header_t *gtp_h = NULL;

    d_assert(bearer, pkbuf_free(sendbuf); return CORE_ERROR,);
    d_assert(bearer->gnode, pkbuf_free(sendbuf); return CORE_ERROR,);
    d_assert(bearer->gnode->sock, pkbuf_free(sendbuf); return CORE_ERROR,);

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
    gtp_h->length = htons(sendbuf->len - GTPV1U_HEADER_LEN);
    gtp_h->teid = htonl(bearer->sgw_s5u_teid);

    /* Send to SGW */
    d_trace(50, "[PGW] SEND : ");
    d_trace_hex(50, sendbuf->payload, sendbuf->len);

    d_trace(3, "[PGW] SEND GPU-U to SGW[%s] : TEID[0x%x]\n",
        CORE_ADDR(sock_remote_addr(bearer->gnode->sock), buf),
        bearer->sgw_s5u_teid);
    rv =  gtp_send(bearer->gnode, sendbuf);

    return rv;
}

static status_t pgw_gtp_send_router_advertisement(
        pgw_sess_t *sess, c_uint8_t *ip6_dst)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;

    pgw_bearer_t *bearer = NULL;
    pgw_ue_ip_t *ue_ip = NULL;
    pgw_subnet_t *subnet = NULL;
    pgw_dev_t *dev = NULL;

    ipsubnet_t src_ipsub;
    c_uint16_t plen = 0;
    c_uint8_t nxt = 0;
    c_uint8_t *p = NULL;
    struct ip6_hdr *ip6_h =  NULL;
    struct nd_router_advert *advert_h = NULL;
    struct nd_opt_prefix_info *prefix = NULL;

    d_assert(sess, return CORE_ERROR,);
    bearer = pgw_default_bearer_in_sess(sess);
    d_assert(bearer, return CORE_ERROR,);
    ue_ip = sess->ipv6;
    d_assert(ue_ip, return CORE_ERROR,);
    subnet = ue_ip->subnet;
    d_assert(subnet, return CORE_ERROR,);
    dev = subnet->dev;
    d_assert(dev, return CORE_ERROR,);

    pkbuf = pkbuf_alloc(GTPV1U_HEADER_LEN, 200);
    d_assert(pkbuf, return CORE_ERROR,);
    pkbuf->len = sizeof *ip6_h + sizeof *advert_h + sizeof *prefix;
    memset(pkbuf->payload, 0, pkbuf->len);

    p = (c_uint8_t *)pkbuf->payload;
    ip6_h = (struct ip6_hdr *)p;
    advert_h = (struct nd_router_advert *)((c_uint8_t *)ip6_h + sizeof *ip6_h);
    prefix = (struct nd_opt_prefix_info *)
        ((c_uint8_t*)advert_h + sizeof *advert_h);

    rv = core_ipsubnet(&src_ipsub, "fe80::1", NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    if (dev->link_local_addr)
        memcpy(src_ipsub.sub, dev->link_local_addr->sin6.sin6_addr.s6_addr,
                sizeof src_ipsub.sub);

    advert_h->nd_ra_type = ND_ROUTER_ADVERT;
    advert_h->nd_ra_code = 0;
    advert_h->nd_ra_curhoplimit = 64;
    advert_h->nd_ra_flags_reserved = 0;
    advert_h->nd_ra_router_lifetime = htons(64800);  /* 64800s */
    advert_h->nd_ra_reachable = 0;
    advert_h->nd_ra_retransmit = 0;

    prefix->nd_opt_pi_type = ND_OPT_PREFIX_INFORMATION;
    prefix->nd_opt_pi_len = 4; /* 32bytes */
    prefix->nd_opt_pi_prefix_len = subnet->prefixlen;
    prefix->nd_opt_pi_flags_reserved =
        ND_OPT_PI_FLAG_ONLINK|ND_OPT_PI_FLAG_AUTO;
    prefix->nd_opt_pi_valid_time = htonl(0xffffffff); /* Infinite */
    prefix->nd_opt_pi_preferred_time = htonl(0xffffffff); /* Infinite */
    memcpy(prefix->nd_opt_pi_prefix.s6_addr,
            subnet->sub.sub, sizeof prefix->nd_opt_pi_prefix.s6_addr);

    /* For IPv6 Pseudo-Header */
    plen = htons(sizeof *advert_h + sizeof *prefix);
    nxt = IPPROTO_ICMPV6;

    memcpy(p, src_ipsub.sub, sizeof src_ipsub.sub);
    p += sizeof src_ipsub.sub;
    memcpy(p, ip6_dst, IPV6_LEN);
    p += IPV6_LEN;
    p += 2; memcpy(p, &plen, 2); p += 2;
    p += 3; *p = nxt; p += 1;
    advert_h->nd_ra_cksum = in_cksum((c_uint16_t *)pkbuf->payload, pkbuf->len);

    ip6_h->ip6_flow = htonl(0x60000001);
    ip6_h->ip6_plen = plen;
    ip6_h->ip6_nxt = nxt;  /* ICMPv6 */
    ip6_h->ip6_hlim = 0xff;
    memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
    memcpy(ip6_h->ip6_dst.s6_addr, ip6_dst, IPV6_LEN);
    
    rv = pgw_gtp_send_to_bearer(bearer, pkbuf);
    d_assert(rv == CORE_OK,, "pgw_gtp_send_to_bearer() faild");

    d_trace(5, "[PGW]      Router Advertisement\n");

    pkbuf_free(pkbuf);
    return rv;
}

c_uint16_t in_cksum(c_uint16_t *addr, int len)
{
    int nleft = len;
    c_uint32_t sum = 0;
    c_uint16_t *w = addr;
    c_uint16_t answer = 0;

    // Adding 16 bits sequentially in sum
    while (nleft > 1)
    {
        sum += *w;
        nleft -= 2;
        w++;
    }

    // If an odd byte is left
    if (nleft == 1)
    {
        *(c_uint8_t *) (&answer) = *(c_uint8_t *) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}
