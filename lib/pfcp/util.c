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

/*
 * Requirements of Node ID:
 * NONE      : Node ID must not be present
 * OPTIONAL  : Node ID may or may not be present
 * MANDATORY : Node ID must be present
 */
#define OGS_PFCP_NODE_ID_NONE      0
#define OGS_PFCP_NODE_ID_OPTIONAL  1
#define OGS_PFCP_NODE_ID_MANDATORY 2

/*
 * This function extracts the PFCP Node ID from the given PFCP message.
 * It determines the Node ID field location and requirement based on
 * the message type. Then it validates presence and copies data into
 * 'node_id'. If Node ID is not consistent with the requirement, an
 * error status is returned.
 */
ogs_pfcp_status_e
ogs_pfcp_extract_node_id(ogs_pfcp_message_t *message,
                         ogs_pfcp_node_id_t *node_id)
{

    /* For C89 compliance, all variables are declared upfront. */
    ogs_pfcp_tlv_node_id_t *tlv_node_id = NULL;
    int requirement = OGS_PFCP_NODE_ID_NONE;
    ogs_pfcp_status_e status = OGS_PFCP_STATUS_SUCCESS;

    /* Validate input pointers */
    ogs_assert(message);
    ogs_assert(node_id);

    /* Initialize the output structure */
    memset(node_id, 0, sizeof(*node_id));

    /* Determine the location of node_id TLV and requirement */
    switch (message->h.type) {
    case OGS_PFCP_PFD_MANAGEMENT_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_pfd_management_request.node_id;
        requirement = OGS_PFCP_NODE_ID_OPTIONAL;
        break;

    case OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_pfd_management_response.node_id;
        requirement = OGS_PFCP_NODE_ID_OPTIONAL;
        break;

    case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_association_setup_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_association_setup_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_association_update_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_association_update_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_association_release_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_association_release_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_NODE_REPORT_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_node_report_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_NODE_REPORT_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_node_report_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_session_set_deletion_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_session_set_deletion_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_SET_MODIFICATION_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_session_set_modification_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_SET_MODIFICATION_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_session_set_modification_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_session_establishment_request.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
        tlv_node_id = &message->pfcp_session_establishment_response.node_id;
        requirement = OGS_PFCP_NODE_ID_MANDATORY;
        break;

    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        tlv_node_id = &message->pfcp_session_modification_request.node_id;
        requirement = OGS_PFCP_NODE_ID_OPTIONAL;
        break;

    /* Add other message types with node_id here as needed */

    case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
    case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
    case OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
    case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
    case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
    case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
    case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
    case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
        /* Node ID must not be present for these messages */
        requirement = OGS_PFCP_NODE_ID_NONE;
        break;

    default:
        /* Unknown message type */
        ogs_error("Unknown message type %d", message->h.type);
        return OGS_PFCP_ERROR_UNKNOWN_MESSAGE;
    }

    /* Check requirement vs. tlv_node_id existence */
    switch (requirement) {
    case OGS_PFCP_NODE_ID_MANDATORY:
        /* Must have tlv_node_id. presence must be 1. */
        ogs_assert(tlv_node_id);
        if (!tlv_node_id->presence) {
            status = OGS_PFCP_ERROR_NODE_ID_NOT_PRESENT;
            goto done;
        }
        break;

    case OGS_PFCP_NODE_ID_OPTIONAL:
        /*
         * Must have tlv_node_id. presence=1 => real Node ID
         * presence=0 => no Node ID
         */
        ogs_assert(tlv_node_id);
        if (!tlv_node_id->presence) {
            status = OGS_PFCP_STATUS_NODE_ID_OPTIONAL_ABSENT;
            goto done;
        }
        break;

    case OGS_PFCP_NODE_ID_NONE:
        /* Must be NULL => no Node ID field */
        ogs_assert(tlv_node_id == NULL);
        status = OGS_PFCP_STATUS_NODE_ID_NONE;
        goto done;

    default:
        status = OGS_PFCP_ERROR_UNKNOWN_MESSAGE;
        goto done;
    }

    memcpy(node_id, tlv_node_id->data, tlv_node_id->len);

    if (node_id->type != OGS_PFCP_NODE_ID_IPV4 &&
        node_id->type != OGS_PFCP_NODE_ID_IPV6 &&
        node_id->type != OGS_PFCP_NODE_ID_FQDN) {
        ogs_error("Semantic incorrect message[%d] type[%d]",
                message->h.type, node_id->type);
        return OGS_PFCP_ERROR_SEMANTIC_INCORRECT_MESSAGE;
    }

     /* Node ID is valid */
    status = OGS_PFCP_STATUS_SUCCESS;

done:
    return status;
}

ogs_sockaddr_t *ogs_pfcp_node_id_to_addrinfo(const ogs_pfcp_node_id_t *node_id)
{
    ogs_sockaddr_t *p;
    int ret;
    uint16_t port = ogs_pfcp_self()->pfcp_port;
    char fqdn[OGS_MAX_FQDN_LEN+1];

    ogs_assert(node_id);
    switch (node_id->type) {

    /*------------------------------------------------
     * 1) IPv4
     *-----------------------------------------------*/
    case OGS_PFCP_NODE_ID_IPV4:
        p = (ogs_sockaddr_t *)ogs_calloc(1, sizeof(*p));
        if (!p) {
            ogs_error("ogs_calloc() failed");
            return NULL;
        }
        p->sa.sa_family = AF_INET;
        p->sin.sin_port = htobe16(port);
        p->sin.sin_addr.s_addr = node_id->addr;
        p->next = NULL;
        return p;

    /*------------------------------------------------
     * 2) IPv6
     *-----------------------------------------------*/
    case OGS_PFCP_NODE_ID_IPV6:
        p = (ogs_sockaddr_t *)ogs_calloc(1, sizeof(*p));
        if (!p) {
            ogs_error("ogs_calloc() failed");
            return NULL;
        }
        p->sa.sa_family = AF_INET6;
        p->sin6.sin6_port = htobe16(port);
        /* Copy 16 bytes of IPv6 address */
        memcpy(&p->sin6.sin6_addr, node_id->addr6, 16);
        p->next = NULL;
        return OGS_OK;

    /*------------------------------------------------
     * 3) FQDN
     *-----------------------------------------------*/
    case OGS_PFCP_NODE_ID_FQDN:
        /* If the FQDN is not empty, we attempt DNS resolution.
         *  ogs_addaddrinfo() is a placeholder for your actual
         *  DNS -> ogs_sockaddr_t function (often wraps getaddrinfo).
         */
        /* Port=0 or set as needed, family=AF_UNSPEC, flags=0. */
        if (ogs_fqdn_parse(fqdn, node_id->fqdn, strlen(node_id->fqdn)) <= 0) {
            ogs_error("ogs_fqdn_parse() error [%s]", node_id->fqdn);
            return NULL;
        }
        ret = ogs_getaddrinfo(&p, AF_UNSPEC, fqdn, port, 0);
        if (ret != 0) {
            /* DNS resolution failed => *out remains NULL */
            ogs_error("ogs_addaddrinfo() failed");
            return NULL;
        }
        /* If FQDN is empty, just return with no addresses. */
        return p;

    /*------------------------------------------------
     * 4) Unsupported type or default
     *-----------------------------------------------*/
    default:
        /* Optionally handle an error or just return success
         * with no addresses.
         */
        ogs_error("Unknown type [%d]", node_id->type);
        return NULL;
    }
}

/* Utility function to convert node_id to string for logging */
const char *ogs_pfcp_node_id_to_string_static(
        const ogs_pfcp_node_id_t *node_id)
{
    static char buffer[OGS_MAX_FQDN_LEN+1] = { 0, };

    if (node_id) {
        switch (node_id->type) {
        case OGS_PFCP_NODE_ID_IPV4:
            inet_ntop(AF_INET, &node_id->addr, buffer, sizeof(buffer));
            break;
        case OGS_PFCP_NODE_ID_IPV6:
            inet_ntop(AF_INET6, node_id->addr6, buffer, sizeof(buffer));
            break;
        case OGS_PFCP_NODE_ID_FQDN:
            if (ogs_fqdn_parse(buffer,
                               node_id->fqdn,
                               strlen(node_id->fqdn)) <= 0)
                snprintf(buffer, sizeof(buffer), "%s", node_id->fqdn);
            break;
        default:
            snprintf(buffer, sizeof(buffer), "Unknown");
            break;
        }
    }

    return buffer;
}
