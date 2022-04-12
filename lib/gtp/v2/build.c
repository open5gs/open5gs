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

#include "ogs-gtp.h"

ogs_pkbuf_t *ogs_gtp2_build_echo_request(
        uint8_t type, uint8_t recovery, uint8_t features)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_echo_request_t *req = NULL;

    req = &gtp_message.echo_request;
    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    req->recovery.presence = 1;
    req->recovery.u8 = recovery;

    req->sending_node_features.presence = 1;
    req->sending_node_features.u8 = features;

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *ogs_gtp2_build_echo_response(
        uint8_t type, uint8_t recovery, uint8_t features)
{
    ogs_gtp2_message_t gtp_message;
    ogs_gtp2_echo_response_t *rsp = NULL;

    rsp = &gtp_message.echo_response;
    memset(&gtp_message, 0, sizeof(ogs_gtp2_message_t));

    rsp->recovery.presence = 1;
    rsp->recovery.u8 = recovery;

    rsp->sending_node_features.presence = 1;
    rsp->sending_node_features.u8 = features;

    gtp_message.h.type = type;
    return ogs_gtp2_build_msg(&gtp_message);
}

ogs_pkbuf_t *ogs_gtp2_build_error_indication(
        uint32_t teid, ogs_sockaddr_t *addr)
{
    ogs_pkbuf_t *pkbuf = NULL;
    unsigned char *p = NULL;
    int family;

    ogs_assert(addr);

    pkbuf = ogs_pkbuf_alloc(
            NULL, 100 /* enough for Error Indiciation; use smaller buffer */);
    ogs_expect_or_return_val(pkbuf, NULL);
    ogs_pkbuf_reserve(pkbuf, OGS_GTPV1U_5GC_HEADER_LEN);

    /*
     * 8.3 Tunnel Endpoint Identifier Data I
     *
     * Octet 1 : Type = 16 (Decimal)
     * Octet 2-5 : Tunnel Endpoint Identitifer Data I
     */
    ogs_pkbuf_put_u8(pkbuf, 16);
    ogs_pkbuf_put_u32(pkbuf, teid);

    /*
     * 8.4 GTP-U Peer Address
     *
     * Octet 1 : Type = 133 (Decimal)
     * Octet 2-3 : Length
     * Octet 4-n : IPv4 or IPv6 Address
     */
    ogs_pkbuf_put_u8(pkbuf, 133);

    family = addr->ogs_sa_family;
    switch(family) {
    case AF_INET:
        ogs_pkbuf_put_u16(pkbuf, OGS_IPV4_LEN);
        p = ogs_pkbuf_put(pkbuf, OGS_IPV4_LEN);
        memcpy(p, &addr->sin.sin_addr, OGS_IPV4_LEN);
        break;
    case AF_INET6:
        ogs_pkbuf_put_u16(pkbuf, OGS_IPV6_LEN);
        p = ogs_pkbuf_put(pkbuf, OGS_IPV6_LEN);
        memcpy(p, &addr->sin6.sin6_addr, OGS_IPV6_LEN);
        break;
    default:
        ogs_fatal("Unknown family(%d)", family);
        ogs_abort();
        return NULL;
    }

    return pkbuf;
}
