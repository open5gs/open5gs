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

/*
 * DHCPv6 Prefix Delegation (IA_PD) server
 *
 * 3GPP TS 23.401 Section 5.3.1.2.6 / RFC 6459 Section 5.3 :
 * the UE sends DHCPv6 messages over the established bearer; the P-GW
 * (SMF in Open5GS) delegates an additional prefix and installs a route
 * for it towards the session.
 *
 * DHCPv6 client messages (UDP dst port 547) are punted from the UPF to
 * the SMF through the CP-function forwarding path, the same mechanism
 * used for ICMPv6 Router Solicitations. Replies are sent down the
 * CP2UP path. The delegated prefix is installed in the UPF as a
 * Framed-IPv6-Route on dedicated PDRs via PFCP Session Modification.
 */

#define _DEFAULT_SOURCE 1
#define _BSD_SOURCE     1

#include "dhcpv6.h"
#include "gtp-path.h"

#include <time.h>

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IP6_H
#include <netinet/ip6.h>
#endif

#if HAVE_NETINET_UDP_H
#include <netinet/udp.h>
#endif

/* Server DUID : DUID-LLT (type 1, hw Ethernet, time, 6 octets) */
#define SERVER_DUID_LEN 14
static uint8_t server_duid[SERVER_DUID_LEN];
static uint16_t server_duid_len = 0;

void smf_dhcpv6_init(void)
{
    uint16_t v16;
    uint32_t v32;
    uint8_t *p = server_duid;

    v16 = htobe16(1);               /* DUID-LLT */
    memcpy(p, &v16, 2); p += 2;
    v16 = htobe16(1);               /* Hardware type : Ethernet */
    memcpy(p, &v16, 2); p += 2;
    v32 = htobe32((uint32_t)(time(NULL) - 946684800)); /* since 2000-01-01 */
    memcpy(p, &v32, 4); p += 4;
    ogs_random(p, 6);               /* Link-layer address (random) */
    p += 6;

    server_duid_len = p - server_duid;
    ogs_assert(server_duid_len == SERVER_DUID_LEN);
}

static const char *dhcpv6_message_name(uint8_t type)
{
    switch (type) {
    case DHCPV6_MSG_SOLICIT: return "SOLICIT";
    case DHCPV6_MSG_ADVERTISE: return "ADVERTISE";
    case DHCPV6_MSG_REQUEST: return "REQUEST";
    case DHCPV6_MSG_CONFIRM: return "CONFIRM";
    case DHCPV6_MSG_RENEW: return "RENEW";
    case DHCPV6_MSG_REBIND: return "REBIND";
    case DHCPV6_MSG_REPLY: return "REPLY";
    case DHCPV6_MSG_RELEASE: return "RELEASE";
    case DHCPV6_MSG_DECLINE: return "DECLINE";
    default: return "UNKNOWN";
    }
}

static const char *dhcpv6_status_name(uint16_t code)
{
    switch (code) {
    case DHCPV6_STATUS_SUCCESS: return "Success";
    case DHCPV6_STATUS_UNSPEC_FAIL: return "UnspecFail";
    case DHCPV6_STATUS_NOADDRS_AVAIL: return "NoAddrsAvail";
    case DHCPV6_STATUS_NOBINDING: return "NoBinding";
    case DHCPV6_STATUS_NOTONLINK: return "NotOnLink";
    case DHCPV6_STATUS_USEMULTICAST: return "UseMulticast";
    case DHCPV6_STATUS_NOPREFIX_AVAIL: return "NoPrefixAvail";
    default: return "Unknown";
    }
}

static uint32_t get_be32(const uint8_t *p)
{
    uint32_t v;
    memcpy(&v, p, 4);
    return be32toh(v);
}

static uint8_t *put_be16(uint8_t *p, uint16_t v)
{
    uint16_t be = htobe16(v);
    memcpy(p, &be, 2);
    return p + 2;
}

static uint8_t *put_be32(uint8_t *p, uint32_t v)
{
    uint32_t be = htobe32(v);
    memcpy(p, &be, 4);
    return p + 4;
}

static uint8_t *put_option_header(uint8_t *p, uint16_t code, uint16_t len)
{
    p = put_be16(p, code);
    return put_be16(p, len);
}

int smf_dhcpv6_parse(
        smf_dhcpv6_message_t *msg, const uint8_t *data, uint32_t len)
{
    const uint8_t *p = data, *end = data + len;

    ogs_assert(msg);
    ogs_assert(data);

    if (len < 4)
        return OGS_ERROR;

    memset(msg, 0, sizeof(*msg));

    msg->type = p[0];
    memcpy(msg->xid, p + 1, 3);
    p += 4;

    while (p + 4 <= end) {
        uint16_t code = ((uint16_t)p[0] << 8) | p[1];
        uint16_t olen = ((uint16_t)p[2] << 8) | p[3];
        const uint8_t *odata = p + 4;

        if (odata + olen > end)
            return OGS_ERROR;

        switch (code) {
        case DHCPV6_OPT_CLIENTID:
            msg->duid = odata;
            msg->duid_len = olen;
            break;
        case DHCPV6_OPT_SERVERID:
            msg->serverid_present = true;
            msg->server_duid = odata;
            msg->server_duid_len = olen;
            break;
        case DHCPV6_OPT_RAPID_COMMIT:
            msg->rapid_commit = true;
            break;
        case DHCPV6_OPT_IA_PD: {
            const uint8_t *sp = NULL, *send = NULL;

            if (olen < 12)
                return OGS_ERROR;

            msg->ia_pd_present = true;
            msg->iaid = get_be32(odata);
            msg->t1 = get_be32(odata + 4);
            msg->t2 = get_be32(odata + 8);

            /* Nested options : look for the first IAPREFIX */
            sp = odata + 12;
            send = odata + olen;
            while (sp + 4 <= send) {
                uint16_t scode = ((uint16_t)sp[0] << 8) | sp[1];
                uint16_t slen = ((uint16_t)sp[2] << 8) | sp[3];
                const uint8_t *sdata = sp + 4;

                if (sdata + slen > send)
                    return OGS_ERROR;

                if (scode == DHCPV6_OPT_IAPREFIX &&
                        !msg->ia_prefix_present) {
                    if (slen < 25)
                        return OGS_ERROR;
                    msg->ia_prefix_present = true;
                    msg->prefix_plen = sdata[8];
                    memcpy(msg->prefix, sdata + 9, OGS_IPV6_LEN);
                }

                sp = sdata + slen;
            }
            break;
        }
        default:
            break;
        }

        p = odata + olen;
    }

    if (p != end)
        return OGS_ERROR;

    return OGS_OK;
}

static void get_link_local_src(ogs_ipsubnet_t *src_ipsub)
{
    int rv;
    char ipstr[OGS_ADDRSTRLEN];

    ogs_assert(src_ipsub);

    /* Fetch link-local address (same rule as Router Advertisement) */
    if (ogs_gtp_self()->link_local_addr) {
        OGS_ADDR(ogs_gtp_self()->link_local_addr, ipstr);
        rv = ogs_ipsubnet(src_ipsub, ipstr, NULL);
        if (rv == OGS_OK)
            return;
        ogs_error("ogs_ipsubnet() failed");
    }

    /* For the case of loopback used for GTPU link-local address is not
     * available, hence set the source IP to fe80::1 */
    memset(src_ipsub->sub, 0, sizeof(src_ipsub->sub));
    src_ipsub->sub[0] = htobe32(0xfe800000);
    src_ipsub->sub[3] = htobe32(0x00000001);
}

/*
 * Build an ADVERTISE or REPLY.
 *
 * On DHCPV6_STATUS_SUCCESS the current lease's IAPREFIX is included;
 * otherwise a STATUS_CODE option is carried inside the IA_PD instead.
 */
static ogs_pkbuf_t *build_response(smf_sess_t *sess,
        smf_dhcpv6_message_t *req, uint8_t type, uint16_t status)
{
    ogs_pkbuf_t *pkbuf = NULL;

    uint8_t dhcp[256];
    uint8_t *p = dhcp;
    uint8_t *ia_pd_len_p = NULL;
    uint16_t ia_pd_len = 0;
    size_t dhcp_len = 0;

    struct ip6_hdr *ip6_h = NULL;
    struct udphdr *udp_h = NULL;
    ogs_ipsubnet_t src_ipsub;
    const uint8_t *ip6_dst = NULL;
    uint16_t udp_len = 0;

    ogs_assert(sess);
    ogs_assert(req);
    ogs_assert(req->duid);
    ogs_assert(req->duid_len <= DHCPV6_MAX_DUID_LEN);

    ogs_assert(server_duid_len == SERVER_DUID_LEN);

    if (status == DHCPV6_STATUS_SUCCESS)
        ogs_assert(sess->pd_lease.active);

    /* Worst-case bound before writing anything */
    {
        const char *message = dhcpv6_status_name(status);
        size_t need = 4 +
            (4 + server_duid_len) + (4 + req->duid_len) +
            (4 + 12) +
            (status == DHCPV6_STATUS_SUCCESS ?
                (4 + 25) : (4 + 2 + strlen(message)));

        ogs_assert(need <= sizeof(dhcp));
    }

    /* DHCPv6 message */
    *p++ = type;
    memcpy(p, req->xid, 3); p += 3;

    p = put_option_header(p, DHCPV6_OPT_SERVERID, server_duid_len);
    memcpy(p, server_duid, server_duid_len); p += server_duid_len;

    p = put_option_header(p, DHCPV6_OPT_CLIENTID, req->duid_len);
    memcpy(p, req->duid, req->duid_len); p += req->duid_len;

    /* IA_PD : IAID + T1 + T2 + nested options */
    ia_pd_len_p = p + 2; /* length filled in below */
    p = put_option_header(p, DHCPV6_OPT_IA_PD, 0);
    p = put_be32(p, req->iaid);
    /* T1/T2 from the preferred lifetime (uint64 to avoid overflow) */
    p = put_be32(p, (uint32_t)
            ((uint64_t)sess->pd_lease.preferred_lifetime / 2));
    p = put_be32(p, (uint32_t)
            ((uint64_t)sess->pd_lease.preferred_lifetime * 4 / 5));

    if (status == DHCPV6_STATUS_SUCCESS) {
        p = put_option_header(p, DHCPV6_OPT_IAPREFIX, 25);
        p = put_be32(p, sess->pd_lease.preferred_lifetime);
        p = put_be32(p, sess->pd_lease.valid_lifetime);
        *p++ = sess->pd_lease.plen;
        memcpy(p, sess->pd_lease.prefix, OGS_IPV6_LEN);
        p += OGS_IPV6_LEN;
    } else {
        const char *message = dhcpv6_status_name(status);
        size_t mlen = strlen(message);

        p = put_option_header(p, DHCPV6_OPT_STATUS_CODE, 2 + mlen);
        p = put_be16(p, status);
        memcpy(p, message, mlen); p += mlen;
    }

    ia_pd_len = (p - ia_pd_len_p) - 2;
    put_be16(ia_pd_len_p, ia_pd_len);

    dhcp_len = p - dhcp;
    ogs_assert(dhcp_len <= sizeof(dhcp));

    /* IPv6 + UDP wrapper */
    udp_len = sizeof(struct udphdr) + dhcp_len;

    pkbuf = ogs_pkbuf_alloc(NULL,
            OGS_GTPV1U_5GC_HEADER_LEN + sizeof(struct ip6_hdr) + udp_len);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_GTPV1U_5GC_HEADER_LEN);
    ogs_pkbuf_put(pkbuf, sizeof(struct ip6_hdr) + udp_len);
    memset(pkbuf->data, 0, pkbuf->len);

    ip6_h = (struct ip6_hdr *)pkbuf->data;
    udp_h = (struct udphdr *)((uint8_t *)ip6_h + sizeof *ip6_h);
    memcpy((uint8_t *)udp_h + sizeof *udp_h, dhcp, dhcp_len);

    udp_h->uh_sport = htobe16(DHCPV6_SERVER_PORT);
    udp_h->uh_dport = htobe16(DHCPV6_CLIENT_PORT);
    udp_h->uh_ulen = htobe16(udp_len);
    udp_h->uh_sum = 0;

    get_link_local_src(&src_ipsub);
    ip6_dst = req->client_addr;

    /* UDP checksum over the pseudo-header (built in place,
     * overlapped with the IPv6 header area) */
    {
        uint8_t *q = (uint8_t *)pkbuf->data;
        uint32_t ul32 = htobe32((uint32_t)udp_len);

        memcpy(q, src_ipsub.sub, sizeof src_ipsub.sub);
        q += sizeof src_ipsub.sub;
        memcpy(q, ip6_dst, OGS_IPV6_LEN);
        q += OGS_IPV6_LEN;
        memcpy(q, &ul32, 4); q += 4;
        q += 3; *q = IPPROTO_UDP;

        udp_h->uh_sum = ogs_in_cksum((uint16_t *)pkbuf->data, pkbuf->len);
        if (udp_h->uh_sum == 0)
            udp_h->uh_sum = 0xffff; /* RFC 768 : zero transmitted as 0xffff */
    }

    ip6_h->ip6_flow = htobe32(0x60000000);
    ip6_h->ip6_plen = htobe16(udp_len);
    ip6_h->ip6_nxt = IPPROTO_UDP;
    ip6_h->ip6_hlim = 64;
    memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
    memcpy(ip6_h->ip6_dst.s6_addr, ip6_dst, OGS_IPV6_LEN);

    return pkbuf;
}

static void send_response(smf_sess_t *sess, smf_dhcpv6_message_t *req,
        uint8_t type, uint16_t status)
{
    ogs_pkbuf_t *pkbuf = NULL;

    pkbuf = build_response(sess, req, type, status);
    if (!pkbuf) {
        ogs_error("build_response() failed");
        return;
    }

    ogs_debug("      Send DHCPv6 %s [%s]",
            dhcpv6_message_name(type), dhcpv6_status_name(status));

    smf_gtp_send_cp_packet_to_ue(sess, pkbuf);
}

bool smf_dhcpv6_check(ogs_pkbuf_t *pkbuf)
{
    struct ip *ip_h = NULL;
    struct ip6_hdr *ip6_h = NULL;
    struct udphdr *udp_h = NULL;

    ogs_assert(pkbuf);

    if (pkbuf->len < sizeof(*ip6_h) + sizeof(*udp_h))
        return false;

    ip_h = (struct ip *)pkbuf->data;
    if (ip_h->ip_v != 6)
        return false;

    ip6_h = (struct ip6_hdr *)pkbuf->data;
    if (ip6_h->ip6_nxt != IPPROTO_UDP)
        return false;

    udp_h = (struct udphdr *)((uint8_t *)ip6_h + sizeof *ip6_h);
    if (udp_h->uh_dport != htobe16(DHCPV6_SERVER_PORT))
        return false;

    return true;
}

void smf_dhcpv6_handle(smf_sess_t *sess, ogs_pkbuf_t *pkbuf)
{
    smf_dhcpv6_message_t msg;

    struct ip6_hdr *ip6_h = NULL;
    struct udphdr *udp_h = NULL;
    const uint8_t *dhcp = NULL;
    uint32_t dhcp_len = 0;

    ogs_pfcp_subnet_t *subnet = NULL;

    ogs_assert(sess);
    ogs_assert(pkbuf);

    if (!sess->ipv6)
        return;
    subnet = sess->ipv6->subnet;
    if (!subnet || !subnet->delegated_prefix.bitmap)
        return;

    ip6_h = (struct ip6_hdr *)pkbuf->data;
    if (ip6_h->ip6_nxt != IPPROTO_UDP)
        return;

    /* Cannot reply to a multicast or unspecified source */
    if (ip6_h->ip6_src.s6_addr[0] == 0xff)
        return;

    if (pkbuf->len < sizeof(*ip6_h) + sizeof(*udp_h))
        return;
    udp_h = (struct udphdr *)((uint8_t *)ip6_h + sizeof(*ip6_h));

    dhcp = (const uint8_t *)(udp_h + 1);
    if (dhcp > pkbuf->data + pkbuf->len)
        return;
    dhcp_len = (pkbuf->data + pkbuf->len) - dhcp;

    memset(&msg, 0, sizeof(msg));
    if (smf_dhcpv6_parse(&msg, dhcp, dhcp_len) != OGS_OK) {
        ogs_error("Cannot parse DHCPv6 message");
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        return;
    }

    /* Client Identifier is required in all client messages */
    if (!msg.duid || msg.duid_len == 0 ||
            msg.duid_len > DHCPV6_MAX_DUID_LEN)
        return;

    /* Discard messages addressed to a different server */
    if (msg.serverid_present) {
        if (msg.server_duid_len != server_duid_len ||
                memcmp(msg.server_duid, server_duid, server_duid_len) != 0) {
            ogs_debug("DHCPv6 %s for another server : dropped",
                    dhcpv6_message_name(msg.type));
            return;
        }
    }

    ogs_debug("DHCPv6 %s [IAID:0x%x%s]",
            dhcpv6_message_name(msg.type), msg.iaid,
            msg.rapid_commit ? " RapidCommit" : "");

    /* Remember where the reply must go */
    memcpy(msg.client_addr, ip6_h->ip6_src.s6_addr, OGS_IPV6_LEN);

    switch (msg.type) {
    case DHCPV6_MSG_SOLICIT:
        if (!msg.ia_pd_present) {
            /* We only serve Prefix Delegation (no IA_NA/IA_TA) */
            ogs_debug("DHCPv6 Solicit without IA_PD : not served");
            break;
        }
        if (smf_sess_pd_lease_grant(
                    sess, msg.iaid, msg.duid, msg.duid_len) != OGS_OK) {
            send_response(sess, &msg,
                    DHCPV6_MSG_ADVERTISE, DHCPV6_STATUS_NOPREFIX_AVAIL);
        } else if (msg.rapid_commit) {
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
        } else {
            send_response(sess, &msg,
                    DHCPV6_MSG_ADVERTISE, DHCPV6_STATUS_SUCCESS);
        }
        break;

    case DHCPV6_MSG_REQUEST:
    case DHCPV6_MSG_RENEW:
    case DHCPV6_MSG_REBIND:
        if (!msg.ia_pd_present)
            break;
        if (!smf_sess_pd_lease_matches(
                    sess, msg.iaid, msg.duid, msg.duid_len) ||
                (msg.ia_prefix_present &&
                 (msg.prefix_plen != sess->pd_lease.plen ||
                  memcmp(msg.prefix, sess->pd_lease.prefix,
                      OGS_IPV6_LEN) != 0))) {
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_NOBINDING);
            break;
        }
        smf_sess_pd_lease_refresh(sess);
        send_response(sess, &msg,
                DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
        break;

    case DHCPV6_MSG_CONFIRM:
        if (msg.ia_pd_present &&
                smf_sess_pd_lease_matches(
                    sess, msg.iaid, msg.duid, msg.duid_len)) {
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
        } else {
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_NOTONLINK);
        }
        break;

    case DHCPV6_MSG_RELEASE:
        if (!msg.ia_pd_present)
            break;
        if (!smf_sess_pd_lease_matches(
                    sess, msg.iaid, msg.duid, msg.duid_len)) {
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_NOBINDING);
            break;
        }
        if (msg.ia_prefix_present &&
                msg.prefix_plen == sess->pd_lease.plen &&
                memcmp(msg.prefix, sess->pd_lease.prefix, OGS_IPV6_LEN) == 0) {
            /* Reply while the lease is still intact, then release */
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
            smf_sess_pd_lease_release(sess);
        } else {
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_NOBINDING);
        }
        break;

    case DHCPV6_MSG_DECLINE:
        if (msg.ia_pd_present &&
                smf_sess_pd_lease_matches(
                    sess, msg.iaid, msg.duid, msg.duid_len)) {
            ogs_warn("DHCPv6 client declined the delegated prefix");
            /* Reply while the lease is still intact, then release */
            send_response(sess, &msg,
                    DHCPV6_MSG_REPLY, DHCPV6_STATUS_SUCCESS);
            smf_sess_pd_lease_release(sess);
        }
        /* No binding : no Reply */
        break;

    default:
        ogs_debug("DHCPv6 message type [%d] not served", msg.type);
        break;
    }
}
