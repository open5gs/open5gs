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

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP_CONV_H
#define OGS_GTP_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_GTP_MAX_INDIRECT_TUNNEL             11

/* Create Indirect Data Forwarding Tunnel Request/Response */
void ogs_gtp_bearers_in_create_indirect_tunnel_request(
    ogs_gtp_tlv_bearer_context_t *bearers[][OGS_GTP_MAX_INDIRECT_TUNNEL],
    ogs_gtp_create_indirect_data_forwarding_tunnel_request_t *req);
void ogs_gtp_bearers_in_create_indirect_tunnel_response(
    ogs_gtp_tlv_bearer_context_t *bearers[][OGS_GTP_MAX_INDIRECT_TUNNEL],
    ogs_gtp_create_indirect_data_forwarding_tunnel_response_t *rsp);

int ogs_gtp_f_teid_to_sockaddr(
    ogs_gtp_f_teid_t *f_teid, uint16_t port, ogs_sockaddr_t **list);
int ogs_gtp_sockaddr_to_f_teid(ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6,
    ogs_gtp_f_teid_t *f_teid, int *len);
int ogs_gtp_f_teid_to_ip(ogs_gtp_f_teid_t *f_teid, ogs_ip_t *ip);
int ogs_gtp_ip_to_f_teid(ogs_ip_t *ip, ogs_gtp_f_teid_t *f_teid, int *len);

#ifdef __cplusplus
}
#endif

#endif

