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

#include <unistd.h>

#include <netinet/ip6.h>
#include <netinet/udp.h>

#include "ogs-pfcp.h"
#include "smf/dhcpv6.h"
#include "n4-handler.h"
#include "metrics.h"
#include "core/abts.h"

/* Build a DHCPv6 Solicit with IA_PD; optionally Rapid Commit */
static int build_solicit(uint8_t *buf, bool with_rapid_commit)
{
    uint8_t *p = buf;

    *p++ = DHCPV6_MSG_SOLICIT;
    *p++ = 0x01; *p++ = 0x02; *p++ = 0x03;     /* xid */

    /* CLIENTID : DUID-LLT-like, 14 bytes */
    *p++ = 0x00; *p++ = DHCPV6_OPT_CLIENTID;
    *p++ = 0x00; *p++ = 14;
    memset(p, 0xAA, 14); p += 14;

    if (with_rapid_commit) {
        *p++ = 0x00; *p++ = DHCPV6_OPT_RAPID_COMMIT;
        *p++ = 0x00; *p++ = 0x00;
    }

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

    len = build_solicit(buf, true);
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

    len = build_solicit(buf, true);

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
    ogs_assert(ogs_ipsubnet(&subnet.sub, "2001:db8:cafe::", "48") == OGS_OK);
    subnet.prefixlen = 48;

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
    ogs_assert(ogs_ipsubnet(&subnet.sub, "2001:db8:cafe::", "48") == OGS_OK);
    subnet.prefixlen = 48;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 56, 100, 200));

    /* delegated range overlapping the session subnet rejected */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ogs_assert(ogs_ipsubnet(&subnet.sub, "2001:db8:cafe::", "48") == OGS_OK);
    subnet.prefixlen = 48;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:cafe::", "48", 56, 0, 0));

    /* non-overlapping range accepted */
    ABTS_INT_EQUAL(tc, OGS_OK,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 56, 0, 0));
    ogs_free(subnet.delegated_prefix.bitmap);

    /* out-of-range length rejected (no uint8_t wrap) */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET6;
    ogs_assert(ogs_ipsubnet(&subnet.sub, "2001:db8:cafe::", "48") == OGS_OK);
    subnet.prefixlen = 48;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "2001:db8:8000::", "48", 300, 0, 0));

    /* IPv4 subnet rejected */
    memset(&subnet, 0, sizeof(subnet));
    subnet.family = AF_INET;
    ABTS_INT_EQUAL(tc, OGS_ERROR,
        ogs_pfcp_subnet_delegated_prefix_set(
            &subnet, "10.0.0.0", "8", 56, 0, 0));
}

/* Locate a top-level DHCPv6 option. Returns 1 if found. */
static int find_option(const uint8_t *data, int len, uint16_t want,
        uint16_t *olen)
{
    const uint8_t *p, *end;

    if (len < 4)
        return 0;

    p = data + 4;
    end = data + len;
    while (p + 4 <= end) {
        uint16_t code = ((uint16_t)p[0] << 8) | p[1];
        uint16_t optlen = ((uint16_t)p[2] << 8) | p[3];

        if (p + 4 + optlen > end)
            return 0;
        if (code == want) {
            if (olen)
                *olen = optlen;
            return 1;
        }
        p += 4 + optlen;
    }
    return 0;
}

/* Rapid Commit must sit after CLIENTID and before IA_PD (common order). */
static int rapid_commit_between_clientid_and_ia_pd(const uint8_t *data, int len)
{
    const uint8_t *p, *end;
    int saw_clientid = 0, saw_rapid = 0, saw_ia_pd = 0;

    if (len < 4)
        return 0;

    p = data + 4;
    end = data + len;
    while (p + 4 <= end) {
        uint16_t code = ((uint16_t)p[0] << 8) | p[1];
        uint16_t optlen = ((uint16_t)p[2] << 8) | p[3];

        if (p + 4 + optlen > end)
            return 0;
        if (code == DHCPV6_OPT_CLIENTID)
            saw_clientid = 1;
        else if (code == DHCPV6_OPT_RAPID_COMMIT) {
            if (!saw_clientid || saw_ia_pd)
                return 0;
            saw_rapid = 1;
        } else if (code == DHCPV6_OPT_IA_PD) {
            saw_ia_pd = 1;
        }
        p += 4 + optlen;
    }
    return saw_clientid && saw_rapid && saw_ia_pd;
}

static void fill_test_lease(smf_sess_t *sess)
{
    ogs_ipsubnet_t ipsub;

    memset(sess, 0, sizeof(*sess));
    sess->pd_lease.state = SMF_PD_LEASE_ACTIVE;
    sess->pd_lease.preferred_lifetime = 3600;
    sess->pd_lease.valid_lifetime = 7200;
    sess->pd_lease.plen = 56;
    ogs_assert(ogs_ipsubnet(&ipsub, "2001:db8:8001::", NULL) == OGS_OK);
    memcpy(sess->pd_lease.prefix, ipsub.sub, OGS_IPV6_LEN);
}

static void dhcpv6_test_rapid_commit_reply(abts_case *tc, void *data)
{
    uint8_t reqbuf[160], resp[256];
    int reqlen, resplen;
    smf_dhcpv6_message_t req;
    smf_sess_t sess;
    uint16_t olen = 0xffff;

    smf_dhcpv6_init();
    fill_test_lease(&sess);

    /* Rapid-Commit Solicit → Reply MUST carry option 14 length 0 */
    reqlen = build_solicit(reqbuf, true);
    memset(&req, 0, sizeof(req));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&req, reqbuf, reqlen));
    ABTS_TRUE(tc, req.rapid_commit);

    resplen = smf_dhcpv6_encode_response(resp, sizeof(resp),
            &sess, &req, DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
    ABTS_TRUE(tc, resplen > 4);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, resp[0]);
    ABTS_TRUE(tc, find_option(resp, resplen, DHCPV6_OPT_RAPID_COMMIT, &olen));
    ABTS_INT_EQUAL(tc, 0, olen);
    ABTS_TRUE(tc, rapid_commit_between_clientid_and_ia_pd(resp, resplen));

    /* Normal Solicit → Advertise must not include Rapid Commit */
    reqlen = build_solicit(reqbuf, false);
    memset(&req, 0, sizeof(req));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&req, reqbuf, reqlen));
    ABTS_TRUE(tc, !req.rapid_commit);

    resplen = smf_dhcpv6_encode_response(resp, sizeof(resp),
            &sess, &req, DHCPV6_MSG_ADVERTISE, DHCPV6_STATUS_SUCCESS);
    ABTS_TRUE(tc, resplen > 4);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_ADVERTISE, resp[0]);
    ABTS_TRUE(tc, !find_option(resp, resplen, DHCPV6_OPT_RAPID_COMMIT, NULL));

    /* Request → Reply must not include Rapid Commit */
    reqlen = build_request(reqbuf, "2001:db8:8001::", 56);
    memset(&req, 0, sizeof(req));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&req, reqbuf, reqlen));

    resplen = smf_dhcpv6_encode_response(resp, sizeof(resp),
            &sess, &req, DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
    ABTS_TRUE(tc, resplen > 4);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, resp[0]);
    ABTS_TRUE(tc, !find_option(resp, resplen, DHCPV6_OPT_RAPID_COMMIT, NULL));

    /* Advertise SUCCESS is valid while the lease is only OFFERED
     * (prefix reserved, PFCP not yet committed). */
    sess.pd_lease.state = SMF_PD_LEASE_OFFERED;
    reqlen = build_solicit(reqbuf, false);
    memset(&req, 0, sizeof(req));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&req, reqbuf, reqlen));
    resplen = smf_dhcpv6_encode_response(resp, sizeof(resp),
            &sess, &req, DHCPV6_MSG_ADVERTISE, DHCPV6_STATUS_SUCCESS);
    ABTS_TRUE(tc, resplen > 4);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_ADVERTISE, resp[0]);
}

/* Copy the Server DUID out of an encoded Advertise/Reply. */
static int extract_serverid(const uint8_t *data, int len,
        uint8_t *duid, uint16_t *duid_len)
{
    const uint8_t *p, *end;

    if (len < 4)
        return 0;

    p = data + 4;
    end = data + len;
    while (p + 4 <= end) {
        uint16_t code = ((uint16_t)p[0] << 8) | p[1];
        uint16_t optlen = ((uint16_t)p[2] << 8) | p[3];

        if (p + 4 + optlen > end)
            return 0;
        if (code == DHCPV6_OPT_SERVERID) {
            if (optlen > DHCPV6_MAX_DUID_LEN)
                return 0;
            memcpy(duid, p + 4, optlen);
            *duid_len = optlen;
            return 1;
        }
        p += 4 + optlen;
    }
    return 0;
}

static int build_typed(uint8_t *buf, uint8_t type, bool with_serverid,
        const uint8_t *duid, uint16_t duid_len)
{
    uint8_t *p = buf;

    *p++ = type;
    *p++ = 0x01; *p++ = 0x02; *p++ = 0x03;

    *p++ = 0x00; *p++ = DHCPV6_OPT_CLIENTID;
    *p++ = 0x00; *p++ = 14;
    memset(p, 0xAA, 14); p += 14;

    if (with_serverid) {
        *p++ = 0x00; *p++ = DHCPV6_OPT_SERVERID;
        *p++ = (uint8_t)(duid_len >> 8);
        *p++ = (uint8_t)(duid_len & 0xff);
        memcpy(p, duid, duid_len); p += duid_len;
    }

    *p++ = 0x00; *p++ = DHCPV6_OPT_IA_PD;
    *p++ = 0x00; *p++ = 12;
    *p++ = 0x11; *p++ = 0x22; *p++ = 0x33; *p++ = 0x44;
    memset(p, 0, 8); p += 8;

    return p - buf;
}

static void dhcpv6_test_rfc8415_serverid(abts_case *tc, void *data)
{
    uint8_t reqbuf[160], resp[256];
    uint8_t our_duid[DHCPV6_MAX_DUID_LEN];
    uint8_t other_duid[14];
    uint16_t our_duid_len = 0;
    int reqlen, resplen;
    smf_dhcpv6_message_t msg;
    smf_sess_t sess;
    static const uint8_t must_not[] = {
        DHCPV6_MSG_SOLICIT, DHCPV6_MSG_CONFIRM, DHCPV6_MSG_REBIND,
    };
    static const uint8_t must[] = {
        DHCPV6_MSG_REQUEST, DHCPV6_MSG_RENEW,
        DHCPV6_MSG_RELEASE, DHCPV6_MSG_DECLINE,
    };
    unsigned i;

    smf_dhcpv6_init();
    fill_test_lease(&sess);

    reqlen = build_solicit(reqbuf, false);
    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
    resplen = smf_dhcpv6_encode_response(resp, sizeof(resp),
            &sess, &msg, DHCPV6_MSG_ADVERTISE, DHCPV6_STATUS_SUCCESS);
    ABTS_TRUE(tc, extract_serverid(resp, resplen, our_duid, &our_duid_len));
    ABTS_TRUE(tc, our_duid_len > 0);
    memset(other_duid, 0xBB, sizeof(other_duid));

    /* Solicit / Confirm / Rebind : Server Identifier is prohibited */
    for (i = 0; i < sizeof(must_not); i++) {
        reqlen = build_typed(reqbuf, must_not[i], false, NULL, 0);
        memset(&msg, 0, sizeof(msg));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_check_serverid(&msg));

        reqlen = build_typed(reqbuf, must_not[i], true, our_duid, our_duid_len);
        memset(&msg, 0, sizeof(msg));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
        ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_check_serverid(&msg));
    }

    /* Request / Renew / Release / Decline : matching Server Identifier required */
    for (i = 0; i < sizeof(must); i++) {
        reqlen = build_typed(reqbuf, must[i], false, NULL, 0);
        memset(&msg, 0, sizeof(msg));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
        ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_check_serverid(&msg));

        reqlen = build_typed(reqbuf, must[i], true,
                other_duid, sizeof(other_duid));
        memset(&msg, 0, sizeof(msg));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
        ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_check_serverid(&msg));

        reqlen = build_typed(reqbuf, must[i], true, our_duid, our_duid_len);
        memset(&msg, 0, sizeof(msg));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
        ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_check_serverid(&msg));
    }

    /* Existing Request builder (foreign SERVERID) is rejected */
    reqlen = build_request(reqbuf, "2001:db8:8001::", 56);
    memset(&msg, 0, sizeof(msg));
    ABTS_INT_EQUAL(tc, OGS_OK, smf_dhcpv6_parse(&msg, reqbuf, reqlen));
    ABTS_INT_EQUAL(tc, OGS_ERROR, smf_dhcpv6_check_serverid(&msg));
}

static void dhcpv6_test_rfc8415_ia_pd(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_SOLICIT));
    ABTS_TRUE(tc, smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_REQUEST));
    ABTS_TRUE(tc, smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_RENEW));
    ABTS_TRUE(tc, smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_REBIND));
    ABTS_TRUE(tc, smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_RELEASE));
    ABTS_TRUE(tc, !smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_CONFIRM));
    ABTS_TRUE(tc, !smf_dhcpv6_ia_pd_permitted(DHCPV6_MSG_DECLINE));
}

static void dhcpv6_test_pd_lease_pending(abts_case *tc, void *data)
{
    smf_sess_t sess;

    memset(&sess, 0, sizeof(sess));
    ABTS_TRUE(tc, !smf_sess_pd_lease_pfcp_pending(&sess));
    ABTS_TRUE(tc, !smf_sess_pd_lease_matches(&sess, 1, NULL, 0));

    sess.pd_lease.state = SMF_PD_LEASE_OFFERED;
    sess.pd_lease.iaid = 0x11223344;
    ABTS_TRUE(tc, !smf_sess_pd_lease_pfcp_pending(&sess));
    ABTS_TRUE(tc, smf_sess_pd_lease_matches(&sess, 0x11223344, NULL, 0));
    ABTS_TRUE(tc, !smf_sess_pd_lease_matches(&sess, 0xdeadbeef, NULL, 0));

    sess.pd_lease.state = SMF_PD_LEASE_PENDING;
    ABTS_TRUE(tc, smf_sess_pd_lease_pfcp_pending(&sess));

    sess.pd_lease.state = SMF_PD_LEASE_ACTIVE;
    ABTS_TRUE(tc, !smf_sess_pd_lease_pfcp_pending(&sess));
    ABTS_TRUE(tc, smf_sess_pd_lease_matches(&sess, 0x11223344, NULL, 0));

    sess.pd_lease.state = SMF_PD_LEASE_RELEASING;
    ABTS_TRUE(tc, smf_sess_pd_lease_pfcp_pending(&sess));
}

/*
 * In-process regression for the review items. Packets are handed to
 * smf_dhcpv6_handle(), PFCP Session Modification is sent to a localhost
 * UDP socket, and the EPC or 5GC N4 handler (or its response timer)
 * completes the lease. No eNB/gNB, no UE, and no subscriber traffic.
 */
#define PD_CAP_MAX 8

typedef struct pd_reply_s {
    uint8_t type;
    int rapid;
    int status;
} pd_reply_t;

static pd_reply_t pd_cap[PD_CAP_MAX];
static int pd_cap_n;
static int pd_runtime_ready;
static uint8_t pd_server_duid[DHCPV6_MAX_DUID_LEN];
static uint16_t pd_server_duid_len;

static const uint8_t pd_duid_a[14] = {
    0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07
};
static const uint8_t pd_duid_b[14] = {
    0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
    0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

typedef struct pd_fix_s {
    smf_ue_t *ue;
    smf_sess_t *sess;
    ogs_pfcp_node_t *node;
    ogs_pfcp_subnet_t *subnet;
    ogs_pfcp_ue_ip_t *ip;
} pd_fix_t;

static void pd_cap_reset(void)
{
    memset(pd_cap, 0, sizeof(pd_cap));
    pd_cap_n = 0;
}

static int pd_walk_dhcp(const uint8_t *p, const uint8_t *end,
        int *rapid, int *status, int *saw_prefix)
{
    while (p + 4 <= end) {
        uint16_t code = ((uint16_t)p[0] << 8) | p[1];
        uint16_t olen = ((uint16_t)p[2] << 8) | p[3];
        const uint8_t *data = p + 4;

        if (data + olen > end)
            return -1;
        if (code == DHCPV6_OPT_RAPID_COMMIT && olen == 0)
            *rapid = 1;
        else if (code == DHCPV6_OPT_STATUS_CODE && olen >= 2)
            *status = ((int)data[0] << 8) | data[1];
        else if (code == DHCPV6_OPT_IAPREFIX)
            *saw_prefix = 1;
        else if (code == DHCPV6_OPT_IA_PD && olen >= 12)
            pd_walk_dhcp(data + 12, data + olen, rapid, status, saw_prefix);
        p = data + olen;
    }
    return 0;
}

static void pd_capture(smf_sess_t *sess, ogs_pkbuf_t *pkbuf)
{
    struct ip6_hdr *ip6;
    struct udphdr *udp;
    const uint8_t *dhcp;
    int dhcp_len, rapid = 0, status = -1, saw_prefix = 0;
    pd_reply_t *reply;

    ogs_assert(pkbuf);
    if (pd_cap_n < PD_CAP_MAX &&
            pkbuf->len >= (int)(sizeof(*ip6) + sizeof(*udp) + 4)) {
        ip6 = (struct ip6_hdr *)pkbuf->data;
        udp = (struct udphdr *)(ip6 + 1);
        dhcp = (const uint8_t *)(udp + 1);
        dhcp_len = pkbuf->len - (int)(sizeof(*ip6) + sizeof(*udp));
        reply = &pd_cap[pd_cap_n];
        reply->type = dhcp[0];
        pd_walk_dhcp(dhcp + 4, dhcp + dhcp_len, &rapid, &status, &saw_prefix);
        if (status < 0 && saw_prefix)
            status = DHCPV6_STATUS_SUCCESS;
        reply->rapid = rapid;
        reply->status = status;
    }
    pd_cap_n++;
    ogs_pkbuf_free(pkbuf);
    (void)sess;
}

static void pd_load_server_duid(void)
{
    uint8_t reqbuf[160], resp[256];
    int reqlen, resplen;
    smf_dhcpv6_message_t msg;
    smf_sess_t sess;

    smf_dhcpv6_init();
    fill_test_lease(&sess);
    reqlen = build_solicit(reqbuf, false);
    memset(&msg, 0, sizeof(msg));
    ogs_assert(smf_dhcpv6_parse(&msg, reqbuf, reqlen) == OGS_OK);
    resplen = smf_dhcpv6_encode_response(resp, sizeof(resp),
            &sess, &msg, DHCPV6_MSG_ADVERTISE, DHCPV6_STATUS_SUCCESS);
    ogs_assert(extract_serverid(resp, resplen, pd_server_duid, &pd_server_duid_len));
}

static void pd_runtime_init(void)
{
    ogs_sockaddr_t *local = NULL;

    if (pd_runtime_ready)
        return;

    ogs_app_context_init();
    ogs_global_conf()->max.ue = 8;
    ogs_app()->pool.sess = 16;
    ogs_app()->pool.bearer = 32;
    ogs_app()->pool.timer = 512;
    ogs_app()->pool.xact = 64;
    ogs_app()->pool.nf = 8;
    ogs_app()->pool.gtp_node = 8;
    ogs_app()->pool.event = 64;
    ogs_app()->metrics.max_specs = 512;
    ogs_app()->timer_mgr = ogs_timer_mgr_create(ogs_app()->pool.timer);
    ogs_assert(ogs_app()->timer_mgr);
    ogs_app()->queue = ogs_queue_create(ogs_app()->pool.event);
    ogs_assert(ogs_app()->queue);

    ogs_local_conf()->time.message.pfcp.t1_response_duration =
            ogs_time_from_sec(60);
    ogs_local_conf()->time.message.pfcp.n1_response_rcount = 1;
    ogs_local_conf()->time.message.pfcp.t1_holding_duration =
            ogs_time_from_sec(60);
    ogs_local_conf()->time.message.pfcp.n1_holding_rcount = 1;

    ogs_pfcp_context_init();
    ogs_pfcp_xact_init();
    ogs_gtp_context_init(8);
    ogs_gtp_xact_init();
    smf_metrics_init();
    smf_context_init();

    ogs_assert(ogs_addaddrinfo(&local, AF_INET, "127.0.0.1", 18805, 0) == OGS_OK);
    ogs_pfcp_self()->pfcp_sock = ogs_udp_server(local, NULL);
    ogs_assert(ogs_pfcp_self()->pfcp_sock);
    ogs_freeaddrinfo(local);

    pd_load_server_duid();
    smf_dhcpv6_test_set_cp_send_hook(pd_capture);
    pd_runtime_ready = 1;
}

static int pd_fix_open(pd_fix_t *fix, bool epc)
{
    ogs_sockaddr_t *addr = NULL;

    memset(fix, 0, sizeof(*fix));
    pd_runtime_init();
    pd_cap_reset();

    fix->subnet = ogs_pfcp_subnet_add(
            "2001:db8:cafe::", "48", "2001:db8:cafe::1", "internet", "lo");
    if (!fix->subnet)
        return -1;
    if (ogs_pfcp_subnet_delegated_prefix_set(
                fix->subnet, "2001:db8:8000::", "48", 56, 0, 0) != OGS_OK)
        return -1;

    if (ogs_addaddrinfo(&addr, AF_INET, "127.0.0.1", 18805, 0) != OGS_OK)
        return -1;
    fix->node = ogs_pfcp_node_new(addr);
    if (!fix->node) {
        ogs_freeaddrinfo(addr);
        return -1;
    }
    fix->node->up_function_features.frrt = 1;

    fix->ue = smf_ue_add_by_supi("imsi-001010000000001");
    if (!fix->ue)
        return -1;
    fix->sess = smf_sess_add_by_psi(fix->ue, 1);
    if (!fix->sess)
        return -1;
    fix->sess->session.name = ogs_strdup("internet");
    fix->sess->epc = epc;
    fix->sess->pfcp_node = fix->node;
    if (!smf_bearer_add(fix->sess))
        return -1;

    fix->ip = ogs_calloc(1, sizeof(*fix->ip));
    ogs_assert(fix->ip);
    fix->ip->subnet = fix->subnet;
    fix->sess->ipv6 = fix->ip;
    return 0;
}

static void pd_fix_close(pd_fix_t *fix)
{
    if (!fix)
        return;
    if (fix->sess) {
        fix->sess->ipv6 = NULL;
        smf_sess_remove(fix->sess);
        fix->sess = NULL;
    }
    if (fix->ip) {
        ogs_free(fix->ip);
        fix->ip = NULL;
    }
    if (fix->ue) {
        smf_ue_remove(fix->ue);
        fix->ue = NULL;
    }
    if (fix->node) {
        ogs_pfcp_node_free(fix->node);
        fix->node = NULL;
    }
    if (fix->subnet) {
        ogs_pfcp_subnet_remove(fix->subnet);
        fix->subnet = NULL;
    }
}

static int pd_xacts(const pd_fix_t *fix)
{
    return ogs_list_count(&fix->node->local_list);
}

static uint8_t *put_be32_test(uint8_t *p, uint32_t v);

static int pd_build_client(uint8_t *buf, uint8_t type, uint32_t iaid,
        const uint8_t *duid, uint16_t duid_len, bool rapid, bool serverid,
        bool ia_pd)
{
    uint8_t *p = buf;

    *p++ = type;
    *p++ = 0x11; *p++ = 0x22; *p++ = 0x33;

    *p++ = 0x00; *p++ = DHCPV6_OPT_CLIENTID;
    *p++ = (uint8_t)(duid_len >> 8);
    *p++ = (uint8_t)(duid_len & 0xff);
    memcpy(p, duid, duid_len);
    p += duid_len;

    if (serverid) {
        *p++ = 0x00; *p++ = DHCPV6_OPT_SERVERID;
        *p++ = (uint8_t)(pd_server_duid_len >> 8);
        *p++ = (uint8_t)(pd_server_duid_len & 0xff);
        memcpy(p, pd_server_duid, pd_server_duid_len);
        p += pd_server_duid_len;
    }

    if (rapid) {
        *p++ = 0x00; *p++ = DHCPV6_OPT_RAPID_COMMIT;
        *p++ = 0x00; *p++ = 0x00;
    }

    if (ia_pd) {
        *p++ = 0x00; *p++ = DHCPV6_OPT_IA_PD;
        *p++ = 0x00; *p++ = 12;
        p = put_be32_test(p, iaid);
        memset(p, 0, 8);
        p += 8;
    }
    return (int)(p - buf);
}

/* Local writer so this builder does not depend on dhcpv6.c statics. */
static uint8_t *put_be32_test(uint8_t *p, uint32_t v)
{
    uint32_t be = htobe32(v);
    memcpy(p, &be, 4);
    return p + 4;
}

static void pd_feed(smf_sess_t *sess, uint8_t type, uint32_t iaid,
        const uint8_t *duid, bool rapid, bool serverid, bool ia_pd)
{
    uint8_t dhcp[256];
    int dlen;
    ogs_pkbuf_t *pkbuf;
    struct ip6_hdr *ip6;
    struct udphdr *udp;
    size_t total;

    dlen = pd_build_client(dhcp, type, iaid, duid, 14, rapid, serverid, ia_pd);
    total = sizeof(*ip6) + sizeof(*udp) + dlen;
    pkbuf = ogs_pkbuf_alloc(NULL, total);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, total);
    memset(pkbuf->data, 0, pkbuf->len);

    ip6 = (struct ip6_hdr *)pkbuf->data;
    ip6->ip6_flow = htobe32(0x60000000);
    ip6->ip6_plen = htobe16(sizeof(*udp) + dlen);
    ip6->ip6_nxt = IPPROTO_UDP;
    ip6->ip6_hlim = 64;
    ip6->ip6_src.s6_addr[0] = 0x20;
    ip6->ip6_src.s6_addr[1] = 0x01;
    ip6->ip6_src.s6_addr[2] = 0x0d;
    ip6->ip6_src.s6_addr[3] = 0xb8;
    ip6->ip6_src.s6_addr[15] = 0x02;
    ip6->ip6_dst.s6_addr[0] = 0xfe;
    ip6->ip6_dst.s6_addr[1] = 0x80;
    ip6->ip6_dst.s6_addr[15] = 0x01;

    udp = (struct udphdr *)(ip6 + 1);
    udp->uh_sport = htobe16(DHCPV6_CLIENT_PORT);
    udp->uh_dport = htobe16(DHCPV6_SERVER_PORT);
    udp->uh_ulen = htobe16(sizeof(*udp) + dlen);
    memcpy(udp + 1, dhcp, dlen);

    smf_dhcpv6_handle(sess, pkbuf);
    ogs_pkbuf_free(pkbuf);
}

static void pd_deliver(pd_fix_t *fix, bool accepted, bool with_cause)
{
    ogs_pfcp_xact_t *xact;
    ogs_pfcp_session_modification_response_t rsp;

    ogs_assert(pd_xacts(fix) == 1);
    xact = ogs_list_first(&fix->node->local_list);
    memset(&rsp, 0, sizeof(rsp));
    if (with_cause) {
        rsp.cause.presence = 1;
        rsp.cause.u8 = accepted ?
            OGS_PFCP_CAUSE_REQUEST_ACCEPTED : OGS_PFCP_CAUSE_REQUEST_REJECTED;
    }

    if (fix->sess->epc)
        smf_epc_n4_handle_session_modification_response(
                fix->sess, xact, NULL, &rsp);
    else
        smf_5gc_n4_handle_session_modification_response(fix->sess, xact, &rsp);

    /* The N4 handler commits the still-open request. Drop it so the
     * response timer cannot complete the lease a second time. */
    if (ogs_list_count(&fix->node->local_list))
        ogs_pfcp_xact_delete_all(fix->node);
}

static void pd_pump_timers(void)
{
    int i;

    for (i = 0; i < 40; i++) {
        usleep(2000);
        ogs_timer_mgr_expire(ogs_app()->timer_mgr);
    }
}

static void dhcpv6_test_handler_rfc8415(abts_case *tc, void *data)
{
    pd_fix_t fix;
    int before;

    ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, false));

    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 1, pd_duid_a,
            false, true, true);
    pd_feed(fix.sess, DHCPV6_MSG_CONFIRM, 1, pd_duid_a,
            false, true, true);
    pd_feed(fix.sess, DHCPV6_MSG_REBIND, 1, pd_duid_a,
            false, true, true);
    pd_feed(fix.sess, DHCPV6_MSG_REQUEST, 1, pd_duid_a,
            false, false, true);
    pd_feed(fix.sess, DHCPV6_MSG_RENEW, 1, pd_duid_a,
            false, false, true);
    pd_feed(fix.sess, DHCPV6_MSG_RELEASE, 1, pd_duid_a,
            false, false, true);
    pd_feed(fix.sess, DHCPV6_MSG_DECLINE, 1, pd_duid_a,
            false, false, true);

    /* Wrong Server Identifier is not this server. Build it by feeding
     * a Decline whose Server DUID is replaced below. */
    {
        uint8_t dhcp[256];
        int dlen, i;
        ogs_pkbuf_t *pkbuf;
        struct ip6_hdr *ip6;
        struct udphdr *udp;
        size_t total;

        dlen = pd_build_client(dhcp, DHCPV6_MSG_RENEW, 1, pd_duid_a,
                14, false, true, true);
        for (i = 0; i + 1 < dlen; i++) {
            if (dhcp[i] == 0x00 && dhcp[i + 1] == DHCPV6_OPT_SERVERID &&
                    i + 4 + pd_server_duid_len <= dlen) {
                memset(dhcp + i + 4, 0x11, pd_server_duid_len);
                break;
            }
        }
        total = sizeof(*ip6) + sizeof(*udp) + dlen;
        pkbuf = ogs_pkbuf_alloc(NULL, total);
        ogs_assert(pkbuf);
        ogs_pkbuf_put(pkbuf, total);
        memset(pkbuf->data, 0, pkbuf->len);
        ip6 = (struct ip6_hdr *)pkbuf->data;
        ip6->ip6_flow = htobe32(0x60000000);
        ip6->ip6_plen = htobe16(sizeof(*udp) + dlen);
        ip6->ip6_nxt = IPPROTO_UDP;
        ip6->ip6_hlim = 64;
        ip6->ip6_src.s6_addr[0] = 0x20;
        ip6->ip6_src.s6_addr[1] = 0x01;
        ip6->ip6_src.s6_addr[2] = 0x0d;
        ip6->ip6_src.s6_addr[3] = 0xb8;
        ip6->ip6_src.s6_addr[15] = 0x02;
        udp = (struct udphdr *)(ip6 + 1);
        udp->uh_sport = htobe16(DHCPV6_CLIENT_PORT);
        udp->uh_dport = htobe16(DHCPV6_SERVER_PORT);
        udp->uh_ulen = htobe16(sizeof(*udp) + dlen);
        memcpy(udp + 1, dhcp, dlen);
        smf_dhcpv6_handle(fix.sess, pkbuf);
        ogs_pkbuf_free(pkbuf);
    }

    /* Confirm carries IA_PD and no Server Identifier: still not a PD op. */
    pd_feed(fix.sess, DHCPV6_MSG_CONFIRM, 1, pd_duid_a,
            false, false, true);

    before = pd_cap_n;
    ABTS_INT_EQUAL(tc, 0, before);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_NONE, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    pd_fix_close(&fix);
}

static void dhcpv6_test_defer_accept_5gc(abts_case *tc, void *data)
{
    pd_fix_t fix;

    ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, false));

    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x11111111, pd_duid_a,
            true, false, true);
    ABTS_INT_EQUAL(tc, 0, pd_cap_n);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_PENDING, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 1, pd_xacts(&fix));

    pd_deliver(&fix, true, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, 1, pd_cap[0].rapid);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_SUCCESS, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_ACTIVE, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    /* Renew/Rebind of an installed lease do not open another modification. */
    pd_cap_reset();
    pd_feed(fix.sess, DHCPV6_MSG_RENEW, 0x11111111, pd_duid_a,
            false, true, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_SUCCESS, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    pd_cap_reset();
    pd_feed(fix.sess, DHCPV6_MSG_REBIND, 0x11111111, pd_duid_a,
            false, false, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_SUCCESS, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_ACTIVE, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    pd_fix_close(&fix);
}

static void dhcpv6_test_defer_reject_epc(abts_case *tc, void *data)
{
    pd_fix_t fix;

    ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, true));

    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x22222222, pd_duid_a,
            false, false, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_ADVERTISE, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, 0, pd_cap[0].rapid);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_SUCCESS, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_OFFERED, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    pd_feed(fix.sess, DHCPV6_MSG_REQUEST, 0x22222222, pd_duid_a,
            false, true, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_PENDING, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 1, pd_xacts(&fix));

    pd_deliver(&fix, false, true);
    ABTS_INT_EQUAL(tc, 2, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, pd_cap[1].type);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_NOPREFIX_AVAIL, pd_cap[1].status);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_NONE, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    pd_fix_close(&fix);
}

static void dhcpv6_test_pfcp_cause_required(abts_case *tc, void *data)
{
    pd_fix_t fix;

    ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, false));
    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x33333333, pd_duid_a,
            true, false, true);
    ABTS_INT_EQUAL(tc, 0, pd_cap_n);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_PENDING, fix.sess->pd_lease.state);

    /* No Cause IE is not acceptance. */
    pd_deliver(&fix, false, false);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_ADVERTISE, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_NOPREFIX_AVAIL, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_NONE, fix.sess->pd_lease.state);

    pd_fix_close(&fix);
}

static void dhcpv6_test_defer_timeout(abts_case *tc, void *data)
{
    int epc;

    for (epc = 0; epc <= 1; epc++) {
        pd_fix_t fix;

        ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, epc ? true : false));
        ogs_local_conf()->time.message.pfcp.t1_response_duration = 1000;
        ogs_local_conf()->time.message.pfcp.n1_response_rcount = 1;

        pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x44444444, pd_duid_a,
                true, false, true);
        ABTS_INT_EQUAL(tc, 0, pd_cap_n);
        ABTS_INT_EQUAL(tc, SMF_PD_LEASE_PENDING, fix.sess->pd_lease.state);
        ABTS_INT_EQUAL(tc, 1, pd_xacts(&fix));

        pd_pump_timers();
        ABTS_INT_EQUAL(tc, 1, pd_cap_n);
        ABTS_INT_EQUAL(tc, DHCPV6_MSG_ADVERTISE, pd_cap[0].type);
        ABTS_INT_EQUAL(tc, DHCPV6_STATUS_NOPREFIX_AVAIL, pd_cap[0].status);
        ABTS_INT_EQUAL(tc, SMF_PD_LEASE_NONE, fix.sess->pd_lease.state);
        ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

        ogs_local_conf()->time.message.pfcp.t1_response_duration =
                ogs_time_from_sec(60);
        pd_fix_close(&fix);
    }
}

static void dhcpv6_test_one_outstanding(abts_case *tc, void *data)
{
    pd_fix_t fix;
    uint8_t prefix[OGS_IPV6_LEN];

    ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, false));
    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x55555555, pd_duid_a,
            true, false, true);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_PENDING, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 1, pd_xacts(&fix));
    ABTS_INT_EQUAL(tc, 0, pd_cap_n);
    memcpy(prefix, fix.sess->pd_lease.prefix, sizeof(prefix));

    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x66666666, pd_duid_b,
            true, false, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_ADVERTISE, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_NOPREFIX_AVAIL, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, 1, pd_xacts(&fix));
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_PENDING, fix.sess->pd_lease.state);
    ABTS_TRUE(tc, memcmp(prefix, fix.sess->pd_lease.prefix, sizeof(prefix)) == 0);
    ABTS_INT_EQUAL(tc, 0x55555555, fix.sess->pd_lease.iaid);

    pd_cap_reset();
    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x55555555, pd_duid_a,
            true, false, true);
    ABTS_INT_EQUAL(tc, 0, pd_cap_n);
    ABTS_INT_EQUAL(tc, 1, pd_xacts(&fix));

    pd_deliver(&fix, true, true);
    ABTS_INT_EQUAL(tc, 1, pd_cap_n);
    ABTS_INT_EQUAL(tc, DHCPV6_MSG_REPLY, pd_cap[0].type);
    ABTS_INT_EQUAL(tc, 1, pd_cap[0].rapid);
    ABTS_INT_EQUAL(tc, DHCPV6_STATUS_SUCCESS, pd_cap[0].status);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_ACTIVE, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));

    pd_fix_close(&fix);
}

static void dhcpv6_test_confirm_decline_ignored(abts_case *tc, void *data)
{
    pd_fix_t fix;
    uint8_t prefix[OGS_IPV6_LEN];

    ABTS_INT_EQUAL(tc, 0, pd_fix_open(&fix, true));
    pd_feed(fix.sess, DHCPV6_MSG_SOLICIT, 0x77777777, pd_duid_a,
            true, false, true);
    pd_deliver(&fix, true, true);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_ACTIVE, fix.sess->pd_lease.state);
    memcpy(prefix, fix.sess->pd_lease.prefix, sizeof(prefix));

    pd_cap_reset();
    pd_feed(fix.sess, DHCPV6_MSG_DECLINE, 0x77777777, pd_duid_a,
            false, true, true);
    pd_feed(fix.sess, DHCPV6_MSG_CONFIRM, 0x77777777, pd_duid_a,
            false, false, true);
    ABTS_INT_EQUAL(tc, 0, pd_cap_n);
    ABTS_INT_EQUAL(tc, SMF_PD_LEASE_ACTIVE, fix.sess->pd_lease.state);
    ABTS_INT_EQUAL(tc, 0, pd_xacts(&fix));
    ABTS_TRUE(tc, memcmp(prefix, fix.sess->pd_lease.prefix, sizeof(prefix)) == 0);

    pd_fix_close(&fix);
}

abts_suite *test_dhcpv6(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, dhcpv6_test_parse_solicit, NULL);
    abts_run_test(suite, dhcpv6_test_parse_request, NULL);
    abts_run_test(suite, dhcpv6_test_parse_malformed, NULL);
    abts_run_test(suite, dhcpv6_test_pool, NULL);
    abts_run_test(suite, dhcpv6_test_pool_validation, NULL);
    abts_run_test(suite, dhcpv6_test_rapid_commit_reply, NULL);
    abts_run_test(suite, dhcpv6_test_rfc8415_serverid, NULL);
    abts_run_test(suite, dhcpv6_test_rfc8415_ia_pd, NULL);
    abts_run_test(suite, dhcpv6_test_pd_lease_pending, NULL);
    abts_run_test(suite, dhcpv6_test_handler_rfc8415, NULL);
    abts_run_test(suite, dhcpv6_test_defer_accept_5gc, NULL);
    abts_run_test(suite, dhcpv6_test_defer_reject_epc, NULL);
    abts_run_test(suite, dhcpv6_test_pfcp_cause_required, NULL);
    abts_run_test(suite, dhcpv6_test_defer_timeout, NULL);
    abts_run_test(suite, dhcpv6_test_one_outstanding, NULL);
    abts_run_test(suite, dhcpv6_test_confirm_decline_ignored, NULL);

    return suite;
}
