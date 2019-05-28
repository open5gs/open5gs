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

#ifndef __GTP_CONV_H__
#define __GTP_CONV_H__

#include "ogs-core.h"

typedef struct ogs_sockaddr_s ogs_sockaddr_t;
typedef struct tlv_bearer_context_s tlv_bearer_context_t;
typedef struct gtp_create_indirect_data_forwarding_tunnel_request_s gtp_create_indirect_data_forwarding_tunnel_request_t;
typedef struct gtp_create_indirect_data_forwarding_tunnel_response_s gtp_create_indirect_data_forwarding_tunnel_response_t;
typedef struct gtp_f_teid_s gtp_f_teid_t;
typedef struct ip_s ip_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTP_MAX_NUM_OF_INDIRECT_TUNNEL      11

/* Create Indirect Data Forwarding Tunnel Request/Response */
void gtp_bearers_in_create_indirect_tunnel_request(
    tlv_bearer_context_t *bearers[][GTP_MAX_NUM_OF_INDIRECT_TUNNEL],
    gtp_create_indirect_data_forwarding_tunnel_request_t *req);
void gtp_bearers_in_create_indirect_tunnel_response(
    tlv_bearer_context_t *bearers[][GTP_MAX_NUM_OF_INDIRECT_TUNNEL],
    gtp_create_indirect_data_forwarding_tunnel_response_t *rsp);

int gtp_f_teid_to_sockaddr(
    gtp_f_teid_t *f_teid, uint16_t port, ogs_sockaddr_t **list);
int gtp_sockaddr_to_f_teid(
    ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6, gtp_f_teid_t *f_teid, int *len);
int gtp_f_teid_to_ip(gtp_f_teid_t *f_teid, ip_t *ip);
int gtp_ip_to_f_teid(ip_t *ip, gtp_f_teid_t *f_teid, int *len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_CONV_H__ */

