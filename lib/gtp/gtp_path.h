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

#ifndef __GTP_PATH_H__
#define __GTP_PATH_H__

typedef struct _gtp_node_t gtp_node_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int gtp_server(ogs_socknode_t *snode);
int gtp_client(gtp_node_t *gnode);
int gtp_connect(ogs_sock_t *ipv4, ogs_sock_t *ipv6, gtp_node_t *gnode);

ogs_sock_t *gtp_local_sock_first(ogs_list_t *list);
ogs_sockaddr_t *gtp_local_addr_first(ogs_list_t *list);

int gtp_recv(ogs_socket_t fd, ogs_pkbuf_t **pkbuf);
int gtp_recvfrom(ogs_socket_t fd, ogs_pkbuf_t **pkbuf, ogs_sockaddr_t *from);
int gtp_send(gtp_node_t *gnode, ogs_pkbuf_t *pkbuf);
int gtp_sendto(gtp_node_t *gnode, ogs_pkbuf_t *pkbuf);

ogs_pkbuf_t *gtp_handle_echo_req(ogs_pkbuf_t *pkt);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
