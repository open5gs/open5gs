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

int ogs_gtp2_send_user_plane(
        ogs_gtp_node_t *gnode,
        ogs_gtp2_header_desc_t *header_desc,
        ogs_pkbuf_t *pkbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int rv, i;

    ogs_gtp2_header_t gtp_hdesc;
    ogs_gtp2_extension_header_t ext_hdesc;

    ogs_assert(header_desc);

    memset(&gtp_hdesc, 0, sizeof(gtp_hdesc));
    memset(&ext_hdesc, 0, sizeof(ext_hdesc));

    gtp_hdesc.flags = header_desc->flags;
    gtp_hdesc.type = header_desc->type;
    gtp_hdesc.teid = header_desc->teid;

    i = 0;

    if (header_desc->qos_flow_identifier) {
        ext_hdesc.array[i].type =
            OGS_GTP2_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER;
        ext_hdesc.array[i].len = 1;
        ext_hdesc.array[i].pdu_type = header_desc->pdu_type;
        ext_hdesc.array[i].qos_flow_identifier =
            header_desc->qos_flow_identifier;
        i++;
    }

    if (header_desc->udp.presence == true) {
        ext_hdesc.array[i].type = OGS_GTP2_EXTENSION_HEADER_TYPE_UDP_PORT;
        ext_hdesc.array[i].len = 1;
        ext_hdesc.array[i].udp_port = htobe16(header_desc->udp.port);
        i++;
    }

    if (header_desc->pdcp_number_presence == true) {
        ext_hdesc.array[i].type = OGS_GTP2_EXTENSION_HEADER_TYPE_PDCP_NUMBER;
        ext_hdesc.array[i].len = 1;
        ext_hdesc.array[i].pdcp_number = htobe16(header_desc->pdcp_number);
        i++;
    }

    ogs_gtp2_fill_header(&gtp_hdesc, &ext_hdesc, pkbuf);

    ogs_trace("SEND GTP-U[%d] to Peer[%s] : TEID[0x%x]",
            header_desc->type,
            OGS_ADDR(&gnode->addr, buf), header_desc->teid);

    rv = ogs_gtp_sendto(gnode, pkbuf);
    if (rv != OGS_OK) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            ogs_error("SEND GTP-U[%d] to Peer[%s] : TEID[0x%x]",
                header_desc->type,
                OGS_ADDR(&gnode->addr, buf), header_desc->teid);
        }
    }

    ogs_pkbuf_free(pkbuf);

    return rv;
}

ogs_pkbuf_t *ogs_gtp2_handle_echo_req(ogs_pkbuf_t *pkb)
{
    ogs_gtp2_header_t *gtph = NULL;
    ogs_pkbuf_t *pkb_resp = NULL;
    ogs_gtp2_header_t *gtph_resp = NULL;
    uint16_t length;
    int idx;

    ogs_assert(pkb);

    gtph = (ogs_gtp2_header_t *)pkb->data;
    /* Check GTP version. Now only support GTPv1(version = 1) */
    if ((gtph->flags >> 5) != 1) {
        return NULL;
    }

    if (gtph->type != OGS_GTPU_MSGTYPE_ECHO_REQ) {
        return NULL;
    }


    pkb_resp = ogs_pkbuf_alloc(NULL,
            100 /* enough for ECHO_RSP; use smaller buffer */);
    if (!pkb_resp) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }
    ogs_pkbuf_put(pkb_resp, 100);
    gtph_resp = (ogs_gtp2_header_t *)pkb_resp->data;

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

void ogs_gtp2_send_error_message(
        ogs_gtp_xact_t *xact, uint32_t teid, uint8_t type, uint8_t cause_value)
{
    int rv;
    ogs_gtp2_message_t errmsg;
    ogs_gtp2_cause_t cause;
    ogs_gtp2_tlv_cause_t *tlv = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    memset(&errmsg, 0, sizeof(ogs_gtp2_message_t));
    errmsg.h.teid = teid;
    errmsg.h.type = type;

    switch (type) {
    case OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE:
        tlv = &errmsg.create_session_response.cause;
        break;
    case OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.modify_bearer_response.cause;
        break;
    case OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE:
        tlv = &errmsg.delete_session_response.cause;
        break;
    case OGS_GTP2_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
        tlv = &errmsg.release_access_bearers_response.cause;
        break;
    case OGS_GTP2_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
        tlv = &errmsg.downlink_data_notification_acknowledge.cause;
        break;
    case OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.create_bearer_response.cause;
        break;
    case OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.update_bearer_response.cause;
        break;
    case OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE:
        tlv = &errmsg.delete_bearer_response.cause;
        break;
    case OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        tlv = &errmsg.create_indirect_data_forwarding_tunnel_response.cause;
        break;
    case OGS_GTP2_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        tlv = &errmsg.delete_indirect_data_forwarding_tunnel_response.cause;
        break;
    case OGS_GTP2_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
        tlv = &errmsg.modify_bearer_failure_indication.cause;
        break;
    case OGS_GTP2_DELETE_BEARER_FAILURE_INDICATION_TYPE:
        tlv = &errmsg.delete_bearer_failure_indication.cause;
        break;
    case OGS_GTP2_BEARER_RESOURCE_FAILURE_INDICATION_TYPE:
        tlv = &errmsg.bearer_resource_failure_indication.cause;
        break;
    default:
        ogs_fatal("Invalid message[%d]", type);
        ogs_assert_if_reached();
        return;
    }

    ogs_assert(tlv);

    memset(&cause, 0, sizeof cause);
    cause.value = cause_value;
    tlv->presence = 1;
    tlv->len = sizeof(cause);
    tlv->data = &cause;

    pkbuf = ogs_gtp2_build_msg(&errmsg);
    if (!pkbuf) {
        ogs_error("ogs_gtp2_build_msg() failed");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &errmsg.h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void ogs_gtp2_send_echo_request(
        ogs_gtp_node_t *gnode, uint8_t recovery, uint8_t features)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_header_t h;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(gnode);

    ogs_debug("[GTP] Sending Echo Request");

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_ECHO_REQUEST_TYPE;
    h.teid = 0;

    pkbuf = ogs_gtp2_build_echo_request(h.type, recovery, features);
    if (!pkbuf) {
        ogs_error("ogs_gtp2_build_echo_request() failed");
        return;
    }

    xact = ogs_gtp_xact_local_create(gnode, &h, pkbuf, NULL, NULL);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void ogs_gtp2_send_echo_response(ogs_gtp_xact_t *xact,
        uint8_t recovery, uint8_t features)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_header_t h;

    ogs_assert(xact);

    ogs_debug("[GTP] Sending Echo Response");

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_ECHO_RESPONSE_TYPE;
    h.teid = 0;

    pkbuf = ogs_gtp2_build_echo_response(h.type, recovery, features);
    if (!pkbuf) {
        ogs_error("ogs_gtp2_build_echo_response() failed");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void ogs_gtp1_send_error_indication(
        ogs_sock_t *sock, uint32_t teid, uint8_t qfi, const ogs_sockaddr_t *to)
{
    ssize_t sent;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_gtp2_header_t gtp_hdesc;
    ogs_gtp2_extension_header_t ext_hdesc;
    int i;

    ogs_assert(sock);
    ogs_assert(to);

    pkbuf = ogs_gtp1_build_error_indication(teid, &sock->local_addr);
    if (!pkbuf) {
        ogs_error("ogs_gtp1_build_error_indication() failed");
        return;
    }

    memset(&gtp_hdesc, 0, sizeof(gtp_hdesc));
    memset(&ext_hdesc, 0, sizeof(ext_hdesc));

    gtp_hdesc.type = OGS_GTPU_MSGTYPE_ERR_IND;
    gtp_hdesc.flags = OGS_GTPU_FLAGS_S|OGS_GTPU_FLAGS_E;

    i = 0;
    if (qfi) {
        ext_hdesc.array[i].type =
            OGS_GTP2_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER;
        ext_hdesc.array[i].len = 1;
        ext_hdesc.array[i].pdu_type =
            OGS_GTP2_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION;
        ext_hdesc.array[i].qos_flow_identifier = qfi;
        i++;
    }
    ext_hdesc.array[i].type = OGS_GTP2_EXTENSION_HEADER_TYPE_UDP_PORT;
    ext_hdesc.array[i].len = 1;
    ext_hdesc.array[i].udp_port = 0;

    ogs_gtp2_fill_header(&gtp_hdesc, &ext_hdesc, pkbuf);

    sent = ogs_sendto(sock->fd, pkbuf->data, pkbuf->len, 0, to);
    if (sent < 0 || sent != pkbuf->len) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_sendto() failed");
    }
    ogs_pkbuf_free(pkbuf);
}
