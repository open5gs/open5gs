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
    /* Success with actual Node ID */
    OGS_PFCP_STATUS_SUCCESS = 0,

    /* Success with no Node ID (NONE type) */
    OGS_PFCP_STATUS_NODE_ID_NONE,

    /* Success with OPTIONAL node_id_tlv, but presence=0 */
    OGS_PFCP_STATUS_NODE_ID_OPTIONAL_ABSENT,

    /* Error codes */
    OGS_PFCP_ERROR_SEMANTIC_INCORRECT_MESSAGE,
    OGS_PFCP_ERROR_NODE_ID_NOT_PRESENT,
    OGS_PFCP_ERROR_NODE_ID_NOT_FOUND,
    OGS_PFCP_ERROR_UNKNOWN_MESSAGE

} ogs_pfcp_status_e;

ogs_pfcp_status_e
ogs_pfcp_extract_node_id(ogs_pfcp_message_t *message,
                         ogs_pfcp_node_id_t *node_id);

ogs_sockaddr_t *ogs_pfcp_node_id_to_addrinfo(const ogs_pfcp_node_id_t *node_id);
const char *ogs_pfcp_node_id_to_string_static(
        const ogs_pfcp_node_id_t *node_id);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_UTIL_H */
