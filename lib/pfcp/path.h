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

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_PATH_H
#define OGS_PFCP_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_pfcp_xact_s ogs_pfcp_xact_t;

ogs_sock_t *ogs_pfcp_server(ogs_socknode_t *node);
int ogs_pfcp_connect(
    ogs_sock_t *ipv4, ogs_sock_t *ipv6, ogs_pfcp_node_t *node);

int ogs_pfcp_send(ogs_pfcp_node_t *node, ogs_pkbuf_t *pkbuf);
int ogs_pfcp_sendto(ogs_pfcp_node_t *node, ogs_pkbuf_t *pkbuf);

ogs_pkbuf_t *ogs_pfcp_handle_echo_req(ogs_pkbuf_t *pkt);

void ogs_pfcp_send_heartbeat_request(ogs_pfcp_node_t *node,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data));
void ogs_pfcp_send_heartbeat_response(ogs_pfcp_xact_t *xact);

void ogs_pfcp_cp_send_association_setup_request(ogs_pfcp_node_t *node,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data));
void ogs_pfcp_cp_send_association_setup_response(ogs_pfcp_xact_t *xact,
        uint8_t cause);

void ogs_pfcp_up_send_association_setup_request(ogs_pfcp_node_t *node,
        void (*cb)(ogs_pfcp_xact_t *xact, void *data));
void ogs_pfcp_up_send_association_setup_response(ogs_pfcp_xact_t *xact,
        uint8_t cause);

void ogs_pfcp_send_g_pdu(ogs_pfcp_pdr_t *pdr, ogs_pkbuf_t *sendbuf);
void ogs_pfcp_send_end_marker(ogs_pfcp_pdr_t *pdr);

void ogs_pfcp_send_buffered_packet(ogs_pfcp_pdr_t *pdr);

void ogs_pfcp_send_error_message(
    ogs_pfcp_xact_t *xact, uint64_t seid, uint8_t type,
    uint8_t cause_value, uint16_t offending_ie_value);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_PATH_H */
