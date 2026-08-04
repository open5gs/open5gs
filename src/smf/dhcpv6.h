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

#ifndef SMF_DHCPV6_H
#define SMF_DHCPV6_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DHCPv6 Message Types (RFC 8415 Section 7.3) */
#define DHCPV6_MSG_SOLICIT          1
#define DHCPV6_MSG_ADVERTISE        2
#define DHCPV6_MSG_REQUEST          3
#define DHCPV6_MSG_CONFIRM          4
#define DHCPV6_MSG_RENEW            5
#define DHCPV6_MSG_REBIND           6
#define DHCPV6_MSG_REPLY            7
#define DHCPV6_MSG_RELEASE          8
#define DHCPV6_MSG_DECLINE          9

/* DHCPv6 Option Codes (RFC 8415 Section 21) */
#define DHCPV6_OPT_CLIENTID         1
#define DHCPV6_OPT_SERVERID         2
#define DHCPV6_OPT_ORO              6
#define DHCPV6_OPT_ELAPSED_TIME     8
#define DHCPV6_OPT_STATUS_CODE      13
#define DHCPV6_OPT_RAPID_COMMIT     14
#define DHCPV6_OPT_IA_PD            25
#define DHCPV6_OPT_IAPREFIX         26

/* DHCPv6 Status Codes (RFC 8415 Section 21.13) */
#define DHCPV6_STATUS_SUCCESS       0
#define DHCPV6_STATUS_UNSPEC_FAIL   1
#define DHCPV6_STATUS_NOADDRS_AVAIL 2
#define DHCPV6_STATUS_NOBINDING     3
#define DHCPV6_STATUS_NOTONLINK     4
#define DHCPV6_STATUS_USEMULTICAST  5
#define DHCPV6_STATUS_NOPREFIX_AVAIL 6

#define DHCPV6_CLIENT_PORT          546
#define DHCPV6_SERVER_PORT          547

#define DHCPV6_MAX_DUID_LEN         130 /* RFC 8415 Section 11 : 128 + type */

typedef struct smf_dhcpv6_message_s {
    uint8_t         type;
    uint8_t         xid[3];

    const uint8_t   *duid;          /* Client Identifier (points into pkt) */
    uint16_t        duid_len;

    bool            serverid_present;
    const uint8_t   *server_duid;   /* (points into pkt) */
    uint16_t        server_duid_len;

    bool            ia_pd_present;
    uint32_t        iaid;
    uint32_t        t1;
    uint32_t        t2;

    /* First IAPREFIX option inside IA_PD (for Request/Release matching) */
    bool            ia_prefix_present;
    uint8_t         prefix_plen;
    uint8_t         prefix[OGS_IPV6_LEN];

    bool            rapid_commit;

    /* Source IPv6 address of the client message (reply destination) */
    uint8_t         client_addr[OGS_IPV6_LEN];
} smf_dhcpv6_message_t;

void smf_dhcpv6_init(void);

/* True if the (GTP-U decapsulated) packet is a DHCPv6 client message */
bool smf_dhcpv6_check(ogs_pkbuf_t *pkbuf);

/* Handle a DHCPv6 client message for the session (does not free pkbuf) */
void smf_dhcpv6_handle(smf_sess_t *sess, ogs_pkbuf_t *pkbuf);

/* Exposed for unit tests */
int smf_dhcpv6_parse(
        smf_dhcpv6_message_t *msg, const uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* SMF_DHCPV6_H */
