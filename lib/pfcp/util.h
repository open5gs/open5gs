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

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_UTIL_H
#define OGS_PFCP_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /* Operation was successful */
    OGS_PFCP_STATUS_SUCCESS = 0,

    /* The message type is unknown */
    OGS_PFCP_ERROR_UNKNOWN_MESSAGE,

    /* The message is semantically incorrect */
    OGS_PFCP_ERROR_SEMANTIC_INCORRECT_MESSAGE,

    /* The node ID is not present in the message */
    OGS_PFCP_ERROR_NODE_ID_NOT_PRESENT,

    /* The node ID was not found in the expected location */
    OGS_PFCP_ERROR_NODE_ID_NOT_FOUND,

    /* Add additional error codes as needed */
} ogs_pfcp_status_e;

ogs_pfcp_status_e ogs_pfcp_get_node_id(
        ogs_pfcp_node_id_t *node_id, ogs_pfcp_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_UTIL_H */
