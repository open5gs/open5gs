/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SBCAP_PATH_H
#define SBCAP_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

int sbcap_open(void);
void sbcap_close(void);

ogs_sock_t *sbcap_server(ogs_socknode_t *node);
void sbcap_recv_upcall(short when, ogs_socket_t fd, void *data);

void sbcap_accept_handler(ogs_sock_t *sock);
void sbcap_recv_handler(ogs_sock_t *sock);

int sbcap_send(ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, ogs_sockaddr_t *addr, uint16_t stream_no);

#ifdef __cplusplus
}
#endif

#endif /* SBCAP_PATH_H */ 