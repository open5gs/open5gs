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

    ogs_debug("[UPF] PROTO:%d SRC:%08x %08x %08x %08x",
            proto, be32toh(src_addr[0]), be32toh(src_addr[1]),
            be32toh(src_addr[2]), be32toh(src_addr[3]));
    ogs_debug("[UPF] HLEN:%d  DST:%08x %08x %08x %08x",
            ip_hlen, be32toh(dst_addr[0]), be32toh(dst_addr[1]),
            be32toh(dst_addr[2]), be32toh(dst_addr[3]));


    if (sess) {
        ogs_pfcp_pdr_t *default_pdr = NULL;
        upf_sdf_filter_t *sdf_filter = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_far_t *far = NULL;

        if (ip_h && sess->ipv4)
            ogs_debug("[UPF] PAA IPv4:%s",
                    INET_NTOP(&sess->ipv4->addr, buf));
        if (ip6_h && sess->ipv6)
            ogs_debug("[UPF] PAA IPv6:%s",
                    INET6_NTOP(&sess->ipv6->addr, buf));

        /* Save the default PDR */
        default_pdr = ogs_pfcp_sess_default_pdr(&sess->pfcp);
        ogs_assert(default_pdr);

        /* Found */
        ogs_debug("[UPF] Found Session : Default PDR-ID[%d]", default_pdr->id);

        ogs_list_for_each(&sess->sdf_filter_list, sdf_filter) {
            int k;
            uint32_t src_mask[4];
            uint32_t dst_mask[4];
            ogs_ipfw_rule_t *rule = NULL;

            pdr = sdf_filter->pdr;
            ogs_assert(pdr);
            far = pdr->far;
            ogs_assert(far);
            rule = &sdf_filter->rule;
            ogs_assert(rule);

            /* Skip if PDR is default */
            if (pdr->id == default_pdr->id)
                continue;

            /* Check if PDR is Downlink */
            if (pdr->src_if != OGS_PFCP_INTERFACE_CORE)
                continue;

            /* Check if FAR is Downlink */
            if (far->dst_if != OGS_PFCP_INTERFACE_ACCESS)
                continue;

            /* Check if Create Bearer Response is received */
            if (far->outer_header_creation.teid == 0)
                continue;

            ogs_debug("PROTO:%d SRC:%d-%d DST:%d-%d",
                    rule->proto,
                    rule->port.local.low,
                    rule->port.local.high,
                    rule->port.remote.low,
                    rule->port.remote.high);
            ogs_debug("SRC:%08x %08x %08x %08x/%08x %08x %08x %08x",
                    be32toh(rule->ip.local.addr[0]),
                    be32toh(rule->ip.local.addr[1]),
                    be32toh(rule->ip.local.addr[2]),
                    be32toh(rule->ip.local.addr[3]),
                    be32toh(rule->ip.local.mask[0]),
                    be32toh(rule->ip.local.mask[1]),
                    be32toh(rule->ip.local.mask[2]),
                    be32toh(rule->ip.local.mask[3]));
            ogs_debug("DST:%08x %08x %08x %08x/%08x %08x %08x %08x",
                    be32toh(rule->ip.remote.addr[0]),
                    be32toh(rule->ip.remote.addr[1]),
                    be32toh(rule->ip.remote.addr[2]),
                    be32toh(rule->ip.remote.addr[3]),
                    be32toh(rule->ip.remote.mask[0]),
                    be32toh(rule->ip.remote.mask[1]),
                    be32toh(rule->ip.remote.mask[2]),
                    be32toh(rule->ip.remote.mask[3]));

            for (k = 0; k < 4; k++) {
                src_mask[k] = src_addr[k] & rule->ip.local.mask[k];
                dst_mask[k] = dst_addr[k] & rule->ip.remote.mask[k];
            }

            if (memcmp(src_mask, rule->ip.local.addr, addr_len) == 0 &&
                memcmp(dst_mask, rule->ip.remote.addr, addr_len) == 0) {
                /* Protocol match */
                if (rule->proto == 0) { /* IP */
                    /* No need to match port */
                    break;
                }

                if (rule->proto == proto) {
                    if (rule->proto == IPPROTO_TCP) {
                        struct tcphdr *tcph =
                            (struct tcphdr *)((char *)pkt->data + ip_hlen);

                        /* Source port */
                        if (rule->port.local.low &&
                              be16toh(tcph->th_sport) < rule->port.local.low) {
                            continue;
                        }

                        if (rule->port.local.high &&
                              be16toh(tcph->th_sport) > rule->port.local.high) {
                            continue;
                        }

                        /* Dst Port*/
                        if (rule->port.remote.low &&
                              be16toh(tcph->th_dport) < rule->port.remote.low) {
                            continue;
                        }

                        if (rule->port.remote.high &&
                              be16toh(tcph->th_dport) >
                                      rule->port.remote.high) {
                            continue;
                        }

                        /* Matched */
                        break;
                    } else if (rule->proto == IPPROTO_UDP) {
                        struct udphdr *udph =
                            (struct udphdr *)((char *)pkt->data + ip_hlen);

                        /* Source port */
                        if (rule->port.local.low &&
                              be16toh(udph->uh_sport) < rule->port.local.low) {
                            continue;
                        }

                        if (rule->port.local.high &&
                              be16toh(udph->uh_sport) > rule->port.local.high) {
                            continue;
                        }

                        /* Dst Port*/
                        if (rule->port.remote.low &&
                              be16toh(udph->uh_dport) < rule->port.remote.low) {
                            continue;
                        }

                        if (rule->port.remote.high &&
                              be16toh(udph->uh_dport) >
                                      rule->port.remote.high) {
                            continue;
                        }

                        /* Matched */
                        break;
                    } else {
                        /* No need to match port */
                        break;
                    }
                }
            }
        }

        if (sdf_filter) {
            ogs_debug("Found Dedicated PDR : PDR ID[%d]", pdr->id);
            return pdr;
        }

        return default_pdr;
    } else {
        ogs_debug("[UPF] No Session");
    }

    return NULL;
}
