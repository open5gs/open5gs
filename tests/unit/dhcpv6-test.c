/*
 * Copyright (C) 2026 by Nimbus Solutions
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
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-pfcp.h"
#include "smf/dhcpv6.h"
#include "core/abts.h"

/* Build a DHCPv6 Solicit with IA_PD + Rapid Commit */
static int build_solicit(uint8_t *buf)
{
    uint8_t *p = buf;

    *p++ = DHCPV6_MSG_SOLICIT;
    *p++ = 0x01; *p++ = 0x02; *p++ = 0x03;     /* xid */

    /* CLIENTID : DUID-LLT-like, 14 bytes */
    *p++ = 0x00; *p++ = DHCPV6_OPT_CLIENTID;
    *p++ = 0x00; *p++ = 14;
    memset(p, 0xAA, 14); p += 14;

    /* RAPID_COMMIT */
    *p++ = 0x00; *p++ = DHCPV6_OPT_RAPID_COMMIT;
    *p++ = 0x00; *p++ = 0x00;

    /* IA_PD : IAID + T1 + T2, no nested options */
    *p++ = 0x00; *p++ = DHCPV6_OPT_IA_PD;
    *p++ = 0x00; *p++ = 12;
    *p++ = 0x11; *p++ = 0x22; *p++ = 0x33; *p++ = 0x44; /* IAID */
    memset(p, 0, 8); p += 8;                            /* T1 + T2 */

    return p - buf;
}

/* Build a DHCPv6 Request with SERVERID + IA_PD{IAPREFIX} */
static int build_request(uint8_t *buf, const char *prefix6, uint8_t plen)
{
    uint8_t *p = buf;
    ogs_ipsubnet_t ipsub;

    ogs_assert(ogs_ipsubnet(&ipsub, prefix6, NULL) == OGS_OK);

    *p++ = DHCPV6_MSG_REQUEST;
    *p++ = 0x0A; *p++ = 0x0B; *p++ = 0x0C;     /* xid */

    /* CLIENTID */
    *p++ = 0x00; *p++ = DHCPV6_OPT_CLIENTID;
    *p++ = 0x00; *p++ = 14;
    memset(p, 0xAA, 14); p += 14;

    /* SERVERID */
    *p++ = 0x00; *p++ = DHCPV6_OPT_SERVERID;
    *p++ = 0x00; *p++ = 14;
    memset(p, 0xBB, 14); p += 14;

    /* IA_PD : IAID + T1 + T2 + IAPREFIX */
    *p++ = 0x00; *p++ = DHCPV6_OPT_IA_PD;
    *p++ = 0x00; *p++ = 12 + 4 + 25;
    *p++ = 0x11; *p++ = 0x22; *p++ = 0x33; *p++ = 0x44; /* IAID */
    memset(p, 0, 8); p += 8;                            /* T1 + T2 */

    /* IAPREFIX */
    *p++ = 0x00; *p++ = DHCPV6_OPT_IAPREFIX;
    *p++ = 0x00; *p++ = 25;
    memset(p, 0, 8); p += 8;                /* lifetimes */
    *p++ = plen;
    memcpy(p, ipsub.sub, OGS_IPV6_LEN);     /* already network-order */
    p += OGS_IPV6_LEN;

    return p - buf;
}

static void dhcpv6_test_parse_solicit(abts_case *tc, void *data)
{
    uint8_t buf[128];
    int len;
    smf_dhcpv6_message_t msg;

    len = build_solicit(buf);
    ABTS_TRUE(tc, len > 4);

    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, buf, len));

    ABTS_INT_EQUAL(tc, DHCPV6_MSG_SOLICIT, msg.type);
    ABTS_INT_EQUAL(tc, 0x01, msg.xid[0]);
    ABTS_INT_EQUAL(tc, 0x03, msg.xid[2]);
    ABTS_TRUE(tc, msg.duid != NULL);
    ABTS_INT_EQUAL(tc, 14, msg.duid_len);
    ABTS_TRUE(tc, msg.rapid_commit);
    ABTS_TRUE(tc, msg.ia_pd_present);
    ABTS_INT_EQUAL(tc, 0x11223344, msg.iaid);
    ABTS_TRUE(tc, !msg.serverid_present);
    ABTS_TRUE(tc, !msg.ia_prefix_present);
}

static void dhcpv6_test_parse_request(abts_case *tc, void *data)
{
    uint8_t buf[160];
    int len;
    smf_dhcpv6_message_t msg;
    ogs_ipsubnet_t expect;

    len = build_request(buf, "2001:db8:8001::", 56);

    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, buf, len));

    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REQUEST, msg.type);
    ABTS_TRUE(tc, msg.serverid_present);
    ABTS_INT_EQUAL(tc, 14, msg.server_duid_len);
    ABTS_TRUE(tc, msg.ia_pd_present);
    ABTS_INT_EQUAL(tc, 0x11223344, msg.iaid);
    ABTS_TRUE(tc, msg.ia_prefix_present);
    ABTS_INT_EQUAL(tc, 56, msg.prefix_plen);

    ogs_assert(ogs_ipsubnet(&expect, "2001:db8:8001::", NULL) == OGS_OK);
    ABTS_TRUE(tc,
        memcmp(msg.prefix, expect.sub, OGS_IPV6_LEN) == 0);
}

static void dhcpv6_test_parse_malformed(abts_case *tc, void *data)
{
    uint8_t buf[160];
    int len;
    smf_dhcpv6_message_t msg;

    memset(buf, 0, sizeof(buf));

    /* Header only : too short for any option */
    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_parse(&msg, buf, 3));

    len = build_solicit(buf);

    /* Truncated : option claims more than the buffer holds */
    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_parse(&msg, buf, len - 2));

    /* Trailing garbage : a lone byte after the last option */
    buf[len] = 0xFF;
    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_parse(&msg, buf, len + 1));

    /* Nested IAPREFIX overruns the IA_PD */
    len = build_request(buf, "2001:db8:8001::", 56);
    buf[len - 26] = 0x7F;   /* inflate IAPREFIX length beyond the IA_PD */
    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_parse(&msg, buf, len));
}

static void dhcpv6_test_pool(abts_case *tc, void *data)
{
    ogs_pfcp_subnet_t subnet;
    ogs_ipsubnet_t expect;
    uint8_t p1[OGS_IPV6_LEN], p2[OGS_IPV6_LEN], p3[OGS_IPV6_LEN];
    int i;

    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;

    /* /48 pool carved into /56 : 256 prefixes */
    ABTS_INT_EQUAL(tc, OGS_OK,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 56, 0, 0));
    ABTS_INT_EQUAL(tc, 256, subnet.delegated_prefix.num);
    ABTS_INT_EQUAL(tc, OGS_PFCP_DEFAULT_PD_VALID_LIFETIME,
        subnet.delegated_prefix.valid_lifetime);

    /* Sequential allocation */
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_pfcp_delegated_prefix_alloc(&subnet, p1));
    ogs_assert(ogs_ipsubnet(&expect, "2001:db8:8000::", NULL) == OGS_OK);
    ABTS_TRUE(tc, memcmp(p1, expect.sub, OGS_IPV6_LEN) == 0);

    ABTS_INT_EQUAL(tc, OGS_OK, ogs_pfcp_delegated_prefix_alloc(&subnet, p2));
    ogs_assert(ogs_ipsubnet(&expect, "2001:db8:8000:100::", NULL) == OGS_OK);
    ABTS_TRUE(tc, memcmp(p2, expect.sub, OGS_IPV6_LEN) == 0);

    /* Free then re-alloc : first free slot comes back */
    ogs_pfcp_delegated_prefix_free(&subnet, p1);
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_pfcp_delegated_prefix_alloc(&subnet, p3));
    ABTS_TRUE(tc, memcmp(p3, p1, OGS_IPV6_LEN) == 0);

    /* Out-of-pool free is rejected without corrupting the bitmap */
    ogs_assert(ogs_ipsubnet(&expect, "2001:db8:9000::", NULL) == OGS_OK);
    ogs_pfcp_delegated_prefix_free(&subnet, (uint8_t *)expect.sub);
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_pfcp_delegated_prefix_alloc(&subnet, p3));
    ogs_assert(ogs_ipsubnet(&expect, "2001:db8:8000:200::", NULL) == OGS_OK);
    ABTS_TRUE(tc, memcmp(p3, expect.sub, OGS_IPV6_LEN) == 0);

    /* Exhaust the pool */
    for (i = 3; i < 256; i++)
        ABTS_INT_EQUAL(tc, OGS_OK,
            ogs_pfcp_delegated_prefix_alloc(&subnet, p3));
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_delegated_prefix_alloc(&subnet, p3));

    ogs_free(subnet.delegated_prefix.bitmap);
}

static void dhcpv6_test_pool_validation(abts_case *tc, void *data)
{
    ogs_pfcp_subnet_t subnet;

    /* length must be > range and <= 64 */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 48, 0, 0));

    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 65, 0, 0));

    /* pool too large (> 16 bits) */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8::", "32", 56, 0, 0));

    /* range must be a network address */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000:1::", "48", 56, 0, 0));

    /* preferred > valid rejected */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 56, 100, 200));

    /* IPv4 subnet rejected */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "10.0.0.0", "8", 56, 0, 0));
}

abts_suite *test_dhcpv6(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, dhcpv6_test_parse_solicit, NULL);
    abts_run_test(suite, dhcpv6_test_parse_request, NULL);
    abts_run_test(suite, dhcpv6_test_parse_malformed, NULL);
    abts_run_test(suite, dhcpv6_test_pool, NULL);
    abts_run_test(suite, dhcpv6_test_pool_validation, NULL);

    return suite;
}
