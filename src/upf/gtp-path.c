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

#include "context.h"

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

#include "event.h"
#include "gtp-path.h"
#include "rule-match.h"

#define UPF_GTP_HANDLED     1

static void upf_gtp_send_to_gnb(ogs_pfcp_far_t *far, ogs_pkbuf_t *sendbuf);
static int upf_gtp_handle_multicast(ogs_pkbuf_t *recvbuf);
static int upf_gtp_handle_slaac(upf_sess_t *sess, ogs_pkbuf_t *recvbuf);
static int upf_gtp_handle_pdr(ogs_pfcp_pdr_t *pdr, ogs_pkbuf_t *recvbuf);
static int upf_gtp_send_router_advertisement(
        upf_sess_t *sess, uint8_t *ip6_dst);

static void _gtpv1_tun_recv_cb(short when, ogs_socket_t fd, void *data)
{
    ogs_pkbuf_t *recvbuf = NULL;
    int n;
    ogs_pfcp_pdr_t *pdr = NULL;

    recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_reserve(recvbuf, OGS_GTPV1U_5GC_HEADER_LEN);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN-OGS_GTPV1U_5GC_HEADER_LEN);

    n = ogs_read(fd, recvbuf->data, recvbuf->len);
    if (n <= 0) {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno, "ogs_read() failed");
        ogs_pkbuf_free(recvbuf);
        return;
    }

    ogs_pkbuf_trim(recvbuf, n);

    /* Find the PDR by packet filter */
    pdr = upf_pdr_find_by_packet(recvbuf);
    if (pdr) {
        /* Unicast */
        upf_gtp_handle_pdr(pdr, recvbuf);
    } else {
        if (ogs_config()->parameter.multicast) {
            upf_gtp_handle_multicast(recvbuf);
        }
    }

    ogs_pkbuf_free(recvbuf);
}

static void _gtpv1_u_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int rv, len;
    ssize_t size;
    char buf[OGS_ADDRSTRLEN];

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;

    ogs_gtp_header_t *gtp_h = NULL;
    struct ip *ip_h = NULL;

    uint32_t teid;
    ogs_pfcp_pdr_t *pdr = NULL;
    upf_sess_t *sess = NULL;
    ogs_pfcp_subnet_t *subnet = NULL;
    ogs_pfcp_dev_t *dev = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recv() failed");
        goto cleanup;
    }

    ogs_pkbuf_trim(pkbuf, size);

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    gtp_h = (ogs_gtp_header_t *)pkbuf->data;
    if (gtp_h->version != OGS_GTP_VERSION_1) {
        ogs_error("[DROP] Invalid GTPU version [%d]", gtp_h->version);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }

    if (gtp_h->type == OGS_GTPU_MSGTYPE_ECHO_REQ) {
        ogs_pkbuf_t *echo_rsp;

        ogs_debug("[RECV] Echo Request from [%s]", OGS_ADDR(&from, buf));
        echo_rsp = ogs_gtp_handle_echo_req(pkbuf);
        if (echo_rsp) {
            ssize_t sent;

            /* Echo reply */
            ogs_debug("[SEND] Echo Response to [%s]", OGS_ADDR(&from, buf));

            sent = ogs_sendto(fd, echo_rsp->data, echo_rsp->len, 0, &from);
            if (sent < 0 || sent != echo_rsp->len) {
                ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                        "ogs_sendto() failed");
            }
            ogs_pkbuf_free(echo_rsp);
        }
        goto cleanup;
    }

    teid = be32toh(gtp_h->teid);

    if (gtp_h->type == OGS_GTPU_MSGTYPE_END_MARKER) {
        ogs_debug("[RECV] End Marker from [%s] : TEID[0x%x]",
                OGS_ADDR(&from, buf), teid);
        goto cleanup;
    }

    if (gtp_h->type == OGS_GTPU_MSGTYPE_ERR_IND) {
        ogs_error("[RECV] Error Indication from [%s]", OGS_ADDR(&from, buf));
        goto cleanup;
    }

    if (gtp_h->type != OGS_GTPU_MSGTYPE_GPDU) {
        ogs_error("[DROP] Invalid GTPU Type [%d]", gtp_h->type);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }

    ogs_debug("[RECV] GPU-U from [%s] : TEID[0x%x]",
            OGS_ADDR(&from, buf), teid);

    if (gtp_h->flags & OGS_GTPU_FLAGS_E) {
        /*
         * TS29.281
         * 5.2.1 General format of the GTP-U Extension Header
         * Figure 5.2.1-3: Definition of Extension Header Type
         *
         * Note 4 : For a GTP-PDU with several Extension Headers, the PDU
         *          Session Container should be the first Extension Header
         */
        ogs_gtp_extension_header_t *extension_header =
            (ogs_gtp_extension_header_t *)(pkbuf->data + OGS_GTPV1U_HEADER_LEN);
        ogs_assert(extension_header);
        if (extension_header->type ==
                OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER) {
            if (extension_header->pdu_type ==
                OGS_GTP_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION) {
                    ogs_debug("   QFI [0x%x]",
                            extension_header->qos_flow_identifier);
            }
        }
    }

    /* Remove GTP header and send packets to TUN interface */
    len = ogs_gtpu_header_len(pkbuf);
    if (len < 0) {
        ogs_error("[DROP] Cannot decode GTPU packet");
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }
    ogs_assert(ogs_pkbuf_pull(pkbuf, len));

    ip_h = (struct ip *)pkbuf->data;
    ogs_assert(ip_h);

    pdr = ogs_pfcp_pdr_find_by_teid(teid);
    if (!pdr) {
        ogs_warn("[DROP] Cannot find PDR : UPF-N3-TEID[0x%x]", teid);
        goto cleanup;
    }
    ogs_assert(pdr->sess);
    sess = UPF_SESS(pdr->sess);
    ogs_assert(sess);

    if (ip_h->ip_v == 4 && sess->ipv4)
        subnet = sess->ipv4->subnet;
    else if (ip_h->ip_v == 6 && sess->ipv6)
        subnet = sess->ipv6->subnet;

    if (!subnet) {
        ogs_error("[DROP] Cannot find subnet V:%d, IPv4:%p, IPv6:%p",
                ip_h->ip_v, sess->ipv4, sess->ipv6);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }

    /* Check IPv6 */
    if (ogs_config()->parameter.no_slaac == 0 && ip_h->ip_v == 6) {
        rv = upf_gtp_handle_slaac(sess, pkbuf);
        if (rv == UPF_GTP_HANDLED) {
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

int upf_gtp_open(void)
{
    ogs_pfcp_dev_t *dev = NULL;
    ogs_pfcp_subnet_t *subnet = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;
    int rc;

    ogs_list_for_each(&upf_self()->gtpu_list, node) {
        sock = ogs_gtp_server(node);
        ogs_assert(sock);

        if (sock->family == AF_INET)
            upf_self()->gtpu_sock = sock;
        else if (sock->family == AF_INET6)
            upf_self()->gtpu_sock6 = sock;

        node->poll = ogs_pollset_add(upf_self()->pollset,
                OGS_POLLIN, sock->fd, _gtpv1_u_recv_cb, sock);
    }

    ogs_assert(upf_self()->gtpu_sock || upf_self()->gtpu_sock6);

    /* NOTE : tun device can be created via following command.
     *
     * $ sudo ip tuntap add name ogstun mode tun
     *
     * Also, before running upf, assign the one IP from IP pool of UE 
     * to ogstun. The IP should not be assigned to UE
     *
     * $ sudo ifconfig ogstun 45.45.0.1/16 up
     *
     */

    /* Open Tun interface */
    ogs_list_for_each(&ogs_pfcp_self()->dev_list, dev) {
        dev->fd = ogs_tun_open(dev->ifname, OGS_MAX_IFNAME_LEN, 0);
        if (dev->fd == INVALID_SOCKET) {
            ogs_error("tun_open(dev:%s) failed", dev->ifname);
            return OGS_ERROR;
        }

        dev->poll = ogs_pollset_add(upf_self()->pollset,
                OGS_POLLIN, dev->fd, _gtpv1_tun_recv_cb, NULL);
        ogs_assert(dev->poll);
    }

    /* 
     * On Linux, it is possible to create a persistent tun/tap 
     * interface which will continue to exist even if open5gs quit, 
     * although this is normally not required. 
     * It can be useful to set up a tun/tap interface owned 
     * by a non-root user, so open5gs can be started without 
     * needing any root privileges at all.
     */

    /* Set P-to-P IP address with Netmask
     * Note that Linux will skip this configuration */
    ogs_list_for_each(&ogs_pfcp_self()->subnet_list, subnet) {
        ogs_assert(subnet->dev);
        rc = ogs_tun_set_ip(subnet->dev->ifname, &subnet->gw, &subnet->sub);
        if (rc != OGS_OK) {
            ogs_error("ogs_tun_set_ip(dev:%s) failed", subnet->dev->ifname);
            return OGS_ERROR;
        }
    }

    /* Link-Local Address for UPF_TUN */
    ogs_list_for_each(&ogs_pfcp_self()->dev_list, dev)
        dev->link_local_addr = ogs_link_local_addr_by_dev(dev->ifname);

    return OGS_OK;
}

void upf_gtp_close(void)
{
    ogs_pfcp_dev_t *dev = NULL;

    ogs_socknode_remove_all(&upf_self()->gtpu_list);

    ogs_list_for_each(&ogs_pfcp_self()->dev_list, dev) {
        if (dev->poll)
            ogs_pollset_remove(dev->poll);
        ogs_closesocket(dev->fd);
    }
}

void upf_gtp_send_buffered_packet(ogs_pfcp_far_t *far)
{
    int i;

    ogs_assert(far);

    if (far->gnode) {
        if (far->apply_action & OGS_PFCP_APPLY_ACTION_FORW) {
            for (i = 0; i < far->num_of_buffered_packet; i++) {
                upf_gtp_send_to_gnb(far, far->buffered_packet[i]);
            }
            far->num_of_buffered_packet = 0;
        }
    }
}

static void upf_gtp_send_to_gnb(ogs_pfcp_far_t *far, ogs_pkbuf_t *sendbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int rv;
    ogs_gtp_header_t *gtp_h = NULL;
    ogs_gtp_extension_header_t *ext_h = NULL;
    ogs_gtp_node_t *gnode = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(far);

    if (far->dst_if != OGS_PFCP_INTERFACE_ACCESS) {
        ogs_error("FAR is NOT Downlink");
        return;
    }

    gnode = far->gnode;
    ogs_assert(gnode);
    ogs_assert(gnode->sock);
    ogs_assert(sendbuf);

    pdr = far->pdr;
    ogs_assert(pdr);
    qer = pdr->qer;

    /* Add GTP-U header */
    if (qer && qer->qfi) {
        ogs_assert(ogs_pkbuf_push(sendbuf, OGS_GTPV1U_5GC_HEADER_LEN));
        gtp_h = (ogs_gtp_header_t *)sendbuf->data;
        /* Bits    8  7  6  5  4  3  2  1
         *        +--+--+--+--+--+--+--+--+
         *        |version |PT| 1| E| S|PN|
         *        +--+--+--+--+--+--+--+--+
         *         0  0  1   1  0  1  0  0
         */
        gtp_h->flags = 0x34;
        gtp_h->type = OGS_GTPU_MSGTYPE_GPDU;
        gtp_h->length = htobe16(sendbuf->len - OGS_GTPV1U_HEADER_LEN);
        gtp_h->teid = htobe32(far->outer_header_creation.teid);

        ext_h = (ogs_gtp_extension_header_t *)(
                sendbuf->data + OGS_GTPV1U_HEADER_LEN);
        ext_h->type = OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER;
        ext_h->len = 1;
        ext_h->pdu_type =
            OGS_GTP_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION;
        ext_h->qos_flow_identifier = qer->qfi;
        ext_h->next_type =
            OGS_GTP_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS;
    } else {
        ogs_assert(ogs_pkbuf_push(sendbuf, OGS_GTPV1U_HEADER_LEN));
        gtp_h = (ogs_gtp_header_t *)sendbuf->data;
        /* Bits    8  7  6  5  4  3  2  1
         *        +--+--+--+--+--+--+--+--+
         *        |version |PT| 1| E| S|PN|
         *        +--+--+--+--+--+--+--+--+
         *         0  0  1   1  0  0  0  0
         */
        gtp_h->flags = 0x30;
        gtp_h->type = OGS_GTPU_MSGTYPE_GPDU;
        gtp_h->length = htobe16(sendbuf->len - OGS_GTPV1U_HEADER_LEN);
        gtp_h->teid = htobe32(far->outer_header_creation.teid);
    }

    /* Send to gNB */
    ogs_debug("SEND GPU-U to gNB[%s] : TEID[0x%x]",
        OGS_ADDR(&gnode->addr, buf), far->outer_header_creation.teid);
    rv = ogs_gtp_sendto(gnode, sendbuf);
    if (rv != OGS_OK)
        ogs_error("ogs_gtp_sendto() failed");

    ogs_pkbuf_free(sendbuf);
}

static int upf_gtp_handle_pdr(ogs_pfcp_pdr_t *pdr, ogs_pkbuf_t *recvbuf)
{
    ogs_pfcp_far_t *far = NULL;
    ogs_pkbuf_t *sendbuf = NULL;

    ogs_assert(recvbuf);
    ogs_assert(pdr);

    if (pdr->src_if != OGS_PFCP_INTERFACE_CORE) {
        ogs_error("PDR is NOT Downlink");
        return OGS_ERROR;
    }

    far = pdr->far;
    ogs_assert(far);

    sendbuf = ogs_pkbuf_copy(recvbuf);
    ogs_assert(sendbuf);
    if (!far->gnode) {
        /* Default apply action : buffering */
        if (far->num_of_buffered_packet < MAX_NUM_OF_PACKET_BUFFER) {
            far->buffered_packet[far->num_of_buffered_packet++] = sendbuf;
            return UPF_GTP_HANDLED;
        }
    } else {
        if (far->apply_action & OGS_PFCP_APPLY_ACTION_FORW) {
            upf_gtp_send_to_gnb(far, sendbuf);
        } else if (far->apply_action & OGS_PFCP_APPLY_ACTION_BUFF) {
            if (far->num_of_buffered_packet < MAX_NUM_OF_PACKET_BUFFER) {
                far->buffered_packet[far->num_of_buffered_packet++] = sendbuf;
                return UPF_GTP_HANDLED;
            }
        }
        return UPF_GTP_HANDLED;
    }

    ogs_pkbuf_free(sendbuf);
    return OGS_OK;
}

static int upf_gtp_handle_multicast(ogs_pkbuf_t *recvbuf)
{
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
            upf_sess_t *sess = NULL;

            /* IPv6 Multicast */
            ogs_list_for_each(&upf_self()->sess_list, sess) {
                if (sess->ipv6) {
                    /* PDN IPv6 is avaiable */
                    ogs_pfcp_pdr_t *pdr = NULL;

                    pdr = ogs_pfcp_sess_default_pdr(&sess->pfcp);
                    ogs_assert(pdr);
                    upf_gtp_handle_pdr(pdr, recvbuf);

                    return UPF_GTP_HANDLED;
                }
            }
        }
    }

    return OGS_OK;
}

static int upf_gtp_handle_slaac(upf_sess_t *sess, ogs_pkbuf_t *recvbuf)
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
                ogs_debug("      Router Solict");
                if (sess->ipv6) {
                    rv = upf_gtp_send_router_advertisement(
                            sess, ip6_h->ip6_src.s6_addr);
                    ogs_assert(rv == OGS_OK);
                }
                return UPF_GTP_HANDLED;
            }
        }
    }

    return OGS_OK;
}

static int upf_gtp_send_router_advertisement(
        upf_sess_t *sess, uint8_t *ip6_dst)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_ue_ip_t *ue_ip = NULL;
    ogs_pfcp_subnet_t *subnet = NULL;
    ogs_pfcp_dev_t *dev = NULL;

    ogs_ipsubnet_t src_ipsub;
    uint16_t plen = 0;
    uint8_t nxt = 0;
    uint8_t *p = NULL;
    struct ip6_hdr *ip6_h =  NULL;
    struct nd_router_advert *advert_h = NULL;
    struct nd_opt_prefix_info *prefix = NULL;

    ogs_assert(sess);
    pdr = ogs_pfcp_sess_default_pdr(&sess->pfcp);
    ogs_assert(pdr);
    far = pdr->far;
    ogs_assert(far);
    ue_ip = sess->ipv6;
    ogs_assert(ue_ip);
    subnet = ue_ip->subnet;
    ogs_assert(subnet);
    dev = subnet->dev;
    ogs_assert(dev);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_GTPV1U_5GC_HEADER_LEN+200);
    ogs_pkbuf_reserve(pkbuf, OGS_GTPV1U_5GC_HEADER_LEN);
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
    advert_h->nd_ra_router_lifetime = htobe16(64800);  /* 64800s */
    advert_h->nd_ra_reachable = 0;
    advert_h->nd_ra_retransmit = 0;

    prefix->nd_opt_pi_type = ND_OPT_PREFIX_INFORMATION;
    prefix->nd_opt_pi_len = 4; /* 32bytes */
    prefix->nd_opt_pi_prefix_len = subnet->prefixlen;
    prefix->nd_opt_pi_flags_reserved =
        ND_OPT_PI_FLAG_ONLINK|ND_OPT_PI_FLAG_AUTO;
    prefix->nd_opt_pi_valid_time = htobe32(0xffffffff); /* Infinite */
    prefix->nd_opt_pi_preferred_time = htobe32(0xffffffff); /* Infinite */
    memcpy(prefix->nd_opt_pi_prefix.s6_addr,
            subnet->sub.sub, sizeof prefix->nd_opt_pi_prefix.s6_addr);

    /* For IPv6 Pseudo-Header */
    plen = htobe16(sizeof *advert_h + sizeof *prefix);
    nxt = IPPROTO_ICMPV6;

    memcpy(p, src_ipsub.sub, sizeof src_ipsub.sub);
    p += sizeof src_ipsub.sub;
    memcpy(p, ip6_dst, OGS_IPV6_LEN);
    p += OGS_IPV6_LEN;
    p += 2; memcpy(p, &plen, 2); p += 2;
    p += 3; *p = nxt; p += 1;
    advert_h->nd_ra_cksum = ogs_in_cksum((uint16_t *)pkbuf->data, pkbuf->len);

    ip6_h->ip6_flow = htobe32(0x60000001);
    ip6_h->ip6_plen = plen;
    ip6_h->ip6_nxt = nxt;  /* ICMPv6 */
    ip6_h->ip6_hlim = 0xff;
    memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
    memcpy(ip6_h->ip6_dst.s6_addr, ip6_dst, OGS_IPV6_LEN);
    
    upf_gtp_handle_pdr(pdr, pkbuf);

    ogs_debug("      Router Advertisement");

    ogs_pkbuf_free(pkbuf);
    return rv;
}
