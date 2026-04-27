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

#include "ogs-pfcp.h"

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IP6_H
#include <netinet/ip6.h>
#endif

#if HAVE_NETINET_UDP_H
#include <netinet/udp.h>
#endif

#if HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

static int decode_ipv6_header(
        struct ip6_hdr *ip6_h, size_t buflen,
        uint8_t *proto, uint16_t *hlen)
{
    int done = 0;
    uint8_t *p, *jp, *endp, *bufend;
    uint8_t nxt;          /* Next Header */

    ogs_assert(ip6_h);
    ogs_assert(proto);
    ogs_assert(hlen);

    if (buflen < sizeof(*ip6_h)) {
        ogs_error("IPv6 buffer too short [%zu < %zu]",
                buflen, sizeof(*ip6_h));
        return OGS_ERROR;
    }

    bufend = (uint8_t *)ip6_h + buflen;
    nxt = ip6_h->ip6_nxt;
    p = (uint8_t *)ip6_h + sizeof(*ip6_h);
    endp = p + be16toh(ip6_h->ip6_plen);

    /*
     * The packet's declared payload length may exceed the buffer we
     * actually received (truncated capture, malformed input, hostile
     * GTP-U); cap endp at the real buffer end so the header walk can
     * never read past it.
     */
    if (endp > bufend)
        endp = bufend;

    jp = p + sizeof(struct ip6_hbh);
    while (p == endp) { /* Jumbo Frame */
        uint32_t jp_len = 0;
        struct ip6_opt_jumbo *jumbo = NULL;

        if (nxt != 0) {
            ogs_error("Invalid IPv6 jumbo: plen=0 but NextHeader=%u", nxt);
            return OGS_ERROR;   /* Drop packet safely */
        }

        if (jp + sizeof(struct ip6_opt_jumbo) > bufend) {
            ogs_error("Truncated IPv6 jumbo option");
            return OGS_ERROR;
        }

        jumbo = (struct ip6_opt_jumbo *)jp;
        memcpy(&jp_len, jumbo->ip6oj_jumbo_len, sizeof(jp_len));
        jp_len = be32toh(jp_len);
        switch (jumbo->ip6oj_type) {
        case IP6OPT_JUMBO:
            endp = p + jp_len;
            if (endp > bufend)
                endp = bufend;
            break;
        case 0:
            jp++;
            break;
        default:
            if (jp_len > (uint32_t)(bufend - jp) ||
                    jp + sizeof(struct ip6_opt) + jp_len > bufend) {
                ogs_error("Truncated IPv6 jumbo PadN option");
                return OGS_ERROR;
            }
            jp += (sizeof(struct ip6_opt) + jp_len);
            break;
        }

        if (jp >= bufend) {
            ogs_error("IPv6 jumbo option walk reached buffer end");
            return OGS_ERROR;
        }
    }

    while (p < endp) {
        struct ip6_ext *ext = (struct ip6_ext *)p;
        size_t step = 0;

        if ((size_t)(endp - p) < sizeof(struct ip6_ext)) {
            ogs_error("Truncated IPv6 extension header");
            return OGS_ERROR;
        }

        switch (nxt) {
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
        case 135: /* mobility */
        case 139: /* host identity, experimental */
        case 140: /* shim6 */
        case 253: /* testing, experimental */
        case 254: /* testing, experimental */
            step = ((size_t)ext->ip6e_len << 3) + 8;
            break;
        case IPPROTO_FRAGMENT:
            step = sizeof(struct ip6_frag);
            break;
        case IPPROTO_AH:
            step = ((size_t)ext->ip6e_len + 2) << 2;
            break;
        default: /* Upper Layer */
            done = 1;
            break;

        }
        if (done)
            break;

        if (step == 0 || step > (size_t)(endp - p)) {
            ogs_error("Invalid IPv6 extension header step [%zu]", step);
            return OGS_ERROR;
        }

        nxt = ext->ip6e_nxt;
        p += step;
    }

    *proto = nxt;
    *hlen = p - (uint8_t *)ip6_h;

    return OGS_OK;
}

ogs_pfcp_rule_t *ogs_pfcp_pdr_rule_find_by_packet(
                    ogs_pfcp_pdr_t *pdr, ogs_pkbuf_t *pkbuf)
{
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h = NULL;
    uint32_t *src_addr = NULL;
    uint32_t *dst_addr = NULL;
    int addr_len = 0;
    uint8_t proto = 0;
    uint16_t ip_hlen = 0;

    ogs_pfcp_rule_t *rule = NULL;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);
    ogs_assert(pkbuf->data);

    ogs_list_for_each(&pdr->rule_list, rule) {
        int k;
        uint32_t src_mask[4];
        uint32_t dst_mask[4];
        ogs_ipfw_rule_t *ipfw = NULL;

        ipfw = &rule->ipfw;
        ogs_assert(ipfw);

        if (pkbuf->len < 1) {
            ogs_error("Empty packet while matching PDR");
            continue;
        }

        ip_h = (struct ip *)pkbuf->data;
        if (ip_h->ip_v == 4) {
            if (pkbuf->len < sizeof(struct ip)) {
                ogs_error("Truncated IPv4 packet [len:%d]", pkbuf->len);
                ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
                continue;
            }
            ip_h = (struct ip *)pkbuf->data;
            ip6_h = NULL;

            proto = ip_h->ip_p;
            ip_hlen = (ip_h->ip_hl)*4;

            if (ip_hlen < sizeof(struct ip) || ip_hlen > pkbuf->len) {
                ogs_error("Invalid IPv4 header length [ihl:%d len:%d]",
                        ip_hlen, pkbuf->len);
                ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
                continue;
            }

            src_addr = (void *)&ip_h->ip_src.s_addr;
            dst_addr = (void *)&ip_h->ip_dst.s_addr;
            addr_len = OGS_IPV4_LEN;
        } else if (ip_h->ip_v == 6) {
            if (pkbuf->len < sizeof(struct ip6_hdr)) {
                ogs_error("Truncated IPv6 packet [len:%d]", pkbuf->len);
                ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
                continue;
            }
            ip_h = NULL;
            ip6_h = (struct ip6_hdr *)pkbuf->data;

            if (OGS_OK != decode_ipv6_header(
                    ip6_h, pkbuf->len, &proto, &ip_hlen)) {
                /* Drop malformed IPv6 packet gracefully */
                ogs_error("Malformed IPv6 packet while matching PDR [plen:%d]",
                        pkbuf->len);
                ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
                continue;
            }

            if (ip_hlen > pkbuf->len) {
                ogs_error("IPv6 header walk overran buffer [hlen:%d len:%d]",
                        ip_hlen, pkbuf->len);
                continue;
            }

            src_addr = (void *)ip6_h->ip6_src.s6_addr;
            dst_addr = (void *)ip6_h->ip6_dst.s6_addr;
            addr_len = OGS_IPV6_LEN;
        } else {
            ogs_error("Invalid packet [IP version:%d, Packet Length:%d]",
                    ip_h->ip_v, pkbuf->len);
            ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
            continue;
        }

        ogs_trace("PROTO:%d SRC:%08x %08x %08x %08x",
                proto, be32toh(src_addr[0]), be32toh(src_addr[1]),
                be32toh(src_addr[2]), be32toh(src_addr[3]));
        ogs_trace("HLEN:%d  DST:%08x %08x %08x %08x",
                ip_hlen, be32toh(dst_addr[0]), be32toh(dst_addr[1]),
                be32toh(dst_addr[2]), be32toh(dst_addr[3]));

        ogs_trace("PROTO:%d SRC:%d-%d DST:%d-%d",
                ipfw->proto,
                ipfw->port.src.low,
                ipfw->port.src.high,
                ipfw->port.dst.low,
                ipfw->port.dst.high);
        ogs_trace("SRC:%08x %08x %08x %08x/%08x %08x %08x %08x",
                be32toh(ipfw->ip.src.addr[0]),
                be32toh(ipfw->ip.src.addr[1]),
                be32toh(ipfw->ip.src.addr[2]),
                be32toh(ipfw->ip.src.addr[3]),
                be32toh(ipfw->ip.src.mask[0]),
                be32toh(ipfw->ip.src.mask[1]),
                be32toh(ipfw->ip.src.mask[2]),
                be32toh(ipfw->ip.src.mask[3]));
        ogs_trace("DST:%08x %08x %08x %08x/%08x %08x %08x %08x",
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
                return rule;
            }

            if (ipfw->proto == proto) {
                if (ipfw->proto == IPPROTO_TCP) {
                    struct tcphdr *tcph;

                    if (pkbuf->len < ip_hlen + sizeof(struct tcphdr)) {
                        ogs_error("Truncated TCP header [hlen:%d len:%d]",
                                ip_hlen, pkbuf->len);
                        continue;
                    }
                    tcph = (struct tcphdr *)
                        ((char *)pkbuf->data + ip_hlen);

                    /* Source port */
                    if (ipfw->port.src.low &&
                          be16toh(tcph->th_sport) < ipfw->port.src.low) {
                        continue;
                    }

                    if (ipfw->port.src.high &&
                          be16toh(tcph->th_sport) > ipfw->port.src.high) {
                        continue;
                    }

                    /* Dst Port*/
                    if (ipfw->port.dst.low &&
                          be16toh(tcph->th_dport) < ipfw->port.dst.low) {
                        continue;
                    }

                    if (ipfw->port.dst.high &&
                          be16toh(tcph->th_dport) > ipfw->port.dst.high) {
                        continue;
                    }

                    /* Matched */
                    return rule;

                } else if (ipfw->proto == IPPROTO_UDP) {
                    struct udphdr *udph;

                    if (pkbuf->len < ip_hlen + sizeof(struct udphdr)) {
                        ogs_error("Truncated UDP header [hlen:%d len:%d]",
                                ip_hlen, pkbuf->len);
                        continue;
                    }
                    udph = (struct udphdr *)
                        ((char *)pkbuf->data + ip_hlen);

                    /* Source port */
                    if (ipfw->port.src.low &&
                          be16toh(udph->uh_sport) < ipfw->port.src.low) {
                        continue;
                    }

                    if (ipfw->port.src.high &&
                          be16toh(udph->uh_sport) > ipfw->port.src.high) {
                        continue;
                    }

                    /* Dst Port*/
                    if (ipfw->port.dst.low &&
                          be16toh(udph->uh_dport) < ipfw->port.dst.low) {
                        continue;
                    }

                    if (ipfw->port.dst.high &&
                          be16toh(udph->uh_dport) > ipfw->port.dst.high) {
                        continue;
                    }

                    /* Matched */
                    return rule;

                } else {

                    /* No need to match port */
                    return rule;

                }
            }
        }
    }

    return NULL;
}
