/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

ogs_pkbuf_t *ogs_gtp1_build_error_indication(
        uint32_t teid, ogs_sockaddr_t *addr)
{
    ogs_pkbuf_t *pkbuf = NULL;
    unsigned char *p = NULL;
    int family;

    ogs_assert(addr);

    pkbuf = ogs_pkbuf_alloc(
            NULL, 100 /* enough for Error Indiciation; use smaller buffer */);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }
    ogs_pkbuf_reserve(pkbuf,
            OGS_GTPV1U_HEADER_LEN + /* 8 bytes */
            4 + /* Seq Number(2) + N PDU Number(1) + Ext Header Type(1) */
            4 + /* If 5GC, QFI Extension Header(4) */
            4); /* UDP Port Extension Header(4) */

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

void ogs_gtp2_fill_header(
        ogs_gtp2_header_t *gtp_hdesc, ogs_gtp2_extension_header_t *ext_hdesc,
        ogs_pkbuf_t *pkbuf)
{
    ogs_gtp2_header_t *gtp_h = NULL;
    uint8_t flags;
    uint8_t gtp_hlen = 0;
    int i;

    ogs_assert(gtp_hdesc);
    ogs_assert(ext_hdesc);
    ogs_assert(pkbuf);

    /* Processing GTP Flags */
    flags = gtp_hdesc->flags;
    flags |= OGS_GTPU_FLAGS_V | OGS_GTPU_FLAGS_PT;
    if (ext_hdesc->array[0].type && ext_hdesc->array[0].len)
        flags |= OGS_GTPU_FLAGS_E;

    /* Define GTP Header Size */
    if (flags & OGS_GTPU_FLAGS_E) {

        gtp_hlen = OGS_GTPV1U_HEADER_LEN+OGS_GTPV1U_EXTENSION_HEADER_LEN;

        i = 0;
        while(ext_hdesc->array[i].len) {
            gtp_hlen += (ext_hdesc->array[i].len*4);
            i++;
        }

    } else if (flags & (OGS_GTPU_FLAGS_S|OGS_GTPU_FLAGS_PN))
        gtp_hlen = OGS_GTPV1U_HEADER_LEN+OGS_GTPV1U_EXTENSION_HEADER_LEN;
    else
        gtp_hlen = OGS_GTPV1U_HEADER_LEN;

    ogs_pkbuf_push(pkbuf, gtp_hlen);

    /* Fill GTP Header */
    gtp_h = (ogs_gtp2_header_t *)pkbuf->data;
    ogs_assert(gtp_h);
    memset(gtp_h, 0, gtp_hlen);

    gtp_h->flags = flags;
    gtp_h->type = gtp_hdesc->type;

    if (gtp_h->type == OGS_GTPU_MSGTYPE_ECHO_REQ ||
        gtp_h->type == OGS_GTPU_MSGTYPE_ECHO_RSP ||
        gtp_h->type == OGS_GTPU_MSGTYPE_ERR_IND) {
        /*
         * TS29.281 5.1 General format in GTP-U header
         *
         * - The Echo Request/Response and Supported Extension Headers
         *   notification messages, where the Tunnel Endpoint Identifier
         *   shall be set to all zeroes.
         * - The Error Indication message where the Tunnel Endpoint Identifier
         *   shall be set to all zeros.
         */
        ogs_assert(gtp_hdesc->teid == 0);
    }

    gtp_h->teid = htobe32(gtp_hdesc->teid);

    /*
     * TS29.281 5.1 General format in GTP-U header
     *
     * Length: This field indicates the length in octets of the payload,
     * i.e. the rest of the packet following the mandatory part of
     * the GTP header (that is the first 8 octets). The Sequence Number,
     * the N-PDU Number or any Extension headers shall be considered
     * to be part of the payload, i.e. included in the length count.
     */
    gtp_h->length = htobe16(pkbuf->len - OGS_GTPV1U_HEADER_LEN);

    /* Fill Extention Header */
    if (gtp_h->flags & OGS_GTPU_FLAGS_E) {
        uint8_t *ext_h = (uint8_t *)(pkbuf->data +
                    OGS_GTPV1U_HEADER_LEN + OGS_GTPV1U_EXTENSION_HEADER_LEN);
        ogs_assert(ext_h);

        /* Copy Header Type */
        *(ext_h-1) = ext_hdesc->array[0].type;

        i = 0;
        while (i < OGS_GTP2_NUM_OF_EXTENSION_HEADER &&
                (ext_h - pkbuf->data) < gtp_hlen) {
            int len = ext_hdesc->array[i].len*4;

            /* Copy Header Content */
            memcpy(ext_h, &ext_hdesc->array[i].len, len-1);

            /* Check if Next Header is Available */
            if (ext_hdesc->array[i+1].len)
                ext_h[len-1] = ext_hdesc->array[i+1].type;
            else
                ext_h[len-1] =
                    OGS_GTP2_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS;

            ext_h += len;
            i++;
        }
    }
}
