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

#ifndef IF_NAMESIZE
#define	IF_NAMESIZE	16
#ifndef IFNAMSIZ
#define	IFNAMSIZ	IF_NAMESIZE
#endif
#endif

#include "ipfw2.h"
#include "objs/include_e/netinet/ip_fw.h"

#define MAX_NUM_OF_TOKEN 32
#define MAX_NUM_OF_RULE_BUFFER 1024

void compile_rule(char *av[], uint32_t *rbuf, int *rbufsize, void *tstate);

int ogs_ipfw_compile_rule(ogs_ipfw_rule_t *ipfw_rule, char *flow_description)
{
    char *token, *dir;
    char *saveptr;
    int i;

    char *av[MAX_NUM_OF_TOKEN];
	uint32_t rulebuf[MAX_NUM_OF_RULE_BUFFER];
	int rbufsize;
	struct ip_fw_rule *rule = (struct ip_fw_rule *)rulebuf;

	int x, l;
	ipfw_insn *cmd;

    char *description = NULL;

    ogs_assert(ipfw_rule);
    ogs_assert(flow_description);

	rbufsize = sizeof(rulebuf);
	memset(rulebuf, 0, rbufsize);

    av[0] = NULL;

    /* ACTION */
    description = ogs_strdup(flow_description);
    ogs_assert(description);

    token = ogs_strtok_r(description, " ", &saveptr);
    if (strcmp(token, "permit") != 0) {
        ogs_error("Not begins with reserved keyword : 'permit'");
        ogs_free(description);
        return OGS_ERROR;
    }
    av[1] = token;

    /* Save DIRECTION */
    dir = token = ogs_strtok_r(NULL, " ", &saveptr);
    if (strcmp(token, "out") != 0) {
        ogs_error("Not begins with reserved keyword : 'permit out'");
        ogs_free(description);
        return OGS_ERROR;
    }

    /* ADDR */
    i = 2;
    token = ogs_strtok_r(NULL, " ", &saveptr);
    while (token != NULL) {
        av[i++] = token;
        token = ogs_strtok_r(NULL, " ", &saveptr);
    }

    /* Add DIRECTION */
    av[i++] = dir;

    av[i] = NULL;

    /* "to assigned" --> "to any" */
    for (x = 2; av[x] != NULL; x++) {
        if (strcmp(av[x], "assigned") == 0 && strcmp(av[x-1], "to") == 0) {
            av[x] = "any";
            break;
        }
    }

	compile_rule(av, (uint32_t *)rule, &rbufsize, NULL);

    memset(ipfw_rule, 0, sizeof(ogs_ipfw_rule_t));
	for (l = rule->act_ofs, cmd = rule->cmd;
			l > 0 ; l -= F_LEN(cmd) , cmd += F_LEN(cmd)) {
        uint32_t *a = NULL;
        uint16_t *p = NULL;
		switch (cmd->opcode) {
        case O_PROTO:
            ipfw_rule->proto = cmd->arg1;
            break;
        case O_IP_SRC:
        case O_IP_SRC_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv4_src = 1;
            ipfw_rule->ip.src.addr[0] = a[0];
            if (cmd->opcode == O_IP_SRC_MASK)
                ipfw_rule->ip.src.mask[0] = a[1];
            else
                ipfw_rule->ip.src.mask[0] = 0xffffffff;
            break;
        case O_IP_DST:
        case O_IP_DST_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv4_dst = 1;
            ipfw_rule->ip.dst.addr[0] = a[0];
            if (cmd->opcode == O_IP_DST_MASK)
                ipfw_rule->ip.dst.mask[0] = a[1];
            else
                ipfw_rule->ip.dst.mask[0] = 0xffffffff;
            break;
        case O_IP6_SRC:
        case O_IP6_SRC_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv6_src = 1;
            memcpy(ipfw_rule->ip.src.addr, a, OGS_IPV6_LEN);
            if (cmd->opcode == O_IP6_SRC_MASK)
                memcpy(ipfw_rule->ip.src.mask, a+4, OGS_IPV6_LEN);
            else
                n2mask((struct in6_addr *)ipfw_rule->ip.src.mask, 128);
            break;
        case O_IP6_DST:
        case O_IP6_DST_MASK:
            a = ((ipfw_insn_u32 *)cmd)->d;
            ipfw_rule->ipv6_dst = 1;
            memcpy(ipfw_rule->ip.dst.addr, a, OGS_IPV6_LEN);
            if (cmd->opcode == O_IP6_DST_MASK)
                memcpy(ipfw_rule->ip.dst.mask, a+4, OGS_IPV6_LEN);
            else
                n2mask((struct in6_addr *)ipfw_rule->ip.dst.mask, 128);
            break;
        case O_IP_SRCPORT:
            p = ((ipfw_insn_u16 *)cmd)->ports;
            ipfw_rule->port.src.low = p[0];
            ipfw_rule->port.src.high = p[1];
            break;
        case O_IP_DSTPORT:
            p = ((ipfw_insn_u16 *)cmd)->ports;
            ipfw_rule->port.dst.low = p[0];
            ipfw_rule->port.dst.high = p[1];
            break;
        }
	}

    ogs_free(description);
    return OGS_OK;
}

char *ogs_ipfw_encode_flow_description(ogs_ipfw_rule_t *ipfw_rule)
{
    char flow_description[OGS_HUGE_LEN];
    char *p, *last;
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t sa;
    int prefixlen = 0;

    p = flow_description;
    last = flow_description + OGS_HUGE_LEN;

    ogs_assert(ipfw_rule);

    p = ogs_slprintf(p, last, "permit out");

    if (ipfw_rule->proto) {
        p = ogs_slprintf(p, last, " %d", ipfw_rule->proto);
    } else {
        p = ogs_slprintf(p, last, " ip");
    }

#define IPV4_BITLEN    (OGS_IPV4_LEN * 8)
#define IPV6_BITLEN    (OGS_IPV6_LEN * 8)

    p = ogs_slprintf(p, last, " from");
    memset(&sa, 0, sizeof(sa));

    if (ipfw_rule->ipv4_src) {
        sa.ogs_sa_family = AF_INET;
        memcpy(&sa.sin.sin_addr,
                ipfw_rule->ip.src.addr, sizeof(struct in_addr));

        OGS_ADDR(&sa, buf);
        prefixlen = contigmask(
                (uint8_t *)ipfw_rule->ip.src.mask, IPV4_BITLEN);

        if (prefixlen < 0) {
            ogs_error("Invalid mask[%x:%x:%x:%x]",
                    ipfw_rule->ip.src.mask[0],
                    ipfw_rule->ip.src.mask[1],
                    ipfw_rule->ip.src.mask[2],
                    ipfw_rule->ip.src.mask[3]);
            return NULL;
        } else if (prefixlen == 0) {
            p = ogs_slprintf(p, last, " any");
        } else if (prefixlen > 0 && prefixlen < IPV4_BITLEN) {
            p = ogs_slprintf(p, last, " %s/%d", buf, prefixlen);
        } else if (prefixlen == IPV4_BITLEN) {
            p = ogs_slprintf(p, last, " %s", buf);
        } else {
            ogs_fatal("Invalid prefixlen[%d]", prefixlen);
            ogs_assert_if_reached();
        }

    } else if (ipfw_rule->ipv6_src) {
        sa.ogs_sa_family = AF_INET6;
        memcpy(&sa.sin6.sin6_addr,
                ipfw_rule->ip.src.addr, sizeof(struct in6_addr));

        OGS_ADDR(&sa, buf);
        prefixlen = contigmask(
                (uint8_t *)ipfw_rule->ip.src.mask, IPV6_BITLEN);

        if (prefixlen < 0) {
            ogs_error("Invalid mask[%x:%x:%x:%x]",
                    ipfw_rule->ip.src.mask[0],
                    ipfw_rule->ip.src.mask[1],
                    ipfw_rule->ip.src.mask[2],
                    ipfw_rule->ip.src.mask[3]);
            return NULL;
        } else if (prefixlen == 0) {
            p = ogs_slprintf(p, last, " any");
        } else if (prefixlen > 0 && prefixlen < IPV6_BITLEN) {
            p = ogs_slprintf(p, last, " %s/%d", buf, prefixlen);
        } else if (prefixlen == IPV6_BITLEN) {
            p = ogs_slprintf(p, last, " %s", buf);
        } else {
            ogs_fatal("Invalid prefixlen[%d]", prefixlen);
            ogs_assert_if_reached();
        }
    } else
        p = ogs_slprintf(p, last, " any");

    if (ipfw_rule->port.src.low == ipfw_rule->port.src.high) {
        if (ipfw_rule->port.src.low == 0) {
            /* Nothing */
        } else {
            p = ogs_slprintf(p, last, " %d", ipfw_rule->port.src.low);
        }
    } else {
        p = ogs_slprintf(p, last, " %d-%d",
                ipfw_rule->port.src.low, ipfw_rule->port.src.high);
    }

    p = ogs_slprintf(p, last, " to");
    memset(&sa, 0, sizeof(sa));

    if (ipfw_rule->ipv4_dst) {
        sa.ogs_sa_family = AF_INET;
        memcpy(&sa.sin.sin_addr,
                ipfw_rule->ip.dst.addr, sizeof(struct in_addr));

        OGS_ADDR(&sa, buf);
        prefixlen = contigmask(
                (uint8_t *)ipfw_rule->ip.dst.mask, IPV4_BITLEN);

        if (prefixlen < 0) {
            ogs_error("Invalid mask[%x:%x:%x:%x]",
                    ipfw_rule->ip.dst.mask[0],
                    ipfw_rule->ip.dst.mask[1],
                    ipfw_rule->ip.dst.mask[2],
                    ipfw_rule->ip.dst.mask[3]);
            return NULL;
        } else if (prefixlen == 0) {
            p = ogs_slprintf(p, last, " assigned");
        } else if (prefixlen > 0 && prefixlen < IPV4_BITLEN) {
            p = ogs_slprintf(p, last, " %s/%d", buf, prefixlen);
        } else if (prefixlen == IPV4_BITLEN) {
            p = ogs_slprintf(p, last, " %s", buf);
        } else {
            ogs_fatal("Invalid prefixlen[%d]", prefixlen);
            ogs_assert_if_reached();
        }

    } else if (ipfw_rule->ipv6_dst) {
        sa.ogs_sa_family = AF_INET6;
        memcpy(&sa.sin6.sin6_addr,
                ipfw_rule->ip.dst.addr, sizeof(struct in6_addr));

        OGS_ADDR(&sa, buf);
        prefixlen = contigmask(
                (uint8_t *)ipfw_rule->ip.dst.mask, IPV6_BITLEN);

        if (prefixlen < 0) {
            ogs_error("Invalid mask[%x:%x:%x:%x]",
                    ipfw_rule->ip.dst.mask[0],
                    ipfw_rule->ip.dst.mask[1],
                    ipfw_rule->ip.dst.mask[2],
                    ipfw_rule->ip.dst.mask[3]);
            return NULL;
        } else if (prefixlen == 0) {
            p = ogs_slprintf(p, last, " assigned");
        } else if (prefixlen > 0 && prefixlen < IPV6_BITLEN) {
            p = ogs_slprintf(p, last, " %s/%d", buf, prefixlen);
        } else if (prefixlen == IPV6_BITLEN) {
            p = ogs_slprintf(p, last, " %s", buf);
        } else {
            ogs_fatal("Invalid prefixlen[%d]", prefixlen);
            ogs_assert_if_reached();
        }
    } else
        p = ogs_slprintf(p, last, " assigned");

    if (ipfw_rule->port.dst.low == ipfw_rule->port.dst.high) {
        if (ipfw_rule->port.dst.low == 0) {
            /* Nothing */
        } else {
            p = ogs_slprintf(p, last, " %d", ipfw_rule->port.dst.low);
        }
    } else {
        p = ogs_slprintf(p, last, " %d-%d",
                ipfw_rule->port.dst.low, ipfw_rule->port.dst.high);
    }

    return ogs_strdup(flow_description);
}

ogs_ipfw_rule_t *ogs_ipfw_copy_and_swap(
        ogs_ipfw_rule_t *dst, ogs_ipfw_rule_t *src)
{
    ogs_assert(src);
    ogs_assert(dst);
    ogs_assert(src != dst);

    memcpy(dst, src, sizeof(ogs_ipfw_rule_t));

    dst->ipv4_src = src->ipv4_dst;
    dst->ipv4_dst = src->ipv4_src;
    dst->ipv6_src = src->ipv6_dst;
    dst->ipv6_dst = src->ipv6_src;

    memcpy(&dst->ip.src, &src->ip.dst, sizeof(dst->ip.src));
    memcpy(&dst->ip.dst, &src->ip.src, sizeof(dst->ip.dst));
    memcpy(&dst->port.src, &src->port.dst, sizeof(dst->port.src));
    memcpy(&dst->port.dst, &src->port.src, sizeof(dst->port.dst));

    return dst;
}

void ogs_ipfw_rule_swap(ogs_ipfw_rule_t *ipfw_rule)
{
    ogs_ipfw_rule_t dst;

    ogs_assert(ipfw_rule);

    ogs_ipfw_copy_and_swap(&dst, ipfw_rule);
    memcpy(ipfw_rule, &dst, sizeof(ogs_ipfw_rule_t));
}

void ogs_pf_content_from_ipfw_rule(
        uint8_t direction, ogs_pf_content_t *content, ogs_ipfw_rule_t *rule,
        bool no_ipv4v6_local_addr_in_packet_filter)
{
    int j, len;

    ogs_assert(content);
    ogs_assert(rule);

    j = 0, len = 0;
    if (rule->proto) {
        content->component[j].type =
            OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        content->component[j].proto = rule->proto;
        j++; len += 2;
    }

    /*
     * As per 3GPP TS 24.008, following Packet filter component type identifier
     * are not supported on the LTE pre release-11 UEs:
     *
     * IPv4 local address type
     * IPv6 remote address/prefix length type
     * IPv6 local address/prefix length type
     *
     * And,
     * IPv6 remote address/prefix length type and
     * IPv6 local address/prefix length type shall be used when both MS and
     * Network support Local Address in TFTs.
     */

    if (rule->ipv4_src && (direction == OGS_FLOW_DOWNLINK_ONLY)) {
        content->component[j].type = OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        content->component[j].ipv4.addr = rule->ip.src.addr[0];
        content->component[j].ipv4.mask = rule->ip.src.mask[0];
        j++; len += 9;
    }

    if (rule->ipv4_src && (direction == OGS_FLOW_UPLINK_ONLY) &&
        !no_ipv4v6_local_addr_in_packet_filter) {
        content->component[j].type = OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        content->component[j].ipv4.addr = rule->ip.src.addr[0];
        content->component[j].ipv4.mask = rule->ip.src.mask[0];
        j++; len += 9;
    }

    if (rule->ipv4_dst && (direction == OGS_FLOW_DOWNLINK_ONLY) &&
        !no_ipv4v6_local_addr_in_packet_filter) {
        content->component[j].type = OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        content->component[j].ipv4.addr = rule->ip.dst.addr[0];
        content->component[j].ipv4.mask = rule->ip.dst.mask[0];
        j++; len += 9;
    }

    if (rule->ipv4_dst && (direction == OGS_FLOW_UPLINK_ONLY)) {
        content->component[j].type = OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        content->component[j].ipv4.addr = rule->ip.dst.addr[0];
        content->component[j].ipv4.mask = rule->ip.dst.mask[0];
        j++; len += 9;
    }

    if (rule->ipv6_src && (direction == OGS_FLOW_DOWNLINK_ONLY)) {
        if (no_ipv4v6_local_addr_in_packet_filter) {
            content->component[j].type =
                OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE;
            memcpy(content->component[j].ipv6_mask.addr,
                rule->ip.src.addr, sizeof rule->ip.src.addr);
            memcpy(content->component[j].ipv6_mask.mask,
                    rule->ip.src.mask, sizeof rule->ip.src.mask);
            j++; len += 33;
        } else {
            content->component[j].type =
                OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(content->component[j].ipv6.addr,
                rule->ip.src.addr, sizeof rule->ip.src.addr);
            content->component[j].ipv6.prefixlen =
                contigmask((uint8_t *)rule->ip.src.mask, 128);
            j++; len += 18;
        }
    }

    if (rule->ipv6_src && (direction == OGS_FLOW_UPLINK_ONLY)) {
        if (!no_ipv4v6_local_addr_in_packet_filter) {
            content->component[j].type =
                OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(content->component[j].ipv6.addr,
                    rule->ip.src.addr, sizeof rule->ip.src.addr);
            content->component[j].ipv6.prefixlen =
                contigmask((uint8_t *)rule->ip.src.mask, 128);
            j++; len += 18;
        }
    }

    if (rule->ipv6_dst && (direction == OGS_FLOW_DOWNLINK_ONLY)) {
        if (!no_ipv4v6_local_addr_in_packet_filter) {
            content->component[j].type =
                OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(content->component[j].ipv6.addr,
                rule->ip.dst.addr, sizeof rule->ip.dst.addr);
            content->component[j].ipv6.prefixlen =
                contigmask((uint8_t *)rule->ip.dst.mask, 128);
            j++; len += 18;
        }
    }

    if (rule->ipv6_dst && (direction == OGS_FLOW_UPLINK_ONLY)) {
        if (no_ipv4v6_local_addr_in_packet_filter) {
            content->component[j].type =
                    OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE;
            memcpy(content->component[j].ipv6_mask.addr,
                    rule->ip.dst.addr, sizeof rule->ip.dst.addr);
            memcpy(content->component[j].ipv6_mask.mask,
                    rule->ip.dst.mask, sizeof rule->ip.dst.mask);
            j++; len += 33;
        } else {
            content->component[j].type =
                OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(content->component[j].ipv6.addr,
                    rule->ip.dst.addr, sizeof rule->ip.dst.addr);
            content->component[j].ipv6.prefixlen =
                contigmask((uint8_t *)rule->ip.dst.mask, 128);
            j++; len += 18;
        }
    }

    if (rule->port.src.low) {
        if (rule->port.src.low == rule->port.src.high) {
            if (direction == OGS_FLOW_DOWNLINK_ONLY)
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
            else
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
            content->component[j].port.low = rule->port.src.low;
            j++; len += 3;
        } else {
            if (direction == OGS_FLOW_DOWNLINK_ONLY)
                content->component[j].type =
                    OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
            else
                content->component[j].type =
                    OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
            content->component[j].port.low = rule->port.src.low;
            content->component[j].port.high = rule->port.src.high;
            j++; len += 5;
        }
    }

    if (rule->port.dst.low) {
        if (rule->port.dst.low == rule->port.dst.high) {
            if (direction == OGS_FLOW_DOWNLINK_ONLY)
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
            else
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
            content->component[j].port.low = rule->port.dst.low;
            j++; len += 3;
        } else {
            if (direction == OGS_FLOW_DOWNLINK_ONLY)
                content->component[j].type =
                    OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
            else
                content->component[j].type =
                    OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
            content->component[j].port.low = rule->port.dst.low;
            content->component[j].port.high = rule->port.dst.high;
            j++; len += 5;
        }
    }

    content->num_of_component = j;
    content->length = len;
}

