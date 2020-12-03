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

#define _DEFAULT_SOURCE 1
#define _BSD_SOURCE     1

#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "rule-match.h"

static int decode_ipv6_header(
        struct ip6_hdr *ip6_h, uint8_t *proto, uint16_t *hlen)
{
    int done = 0;
    uint8_t *p, *jp, *endp;
    uint8_t nxt;          /* Next Header */

    ogs_assert(ip6_h);
    ogs_assert(proto);
    ogs_assert(hlen);

    nxt = ip6_h->ip6_nxt;
    p = (uint8_t *)ip6_h + sizeof(*ip6_h);
    endp = p + be16toh(ip6_h->ip6_plen);

    jp = p + sizeof(struct ip6_hbh);
    while (p == endp) { /* Jumbo Frame */
        uint32_t jp_len = 0;
        struct ip6_opt_jumbo *jumbo = NULL;

        ogs_assert(nxt == 0);

        jumbo = (struct ip6_opt_jumbo *)jp;
        memcpy(&jp_len, jumbo->ip6oj_jumbo_len, sizeof(jp_len));
        jp_len = be32toh(jp_len);
        switch (jumbo->ip6oj_type) {
        case IP6OPT_JUMBO:
            endp = p + jp_len;
            break;
        case 0:
            jp++;
            break;
        default:
            jp += (sizeof(struct ip6_opt) + jp_len);
            break;
        }
    }

    while (p < endp) {
        struct ip6_ext *ext = (struct ip6_ext *)p;
        switch (nxt) {
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
        case 135: /* mobility */
        case 139: /* host identity, experimental */
        case 140: /* shim6 */
        case 253: /* testing, experimental */
        case 254: /* testing, experimental */
            p += ((ext->ip6e_len << 3) + 8);
            break;
        case IPPROTO_FRAGMENT:
            p += sizeof(struct ip6_frag);
            break;
        case IPPROTO_AH:
            p += ((ext->ip6e_len + 2) << 2);
            break;
        default: /* Upper Layer */
            done = 1;
            break;     

        }
        if (done)
            break;

        nxt = ext->ip6e_nxt;
    }

    *proto = nxt;
    *hlen = p - (uint8_t *)ip6_h;

    return OGS_OK;
}

ogs_pfcp_pdr_t *upf_pdr_find_by_packet(ogs_pkbuf_t *pkt)
{
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h =  NULL;
    uint32_t *src_addr = NULL;
    uint32_t *dst_addr = NULL;
    int addr_len = 0;
    uint8_t proto = 0;
    uint16_t ip_hlen = 0;
    char buf[OGS_ADDRSTRLEN];
    upf_sess_t *sess = NULL;

    ogs_assert(pkt);
    ogs_assert(pkt->len);

    ip_h = (struct ip *)pkt->data;
    if (ip_h->ip_v == 4) {
        ip_h = (struct ip *)pkt->data;
        ip6_h = NULL;

        proto = ip_h->ip_p;
        ip_hlen = (ip_h->ip_hl)*4;

        src_addr = &ip_h->ip_src.s_addr;
        dst_addr = &ip_h->ip_dst.s_addr;
        addr_len = OGS_IPV4_LEN;

        sess = upf_sess_find_by_ipv4(dst_addr[0]);
    } else if (ip_h->ip_v == 6) {
        ip_h = NULL;
        ip6_h = (struct ip6_hdr *)pkt->data;

        decode_ipv6_header(ip6_h, &proto, &ip_hlen);

        src_addr = (uint32_t *)ip6_h->ip6_src.s6_addr;
        dst_addr = (uint32_t *)ip6_h->ip6_dst.s6_addr;
        addr_len = OGS_IPV6_LEN;

        sess = upf_sess_find_by_ipv6(dst_addr);
    } else
        ogs_error("Invalid IP version = %d", ip_h->ip_v);

    ogs_debug("PROTO:%d SRC:%08x %08x %08x %08x",
            proto, be32toh(src_addr[0]), be32toh(src_addr[1]),
            be32toh(src_addr[2]), be32toh(src_addr[3]));
    ogs_debug("HLEN:%d  DST:%08x %08x %08x %08x",
            ip_hlen, be32toh(dst_addr[0]), be32toh(dst_addr[1]),
            be32toh(dst_addr[2]), be32toh(dst_addr[3]));


    if (sess) {
        ogs_pfcp_pdr_t *fallback_pdr = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_far_t *far = NULL;
        ogs_pfcp_rule_t *rule = NULL;

        if (ip_h && sess->ipv4)
            ogs_debug("PAA IPv4:%s", OGS_INET_NTOP(&sess->ipv4->addr, buf));
        if (ip6_h && sess->ipv6)
            ogs_debug("PAA IPv6:%s", OGS_INET6_NTOP(&sess->ipv6->addr, buf));

        /* Found */
        ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
            far = pdr->far;
            ogs_assert(far);

            /* Check if PDR is Downlink */
            if (pdr->src_if != OGS_PFCP_INTERFACE_CORE)
                continue;

            /* Save the Fallback PDR : Lowest precedence downlink PDR */
            fallback_pdr = pdr;

            /* Check if FAR is Downlink */
            if (far->dst_if != OGS_PFCP_INTERFACE_ACCESS)
                continue;

            /* Check if Outer header creation */
            if (far->outer_header_creation.teid == 0)
                continue;

            ogs_list_for_each(&pdr->rule_list, rule) {
                int k;
                uint32_t src_mask[4];
                uint32_t dst_mask[4];
                ogs_ipfw_rule_t *ipfw = NULL;

                ipfw = &rule->ipfw;
                ogs_assert(ipfw);

                ogs_debug("PROTO:%d SRC:%d-%d DST:%d-%d",
                        ipfw->proto,
                        ipfw->port.src.low,
                        ipfw->port.src.high,
                        ipfw->port.dst.low,
                        ipfw->port.dst.high);
                ogs_debug("SRC:%08x %08x %08x %08x/%08x %08x %08x %08x",
                        be32toh(ipfw->ip.src.addr[0]),
                        be32toh(ipfw->ip.src.addr[1]),
                        be32toh(ipfw->ip.src.addr[2]),
                        be32toh(ipfw->ip.src.addr[3]),
                        be32toh(ipfw->ip.src.mask[0]),
                        be32toh(ipfw->ip.src.mask[1]),
                        be32toh(ipfw->ip.src.mask[2]),
                        be32toh(ipfw->ip.src.mask[3]));
                ogs_debug("DST:%08x %08x %08x %08x/%08x %08x %08x %08x",
                        be32toh(ipfw->ip.dst.addr[0]),
                        be32toh(ipfw->ip.dst.addr[1]),
                        be32toh(ipfw->ip.dst.addr[2]),
                        be32toh(ipfw->ip.dst.addr[3]),
                        be32toh(ipfw->ip.dst.mask[0]),
                        be32toh(ipfw->ip.dst.mask[1]),
                        be32toh(ipfw->ip.dst.mask[2]),
                        be32toh(ipfw->ip.dst.mask[3]));

                for (k = 0; k < 4; k++) {
                    src_mask[k] = src_addr[k] & ipfw->ip.src.mask[k];
                    dst_mask[k] = dst_addr[k] & ipfw->ip.dst.mask[k];
                }

                if (memcmp(src_mask, ipfw->ip.src.addr, addr_len) == 0 &&
                    memcmp(dst_mask, ipfw->ip.dst.addr, addr_len) == 0) {
                    /* Protocol match */
                    if (ipfw->proto == 0) { /* IP */
                        /* No need to match port */
                        goto found;
                    }

                    if (ipfw->proto == proto) {
                        if (ipfw->proto == IPPROTO_TCP) {
                            struct tcphdr *tcph =
                                (struct tcphdr *)((char *)pkt->data + ip_hlen);

                            /* Source port */
                            if (ipfw->port.src.low &&
                                  be16toh(tcph->th_sport) <
                                      ipfw->port.src.low) {
                                continue;
                            }

                            if (ipfw->port.src.high &&
                                  be16toh(tcph->th_sport) >
                                      ipfw->port.src.high) {
                                continue;
                            }

                            /* Dst Port*/
                            if (ipfw->port.dst.low &&
                                  be16toh(tcph->th_dport) <
                                      ipfw->port.dst.low) {
                                continue;
                            }

                            if (ipfw->port.dst.high &&
                                  be16toh(tcph->th_dport) >
                                          ipfw->port.dst.high) {
                                continue;
                            }

                            /* Matched */
                            goto found;
                        } else if (ipfw->proto == IPPROTO_UDP) {
                            struct udphdr *udph =
                                (struct udphdr *)((char *)pkt->data + ip_hlen);

                            /* Source port */
                            if (ipfw->port.src.low &&
                                  be16toh(udph->uh_sport) <
                                      ipfw->port.src.low) {
                                continue;
                            }

                            if (ipfw->port.src.high &&
                                  be16toh(udph->uh_sport) >
                                      ipfw->port.src.high) {
                                continue;
                            }

                            /* Dst Port*/
                            if (ipfw->port.dst.low &&
                                  be16toh(udph->uh_dport) <
                                      ipfw->port.dst.low) {
                                continue;
                            }

                            if (ipfw->port.dst.high &&
                                  be16toh(udph->uh_dport) >
                                      ipfw->port.dst.high) {
                                continue;
                            }

                            /* Matched */
                            goto found;
                        } else {
                            /* No need to match port */
                            goto found;
                        }
                    }
                }
            }
        }

found:
        if (rule) {
            ogs_debug("Found Dedicated PDR : PDR ID[%d]", pdr->id);
            return pdr;
        }

        if (fallback_pdr) {
            ogs_debug("Found Session : Fallback PDR-ID[%d]", fallback_pdr->id);
            return fallback_pdr;
        }

        ogs_error("No PDR in Session");
    } else {
        ogs_debug("No Session");
    }

    return NULL;
}
