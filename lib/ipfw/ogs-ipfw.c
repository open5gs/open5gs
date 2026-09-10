/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

/* Set by the errx() override in ipfw2.c when a rule fails to parse */
extern int ogs_ipfw_parse_error;

/* Protocol name table shared with add_proto0() in ipfw2.c */
extern int ipfw_proto_by_name(const char *name);

/*
 * 3GPP TS 29.212 limits the IPFilterRule carried in a Flow-Description to
 *
 *     permit out <proto> from <src> [<ports>] to <dst> [<ports>]
 *
 * compile_rule() below is the full ipfw(8) command line parser.  It also
 * accepts hostnames, address sets, lookup tables and the entire option
 * keyword space, and -- because errx() no longer exits -- it keeps parsing
 * after it has already rejected a token, dereferencing arguments that are
 * not there.  A Flow-Description from a PCF, PCRF or AF can therefore crash
 * an SMF, UPF or SGW-U:
 *
 *     permit out ip                              -> SEGV in add_src()
 *     permit out ip from 1.2.3.4                 -> SEGV in add_dst()
 *     permit out ip from 1.2.3.4 to 1.2.3.4 uid  -> SEGV in compile_rule()
 *     permit out ip from table(1) to assigned    -> SEGV in pack_table()
 *     permit out 58 from ff02::2/129 to assigned -> exit() from ipv6.c
 *
 * Check the token stream against the grammar we accept before handing it
 * over.  The structure, the addresses and the ports are settled here; an
 * unrecognised protocol name is the one thing still left to the parser,
 * which rejects it through ogs_ipfw_parse_error below.
 */
static bool ipfw_parse_number(const char *s, int min, int max, int *value)
{
    int v = 0;

    if (!s || !*s)
        return false;

    for (; *s; s++) {
        if (*s < '0' || *s > '9')
            return false;
        v = v * 10 + (*s - '0');
        if (v > max)
            return false;
    }

    if (v < min)
        return false;

    if (value)
        *value = v;

    return true;
}

static bool ipfw_is_number(const char *s, int min, int max)
{
    return ipfw_parse_number(s, min, max, NULL);
}

/*
 * "ip", a protocol number, or a name that resolves to one.
 *
 * The token is rewritten in place as a number so that compile_rule() only
 * ever sees "ip" or a decimal, because every other spelling it understands
 * ends up meaning "no protocol restriction" once the rule reaches
 * ogs_ipfw_rule_t:
 *
 *   - add_proto0() stores the number in a u_char without a range check, so
 *     "256" is truncated to 0 and a UDP filter turns into "ip"; "999"
 *     becomes protocol 231.
 *   - add_proto() matches "all" with _substrcmp(), which succeeds on a
 *     prefix, so "a" and "al" are accepted as "all" -> protocol 0.
 *   - "ip4", "ipv4", "ip6" and "ipv6" are family selectors emitted as O_IP4
 *     or O_IP6, opcodes the loop below does not read, so the restriction is
 *     dropped and protocol 0 is what remains.
 *   - a name whose number is 0 -- "hopopt" -- passes add_proto0() unchanged
 *     and lands as protocol 0 as well.
 *
 * Resolve the name through ipfw_proto_by_name(), the table add_proto0()
 * itself uses, rather than keeping a second one here.
 */
static int ipfw_check_proto(char **token, char *buf, int size, int *protop)
{
    const char *s = *token;
    const char *p;
    int proto;

    if (!s || !*s)
        return OGS_ERROR;

    /* the only name carried through: no protocol restriction */
    if (strcmp(s, "ip") == 0) {
        *protop = IPPROTO_IP;
        return OGS_OK;
    }

    if (*s >= '0' && *s <= '9') {
        if (!ipfw_parse_number(s, 1, 255, &proto))
            return OGS_ERROR;
    } else {
        for (p = s; *p; p++) {
            if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') ||
                (*p >= '0' && *p <= '9') || *p == '-')
                continue;
            return OGS_ERROR;
        }

        /*
         * "all" and the family selectors would resolve to something, or to
         * nothing, that no longer means what was written.  Refuse them by
         * name so that the operator is told to use "ip" instead.
         */
        if (strcmp(s, "all") == 0 || strcmp(s, "ip4") == 0 ||
            strcmp(s, "ipv4") == 0 || strcmp(s, "ip6") == 0 ||
            strcmp(s, "ipv6") == 0)
            return OGS_ERROR;

        proto = ipfw_proto_by_name(s);
        if (proto < 1 || proto > 255)
            return OGS_ERROR;
    }

    ogs_snprintf(buf, size, "%d", proto);
    *token = buf;
    *protop = proto;

    return OGS_OK;
}

/*
 * "any", "assigned", or a literal IPv4/IPv6 address with optional prefix.
 *
 * Returns the address family, AF_UNSPEC for the two keywords -- which carry
 * no family of their own -- or -1 when the token is not an address at all.
 */
static int ipfw_addr_family(const char *s)
{
    char buf[OGS_ADDRSTRLEN];
    struct in_addr addr4;
    struct in6_addr addr6;
    const char *slash;
    size_t len;

    if (!s || !*s)
        return -1;

    if (strcmp(s, "any") == 0 || strcmp(s, "assigned") == 0)
        return AF_UNSPEC;

    slash = strchr(s, '/');
    len = slash ? (size_t)(slash - s) : strlen(s);
    if (len == 0 || len >= sizeof(buf))
        return -1;
    memcpy(buf, s, len);
    buf[len] = '\0';

    if (inet_pton(AF_INET, buf, &addr4) == 1)
        return (!slash || ipfw_is_number(slash + 1, 0, 32)) ? AF_INET : -1;
    if (inet_pton(AF_INET6, buf, &addr6) == 1)
        return (!slash || ipfw_is_number(slash + 1, 0, 128)) ? AF_INET6 : -1;

    return -1;
}

/*
 * A single port or a "low-high" range, 1..65535.
 *
 * Port 0 must not be accepted: ogs_ipfw_rule_t uses 0 to mean "no port
 * condition", so "from 1.2.3.4 0" would silently become "from 1.2.3.4" and
 * widen the filter to every port.  fill_newports() stores the two ends of a
 * range without comparing them, so a reversed range has to be caught here
 * as well.
 */
static bool ipfw_is_ports(const char *s)
{
    char buf[8];
    const char *dash;
    size_t len;
    int low, high;

    if (!s || !*s)
        return false;

    dash = strchr(s, '-');
    if (!dash)
        return ipfw_is_number(s, 1, 65535);

    len = dash - s;
    if (len == 0 || len >= sizeof(buf))
        return false;
    memcpy(buf, s, len);
    buf[len] = '\0';

    if (!ipfw_parse_number(buf, 1, 65535, &low))
        return false;
    if (!ipfw_parse_number(dash + 1, 1, 65535, &high))
        return false;

    return low <= high;
}

/*
 * av[0] is unused and av[1] is "permit", so the tokens to check are
 * av[2] .. av[last-1].  The direction, which the caller has moved to the
 * end of the array, is not part of this grammar.
 */
static int ipfw_check_tokens(
        char *av[], int last, char *proto_buf, int proto_buf_size,
        char *flow_description)
{
    int x = 2;
    int proto = IPPROTO_IP;
    int src_family, dst_family;

#define REJECT(reason) \
    do { \
        ogs_error("Invalid Flow-Description [%s] : %s", \
                flow_description, reason); \
        return OGS_ERROR; \
    } while (0)
#define REQUIRE(cond, reason) \
    do { if (!(cond)) REJECT(reason); } while (0)

    REQUIRE(x < last, "no protocol");
    REQUIRE(ipfw_check_proto(
                &av[x], proto_buf, proto_buf_size, &proto) == OGS_OK,
            "bad protocol");
    x++;

    REQUIRE(x < last && strcmp(av[x], "from") == 0, "missing 'from'");
    x++;

    REQUIRE(x < last, "no source address");
/*
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338
 *
 * A Flow-Description is always written in the downlink orientation, so the
 * UE address -- "assigned" -- can only appear after "to".  An uplink flow
 * arrives as RX "permit in from <UE> to <REMOTE>", which flow_rx_to_gx()
 * has already rewritten into that form before it reaches here.
 */
    if (strcmp(av[x], "assigned") == 0)
        REJECT("'assigned' is the UE address and is only valid after 'to'");
    src_family = ipfw_addr_family(av[x]);
    REQUIRE(src_family >= 0, "bad source address");
    x++;

    if (x < last && strcmp(av[x], "to") != 0) {
        REQUIRE(ipfw_is_ports(av[x]), "bad source port");
        x++;
    }

    REQUIRE(x < last && strcmp(av[x], "to") == 0, "missing 'to'");
    x++;

    REQUIRE(x < last, "no destination address");
    dst_family = ipfw_addr_family(av[x]);
    REQUIRE(dst_family >= 0, "bad destination address");
    x++;

    if (x < last) {
        REQUIRE(ipfw_is_ports(av[x]), "bad destination port");
        x++;
    }

    REQUIRE(x == last, "trailing token");

    /* one packet cannot carry both families */
    REQUIRE(src_family == AF_UNSPEC || dst_family == AF_UNSPEC ||
            src_family == dst_family,
            "source and destination address family differ");

    /*
     * Protocol 41 is IPv6 encapsulation, and the outer addresses of a
     * 6in4 tunnel are legitimately IPv4.  add_src() and add_dst(), however,
     * treat the number as if it selected the address family: once the
     * protocol is IPPROTO_IPV6 they send every address through
     * add_srcip6() and add_dstip6(), whatever it looks like.  ipv6.c calls
     * the real errx() from err.h -- it never sees the override in
     * ipfw2.c -- so an IPv4 literal exits the daemon rather than failing
     * the rule.  Refuse the combination until the parser can carry it.
     */
    if (proto == IPPROTO_IPV6)
        REQUIRE(src_family != AF_INET && dst_family != AF_INET,
                "IPv4 literal with protocol 41 is unsupported");

#undef REQUIRE
#undef REJECT

    return OGS_OK;
}


int ogs_ipfw_compile_rule(ogs_ipfw_rule_t *ipfw_rule, char *flow_description)
{
    char *token, *dir;
    char *saveptr;
    int i;

    char *av[MAX_NUM_OF_TOKEN];
    char proto_buf[4];
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
    if (!token || strcmp(token, "permit") != 0) {
        ogs_error("Not begins with reserved keyword : 'permit'");
        ogs_free(description);
        return OGS_ERROR;
    }
    av[1] = token;

    /* Save DIRECTION */
    dir = token = ogs_strtok_r(NULL, " ", &saveptr);
    if (!token || strcmp(token, "out") != 0) {
        ogs_error("Not begins with reserved keyword : 'permit out'");
        ogs_free(description);
        return OGS_ERROR;
    }

    /* ADDR */
    i = 2;
    token = ogs_strtok_r(NULL, " ", &saveptr);
    while ((token != NULL) && (i < (MAX_NUM_OF_TOKEN-2))) {
        av[i++] = token;
        token = ogs_strtok_r(NULL, " ", &saveptr);
    }

    /* Add DIRECTION */
    av[i++] = dir;

    av[i] = NULL;

    if (ipfw_check_tokens(av, i-1, proto_buf, sizeof(proto_buf),
                flow_description) != OGS_OK) {
        ogs_free(description);
        return OGS_ERROR;
    }

    /* "to assigned" --> "to any" */
    for (x = 2; av[x] != NULL; x++) {
        if (strcmp(av[x], "assigned") == 0 && strcmp(av[x-1], "to") == 0) {
            av[x] = "any";
            break;
        }
    }

    ogs_ipfw_parse_error = 0;
	compile_rule(av, (uint32_t *)rule, &rbufsize, NULL);

    memset(ipfw_rule, 0, sizeof(ogs_ipfw_rule_t));

    if (ogs_ipfw_parse_error) {
        ogs_error("Cannot compile Flow-Description [%s]", flow_description);
        ogs_free(description);
        return OGS_ERROR;
    }
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

    if (rule->ipv4_src) {
        switch (direction) {
        case OGS_FLOW_DOWNLINK_ONLY:
        case OGS_FLOW_BIDIRECTIONAL:
            content->component[j].type =
                OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
            content->component[j].ipv4.addr = rule->ip.src.addr[0];
            content->component[j].ipv4.mask = rule->ip.src.mask[0];
            j++; len += 9;
            break;
        case OGS_FLOW_UPLINK_ONLY:
            if (!no_ipv4v6_local_addr_in_packet_filter) {
                content->component[j].type =
                    OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
                content->component[j].ipv4.addr = rule->ip.src.addr[0];
                content->component[j].ipv4.mask = rule->ip.src.mask[0];
                j++; len += 9;
            }
            break;
        default:
            ogs_fatal("Unsupported direction [%d]", direction);
            ogs_assert_if_reached();
        }
    }

    if (rule->ipv4_dst) {
        switch (direction) {
        case OGS_FLOW_DOWNLINK_ONLY:
        case OGS_FLOW_BIDIRECTIONAL:
            if (!no_ipv4v6_local_addr_in_packet_filter) {
                content->component[j].type =
                    OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
                content->component[j].ipv4.addr = rule->ip.dst.addr[0];
                content->component[j].ipv4.mask = rule->ip.dst.mask[0];
                j++; len += 9;
            }
            break;
        case OGS_FLOW_UPLINK_ONLY:
            content->component[j].type =
                OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
            content->component[j].ipv4.addr = rule->ip.dst.addr[0];
            content->component[j].ipv4.mask = rule->ip.dst.mask[0];
            j++; len += 9;
            break;
        default:
            ogs_fatal("Unsupported direction [%d]", direction);
            ogs_assert_if_reached();
        }
    }

    if (rule->ipv6_src) {
        switch (direction) {
        case OGS_FLOW_DOWNLINK_ONLY:
        case OGS_FLOW_BIDIRECTIONAL:
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
            break;
        case OGS_FLOW_UPLINK_ONLY:
            if (!no_ipv4v6_local_addr_in_packet_filter) {
                content->component[j].type =
                    OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
                memcpy(content->component[j].ipv6.addr,
                        rule->ip.src.addr, sizeof rule->ip.src.addr);
                content->component[j].ipv6.prefixlen =
                    contigmask((uint8_t *)rule->ip.src.mask, 128);
                j++; len += 18;
            }
            break;
        default:
            ogs_fatal("Unsupported direction [%d]", direction);
            ogs_assert_if_reached();
        }
    }

    if (rule->ipv6_dst) {
        switch (direction) {
        case OGS_FLOW_DOWNLINK_ONLY:
        case OGS_FLOW_BIDIRECTIONAL:
            if (!no_ipv4v6_local_addr_in_packet_filter) {
                content->component[j].type =
                    OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
                memcpy(content->component[j].ipv6.addr,
                    rule->ip.dst.addr, sizeof rule->ip.dst.addr);
                content->component[j].ipv6.prefixlen =
                    contigmask((uint8_t *)rule->ip.dst.mask, 128);
                j++; len += 18;
            }
            break;
        case OGS_FLOW_UPLINK_ONLY:
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
            break;
        default:
            ogs_fatal("Unsupported direction [%d]", direction);
            ogs_assert_if_reached();
        }
    }

    if (rule->port.src.low) {
        if (rule->port.src.low == rule->port.src.high) {
            switch (direction) {
            case OGS_FLOW_DOWNLINK_ONLY:
            case OGS_FLOW_BIDIRECTIONAL:
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
                break;
            case OGS_FLOW_UPLINK_ONLY:
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
                break;
            default:
                ogs_fatal("Unsupported direction [%d]", direction);
                ogs_assert_if_reached();
            }
            content->component[j].port.low = rule->port.src.low;
            j++; len += 3;
        } else {
            switch (direction) {
            case OGS_FLOW_DOWNLINK_ONLY:
            case OGS_FLOW_BIDIRECTIONAL:
                content->component[j].type =
                    OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
                break;
            case OGS_FLOW_UPLINK_ONLY:
                content->component[j].type =
                    OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
                break;
            default:
                ogs_fatal("Unsupported direction [%d]", direction);
                ogs_assert_if_reached();
            }
            content->component[j].port.low = rule->port.src.low;
            content->component[j].port.high = rule->port.src.high;
            j++; len += 5;
        }
    }

    if (rule->port.dst.low) {
        if (rule->port.dst.low == rule->port.dst.high) {
            switch (direction) {
            case OGS_FLOW_DOWNLINK_ONLY:
            case OGS_FLOW_BIDIRECTIONAL:
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
                break;
            case OGS_FLOW_UPLINK_ONLY:
                content->component[j].type =
                    OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
                break;
            default:
                ogs_fatal("Unsupported direction [%d]", direction);
                ogs_assert_if_reached();
            }
            content->component[j].port.low = rule->port.dst.low;
            j++; len += 3;
        } else {
            switch (direction) {
            case OGS_FLOW_DOWNLINK_ONLY:
            case OGS_FLOW_BIDIRECTIONAL:
                content->component[j].type =
                    OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
                break;
            case OGS_FLOW_UPLINK_ONLY:
                content->component[j].type =
                    OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
                break;
            default:
                ogs_fatal("Unsupported direction [%d]", direction);
                ogs_assert_if_reached();
            }
            content->component[j].port.low = rule->port.dst.low;
            content->component[j].port.high = rule->port.dst.high;
            j++; len += 5;
        }
    }

    content->num_of_component = j;
    content->length = len;
}
