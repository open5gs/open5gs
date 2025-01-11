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

#include "ogs-pfcp.h"

ogs_sock_t *ogs_pfcp_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *pfcp;
    ogs_assert(node);

    pfcp = ogs_udp_server(node->addr, node->option);
    if (pfcp) {
        ogs_info("pfcp_server() [%s]:%d",
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

        node->sock = pfcp;
    }

    return pfcp;
}

int ogs_pfcp_sendto(ogs_pfcp_node_t *node, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(node);
    ogs_assert(pkbuf);
    ogs_assert(node->addr_list);

    /* Initialize round-robin iterator if needed */
    if (node->current_addr == NULL) {
        node->current_addr = node->addr_list;
    }
    addr = node->current_addr;
    ogs_assert(addr);

    if (addr->ogs_sa_family == AF_INET) {
        sock = ogs_pfcp_self()->pfcp_sock;
        if (!sock) {
            ogs_error("IPv4 socket (pfcp_sock) is not available. "
                    "Ensure that 'pfcp.server.address: 127.0.0.1' "
                    "is set in the YAML configuration file.");
            return OGS_ERROR;
        }
    } else if (addr->ogs_sa_family == AF_INET6) {
        sock = ogs_pfcp_self()->pfcp_sock6;
        if (!sock) {
            ogs_error("IPv6 socket (pfcp_sock) is not available. "
                    "Ensure that 'pfcp.server.address: [::1]' "
                    "is set in the YAML configuration file.");
            return OGS_ERROR;
        }
    } else
        ogs_assert_if_reached();

    sent = ogs_sendto(sock->fd, pkbuf->data, pkbuf->len, 0, addr);
    if (sent < 0 || sent != pkbuf->len) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            char buf[OGS_ADDRSTRLEN];
            int err = ogs_socket_errno;
            ogs_log_message(OGS_LOG_ERROR, err,
                    "ogs_sendto(%u, %p, %u, 0, %s:%u) failed",
                    sock->fd, pkbuf->data, pkbuf->len,
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        }
        return OGS_ERROR;
    }

    /* Move to next address in round-robin sequence */
    if (node->current_addr->next)
        node->current_addr = node->current_addr->next;
    else
        /* If end of list reached, wrap around to the start */
        node->current_addr = node->addr_list;

    return OGS_OK;
}

int ogs_pfcp_send_heartbeat_request(ogs_pfcp_node_t *node,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data))
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(node);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_HEARTBEAT_REQUEST_TYPE;
    h.seid = 0;

    xact = ogs_pfcp_xact_local_create(node, cb, node);
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    pkbuf = ogs_pfcp_build_heartbeat_request(h.type);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_build_heartbeat_request() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ogs_pfcp_send_heartbeat_response(ogs_pfcp_xact_t *xact)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_header_t h;

    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_HEARTBEAT_RESPONSE_TYPE;
    h.seid = 0;

    pkbuf = ogs_pfcp_build_heartbeat_response(h.type);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_build_heartbeat_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    /*
     * Force delete the PFCP transaction to check the PFCP recovery timestamp.
     *
     * Otherwise, duplicated request (lib/pfcp/xact.c:384) prevents the message
     * from being passed to the state machine, so the PFCP recovery timestamp
     * cannot be delivered in the handler routine.
     */
    ogs_pfcp_xact_delete(xact);

    return rv;
}

int ogs_pfcp_cp_send_association_setup_request(ogs_pfcp_node_t *node,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data))
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(node);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE;
    h.seid = 0;

    xact = ogs_pfcp_xact_local_create(node, cb, node);
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    pkbuf = ogs_pfcp_cp_build_association_setup_request(h.type);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_cp_build_association_setup_request() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ogs_pfcp_cp_send_association_setup_response(ogs_pfcp_xact_t *xact,
        uint8_t cause)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_header_t h;

    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE;
    h.seid = 0;

    pkbuf = ogs_pfcp_cp_build_association_setup_response(h.type, cause);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_cp_build_association_setup_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ogs_pfcp_up_send_association_setup_request(ogs_pfcp_node_t *node,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data))
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(node);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE;
    h.seid = 0;

    xact = ogs_pfcp_xact_local_create(node, cb, node);
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    pkbuf = ogs_pfcp_up_build_association_setup_request(h.type);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_build_heartbeat_request() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ogs_pfcp_up_send_association_setup_response(ogs_pfcp_xact_t *xact,
        uint8_t cause)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_header_t h;

    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE;
    h.seid = 0;

    pkbuf = ogs_pfcp_up_build_association_setup_response(h.type, cause);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_up_build_association_setup_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

void ogs_pfcp_send_g_pdu(
        ogs_pfcp_pdr_t *pdr,
        ogs_gtp2_header_desc_t *sendhdr, ogs_pkbuf_t *sendbuf)
{
    ogs_gtp_node_t *gnode = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_gtp2_header_desc_t header_desc;

    ogs_assert(pdr);
    ogs_assert(sendhdr);
    ogs_assert(sendbuf);

    far = pdr->far;
    if (!far) {
        ogs_error("No FAR");
        ogs_pkbuf_free(sendbuf);
        return;
    }

    if (far->dst_if == OGS_PFCP_INTERFACE_UNKNOWN) {
        ogs_error("No Destination Interface");
        ogs_pkbuf_free(sendbuf);
        return;
    }

    gnode = far->gnode;
    ogs_assert(gnode);
    ogs_assert(gnode->sock);

    memset(&header_desc, 0, sizeof(header_desc));

    header_desc.type = sendhdr->type;
    header_desc.teid = far->outer_header_creation.teid;

    if (pdr->qer && pdr->qer->qfi) {
        header_desc.pdu_type =
            OGS_GTP2_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION;
        header_desc.qos_flow_identifier = pdr->qer->qfi;
    }

    if (sendhdr->udp.presence == true) {
        header_desc.udp.presence = sendhdr->udp.presence;
        header_desc.udp.port = sendhdr->udp.port;
    }

    if (sendhdr->pdcp_number_presence == true) {
        header_desc.pdcp_number_presence = sendhdr->pdcp_number_presence;
        header_desc.pdcp_number = sendhdr->pdcp_number;
    }

    ogs_gtp2_send_user_plane(gnode, &header_desc, sendbuf);
}

int ogs_pfcp_send_end_marker(ogs_pfcp_pdr_t *pdr)
{
    ogs_gtp_node_t *gnode = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_pkbuf_t *sendbuf = NULL;

    ogs_gtp2_header_desc_t header_desc;

    ogs_assert(pdr);
    far = pdr->far;
    ogs_assert(far);

    gnode = far->gnode;
    if (!gnode) {
        ogs_error("No GTP Node Setup");
        return OGS_DONE;
    }
    if (!gnode->sock) {
        ogs_error("No GTP Socket Setup");
        return OGS_DONE;
    }

    sendbuf = ogs_pkbuf_alloc(NULL, OGS_GTPV1U_5GC_HEADER_LEN);
    if (!sendbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return OGS_ERROR;
    }
    ogs_pkbuf_reserve(sendbuf, OGS_GTPV1U_5GC_HEADER_LEN);

    memset(&header_desc, 0, sizeof(header_desc));

    header_desc.type = OGS_GTPU_MSGTYPE_END_MARKER;
    header_desc.teid = far->outer_header_creation.teid;

    if (pdr->qer && pdr->qer->qfi) {
        header_desc.pdu_type =
            OGS_GTP2_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION;
        header_desc.qos_flow_identifier = pdr->qer->qfi;
    }

    ogs_gtp2_send_user_plane(gnode, &header_desc, sendbuf);

    return OGS_OK;
}

void ogs_pfcp_send_buffered_packet(ogs_pfcp_pdr_t *pdr)
{
    ogs_pfcp_far_t *far = NULL;
    int i;

    ogs_assert(pdr);
    far = pdr->far;

    if (far && far->gnode) {
        if (far->apply_action & OGS_PFCP_APPLY_ACTION_FORW) {
            for (i = 0; i < far->num_of_buffered_packet; i++) {
                ogs_gtp2_header_desc_t sendhdr;

                memset(&sendhdr, 0, sizeof(sendhdr));
                sendhdr.type = OGS_GTPU_MSGTYPE_GPDU;

                ogs_pfcp_send_g_pdu(
                        pdr, &sendhdr, far->buffered_packet[i]);
            }
            far->num_of_buffered_packet = 0;
        }
    }
}

void ogs_pfcp_send_error_message(
    ogs_pfcp_xact_t *xact, uint64_t seid, uint8_t type,
    uint8_t cause_value, uint16_t offending_ie_value)
{
    int rv;
    ogs_pfcp_message_t errmsg;
    ogs_pfcp_tlv_cause_t *cause = NULL;
    ogs_pfcp_tlv_offending_ie_t *offending_ie = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(xact);

    memset(&errmsg, 0, sizeof(ogs_pfcp_message_t));
    errmsg.h.seid = seid;
    errmsg.h.type = type;

    switch (type) {
    case OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE:
        cause = &errmsg.pfcp_pfd_management_response.cause;
        offending_ie = &errmsg.pfcp_pfd_management_response.offending_ie;
        break;
    case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
        cause = &errmsg.pfcp_association_setup_response.cause;
        break;
    case OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
        cause = &errmsg.pfcp_association_update_response.cause;
        break;
    case OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
        cause = &errmsg.pfcp_association_release_response.cause;
        break;
    case OGS_PFCP_NODE_REPORT_RESPONSE_TYPE:
        cause = &errmsg.pfcp_node_report_response.cause;
        offending_ie = &errmsg.pfcp_node_report_response.offending_ie;
        break;
    case OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
        cause = &errmsg.pfcp_session_set_deletion_response.cause;
        offending_ie = &errmsg.pfcp_session_set_deletion_response.offending_ie;
        break;
    case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
        cause = &errmsg.pfcp_session_establishment_response.cause;
        offending_ie = &errmsg.pfcp_session_establishment_response.offending_ie;
        break;
    case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
        cause = &errmsg.pfcp_session_modification_response.cause;
        offending_ie = &errmsg.pfcp_session_modification_response.offending_ie;
        break;
    case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
        cause = &errmsg.pfcp_session_deletion_response.cause;
        offending_ie = &errmsg.pfcp_session_deletion_response.offending_ie;
        break;
    case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
        cause = &errmsg.pfcp_session_report_response.cause;
        offending_ie = &errmsg.pfcp_session_report_response.offending_ie;
        break;
    default:
        ogs_assert_if_reached();
        return;
    }

    ogs_assert(cause);

    cause->presence = 1;
    cause->u8 = cause_value;

    if (offending_ie && offending_ie_value) {
        offending_ie->presence = 1;
        offending_ie->u16 = offending_ie_value;
    }

    pkbuf = ogs_pfcp_build_msg(&errmsg);
    if (!pkbuf) {
        ogs_error("ogs_pfcp_build_msg() failed");
        return;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &errmsg.h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}
