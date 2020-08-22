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

ogs_socknode_t *test_gtpu_server(int index, int family)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    if (index == 1) {
        if (family == AF_INET6)
            ogs_copyaddrinfo(&addr, test_self()->gnb1_addr6);
        else
            ogs_copyaddrinfo(&addr, test_self()->gnb1_addr);
    } else if (index == 2) {
        if (family == AF_INET6)
            ogs_copyaddrinfo(&addr, test_self()->gnb2_addr6);
        else
            ogs_copyaddrinfo(&addr, test_self()->gnb2_addr);
    } else
        ogs_assert_if_reached();

    node = ogs_socknode_new(addr);
    ogs_assert(node);

    sock = ogs_udp_server(node);
    ogs_assert(sock);

    return node;
}

ogs_pkbuf_t *test_gtpu_read(ogs_socknode_t *node)
{
    int rc = 0;
    ogs_pkbuf_t *recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    ogs_assert(node);
    ogs_assert(node->sock);

    while (1) {
        rc = ogs_recv(node->sock->fd, recvbuf->data, recvbuf->len, 0);
        if (rc <= 0) {
            if (errno == EAGAIN) {
                continue;
            }
            break;
        } else {
            break;
        }
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

int test_gtpu_send_ping(
        ogs_socknode_t *node, test_bearer_t *bearer, const char *dst_ip)
{
    int rv;
    ssize_t sent;

    test_sess_t *sess = NULL;
    ogs_sockaddr_t upf;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_header_t *gtp_h = NULL;
    ogs_gtp_extension_header_t *ext_h = NULL;
    ogs_ipsubnet_t dst_ipsub;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    ogs_assert(dst_ip);

    rv = ogs_ipsubnet(&dst_ipsub, dst_ip, NULL);
    ogs_assert(rv == OGS_OK);

    memset(&upf, 0, sizeof(ogs_sockaddr_t));
    upf.ogs_sin_port = htobe16(OGS_GTPV1_U_UDP_PORT);

    pkbuf = ogs_pkbuf_alloc(
            NULL, 200 /* enough for ICMP; use smaller buffer */);
    ogs_pkbuf_put(pkbuf, 200);
    memset(pkbuf->data, 0, pkbuf->len);

    gtp_h = (ogs_gtp_header_t *)pkbuf->data;
    if (bearer->qfi) {
        /* 5G Core */
        gtp_h->flags = 0x34;
        gtp_h->teid = htobe32(sess->upf_n3_teid);

        if (sess->upf_n3_ip.ipv4) {
            upf.ogs_sa_family = AF_INET;
            upf.sin.sin_addr.s_addr = sess->upf_n3_ip.addr;
        } else {
            ogs_fatal("Not implemented");
            ogs_assert_if_reached();
        }

    } else if (bearer->ebi) {
        /* EPC */
        gtp_h->flags = 0x30;
        gtp_h->teid = htobe32(bearer->sgw_s1u_teid);

        if (bearer->sgw_s1u_ip.ipv4) {
            upf.ogs_sa_family = AF_INET;
            upf.sin.sin_addr.s_addr = bearer->sgw_s1u_ip.addr;
        } else {
            ogs_fatal("Not implemented");
            ogs_assert_if_reached();
        }
    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    gtp_h->type = OGS_GTPU_MSGTYPE_GPDU;

    if (bearer->qfi) {
        ext_h = (ogs_gtp_extension_header_t *)
            (pkbuf->data + OGS_GTPV1U_HEADER_LEN);
        ext_h->type = OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER;
        ext_h->len = 1;
        ext_h->pdu_type =
            OGS_GTP_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION;
        ext_h->qos_flow_identifier = bearer->qfi;
        ext_h->next_type =
            OGS_GTP_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS;
    }

    if (dst_ipsub.family == AF_INET) {
        struct ip *ip_h = NULL;
        struct icmp *icmp_h = NULL;

        if (bearer->qfi) {
            gtp_h->length = htobe16(
                    sizeof *ip_h + ICMP_MINLEN +
                    OGS_GTPV1U_EXTENSION_HEADER_LEN + ext_h->len * 4);

            ip_h = (struct ip *)(pkbuf->data +
                    OGS_GTPV1U_HEADER_LEN +
                    OGS_GTPV1U_EXTENSION_HEADER_LEN + ext_h->len * 4);
            icmp_h = (struct icmp *)((uint8_t *)ip_h + sizeof *ip_h);
        } else {
            gtp_h->length = htobe16(sizeof *ip_h + ICMP_MINLEN);

            ip_h = (struct ip *)(pkbuf->data + OGS_GTPV1U_HEADER_LEN);
            icmp_h = (struct icmp *)((uint8_t *)ip_h + sizeof *ip_h);
        }

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

        if (bearer->qfi) {
            gtp_h->length = htobe16(sizeof *ip6_h + sizeof *icmp6_h) +
                    OGS_GTPV1U_EXTENSION_HEADER_LEN + ext_h->len * 4;
            p = (uint8_t *)pkbuf->data + OGS_GTPV1U_HEADER_LEN +
                    OGS_GTPV1U_EXTENSION_HEADER_LEN + ext_h->len * 4;
        } else {
            gtp_h->length = htobe16(sizeof *ip6_h + sizeof *icmp6_h);
            p = (uint8_t *)pkbuf->data + OGS_GTPV1U_HEADER_LEN;
        }
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

        ip6_h->ip6_flow = htonl(0x60000001);
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

    ogs_assert(node);
    ogs_assert(node->sock);

    sent = ogs_sendto(node->sock->fd, pkbuf->data, pkbuf->len, 0, &upf);
    ogs_pkbuf_free(pkbuf);
    if (sent < 0 || sent != pkbuf->len)
        return OGS_ERROR;

    return OGS_OK;
}

int test_gtpu_send_slacc_rs(ogs_socknode_t *node, test_bearer_t *bearer)
{
    int rv;
    ssize_t sent;

    test_sess_t *sess = NULL;
    ogs_sockaddr_t upf;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_header_t *gtp_h = NULL;
    ogs_gtp_extension_header_t *ext_h = NULL;

    unsigned char *ip_h = NULL;

    const char *payload =
        "6000000000083aff fe80000000000000 0000000000000002"
        "ff02000000000000 0000000000000002 85007d3500000000";
    unsigned char tmp[OGS_MAX_SDU_LEN];

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    memset(&upf, 0, sizeof(ogs_sockaddr_t));
    upf.ogs_sin_port = htobe16(OGS_GTPV1_U_UDP_PORT);

    pkbuf = ogs_pkbuf_alloc(
            NULL, 200 /* enough for ICMP; use smaller buffer */);
    ogs_pkbuf_put(pkbuf, 200);
    memset(pkbuf->data, 0, pkbuf->len);

    gtp_h = (ogs_gtp_header_t *)pkbuf->data;
    if (bearer->qfi) {
        /* 5G Core */
        gtp_h->flags = 0x36;
        gtp_h->teid = htobe32(sess->upf_n3_teid);

        if (sess->upf_n3_ip.ipv4) {
            upf.ogs_sa_family = AF_INET;
            upf.sin.sin_addr.s_addr = sess->upf_n3_ip.addr;
        } else {
            ogs_fatal("Not implemented");
            ogs_assert_if_reached();
        }

    } else if (bearer->ebi) {
        /* EPC */
        gtp_h->flags = 0x32;
        gtp_h->teid = htobe32(bearer->sgw_s1u_teid);

        if (bearer->sgw_s1u_ip.ipv4) {
            upf.ogs_sa_family = AF_INET;
            upf.sin.sin_addr.s_addr = bearer->sgw_s1u_ip.addr;
        } else {
            ogs_fatal("Not implemented");
            ogs_assert_if_reached();
        }
    } else {
        ogs_fatal("No QFI[%d] and EBI[%d]", bearer->qfi, bearer->ebi);
        ogs_assert_if_reached();
    }

    gtp_h->type = OGS_GTPU_MSGTYPE_GPDU;

    if (bearer->qfi) {
        ext_h = (ogs_gtp_extension_header_t *)
            (pkbuf->data + OGS_GTPV1U_HEADER_LEN);
        ext_h->type = OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER;
        ext_h->len = 1;
        ext_h->pdu_type =
            OGS_GTP_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION;
        ext_h->qos_flow_identifier = bearer->qfi;
        ext_h->next_type =
            OGS_GTP_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS;
    }

    if (bearer->qfi) {
        gtp_h->length = htobe16(52 +
                OGS_GTPV1U_EXTENSION_HEADER_LEN + ext_h->len * 4);

        ip_h = (pkbuf->data +
                OGS_GTPV1U_HEADER_LEN + 4 +
                OGS_GTPV1U_EXTENSION_HEADER_LEN + ext_h->len * 4);
    } else {
        gtp_h->length = htobe16(52);

        ip_h = (pkbuf->data + OGS_GTPV1U_HEADER_LEN + 4);
    }

    ogs_assert(node);
    ogs_assert(node->sock);

    OGS_HEX(payload, strlen(payload), tmp);
    memcpy(ip_h, tmp, 48);

    sent = ogs_sendto(node->sock->fd, pkbuf->data, pkbuf->len, 0, &upf);
    ogs_pkbuf_free(pkbuf);
    if (sent < 0 || sent != pkbuf->len)
        return OGS_ERROR;

    return OGS_OK;
}
