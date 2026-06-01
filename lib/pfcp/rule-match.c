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

static int decode_ipv6_header(struct ip6_hdr *ip6_h, size_t buflen,
        uint8_t *proto, uint16_t *hlen, size_t *plen)
{
    uint8_t *start, *p, *endp, *bufend;
    uint8_t nxt;          /* Next Header */

    ogs_assert(ip6_h);
    ogs_assert(proto);
    ogs_assert(hlen);
    ogs_assert(plen);

    if (buflen < sizeof(*ip6_h)) {
        ogs_error("Invalid IPv6 packet: too short [len:%zu]", buflen);
        return OGS_ERROR;
    }

    start = (uint8_t *)ip6_h;
    bufend = start + buflen;
    nxt = ip6_h->ip6_nxt;
    p = start + sizeof(*ip6_h);

    if (be16toh(ip6_h->ip6_plen) == 0) {
        uint8_t *jp, *optend;
        size_t ext_len;
        int jumbo_found = 0;
        struct ip6_ext *ext = NULL;

        if (nxt != IPPROTO_HOPOPTS) {
            ogs_error("Invalid IPv6 jumbo packet: missing hop-by-hop header "
                    "[nxt:%u len:%zu]", nxt, buflen);
            return OGS_ERROR;
        }

        if ((size_t)(bufend - p) < sizeof(struct ip6_ext)) {
            ogs_error("Invalid IPv6 jumbo packet: truncated hop-by-hop "
                    "header [remaining:%zu len:%zu]",
                    (size_t)(bufend - p), buflen);
            return OGS_ERROR;
        }

        ext = (struct ip6_ext *)p;
        ext_len = ((size_t)ext->ip6e_len << 3) + 8;
        if (ext_len > (size_t)(bufend - p)) {
            ogs_error("Invalid IPv6 jumbo packet: hop-by-hop header exceeds "
                    "packet [ext_len:%zu remaining:%zu len:%zu]",
                    ext_len, (size_t)(bufend - p), buflen);
            return OGS_ERROR;
        }

        jp = p + sizeof(struct ip6_hbh);
        optend = p + ext_len;

        while (jp < optend) {
            uint8_t opt_type = *jp;
            struct ip6_opt *opt = NULL;
            size_t opt_len;

            if (opt_type == 0) { /* Pad1 */
                jp++;
                continue;
            }

            if ((size_t)(optend - jp) < sizeof(struct ip6_opt)) {
                ogs_error("Invalid IPv6 jumbo packet: truncated option "
                        "[remaining:%zu len:%zu]",
                        (size_t)(optend - jp), buflen);
                return OGS_ERROR;
            }

            opt = (struct ip6_opt *)jp;
            opt_len = sizeof(struct ip6_opt) + opt->ip6o_len;
            if (opt_len > (size_t)(optend - jp)) {
                ogs_error("Invalid IPv6 jumbo packet: option exceeds "
                        "hop-by-hop header [type:%u opt_len:%zu "
                        "remaining:%zu len:%zu]",
                        opt->ip6o_type, opt_len, (size_t)(optend - jp),
                        buflen);
                return OGS_ERROR;
            }

            if (opt->ip6o_type == IP6OPT_JUMBO) {
                uint32_t jp_len = 0;
                struct ip6_opt_jumbo *jumbo = NULL;

                if (opt->ip6o_len != sizeof(jp_len)) {
                    ogs_error("Invalid IPv6 jumbo packet: invalid jumbo "
                            "option length [opt_len:%u len:%zu]",
                            opt->ip6o_len, buflen);
                    return OGS_ERROR;
                }

                jumbo = (struct ip6_opt_jumbo *)jp;
                memcpy(&jp_len, jumbo->ip6oj_jumbo_len, sizeof(jp_len));
                jp_len = be32toh(jp_len);

                if (jp_len <= 0xffff) {
                    ogs_error("Invalid IPv6 jumbo packet: jumbo payload "
                            "length is too small [jumbo_len:%u len:%zu]",
                            jp_len, buflen);
                    return OGS_ERROR;
                }

                if (jp_len > (size_t)(bufend - p)) {
                    ogs_error("Invalid IPv6 jumbo packet: jumbo payload "
                            "exceeds packet [jumbo_len:%u remaining:%zu "
                            "len:%zu]", jp_len, (size_t)(bufend - p),
                            buflen);
                    return OGS_ERROR;
                }

                endp = p + jp_len;
                jumbo_found = 1;
                break;
            }

            jp += opt_len;
        }

        if (!jumbo_found) {
            ogs_error("Invalid IPv6 jumbo packet: jumbo option not found "
                    "[len:%zu]", buflen);
            return OGS_ERROR;
        }
    } else {
        size_t payload_len = be16toh(ip6_h->ip6_plen);

        if (payload_len > (size_t)(bufend - p)) {
            ogs_error("Invalid IPv6 packet: payload length exceeds packet "
                    "[payload_len:%zu remaining:%zu len:%zu]",
                    payload_len, (size_t)(bufend - p), buflen);
            return OGS_ERROR;
        }

        endp = p + payload_len;
    }

    while (p < endp) {
        struct ip6_ext *ext = NULL;
        size_t ext_len;

        switch (nxt) {
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
        case 135: /* mobility */
        case 139: /* host identity, experimental */
        case 140: /* shim6 */
        case 253: /* testing, experimental */
        case 254: /* testing, experimental */
            if ((size_t)(endp - p) < sizeof(struct ip6_ext)) {
                ogs_error("Invalid IPv6 packet: truncated extension header "
                        "[nxt:%u remaining:%zu len:%zu]",
                        nxt, (size_t)(endp - p), buflen);
                return OGS_ERROR;
            }
            ext = (struct ip6_ext *)p;
            ext_len = ((size_t)ext->ip6e_len << 3) + 8;
            if (ext_len > (size_t)(endp - p)) {
                ogs_error("Invalid IPv6 packet: extension header exceeds "
                        "payload [nxt:%u ext_len:%zu remaining:%zu "
                        "len:%zu]", nxt, ext_len, (size_t)(endp - p),
                        buflen);
                return OGS_ERROR;
            }
            nxt = ext->ip6e_nxt;
            p += ext_len;
            break;
        case IPPROTO_FRAGMENT:
            if ((size_t)(endp - p) < sizeof(struct ip6_frag)) {
                ogs_error("Invalid IPv6 packet: truncated fragment header "
                        "[remaining:%zu len:%zu]",
                        (size_t)(endp - p), buflen);
                return OGS_ERROR;
            }
            ext = (struct ip6_ext *)p;
            nxt = ext->ip6e_nxt;
            p += sizeof(struct ip6_frag);
            break;
        case IPPROTO_AH:
            if ((size_t)(endp - p) < sizeof(struct ip6_ext)) {
                ogs_error("Invalid IPv6 packet: truncated AH header "
                        "[remaining:%zu len:%zu]",
                        (size_t)(endp - p), buflen);
                return OGS_ERROR;
            }
            ext = (struct ip6_ext *)p;
            ext_len = ((size_t)ext->ip6e_len + 2) << 2;
            if (ext_len > (size_t)(endp - p)) {
                ogs_error("Invalid IPv6 packet: AH header exceeds payload "
                        "[ext_len:%zu remaining:%zu len:%zu]",
                        ext_len, (size_t)(endp - p), buflen);
                return OGS_ERROR;
            }
            nxt = ext->ip6e_nxt;
            p += ext_len;
            break;
        default: /* Upper Layer */
            goto done;
        }
    }

done:
    if ((size_t)(p - start) > 0xffff) {
        ogs_error("Invalid IPv6 packet: header length too large "
                "[hlen:%zu len:%zu]", (size_t)(p - start), buflen);
        return OGS_ERROR;
    }

    *proto = nxt;
    *hlen = p - start;
    *plen = endp - start;

    return OGS_OK;
}

ogs_pfcp_rule_t *ogs_pfcp_pdr_rule_find_by_packet(
                    ogs_pfcp_pdr_t *pdr, ogs_pkbuf_t *pkbuf)
{
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h = NULL;
    uint32_t src_addr[4] = {0, };
    uint32_t dst_addr[4] = {0, };
    int addr_len = 0;
    uint8_t proto = 0;
    uint16_t ip_hlen = 0;
    size_t pkt_len = 0;
    uint8_t ip_v;

    ogs_pfcp_rule_t *rule = NULL;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);
    ogs_assert(pkbuf->data);

    pkt_len = pkbuf->len;
    ip_v = (*(uint8_t *)pkbuf->data) >> 4;

    if (ip_v == 4) {
        size_t ip_len;

        if (pkt_len < sizeof(struct ip)) {
            ogs_error("Invalid IPv4 packet: too short [len:%zu]", pkt_len);
            return NULL;
        }

        ip_h = (struct ip *)pkbuf->data;

        ip_hlen = ip_h->ip_hl * 4;
        ip_len = be16toh(ip_h->ip_len);

        if (ip_hlen < sizeof(struct ip) || ip_hlen > ip_len) {
            ogs_error("Invalid IPv4 packet: invalid header length "
                    "[hlen:%u ip_len:%zu len:%zu]",
                    ip_hlen, ip_len, pkt_len);
            return NULL;
        }
        if (ip_len > pkt_len) {
            ogs_error("Invalid IPv4 packet: total length exceeds packet "
                    "[ip_len:%zu len:%zu]", ip_len, pkt_len);
            return NULL;
        }

        pkt_len = ip_len;
        proto = ip_h->ip_p;

        src_addr[0] = ip_h->ip_src.s_addr;
        dst_addr[0] = ip_h->ip_dst.s_addr;
        addr_len = OGS_IPV4_LEN;
    } else if (ip_v == 6) {
        if (pkt_len < sizeof(struct ip6_hdr)) {
            ogs_error("Invalid IPv6 packet: too short [len:%zu]", pkt_len);
            return NULL;
        }

        ip6_h = (struct ip6_hdr *)pkbuf->data;

        if (OGS_OK != decode_ipv6_header(ip6_h, pkt_len,
                    &proto, &ip_hlen, &pkt_len))
            return NULL;

        memcpy(src_addr, ip6_h->ip6_src.s6_addr, OGS_IPV6_LEN);
        memcpy(dst_addr, ip6_h->ip6_dst.s6_addr, OGS_IPV6_LEN);
        addr_len = OGS_IPV6_LEN;
    } else {
        ogs_error("Invalid IP packet: unsupported version "
                "[version:%u len:%zu]", ip_v, pkt_len);
        return NULL;
    }

    if (proto == IPPROTO_TCP) {
        if ((size_t)ip_hlen > pkt_len ||
            pkt_len - ip_hlen < sizeof(struct tcphdr)) {
            ogs_error("Invalid TCP packet: truncated header "
                    "[hlen:%u len:%zu]", ip_hlen, pkt_len);
            return NULL;
        }
    } else if (proto == IPPROTO_UDP) {
        if ((size_t)ip_hlen > pkt_len ||
            pkt_len - ip_hlen < sizeof(struct udphdr)) {
            ogs_error("Invalid UDP packet: truncated header "
                    "[hlen:%u len:%zu]", ip_hlen, pkt_len);
            return NULL;
        }
    }

    ogs_list_for_each(&pdr->rule_list, rule) {
        int k;
        uint32_t src_mask[4];
        uint32_t dst_mask[4];
        ogs_ipfw_rule_t *ipfw = NULL;

        ipfw = &rule->ipfw;
        ogs_assert(ipfw);

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
                    struct tcphdr *tcph =
                        (struct tcphdr *)((char *)pkbuf->data + ip_hlen);

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
                    struct udphdr *udph =
                        (struct udphdr *)((char *)pkbuf->data + ip_hlen);

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
