/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "test-common.h"
#include "ipfw/ipfw2.h"

ogs_socknode_t *test_gtpu_server(int index, int family)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    if (index == 1) {
        if (family == AF_INET6)
            ogs_assert(OGS_OK ==
                ogs_copyaddrinfo(&addr, test_self()->gnb1_addr6));
        else
            ogs_assert(OGS_OK ==
                ogs_copyaddrinfo(&addr, test_self()->gnb1_addr));
    } else if (index == 2) {
        if (family == AF_INET6)
            ogs_assert(OGS_OK ==
                ogs_copyaddrinfo(&addr, test_self()->gnb2_addr6));
        else
            ogs_assert(OGS_OK ==
                ogs_copyaddrinfo(&addr, test_self()->gnb2_addr));
    } else
        ogs_assert_if_reached();

    node = ogs_socknode_new(addr);
    ogs_assert(node);

    sock = ogs_udp_server(node->addr, NULL);
    ogs_assert(sock);

    node->sock = sock;

    return node;
}

ogs_pkbuf_t *test_gtpu_read(ogs_socknode_t *node)
{
    int rc = 0;
    ogs_sockaddr_t from;
    ogs_pkbuf_t *recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(recvbuf);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    ogs_assert(node);
    ogs_assert(node->sock);

    while (1) {
        rc = ogs_recvfrom(
                node->sock->fd, recvbuf->data, recvbuf->len, 0, &from);
        if (rc <= 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_recvfrom() failed");
        }
        break;
    }
    recvbuf->len = rc;

    return recvbuf;
}

void test_gtpu_close(ogs_socknode_t *node)
{
    ogs_socknode_free(node);
}

#include "upf/upf-config.h"

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

void testgtpu_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    ogs_gtp2_header_t *gtp_h = NULL;
    struct ip6_hdr *ip6_h =  NULL;
    struct nd_router_advert *advert_h = NULL;
    struct nd_opt_prefix_info *prefix = NULL;

    uint32_t teid;
    uint8_t mask[OGS_IPV6_LEN];

    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    gtp_h = (ogs_gtp2_header_t *)pkbuf->data;
    ogs_assert(gtp_h);

    ogs_assert(gtp_h->version == OGS_GTP1_VERSION_1);
    ogs_assert(gtp_h->type == OGS_GTPU_MSGTYPE_GPDU);

    teid = be32toh(gtp_h->teid);

    if (test_ue->mme_ue_s1ap_id) {
        /* EPC */
        ogs_list_for_each(&test_ue->sess_list, sess) {
            ogs_list_for_each(&sess->bearer_list, bearer) {
                if (teid == bearer->enb_s1u_teid) goto found;
            }
            ogs_assert(bearer);
        }
        ogs_assert(sess);
    } else if (test_ue->amf_ue_ngap_id) {
        /* 5GC */
        ogs_list_for_each(&test_ue->sess_list, sess) {
            if (sess->gnb_n3_teid == teid) goto found;
        }
        ogs_assert(sess);
    } else {
        ogs_assert_if_reached();
    }

found:
    ogs_assert(sess);

    ip6_h = pkbuf->data + ogs_gtpu_header_len(pkbuf);
    ogs_assert(ip6_h);
    if (ip6_h->ip6_nxt == IPPROTO_ICMPV6) {
        struct nd_router_advert *advert_h = (struct nd_router_advert *)
            ((unsigned char*)ip6_h + sizeof(struct ip6_hdr));
        ogs_assert(advert_h);
        if (advert_h->nd_ra_hdr.icmp6_type == ND_ROUTER_ADVERT) {
            int i;
            struct nd_opt_prefix_info *prefix = (struct nd_opt_prefix_info *)
                ((unsigned char*)advert_h + sizeof(struct nd_router_advert));
            ogs_assert(prefix);
            n2mask(mask, prefix->nd_opt_pi_prefix_len);
            for (i = 0; i < OGS_IPV6_LEN; i++) {
                sess->ue_ip.addr6[i] |=
                    (mask[i] & prefix->nd_opt_pi_prefix.s6_addr[i]);
            }
        }
    }
    ogs_pkbuf_free(pkbuf);
}

int test_gtpu_send(
        ogs_socknode_t *node, test_bearer_t *bearer,
        ogs_gtp2_header_t *gtp_hdesc, ogs_gtp2_extension_header_t *ext_hdesc,
        ogs_pkbuf_t *pkbuf)
{
    ogs_gtp_node_t gnode;
    test_sess_t *sess = NULL;

    ogs_assert(gtp_hdesc);
    ogs_assert(ext_hdesc);
    ogs_assert(pkbuf);

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    memset(&gnode, 0, sizeof(ogs_gtp_node_t));

    gnode.addr.ogs_sin_port = htobe16(OGS_GTPV1_U_UDP_PORT);
    gnode.sock = node->sock;

    if (bearer->qfi) {
        if (sess->upf_n3_ip.ipv4) {
            gnode.addr.ogs_sa_family = AF_INET;
            gnode.addr.sin.sin_addr.s_addr = sess->upf_n3_ip.addr;
        } else if (sess->upf_n3_ip.ipv6) {
            gnode.addr.ogs_sa_family = AF_INET6;
            memcpy(gnode.addr.sin6.sin6_addr.s6_addr,
                    sess->upf_n3_ip.addr6, OGS_IPV6_LEN);
        } else {
            ogs_fatal("Not implemented");
            ogs_assert_if_reached();
        }

    } else if (bearer->ebi) {
        if (bearer->sgw_s1u_ip.ipv4) {
            gnode.addr.ogs_sa_family = AF_INET;
            gnode.addr.sin.sin_addr.s_addr = bearer->sgw_s1u_ip.addr;
        } else if (bearer->sgw_s1u_ip.ipv6) {
            gnode.addr.ogs_sa_family = AF_INET6;
            memcpy(gnode.addr.sin6.sin6_addr.s6_addr,
                    bearer->sgw_s1u_ip.addr6, OGS_IPV6_LEN);
        } else {
            ogs_fatal("Not implemented");
            ogs_assert_if_reached();
        }
    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    ext_hdesc->pdu_type =
        OGS_GTP2_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION;

    return ogs_gtp2_send_user_plane(&gnode, gtp_hdesc, ext_hdesc, pkbuf);
}

int test_gtpu_send_ping(
        ogs_socknode_t *node, test_bearer_t *bearer, const char *dst_ip)
{
    int rv;
    test_sess_t *sess = NULL;

    ogs_gtp2_header_t gtp_hdesc;
    ogs_gtp2_extension_header_t ext_hdesc;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_ipsubnet_t dst_ipsub;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    ogs_assert(dst_ip);

    rv = ogs_ipsubnet(&dst_ipsub, dst_ip, NULL);
    ogs_assert(rv == OGS_OK);

    pkbuf = ogs_pkbuf_alloc(
            NULL, 200 /* enough for ICMP; use smaller buffer */);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_GTPV1U_5GC_HEADER_LEN);
    ogs_pkbuf_put(pkbuf, 200-OGS_GTPV1U_5GC_HEADER_LEN);
    memset(pkbuf->data, 0, pkbuf->len);

    if (dst_ipsub.family == AF_INET) {
        struct ip *ip_h = NULL;
        struct icmp *icmp_h = NULL;

        ogs_pkbuf_trim(pkbuf, sizeof *ip_h + ICMP_MINLEN);

        ip_h = (struct ip *)pkbuf->data;
        icmp_h = (struct icmp *)((uint8_t *)ip_h + sizeof *ip_h);

        ip_h->ip_v = 4;
        ip_h->ip_hl = 5;
        ip_h->ip_tos = 0;
        ip_h->ip_id = rand();
        ip_h->ip_off = 0;
        ip_h->ip_ttl = 255;
        ip_h->ip_p = IPPROTO_ICMP;
        ip_h->ip_len = htobe16(sizeof *ip_h + ICMP_MINLEN);
        ip_h->ip_src.s_addr = sess->ue_ip.addr;
        ip_h->ip_dst.s_addr = dst_ipsub.sub[0];
        ip_h->ip_sum = ogs_in_cksum((uint16_t *)ip_h, sizeof *ip_h);

        icmp_h->icmp_type = 8;
        icmp_h->icmp_seq = rand();
        icmp_h->icmp_id = rand();
        icmp_h->icmp_cksum = ogs_in_cksum((uint16_t *)icmp_h, ICMP_MINLEN);

    } else if (dst_ipsub.family == AF_INET6) {
        struct ip6_hdr *ip6_h = NULL;
        struct icmp6_hdr *icmp6_h = NULL;
        uint16_t plen = 0;
        uint8_t nxt = 0;
        uint8_t *p = NULL;

        ogs_pkbuf_trim(pkbuf, sizeof *ip6_h + sizeof *icmp6_h);

        p = (uint8_t *)pkbuf->data;
        plen =  htobe16(sizeof *icmp6_h);
        nxt = IPPROTO_ICMPV6;

        ip6_h = (struct ip6_hdr *)p;
        icmp6_h = (struct icmp6_hdr *)((uint8_t *)ip6_h + sizeof *ip6_h);

        memcpy(p, sess->ue_ip.addr6, sizeof sess->ue_ip.addr6);
        p += sizeof sess->ue_ip.addr6;
        memcpy(p, dst_ipsub.sub, sizeof dst_ipsub.sub);
        p += sizeof dst_ipsub.sub;
        p += 2; memcpy(p, &plen, 2); p += 2;
        p += 3; *p = nxt; p += 1;

        icmp6_h->icmp6_type = ICMP6_ECHO_REQUEST;
        icmp6_h->icmp6_seq = rand();
        icmp6_h->icmp6_id = rand();

        icmp6_h->icmp6_cksum = ogs_in_cksum(
                (uint16_t *)ip6_h, sizeof *ip6_h + sizeof *icmp6_h);

        ip6_h->ip6_flow = htobe32(0x60000001);
        ip6_h->ip6_plen = plen;
        ip6_h->ip6_nxt = nxt;;
        ip6_h->ip6_hlim = 0xff;
        memcpy(ip6_h->ip6_src.s6_addr,
                sess->ue_ip.addr6, sizeof sess->ue_ip.addr6);
        memcpy(ip6_h->ip6_dst.s6_addr, dst_ipsub.sub, sizeof dst_ipsub.sub);
    } else {
        ogs_fatal("Invalid family[%d]", dst_ipsub.family);
        ogs_assert_if_reached();
    }

    memset(&gtp_hdesc, 0, sizeof(gtp_hdesc));
    memset(&ext_hdesc, 0, sizeof(ext_hdesc));

    gtp_hdesc.type = OGS_GTPU_MSGTYPE_GPDU;

    if (bearer->qfi) {
        gtp_hdesc.teid = sess->upf_n3_teid;
        ext_hdesc.qos_flow_identifier = bearer->qfi;

    } else if (bearer->ebi) {
        gtp_hdesc.teid = bearer->sgw_s1u_teid;

    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    return test_gtpu_send(node, bearer, &gtp_hdesc, &ext_hdesc, pkbuf);
}

int test_gtpu_send_slacc_rs(ogs_socknode_t *node, test_bearer_t *bearer)
{
    test_sess_t *sess = NULL;

    ogs_gtp2_header_t gtp_hdesc;
    ogs_gtp2_extension_header_t ext_hdesc;

    ogs_pkbuf_t *pkbuf = NULL;
    struct ip6_hdr *ip6_h = NULL;
    uint8_t *src_addr = NULL;

    const char *payload =
        "6000000000083aff fe80000000000000 0000000000000002"
        "ff02000000000000 0000000000000002 85007d3500000000";
    unsigned char tmp[OGS_MAX_SDU_LEN];
    int payload_len = 48;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    pkbuf = ogs_pkbuf_alloc(
            NULL, 200 /* enough for ICMP; use smaller buffer */);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_GTPV1U_5GC_HEADER_LEN);
    ogs_pkbuf_put(pkbuf, 200-OGS_GTPV1U_5GC_HEADER_LEN);
    memset(pkbuf->data, 0, pkbuf->len);

    OGS_HEX(payload, strlen(payload), tmp);
    memcpy(pkbuf->data, tmp, payload_len);

    ip6_h = pkbuf->data;
    ogs_assert(ip6_h);

    src_addr = (uint8_t *)ip6_h->ip6_src.s6_addr;
    ogs_assert(src_addr);

    memcpy(src_addr + 8, sess->ue_ip.addr6 + 8, 8);

    ogs_pkbuf_trim(pkbuf, payload_len);

    memset(&gtp_hdesc, 0, sizeof(gtp_hdesc));
    memset(&ext_hdesc, 0, sizeof(ext_hdesc));

    gtp_hdesc.type = OGS_GTPU_MSGTYPE_GPDU;
    gtp_hdesc.flags = OGS_GTPU_FLAGS_S;

    if (bearer->qfi) {
        gtp_hdesc.teid = sess->upf_n3_teid;

/*
 * Discussion #1506
 * Router Soliciation should include QFI in 5G Core
 */
        ext_hdesc.qos_flow_identifier = bearer->qfi;

    } else if (bearer->ebi) {
        gtp_hdesc.teid = bearer->sgw_s1u_teid;

    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    return test_gtpu_send(node, bearer, &gtp_hdesc, &ext_hdesc, pkbuf);
}

int test_gtpu_send_error_indication(
        ogs_socknode_t *node, test_bearer_t *bearer)
{
    test_sess_t *sess = NULL;
    uint32_t teid = 0;

    ogs_gtp2_header_t gtp_hdesc;
    ogs_gtp2_extension_header_t ext_hdesc;

    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    if (bearer->qfi) {
        /* 5GC */
        teid = sess->gnb_n3_teid;

    } else if (bearer->ebi) {
        /* EPC */
        teid = bearer->enb_s1u_teid;

    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    pkbuf = ogs_gtp2_build_error_indication(teid, node->addr);
    ogs_assert(pkbuf);

    memset(&gtp_hdesc, 0, sizeof(gtp_hdesc));
    memset(&ext_hdesc, 0, sizeof(ext_hdesc));

    gtp_hdesc.type = OGS_GTPU_MSGTYPE_ERR_IND;
    gtp_hdesc.flags = OGS_GTPU_FLAGS_S|OGS_GTPU_FLAGS_E;
    ext_hdesc.type = OGS_GTP2_EXTENSION_HEADER_TYPE_UDP_PORT;

    return test_gtpu_send(node, bearer, &gtp_hdesc, &ext_hdesc, pkbuf);
}

int test_gtpu_send_indirect_data_forwarding(
        ogs_socknode_t *node, test_bearer_t *bearer, ogs_pkbuf_t *pkbuf)
{
    test_sess_t *sess = NULL;

    ogs_gtp2_header_t gtp_hdesc;
    ogs_gtp2_extension_header_t ext_hdesc;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    ogs_assert(pkbuf);

    memset(&gtp_hdesc, 0, sizeof(gtp_hdesc));
    memset(&ext_hdesc, 0, sizeof(ext_hdesc));

    gtp_hdesc.type = OGS_GTPU_MSGTYPE_GPDU;

    if (bearer->qfi) {
        gtp_hdesc.teid = sess->handover.upf_dl_teid;
        ext_hdesc.qos_flow_identifier = bearer->qfi;

    } else if (bearer->ebi) {

        ogs_fatal("Not implmented EPC Indirect Tunnel");
        ogs_assert_if_reached();

    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    return test_gtpu_send(node, bearer, &gtp_hdesc, &ext_hdesc, pkbuf);
}
