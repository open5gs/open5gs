/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "base/base.h"

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IP6_H
#include <netinet/ip6.h>
#endif

#if HAVE_NETINET_IP_ICMP_H
#include <netinet/ip_icmp.h>
#endif

#if HAVE_NETINET_ICMP6_H
#include <netinet/icmp6.h>
#endif

#include "gtp/gtp-node.h"
#include "gtp/gtp-path.h"

#include "app/context.h"
#include "pgw-context.h"
#include "pgw-event.h"
#include "pgw-gtp-path.h"
#include "pgw-ipfw.h"

#define PGW_GTP_HANDLED     1

uint16_t in_cksum(uint16_t *addr, int len);
static int pgw_gtp_handle_multicast(ogs_pkbuf_t *recvbuf);
static int pgw_gtp_handle_slaac(pgw_sess_t *sess, ogs_pkbuf_t *recvbuf);
static int pgw_gtp_send_to_bearer(pgw_bearer_t *bearer, ogs_pkbuf_t *sendbuf);
static int pgw_gtp_send_router_advertisement(
        pgw_sess_t *sess, uint8_t *ip6_dst);

static void _gtpv1_tun_recv_cb(short when, ogs_socket_t fd, void *data)
{
    ogs_pkbuf_t *recvbuf = NULL;
    int n;
    int rv;
    pgw_bearer_t *bearer = NULL;

    recvbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_reserve(recvbuf, GTPV1U_HEADER_LEN);
    ogs_pkbuf_put(recvbuf, MAX_SDU_LEN-GTPV1U_HEADER_LEN);

    n = ogs_read(fd, recvbuf->data, recvbuf->len);
    if (n <= 0) {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno, "ogs_read() failed");
        ogs_pkbuf_free(recvbuf);
        return;
    }

    ogs_pkbuf_trim(recvbuf, n);

    /* Find the bearer by packet filter */
    bearer = pgw_bearer_find_by_packet(recvbuf);
    if (bearer) {
        /* Unicast */
        rv = pgw_gtp_send_to_bearer(bearer, recvbuf);
        ogs_assert(rv == OGS_OK);
    } else {
        if (context_self()->config.parameter.multicast) {
            rv = pgw_gtp_handle_multicast(recvbuf);
            ogs_assert(rv != OGS_ERROR);
        }
    }

    ogs_pkbuf_free(recvbuf);
}

static void _gtpv2_c_recv_cb(short when, ogs_socket_t fd, void *data)
{
    pgw_event_t *e = NULL;
    int rv;
    ssize_t size;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN);

    size = ogs_recv(fd, pkbuf->data, pkbuf->len, 0);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recv() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    ogs_pkbuf_trim(pkbuf, size);

    e = pgw_event_new(PGW_EVT_S5C_MESSAGE);
    ogs_assert(e);
    e->gtpbuf = pkbuf;

    rv = ogs_queue_push(pgw_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->gtpbuf);
        pgw_event_free(e);
    }
}

static void _gtpv1_u_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int rv;
    ssize_t size;
    ogs_pkbuf_t *pkbuf = NULL;
    uint32_t len = GTPV1U_HEADER_LEN;
    gtp_header_t *gtp_h = NULL;
    struct ip *ip_h = NULL;

    uint32_t teid;
    pgw_bearer_t *bearer = NULL;
    pgw_sess_t *sess = NULL;
    pgw_subnet_t *subnet = NULL;
    pgw_dev_t *dev = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN);

    size = ogs_recv(fd, pkbuf->data, pkbuf->len, 0);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recv() failed");
        goto cleanup;
    }

    ogs_pkbuf_trim(pkbuf, size);

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    gtp_h = pkbuf->data;
    if (gtp_h->flags & GTPU_FLAGS_S) len += 4;
    teid = ntohl(gtp_h->teid);

    ogs_debug("[PGW] RECV GPU-U from SGW : TEID[0x%x]", teid);

    /* Remove GTP header and send packets to TUN interface */
    ogs_assert(ogs_pkbuf_pull(pkbuf, len));

    ip_h = pkbuf->data;
    ogs_assert(ip_h);

    bearer = pgw_bearer_find_by_pgw_s5u_teid(teid);
    if (!bearer) {
        ogs_warn("[DROP] Cannot find PGW S5U bearer : TEID[0x%x]", teid);
        goto cleanup;
    }
    sess = bearer->sess;
    ogs_assert(sess);

    if (ip_h->ip_v == 4 && sess->ipv4)
        subnet = sess->ipv4->subnet;
    else if (ip_h->ip_v == 6 && sess->ipv6)
        subnet = sess->ipv6->subnet;

    if (!subnet) {
        ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data, pkbuf->len);
        ogs_trace("[DROP] Cannot find subnet V:%d, IPv4:%p, IPv6:%p",
                ip_h->ip_v, sess->ipv4, sess->ipv6);
        goto cleanup;
    }

    /* Check IPv6 */
    if (context_self()->config.parameter.no_slaac == 0 && ip_h->ip_v == 6) {
        rv = pgw_gtp_handle_slaac(sess, pkbuf);
        if (rv == PGW_GTP_HANDLED) {
            goto cleanup;
        }
        ogs_assert(rv == OGS_OK);
    }

    dev = subnet->dev;
    ogs_assert(dev);
    if (ogs_write(dev->fd, pkbuf->data, pkbuf->len) <= 0)
        ogs_error("ogs_write() failed");

cleanup:
    ogs_pkbuf_free(pkbuf);
}

int pgw_gtp_open()
{
    pgw_dev_t *dev = NULL;
    pgw_subnet_t *subnet = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;
    int rc;

    ogs_list_for_each(&pgw_self()->gtpc_list, node) {
        ogs_socknode_set_poll(node, pgw_self()->pollset,
                OGS_POLLIN, _gtpv2_c_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }
    ogs_list_for_each(&pgw_self()->gtpc_list6, node) {
        ogs_socknode_set_poll(node, pgw_self()->pollset,
                OGS_POLLIN, _gtpv2_c_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }

    pgw_self()->gtpc_sock = gtp_local_sock_first(&pgw_self()->gtpc_list);
    pgw_self()->gtpc_sock6 = gtp_local_sock_first(&pgw_self()->gtpc_list6);
    pgw_self()->gtpc_addr = gtp_local_addr_first(&pgw_self()->gtpc_list);
    pgw_self()->gtpc_addr6 = gtp_local_addr_first(&pgw_self()->gtpc_list6);

    ogs_assert(pgw_self()->gtpc_addr || pgw_self()->gtpc_addr6);

    ogs_list_for_each(&pgw_self()->gtpu_list, node) {
        ogs_socknode_set_poll(node, pgw_self()->pollset,
                OGS_POLLIN, _gtpv1_u_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }
    ogs_list_for_each(&pgw_self()->gtpu_list6, node) {
        ogs_socknode_set_poll(node, pgw_self()->pollset,
                OGS_POLLIN, _gtpv1_u_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }

    pgw_self()->gtpu_sock = gtp_local_sock_first(&pgw_self()->gtpu_list);
    pgw_self()->gtpu_sock6 = gtp_local_sock_first(&pgw_self()->gtpu_list6);
    pgw_self()->gtpu_addr = gtp_local_addr_first(&pgw_self()->gtpu_list);
    pgw_self()->gtpu_addr6 = gtp_local_addr_first(&pgw_self()->gtpu_list6);

    ogs_assert(pgw_self()->gtpu_addr || pgw_self()->gtpu_addr6);

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
    for (dev = pgw_dev_first(); dev; dev = pgw_dev_next(dev)) {
        dev->fd = ogs_tun_open(dev->ifname, IFNAMSIZ, 0);
        if (dev->fd == INVALID_SOCKET) {
            ogs_error("tun_open(dev:%s) failed", dev->ifname);
            return OGS_ERROR;
        }

        dev->poll = ogs_pollset_add(pgw_self()->pollset,
                OGS_POLLIN, dev->fd, _gtpv1_tun_recv_cb, NULL);
        ogs_assert(dev->poll);
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
    for (subnet = pgw_subnet_first(); 
            subnet; subnet = pgw_subnet_next(subnet)) {
        ogs_assert(subnet->dev);
        rc = ogs_tun_set_ip(subnet->dev->ifname, &subnet->gw, &subnet->sub);
        if (rc != OGS_OK) {
            ogs_error("ogs_tun_set_ip(dev:%s) failed", subnet->dev->ifname);
            return OGS_ERROR;
        }
    }

    /* Link-Local Address for PGW_TUN */
    for (dev = pgw_dev_first(); dev; dev = pgw_dev_next(dev))
        dev->link_local_addr = ogs_link_local_addr_by_dev(dev->ifname);

    return OGS_OK;
}

void pgw_gtp_close()
{
    pgw_dev_t *dev = NULL;

    ogs_socknode_remove_all(&pgw_self()->gtpc_list);
    ogs_socknode_remove_all(&pgw_self()->gtpc_list6);
    ogs_socknode_remove_all(&pgw_self()->gtpu_list);
    ogs_socknode_remove_all(&pgw_self()->gtpu_list6);

    for (dev = pgw_dev_first(); dev; dev = pgw_dev_next(dev)) {
        ogs_pollset_remove(dev->poll);
        ogs_closesocket(dev->fd);
    }
}

static int pgw_gtp_handle_multicast(ogs_pkbuf_t *recvbuf)
{
    int rv;
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h =  NULL;

    ip_h = (struct ip *)recvbuf->data;
    if (ip_h->ip_v == 6) {
#if COMPILE_ERROR_IN_MAC_OS_X  /* Compiler error in Mac OS X platform */
        ip6_h = (struct ip6_hdr *)recvbuf->data;
        if (IN6_IS_ADDR_MULTICAST(&ip6_h->ip6_dst))
#else
        struct in6_addr ip6_dst;
        ip6_h = (struct ip6_hdr *)recvbuf->data;
        memcpy(&ip6_dst, &ip6_h->ip6_dst, sizeof(struct in6_addr));
        if (IN6_IS_ADDR_MULTICAST(&ip6_dst))
#endif
        {
            pgw_sess_t *sess = NULL;

            /* IPv6 Multicast */
            ogs_list_for_each(&pgw_self()->sess_list, sess) {
                if (sess->ipv6) {
                    /* PDN IPv6 is avaiable */
                    pgw_bearer_t *bearer = pgw_default_bearer_in_sess(sess);
                    ogs_assert(bearer);

                    rv = pgw_gtp_send_to_bearer(bearer, recvbuf);
                    ogs_assert(rv == OGS_OK);

                    return PGW_GTP_HANDLED;
                }
            }
        }
    }

    return OGS_OK;
}

static int pgw_gtp_handle_slaac(pgw_sess_t *sess, ogs_pkbuf_t *recvbuf)
{
    int rv;
    struct ip *ip_h = NULL;

    ogs_assert(sess);
    ogs_assert(recvbuf);
    ogs_assert(recvbuf->len);
    ip_h = (struct ip *)recvbuf->data;
    if (ip_h->ip_v == 6) {
        struct ip6_hdr *ip6_h = (struct ip6_hdr *)recvbuf->data;
        if (ip6_h->ip6_nxt == IPPROTO_ICMPV6) {
            struct icmp6_hdr *icmp_h =
                (struct icmp6_hdr *)(recvbuf->data + sizeof(struct ip6_hdr));
            if (icmp_h->icmp6_type == ND_ROUTER_SOLICIT) {
                ogs_debug("[PGW]      Router Solict");
                if (sess->ipv6) {
                    rv = pgw_gtp_send_router_advertisement(
                            sess, ip6_h->ip6_src.s6_addr);
                    ogs_assert(rv == OGS_OK);
                }
                return PGW_GTP_HANDLED;
            }
        }
    }

    return OGS_OK;
}

static int pgw_gtp_send_to_bearer(pgw_bearer_t *bearer, ogs_pkbuf_t *sendbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int rv;
    gtp_header_t *gtp_h = NULL;

    ogs_assert(bearer);
    ogs_assert(bearer->gnode);
    ogs_assert(bearer->gnode->sock);

    /* Add GTP-U header */
    ogs_assert(ogs_pkbuf_push(sendbuf, GTPV1U_HEADER_LEN));
    gtp_h = (gtp_header_t *)sendbuf->data;
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
    ogs_debug("[PGW] SEND GPU-U to SGW[%s] : TEID[0x%x]",
        OGS_ADDR(&bearer->gnode->conn, buf),
        bearer->sgw_s5u_teid);
    rv =  gtp_sendto(bearer->gnode, sendbuf);

    return rv;
}

static int pgw_gtp_send_router_advertisement(
        pgw_sess_t *sess, uint8_t *ip6_dst)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;

    pgw_bearer_t *bearer = NULL;
    pgw_ue_ip_t *ue_ip = NULL;
    pgw_subnet_t *subnet = NULL;
    pgw_dev_t *dev = NULL;

    ogs_ipsubnet_t src_ipsub;
    uint16_t plen = 0;
    uint8_t nxt = 0;
    uint8_t *p = NULL;
    struct ip6_hdr *ip6_h =  NULL;
    struct nd_router_advert *advert_h = NULL;
    struct nd_opt_prefix_info *prefix = NULL;

    ogs_assert(sess);
    bearer = pgw_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    ue_ip = sess->ipv6;
    ogs_assert(ue_ip);
    subnet = ue_ip->subnet;
    ogs_assert(subnet);
    dev = subnet->dev;
    ogs_assert(dev);

    pkbuf = ogs_pkbuf_alloc(NULL, GTPV1U_HEADER_LEN+200);
    ogs_pkbuf_reserve(pkbuf, GTPV1U_HEADER_LEN);
    ogs_pkbuf_put(pkbuf, 200);
    pkbuf->len = sizeof *ip6_h + sizeof *advert_h + sizeof *prefix;
    memset(pkbuf->data, 0, pkbuf->len);

    p = (uint8_t *)pkbuf->data;
    ip6_h = (struct ip6_hdr *)p;
    advert_h = (struct nd_router_advert *)((uint8_t *)ip6_h + sizeof *ip6_h);
    prefix = (struct nd_opt_prefix_info *)
        ((uint8_t*)advert_h + sizeof *advert_h);

    rv = ogs_ipsubnet(&src_ipsub, "fe80::1", NULL);
    ogs_assert(rv == OGS_OK);
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
    advert_h->nd_ra_cksum = in_cksum((uint16_t *)pkbuf->data, pkbuf->len);

    ip6_h->ip6_flow = htonl(0x60000001);
    ip6_h->ip6_plen = plen;
    ip6_h->ip6_nxt = nxt;  /* ICMPv6 */
    ip6_h->ip6_hlim = 0xff;
    memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
    memcpy(ip6_h->ip6_dst.s6_addr, ip6_dst, IPV6_LEN);
    
    rv = pgw_gtp_send_to_bearer(bearer, pkbuf);
    ogs_assert(rv == OGS_OK);

    ogs_debug("[PGW]      Router Advertisement");

    ogs_pkbuf_free(pkbuf);
    return rv;
}

uint16_t in_cksum(uint16_t *addr, int len)
{
    int nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;

    // Adding 16 bits sequentially in sum
    while (nleft > 1) {
        sum += *w;
        nleft -= 2;
        w++;
    }

    // If an odd byte is left
    if (nleft == 1) {
        *(uint8_t *) (&answer) = *(uint8_t *) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}
