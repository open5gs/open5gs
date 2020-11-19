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

ogs_sock_t *ogs_gtp_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *gtp;
    ogs_assert(node);

    gtp = ogs_udp_server(node);
    ogs_assert(gtp);

    ogs_info("gtp_server() [%s]:%d",
            OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

    return gtp;
}

int ogs_gtp_connect(ogs_sock_t *ipv4, ogs_sock_t *ipv6, ogs_gtp_node_t *gnode)
{
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(ipv4 || ipv6);
    ogs_assert(gnode);
    ogs_assert(gnode->sa_list);

    addr = gnode->sa_list;
    while (addr) {
        ogs_sock_t *sock = NULL;

        if (addr->ogs_sa_family == AF_INET)
            sock = ipv4;
        else if (addr->ogs_sa_family == AF_INET6)
            sock = ipv6;
        else
            ogs_assert_if_reached();

        if (sock) {
            ogs_info("gtp_connect() [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));

            gnode->sock = sock;
            memcpy(&gnode->addr, addr, sizeof gnode->addr);
            break;
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno,
                "gtp_connect() [%s]:%d failed",
                OGS_ADDR(gnode->sa_list, buf), OGS_PORT(gnode->sa_list));
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp_send(ogs_gtp_node_t *gnode, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;

    ogs_assert(gnode);
    ogs_assert(pkbuf);
    sock = gnode->sock;
    ogs_assert(sock);

    sent = ogs_send(sock->fd, pkbuf->data, pkbuf->len, 0);
    if (sent < 0 || sent != pkbuf->len) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_gtp_send() failed");
        }
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp_sendto(ogs_gtp_node_t *gnode, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(gnode);
    ogs_assert(pkbuf);
    sock = gnode->sock;
    ogs_assert(sock);
    addr = &gnode->addr;
    ogs_assert(addr);

    sent = ogs_sendto(sock->fd, pkbuf->data, pkbuf->len, 0, addr);
    if (sent < 0 || sent != pkbuf->len) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_gtp_sendto() failed");
        }
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp_send_user_plane(
        ogs_gtp_node_t *gnode,
        ogs_gtp_header_t *gtp_hdesc, ogs_gtp_extension_header_t *ext_hdesc,
        ogs_pkbuf_t *pkbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int rv;

    ogs_gtp_header_t *gtp_h = NULL;
    ogs_gtp_extension_header_t *ext_h = NULL;
    uint8_t flags;
    uint8_t gtp_hlen = 0;

    ogs_assert(gnode);
    ogs_assert(gtp_hdesc);
    ogs_assert(ext_hdesc);
    ogs_assert(pkbuf);

    /* Processing GTP Flags */
    flags = gtp_hdesc->flags;
    flags |= OGS_GTPU_FLAGS_V | OGS_GTPU_FLAGS_PT;
    if (ext_hdesc->qos_flow_identifier) flags |= OGS_GTPU_FLAGS_E;

    /* Define GTP Header Size */
    if (flags & OGS_GTPU_FLAGS_E)
        gtp_hlen = OGS_GTPV1U_HEADER_LEN+8;
    else if (flags & (OGS_GTPU_FLAGS_S|OGS_GTPU_FLAGS_PN))
        gtp_hlen = OGS_GTPV1U_HEADER_LEN+4;
    else
        gtp_hlen = OGS_GTPV1U_HEADER_LEN;

    ogs_pkbuf_push(pkbuf, gtp_hlen);

    /* Fill GTP Header */
    gtp_h = (ogs_gtp_header_t *)pkbuf->data;
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
        ext_h = (ogs_gtp_extension_header_t *)
            (pkbuf->data + OGS_GTPV1U_HEADER_LEN);
        ogs_assert(ext_h);

        if (ext_hdesc->qos_flow_identifier) {
            /* 5G Core */
            ext_h->type = OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER;
            ext_h->len = 1;
            ext_h->pdu_type = ext_hdesc->pdu_type;
            ext_h->qos_flow_identifier = ext_hdesc->qos_flow_identifier;
            ext_h->next_type =
                OGS_GTP_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS;
        } else {
            /* EPC */
            ext_h->type = ext_hdesc->type;
            ext_h->len = 1;
            ext_h->next_type =
                OGS_GTP_EXTENSION_HEADER_TYPE_NO_MORE_EXTENSION_HEADERS;
        }
    }

    ogs_debug("SEND GTP-U[%d] to Peer[%s] : TEID[0x%x]",
            gtp_hdesc->type, OGS_ADDR(&gnode->addr, buf), gtp_hdesc->teid);
    rv = ogs_gtp_sendto(gnode, pkbuf);
    if (rv != OGS_OK) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            ogs_error("SEND GTP-U[%d] to Peer[%s] : TEID[0x%x]",
                gtp_hdesc->type, OGS_ADDR(&gnode->addr, buf), gtp_hdesc->teid);
        }
    }

    ogs_pkbuf_free(pkbuf);

    return rv;
}

ogs_pkbuf_t *ogs_gtp_handle_echo_req(ogs_pkbuf_t *pkb)
{
    ogs_gtp_header_t *gtph = NULL;
    ogs_pkbuf_t *pkb_resp = NULL;
    ogs_gtp_header_t *gtph_resp = NULL;
    uint16_t length;
    int idx;

    ogs_assert(pkb);

    gtph = (ogs_gtp_header_t *)pkb->data;
    /* Check GTP version. Now only support GTPv1(version = 1) */
    if ((gtph->flags >> 5) != 1) {
        return NULL;
    }

    if (gtph->type != OGS_GTPU_MSGTYPE_ECHO_REQ) {
        return NULL;
    }


    pkb_resp = ogs_pkbuf_alloc(NULL,
            100 /* enough for ECHO_RSP; use smaller buffer */);
    ogs_assert(pkb_resp);
    ogs_pkbuf_put(pkb_resp, 100);
    gtph_resp = (ogs_gtp_header_t *)pkb_resp->data;

    /* reply back immediately */
    gtph_resp->flags = (1 << 5); /* set version */
    gtph_resp->flags |= (1 << 4); /* set PT */
    gtph_resp->type = OGS_GTPU_MSGTYPE_ECHO_RSP;
    length = 0;     /* length of Recovery IE */
    gtph_resp->length = htobe16(length); /* to be overwriten */
    gtph_resp->teid = 0;
    idx = 8;

    if (gtph->flags & (OGS_GTPU_FLAGS_PN | OGS_GTPU_FLAGS_S)) {
        length += 4;
        if (gtph->flags & OGS_GTPU_FLAGS_S) {
            /* sequence exists */
            gtph_resp->flags |= OGS_GTPU_FLAGS_S;
            *((uint8_t *)pkb_resp->data + idx) = *((uint8_t *)pkb->data + idx);
            *((uint8_t *)pkb_resp->data + idx + 1) =
                *((uint8_t *)pkb->data + idx + 1);
        } else {
            *((uint8_t *)pkb_resp->data + idx) = 0;
            *((uint8_t *)pkb_resp->data + idx + 1) = 0;
        }
        idx += 2;
        if (gtph->flags & OGS_GTPU_FLAGS_PN) {
            /* sequence exists */
            gtph_resp->flags |= OGS_GTPU_FLAGS_PN;
            *((uint8_t *)pkb_resp->data + idx) = *((uint8_t *)pkb->data + idx);
        } else {
            *((uint8_t *)pkb_resp->data + idx) = 0;
        }
        idx++;
        *((uint8_t *)pkb_resp->data + idx) = 0; /* next-extension header */
        idx++;
    }
    
    /* fill Recovery IE */
    length += 2;
    *((uint8_t *)pkb_resp->data + idx) = 14; idx++; /* type */
    *((uint8_t *)pkb_resp->data + idx) = 0; idx++; /* restart counter */

    gtph_resp->length = htobe16(length);
    ogs_pkbuf_trim(pkb_resp, idx); /* buffer length */

    return pkb_resp;
}

void ogs_gtp_send_error_message(
        ogs_gtp_xact_t *xact, uint32_t teid, uint8_t type, uint8_t cause_value)
{
    int rv;
    ogs_gtp_message_t errmsg;
    ogs_gtp_cause_t cause;
    ogs_gtp_tlv_cause_t *tlv = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    memset(&errmsg, 0, sizeof(ogs_gtp_message_t));
    errmsg.h.teid = teid;
    errmsg.h.type = type;

    switch (type) {
    case OGS_GTP_CREATE_SESSION_RESPONSE_TYPE:
        tlv = &errmsg.create_session_response.cause;
        break;
    case OGS_GTP_MODIFY_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.modify_bearer_response.cause;
        break;
    case OGS_GTP_DELETE_SESSION_RESPONSE_TYPE:
        tlv = &errmsg.delete_session_response.cause;
        break;
    case OGS_GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
        tlv = &errmsg.release_access_bearers_response.cause;
        break;
    case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
        tlv = &errmsg.downlink_data_notification_acknowledge.cause;
        break;
    case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.create_bearer_response.cause;
        break;
    case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.update_bearer_response.cause;
        break;
    case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.delete_bearer_response.cause;
        break;
    case OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        tlv = &errmsg.create_indirect_data_forwarding_tunnel_response.cause;
        break;
    case OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        tlv = &errmsg.delete_indirect_data_forwarding_tunnel_response.cause;
        break;
    case OGS_GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
        tlv = &errmsg.modify_bearer_failure_indication.cause;
        break;
    case OGS_GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE:
        tlv = &errmsg.delete_bearer_failure_indication.cause;
        break;
    case OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE:
        tlv = &errmsg.bearer_resource_failure_indication.cause;
        break;
    default:
        ogs_assert_if_reached();
        return;
    }

    ogs_assert(tlv);

    memset(&cause, 0, sizeof cause);
    cause.value = cause_value;
    tlv->presence = 1;
    tlv->len = sizeof(cause);
    tlv->data = &cause;

    pkbuf = ogs_gtp_build_msg(&errmsg);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(xact, &errmsg.h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void ogs_gtp_send_echo_request(
        ogs_gtp_node_t *gnode, uint8_t recovery, uint8_t features)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_header_t h;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(gnode);

    ogs_debug("[GTP] Sending Echo Request");

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_ECHO_REQUEST_TYPE;
    h.teid = 0;

    pkbuf = ogs_gtp_build_echo_request(h.type, recovery, features);
    ogs_expect_or_return(pkbuf);

    xact = ogs_gtp_xact_local_create(gnode, &h, pkbuf, NULL, NULL);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void ogs_gtp_send_echo_response(ogs_gtp_xact_t *xact,
        uint8_t recovery, uint8_t features)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_header_t h;

    ogs_assert(xact);

    ogs_debug("[GTP] Sending Echo Response");

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_ECHO_RESPONSE_TYPE;
    h.teid = 0;

    pkbuf = ogs_gtp_build_echo_response(h.type, recovery, features);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}
