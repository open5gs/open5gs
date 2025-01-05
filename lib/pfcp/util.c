/*
 * Copyright (C) 2025 by Sukchan Lee <acetcom@gmail.com>
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

/**
 * Extracts the `node_id` from a PFCP message.
 *
 * @param node_id Pointer to store the extracted node_id.
 * @param msg Pointer to the parsed PFCP message.
 * @return ogs_pfcp_status_e Status of the operation.
 */
ogs_pfcp_status_e ogs_pfcp_get_node_id(
        ogs_pfcp_node_id_t *node_id, ogs_pfcp_message_t *message)
{
    ogs_pfcp_tlv_node_id_t *tlv_node_id = NULL;

    ogs_assert(node_id);
    ogs_assert(message);

    switch (message->h.type) {
        /* Message Types with node_id */
        case OGS_PFCP_PFD_MANAGEMENT_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_pfd_management_request.node_id;
            break;

        case OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_pfd_management_response.node_id;
            break;

        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_association_setup_request.node_id;
            break;

        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_association_setup_response.node_id;
            break;

        case OGS_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_association_update_request.node_id;
            break;

        case OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_association_update_response.node_id;
            break;

        case OGS_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_association_release_request.node_id;
            break;

        case OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_association_release_response.node_id;
            break;

        case OGS_PFCP_NODE_REPORT_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_node_report_request.node_id;
            break;

        case OGS_PFCP_NODE_REPORT_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_node_report_response.node_id;
            break;

        case OGS_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_session_set_deletion_request.node_id;
            break;

        case OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_session_set_deletion_response.node_id;
            break;

        case OGS_PFCP_SESSION_SET_MODIFICATION_REQUEST_TYPE:
            tlv_node_id =
                &message->pfcp_session_set_modification_request.node_id;
            break;

        case OGS_PFCP_SESSION_SET_MODIFICATION_RESPONSE_TYPE:
            tlv_node_id =
                &message->pfcp_session_set_modification_response.node_id;
            break;

        case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_session_establishment_request.node_id;
            break;

        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            tlv_node_id = &message->pfcp_session_establishment_response.node_id;
            break;

        case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            tlv_node_id = &message->pfcp_session_modification_request.node_id;
            break;

        /* Add other message types with node_id here as needed */

        /* Message Types without node_id */
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
        case OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
        case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
        case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
        case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
        case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            break;

        default:
            ogs_error("Unknown message type %d", message->h.type);
            return OGS_PFCP_ERROR_UNKNOWN_MESSAGE;
    }

    if (!tlv_node_id)
        return OGS_PFCP_ERROR_NODE_ID_NOT_FOUND;

    if (!tlv_node_id->presence)
        return OGS_PFCP_ERROR_NODE_ID_NOT_PRESENT;

    memcpy(node_id, tlv_node_id->data, tlv_node_id->len);

    if (node_id->type != OGS_PFCP_NODE_ID_IPV4 &&
        node_id->type != OGS_PFCP_NODE_ID_IPV6 &&
        node_id->type != OGS_PFCP_NODE_ID_FQDN) {
        ogs_error("Semantic incorrect message[%d] type[%d]",
                message->h.type, node_id->type);
        return OGS_PFCP_ERROR_SEMANTIC_INCORRECT_MESSAGE;
    }

    return OGS_PFCP_STATUS_SUCCESS;
}
