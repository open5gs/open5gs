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

#ifndef OGS_GTP_PATH_H
#define OGS_GTP_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_SETUP_GTPC_SERVER \
    do { \
        ogs_gtp_self()->gtpc_sock = \
            ogs_socknode_sock_first(&ogs_gtp_self()->gtpc_list); \
        ogs_gtp_self()->gtpc_sock6 = \
            ogs_socknode_sock_first(&ogs_gtp_self()->gtpc_list6); \
        \
        ogs_assert(ogs_gtp_self()->gtpc_sock || ogs_gtp_self()->gtpc_sock6); \
        \
        if (ogs_gtp_self()->gtpc_sock) \
            ogs_gtp_self()->gtpc_addr = \
                &ogs_gtp_self()->gtpc_sock->local_addr; \
        if (ogs_gtp_self()->gtpc_sock6) \
            ogs_gtp_self()->gtpc_addr6 = \
                &ogs_gtp_self()->gtpc_sock6->local_addr; \
        \
        ogs_assert(ogs_gtp_self()->gtpc_addr || ogs_gtp_self()->gtpc_addr6); \
        \
    } while(0)

#define OGS_SETUP_GTPU_SERVER \
    do { \
        ogs_assert(ogs_gtp_self()->gtpu_sock || ogs_gtp_self()->gtpu_sock6); \
        \
        if (ogs_gtp_self()->gtpu_sock) \
            ogs_gtp_self()->gtpu_addr = \
                &ogs_gtp_self()->gtpu_sock->local_addr; \
        if (ogs_gtp_self()->gtpu_sock6) \
            ogs_gtp_self()->gtpu_addr6 = \
                &ogs_gtp_self()->gtpu_sock6->local_addr; \
        \
        ogs_assert(ogs_gtp_self()->gtpu_addr || ogs_gtp_self()->gtpu_addr6); \
        \
        ogs_sockaddr_to_ip( \
                ogs_gtp_self()->gtpu_addr, ogs_gtp_self()->gtpu_addr6, \
                &ogs_gtp_self()->gtpu_ip); \
    } while(0)

ogs_sock_t *ogs_gtp_server(ogs_socknode_t *node);
int ogs_gtp_connect(ogs_sock_t *ipv4, ogs_sock_t *ipv6, ogs_gtp_node_t *gnode);

int ogs_gtp_send(ogs_gtp_node_t *gnode, ogs_pkbuf_t *pkbuf);
int ogs_gtp_sendto(ogs_gtp_node_t *gnode, ogs_pkbuf_t *pkbuf);

void ogs_gtp_send_error_message(
        ogs_gtp_xact_t *xact, uint32_t teid, uint8_t type, uint8_t cause_value);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_PATH_H */
