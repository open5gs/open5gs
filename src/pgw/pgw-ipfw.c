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

#include "pgw-ipfw.h"

#include "ipfw/ipfw2.h"
#include "ipfw/objs/include_e/netinet/ip_fw.h"

#define MAX_NUM_OF_TOKEN 32
#define MAX_NUM_OF_RULE_BUFFER 1024

void compile_rule(char *av[], uint32_t *rbuf, int *rbufsize, void *tstate);

int pgw_compile_packet_filter(pgw_rule_t *pgw_rule, char *description)
{
    pgw_rule_t zero_rule;
    char *token, *dir;
    char *saveptr;
    int i = 2;

    char *av[MAX_NUM_OF_TOKEN];
	uint32_t rulebuf[MAX_NUM_OF_RULE_BUFFER];
	int rbufsize;
	struct ip_fw_rule *rule = (struct ip_fw_rule *)rulebuf;

	int l;
	ipfw_insn *cmd;

    ogs_assert(pgw_rule);

	rbufsize = sizeof(rulebuf);
	memset(rulebuf, 0, rbufsize);

    av[0] = NULL;

    /* ACTION */
    if (!description) { /* FIXME : OLD gcc generates uninitialized warning */
        ogs_assert_if_reached();
        return OGS_ERROR;
    }
    token = strtok_r(description, " ", &saveptr);
    if (strcmp(token, "permit") != 0) {
        ogs_error("Not begins with reserved keyword : 'permit'");
        return OGS_ERROR;
    }
    av[1] = token;

    /* Save DIRECTION */
    dir = token = strtok_r(NULL, " ", &saveptr);
    if (strcmp(token, "out") != 0) {
        ogs_error("Not begins with reserved keyword : 'permit out'");
        return OGS_ERROR;
    }

    /* ADDR */
    token = strtok_r(NULL, " ", &saveptr);
    while (token != NULL) {
        av[i++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }

    /* Add DIRECTION */
    av[i++] = dir;

    av[i] = NULL;

	compile_rule(av, (uint32_t *)rule, &rbufsize, NULL);

    memset(pgw_rule, 0, sizeof(pgw_rule_t));
	for (l = rule->act_ofs, cmd = rule->cmd;
			l > 0 ; l -= F_LEN(cmd) , cmd += F_LEN(cmd)) {
        uint32_t *a = NULL;
        uint16_t *p = NULL;
		switch (cmd->opcode) {
        case O_PROTO:
            pgw_rule->proto = cmd->arg1;
            break;
        case O_IP_SRC:
        case O_IP_SRC_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            pgw_rule->ipv4_local = 1;
            pgw_rule->ip.local.addr[0] = a[0];
            if (cmd->opcode == O_IP_SRC_MASK)
                pgw_rule->ip.local.mask[0] = a[1];
            else
                pgw_rule->ip.local.mask[0] = 0xffffffff;
            break;
        case O_IP_DST:
        case O_IP_DST_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            pgw_rule->ipv4_remote = 1;
            pgw_rule->ip.remote.addr[0] = a[0];
            if (cmd->opcode == O_IP_DST_MASK)
                pgw_rule->ip.remote.mask[0] = a[1];
            else
                pgw_rule->ip.remote.mask[0] = 0xffffffff;
            break;
        case O_IP6_SRC:
        case O_IP6_SRC_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            pgw_rule->ipv6_local = 1;
            memcpy(pgw_rule->ip.local.addr, a, OGS_IPV6_LEN);
            if (cmd->opcode == O_IP6_SRC_MASK)
                memcpy(pgw_rule->ip.local.mask, a+4, OGS_IPV6_LEN);
            else
                n2mask((struct in6_addr *)pgw_rule->ip.local.mask, 128);
            break;
        case O_IP6_DST:
        case O_IP6_DST_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            pgw_rule->ipv6_remote = 1;
            memcpy(pgw_rule->ip.remote.addr, a, OGS_IPV6_LEN);
            if (cmd->opcode == O_IP6_DST_MASK)
                memcpy(pgw_rule->ip.remote.mask, a+4, OGS_IPV6_LEN);
            else
                n2mask((struct in6_addr *)pgw_rule->ip.remote.mask, 128);
            break;
        case O_IP_SRCPORT:
            p = ((ipfw_insn_u16 *)cmd)->ports;
            pgw_rule->port.local.low = p[0];
            pgw_rule->port.local.high = p[1];
            break;
        case O_IP_DSTPORT:
            p = ((ipfw_insn_u16 *)cmd)->ports;
            pgw_rule->port.remote.low = p[0];
            pgw_rule->port.remote.high = p[1];
            break;
        }
	}

    memset(&zero_rule, 0, sizeof(pgw_rule_t));
    if (memcmp(pgw_rule, &zero_rule, sizeof(pgw_rule_t)) == 0) {
        ogs_error("Cannot find Flow-Description");
        return OGS_ERROR;
    }

    return OGS_OK;
}

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

pgw_bearer_t *pgw_bearer_find_by_packet(ogs_pkbuf_t *pkt)
{
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h =  NULL;
    uint32_t *src_addr = NULL;
    uint32_t *dst_addr = NULL;
    int addr_len = 0;
    uint8_t proto = 0;
    uint16_t ip_hlen = 0;
    char buf[OGS_ADDRSTRLEN];
    pgw_sess_t *sess = NULL;

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

        sess = pgw_sess_find_by_ipv4(dst_addr[0]);
    } else if (ip_h->ip_v == 6) {
        ip_h = NULL;
        ip6_h = (struct ip6_hdr *)pkt->data;

        decode_ipv6_header(ip6_h, &proto, &ip_hlen);

        src_addr = (uint32_t *)ip6_h->ip6_src.s6_addr;
        dst_addr = (uint32_t *)ip6_h->ip6_dst.s6_addr;
        addr_len = OGS_IPV6_LEN;

        sess = pgw_sess_find_by_ipv6(dst_addr);
    } else
        ogs_error("Invalid IP version = %d", ip_h->ip_v);

    ogs_debug("[PGW] PROTO:%d SRC:%08x %08x %08x %08x",
            proto, be32toh(src_addr[0]), be32toh(src_addr[1]),
            be32toh(src_addr[2]), be32toh(src_addr[3]));
    ogs_debug("[PGW] HLEN:%d  DST:%08x %08x %08x %08x",
            ip_hlen, be32toh(dst_addr[0]), be32toh(dst_addr[1]),
            be32toh(dst_addr[2]), be32toh(dst_addr[3]));

    if (sess) {
        pgw_bearer_t *default_bearer = NULL;
        pgw_bearer_t *bearer = NULL;

        if (ip_h && sess->ipv4)
            ogs_debug("[PGW] PAA IPv4:%s",
                    INET_NTOP(&sess->ipv4->addr, buf));
        if (ip6_h && sess->ipv6)
            ogs_debug("[PGW] PAA IPv6:%s",
                    INET6_NTOP(&sess->ipv6->addr, buf));

        /* Save the default bearer */
        default_bearer = pgw_default_bearer_in_sess(sess);
        ogs_assert(default_bearer);

        /* Found */
        ogs_debug("[PGW] Found Session : EBI[%d]", default_bearer->ebi);

        bearer = pgw_bearer_next(default_bearer);
        /* Find the bearer with matched */
        for (; bearer; bearer = pgw_bearer_next(bearer)) {
            pgw_pf_t *pf = NULL;

            if (bearer->ebi == 0) {
                /* Create Bearer Response is not received */
                continue;
            }

            for (pf = pgw_pf_first(bearer); pf; pf = pgw_pf_next(pf)) {
                int k;
                uint32_t src_mask[4];
                uint32_t dst_mask[4];

                ogs_debug("DIR:%d PROTO:%d SRC:%d-%d DST:%d-%d",
                        pf->direction, pf->rule.proto,
                        pf->rule.port.local.low,
                        pf->rule.port.local.high,
                        pf->rule.port.remote.low,
                        pf->rule.port.remote.high);
                ogs_debug("SRC:%08x %08x %08x %08x/%08x %08x %08x %08x",
                        be32toh(pf->rule.ip.local.addr[0]),
                        be32toh(pf->rule.ip.local.addr[1]),
                        be32toh(pf->rule.ip.local.addr[2]),
                        be32toh(pf->rule.ip.local.addr[3]),
                        be32toh(pf->rule.ip.local.mask[0]),
                        be32toh(pf->rule.ip.local.mask[1]),
                        be32toh(pf->rule.ip.local.mask[2]),
                        be32toh(pf->rule.ip.local.mask[3]));
                ogs_debug("DST:%08x %08x %08x %08x/%08x %08x %08x %08x",
                        be32toh(pf->rule.ip.remote.addr[0]),
                        be32toh(pf->rule.ip.remote.addr[1]),
                        be32toh(pf->rule.ip.remote.addr[2]),
                        be32toh(pf->rule.ip.remote.addr[3]),
                        be32toh(pf->rule.ip.remote.mask[0]),
                        be32toh(pf->rule.ip.remote.mask[1]),
                        be32toh(pf->rule.ip.remote.mask[2]),
                        be32toh(pf->rule.ip.remote.mask[3]));

                if (pf->direction != 1) {
                    continue;
                }

                for (k = 0; k < 4; k++) {
                    src_mask[k] = src_addr[k] & pf->rule.ip.local.mask[k];
                    dst_mask[k] = dst_addr[k] & pf->rule.ip.remote.mask[k];
                }

                if (memcmp(src_mask, pf->rule.ip.local.addr,
                            addr_len) == 0 &&
                    memcmp(dst_mask, pf->rule.ip.remote.addr,
                            addr_len) == 0) {
                    /* Protocol match */
                    if (pf->rule.proto == 0) { /* IP */
                        /* No need to match port */
                        break;
                    }

                    if (pf->rule.proto == proto) {
                        if (pf->rule.proto == IPPROTO_TCP) {
                            struct tcphdr *tcph = 
                                (struct tcphdr *)
                                ((char *)pkt->data + ip_hlen);

                            /* Source port */
                            if (pf->rule.port.local.low && 
                                  be16toh(tcph->th_sport) < 
                                          pf->rule.port.local.low) {
                                continue;
                            }

                            if (pf->rule.port.local.high && 
                                  be16toh(tcph->th_sport) > 
                                          pf->rule.port.local.high) {
                                continue;
                            }

                            /* Dst Port*/
                            if (pf->rule.port.remote.low && 
                                  be16toh(tcph->th_dport) < 
                                          pf->rule.port.remote.low) {
                                continue;
                            }

                            if (pf->rule.port.remote.high && 
                                  be16toh(tcph->th_dport) > 
                                          pf->rule.port.remote.high) {
                                continue;
                            }

                            /* Matched */
                            break;
                        } else if (pf->rule.proto == IPPROTO_UDP) {
                            struct udphdr *udph = 
                                (struct udphdr *)
                                ((char *)pkt->data + ip_hlen);

                            /* Source port */
                            if (pf->rule.port.local.low && 
                                  be16toh(udph->uh_sport) < 
                                          pf->rule.port.local.low) {
                                continue;
                            }

                            if (pf->rule.port.local.high && 
                                  be16toh(udph->uh_sport) > 
                                          pf->rule.port.local.high) {
                                continue;
                            }

                            /* Dst Port*/
                            if (pf->rule.port.remote.low && 
                                  be16toh(udph->uh_dport) < 
                                          pf->rule.port.remote.low) {
                                continue;
                            }

                            if (pf->rule.port.remote.high && 
                                  be16toh(udph->uh_dport) > 
                                          pf->rule.port.remote.high) {
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

            if (pf) {
                bearer = pf->bearer;
                ogs_debug("Found Dedicated Bearer : EBI[%d]", bearer->ebi);
                break;
            }
        }

        return (bearer ? bearer : default_bearer);
    } else {
        ogs_debug("[PGW] No Session");
    }

    return NULL;
}

