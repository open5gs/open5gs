/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "ipfw/ogs-ipfw.h"
#include "core/abts.h"

static int compile(ogs_ipfw_rule_t *rule, const char *flow_description)
{
    char buf[OGS_HUGE_LEN];

    ogs_cpystrn(buf, flow_description, sizeof(buf));

    memset(rule, 0, sizeof(ogs_ipfw_rule_t));
    return ogs_ipfw_compile_rule(rule, buf);
}

#define ACCEPT(tc, s) ABTS_INT_EQUAL(tc, OGS_OK, compile(&rule, s))
#define REJECT(tc, s) ABTS_INT_EQUAL(tc, OGS_ERROR, compile(&rule, s))

/*
 * A rule that compiles can still mean something other than what was
 * written, so check the canonical form the encoder produces rather than
 * the return value alone.
 */
static void encodes_to(abts_case *tc,
        const char *flow_description, const char *expected, int lineno)
{
    ogs_ipfw_rule_t rule;
    char *encoded = NULL;

    if (compile(&rule, flow_description) != OGS_OK) {
        abts_fail(tc, "Flow-Description was rejected", lineno);
        return;
    }

    encoded = ogs_ipfw_encode_flow_description(&rule);
    abts_str_equal(tc, expected, encoded, lineno);

    if (encoded)
        ogs_free(encoded);
}

#define ENCODES_TO(tc, s, e) encodes_to(tc, s, e, __LINE__)

/* what Open5GS itself emits and what configs/ and tests/ use */
static void ipfw_test_accepted(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    ACCEPT(tc, "permit out ip from any to assigned");
    ACCEPT(tc, "permit out icmp from any to any");
    ACCEPT(tc, "permit out icmp from any to assigned");
    ACCEPT(tc, "permit out 58 from ff02::2/128 to assigned");
    ACCEPT(tc, "permit out 17 from 172.20.166.84 to 10.45.0.2 20001");
    ACCEPT(tc, "permit out udp from 172.30.0.50 49000-50000 to assigned");
    ACCEPT(tc, "permit out udp from 10.200.136.98/32 1-65535 to assigned 50021");
    ACCEPT(tc, "permit out 6 from 2001:db8::1/64 to assigned");

    ENCODES_TO(tc, "permit out ip from any to assigned",
            "permit out ip from any to assigned");
    ENCODES_TO(tc, "permit out 17 from 172.20.166.84 to 10.45.0.2 20001",
            "permit out 17 from 172.20.166.84 to 10.45.0.2 20001");
    ENCODES_TO(tc, "permit out udp from 172.30.0.50 49000-50000 to assigned",
            "permit out 17 from 172.30.0.50 49000-50000 to assigned");
    ENCODES_TO(tc,
            "permit out udp from 10.200.136.98/32 1-65535 to assigned 50021",
            "permit out 17 from 10.200.136.98 1-65535 to assigned 50021");
    ENCODES_TO(tc, "permit out 6 from 2001:db8::1/64 to assigned",
            "permit out 6 from 2001:db8::/64 to assigned");
}

static void ipfw_test_structure(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    REJECT(tc, "permit out");
    REJECT(tc, "permit out ip");
    REJECT(tc, "permit out ip from");
    REJECT(tc, "permit out ip from 1.2.3.4");
    REJECT(tc, "permit out ip from 1.2.3.4 to");
    REJECT(tc, "permit out ip from to");
    REJECT(tc, "permit out ip from any to");
    REJECT(tc, "permit in ip from any to assigned");

    /* ipfw(8) syntax that a Flow-Description must not carry */
    REJECT(tc, "permit out ip from 1.2.3.4 to 1.2.3.4 uid");
    REJECT(tc, "permit out ip from 1.2.3.4 to 1.2.3.4 keep-state");
    REJECT(tc, "permit out ip from 1.2.3.4 to 1.2.3.4 fwd 1.2.3.4");
    REJECT(tc, "permit out ip from table(1) to assigned");
    REJECT(tc, "permit out ip from me to assigned");
    REJECT(tc, "permit out ip from 1.2.3.4,2.3.4.5 to any");
    REJECT(tc, "permit out ip from 1.2.3.4{1,2,3,4,5,6,7,8} to any");
    REJECT(tc, "permit out ip from no.such.host.invalid to assigned");
}

static void ipfw_test_proto(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    ACCEPT(tc, "permit out 1 from 1.2.3.4 to assigned");
    ABTS_INT_EQUAL(tc, 1, rule.proto);
    ACCEPT(tc, "permit out 255 from 1.2.3.4 to assigned");
    ABTS_INT_EQUAL(tc, 255, rule.proto);
    ACCEPT(tc, "permit out udp from 1.2.3.4 to assigned");
    ABTS_INT_EQUAL(tc, IPPROTO_UDP, rule.proto);
    ACCEPT(tc, "permit out ipv6-icmp from ff02::2/128 to assigned");
    ABTS_INT_EQUAL(tc, IPPROTO_ICMPV6, rule.proto);
    /* the one name carried through: no protocol restriction */
    ACCEPT(tc, "permit out ip from 1.2.3.4 to assigned");
    ABTS_INT_EQUAL(tc, IPPROTO_IP, rule.proto);

    /*
     * add_proto0() stores the protocol in a u_char without a range check,
     * so 256 would be truncated to 0 -- widening the filter to all of IP --
     * and 999 would become 231.
     */
    REJECT(tc, "permit out 0 from 1.2.3.4 to assigned");
    REJECT(tc, "permit out 256 from 1.2.3.4 to assigned");
    REJECT(tc, "permit out 999 from 1.2.3.4 to assigned");
    REJECT(tc, "permit out unknown-protocol from 1.2.3.4 to assigned");

    /* add_proto() matches "all" with _substrcmp(), which takes a prefix */
    REJECT(tc, "permit out a from 1.2.3.4 to assigned");
    REJECT(tc, "permit out al from 1.2.3.4 to assigned");
    REJECT(tc, "permit out all from 1.2.3.4 to assigned");

    /* family selectors: O_IP4/O_IP6 are dropped, leaving protocol 0 */
    REJECT(tc, "permit out ip4 from any to assigned");
    REJECT(tc, "permit out ipv4 from any to assigned");
    REJECT(tc, "permit out ip6 from any to assigned");
    REJECT(tc, "permit out ipv6 from any to assigned");

    /* a name whose protocol number is 0 */
    REJECT(tc, "permit out hopopt from 1.2.3.4 to assigned");
}

static void ipfw_test_port(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    ACCEPT(tc, "permit out udp from 1.2.3.4 1 to assigned");
    ABTS_INT_EQUAL(tc, 1, rule.port.src.low);
    ACCEPT(tc, "permit out udp from 1.2.3.4 65535 to assigned");
    ABTS_INT_EQUAL(tc, 65535, rule.port.src.high);
    ACCEPT(tc, "permit out udp from 1.2.3.4 1-65535 to assigned");
    ABTS_INT_EQUAL(tc, 1, rule.port.src.low);
    ABTS_INT_EQUAL(tc, 65535, rule.port.src.high);

    /* 0 means "no port condition", so it must not arrive as a port */
    REJECT(tc, "permit out udp from 1.2.3.4 0 to assigned");
    REJECT(tc, "permit out udp from 1.2.3.4 to assigned 0");

    REJECT(tc, "permit out udp from 1.2.3.4 65536 to assigned");
    /* fill_newports() never compares the two ends of a range */
    REJECT(tc, "permit out udp from 1.2.3.4 50000-40000 to assigned");
    REJECT(tc, "permit out udp from 1.2.3.4 1-0 to assigned");
}

static void ipfw_test_prefix(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    ACCEPT(tc, "permit out ip from 1.2.3.4/0 to assigned");
    ACCEPT(tc, "permit out ip from 1.2.3.4/32 to assigned");
    REJECT(tc, "permit out ip from 1.2.3.4/33 to assigned");
    REJECT(tc, "permit out ip from 10.45.0.0/99 to assigned");

    ACCEPT(tc, "permit out 58 from ff02::2/0 to assigned");
    ACCEPT(tc, "permit out 58 from ff02::2/128 to assigned");
    /* ipv6.c calls the real errx(), which would exit() the daemon */
    REJECT(tc, "permit out 58 from ff02::2/129 to assigned");
    REJECT(tc, "permit out 58 from :::::: to assigned");
}

/*
 * add_src() and add_dst() send every address through the IPv6 helpers once
 * the protocol is IPPROTO_IPV6, and ipv6.c calls the real errx(), which
 * exits the daemon.  A packet also cannot carry both families at once.
 */
static void ipfw_test_family(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    ACCEPT(tc, "permit out 41 from 2001:db8::1 to assigned");
    ACCEPT(tc, "permit out 41 from any to assigned");
    REJECT(tc, "permit out 41 from 192.0.2.1 to assigned");
    /* "any" carries no family, so this can only be the protocol 41 rule */
    REJECT(tc, "permit out 41 from any to 192.0.2.1");
    REJECT(tc, "permit out 41 from 192.0.2.1 to 192.0.2.2");

    ACCEPT(tc, "permit out 17 from 10.45.0.5 50026 to 10.45.0.3 50022");
    REJECT(tc, "permit out tcp from 2001:db8::1 to 192.0.2.1");
    REJECT(tc, "permit out tcp from 192.0.2.1 to 2001:db8::1");

    ACCEPT(tc, "permit out 41 from any to 2001:db8::1");
}

/*
 * ogs_ipfw_rule_t holds one contiguous range per direction, so the comma
 * separated lists an IPFilterRule may carry cannot be represented -- the
 * parser would keep the first range and drop the rest.
 */
static void ipfw_test_port_list(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    REJECT(tc, "permit out tcp from any to assigned 80,443");
    REJECT(tc, "permit out udp from any to assigned 1000-2000,3000");
    REJECT(tc, "permit out tcp from any 80,443 to assigned");
}

/*
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338, Issue #4711
 *
 * A Flow-Description is always written in the downlink orientation, so the
 * UE address -- "assigned" -- can only appear after "to".
 */
static void ipfw_test_assigned(abts_case *tc, void *data)
{
    ogs_ipfw_rule_t rule;

    ACCEPT(tc, "permit out udp from 172.30.0.50 49000-50000 to assigned");
    REJECT(tc, "permit out udp from assigned to 172.30.0.50 49000-50000");
    REJECT(tc, "permit out ip from assigned to assigned");
}

abts_suite *test_ipfw(abts_suite *suite)
{
    int id = ogs_log_get_domain_id("core");
    ogs_log_level_e level = ogs_log_get_domain_level(id);

    suite = ADD_SUITE(suite)

    /*
     * These cases feed malformed input to the parser to prove that it
     * neither crashes (SEGV, exit) nor accepts the rule.  Only the return
     * value matters here, so the diagnostic each rejection prints is not
     * wanted -- silence the domain while the suite runs.
     */
    ogs_log_set_domain_level(id, OGS_LOG_NONE);

    abts_run_test(suite, ipfw_test_accepted, NULL);
    abts_run_test(suite, ipfw_test_structure, NULL);
    abts_run_test(suite, ipfw_test_proto, NULL);
    abts_run_test(suite, ipfw_test_port, NULL);
    abts_run_test(suite, ipfw_test_prefix, NULL);
    abts_run_test(suite, ipfw_test_family, NULL);
    abts_run_test(suite, ipfw_test_port_list, NULL);
    abts_run_test(suite, ipfw_test_assigned, NULL);

    ogs_log_set_domain_level(id, level);

    return suite;
}
