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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SOCKET_H
#define OGS_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef SOCKET ogs_socket_t;
#else
typedef int ogs_socket_t;
#endif

#if !defined(_WIN32) && !defined(INVALID_SOCKET)
#define INVALID_SOCKET -1
#endif

typedef struct ogs_sock_s {
    int family;
    ogs_socket_t fd;

    ogs_sockaddr_t local_addr;
    ogs_sockaddr_t remote_addr;
} ogs_sock_t;

void ogs_socket_init(void);
void ogs_socket_final(void);

ogs_sock_t *ogs_sock_create(void);
void ogs_sock_destroy(ogs_sock_t *sock);

ogs_sock_t *ogs_sock_socket(int family, int type, int protocol);
int ogs_sock_bind(ogs_sock_t *sock, ogs_sockaddr_t *addr);
int ogs_sock_connect(ogs_sock_t *sock, ogs_sockaddr_t *addr);

int ogs_sock_listen(ogs_sock_t *sock);
ogs_sock_t *ogs_sock_accept(ogs_sock_t *sock);

ssize_t ogs_write(ogs_socket_t fd, const void *buf, size_t len);
ssize_t ogs_read(ogs_socket_t fd, void *buf, size_t len);

ssize_t ogs_send(ogs_socket_t fd, const void *buf, size_t len, int flags);
ssize_t ogs_sendto(ogs_socket_t fd,
        const void *buf, size_t len, int flags, const ogs_sockaddr_t *to);
ssize_t ogs_recv(ogs_socket_t fd, void *buf, size_t len, int flags);
ssize_t ogs_recvfrom(ogs_socket_t fd,
        void *buf, size_t len, int flags, ogs_sockaddr_t *from);

int ogs_closesocket(ogs_socket_t fd);

int ogs_nonblocking(ogs_socket_t fd);
int ogs_closeonexec(ogs_socket_t fd);
int ogs_listen_reusable(ogs_socket_t fd);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SOCKET_H */
