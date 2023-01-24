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

#include "core-config-private.h"

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

void ogs_socket_init(void)
{
#if _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    ogs_assert(err == 0);
#endif
}

void ogs_socket_final(void)
{
}

ogs_sock_t *ogs_sock_create(void)
{
    ogs_sock_t *sock = NULL;

    sock = ogs_calloc(1, sizeof(*sock));
    if (!sock) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    sock->fd = INVALID_SOCKET;

    return sock;
}

void ogs_sock_destroy(ogs_sock_t *sock)
{
    ogs_assert(sock);

    if (sock->fd != INVALID_SOCKET) {
        ogs_closesocket(sock->fd);
    }
    sock->fd = INVALID_SOCKET;

    ogs_free(sock);
}

ogs_sock_t *ogs_sock_socket(int family, int type, int protocol)
{
    ogs_sock_t *sock = NULL;

    sock = ogs_sock_create();
    ogs_assert(sock);

    sock->family = family;
    sock->fd = socket(sock->family, type, protocol);
    if (sock->fd < 0) {
        ogs_sock_destroy(sock);
        
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
            "socket create(%d:%d:%d) failed", sock->family, type, protocol);
        return NULL;
    }

    ogs_debug("socket create(%d:%d:%d)", sock->family, type, protocol);

    return sock;
}

int ogs_sock_bind(ogs_sock_t *sock, ogs_sockaddr_t *addr)
{
    char buf[OGS_ADDRSTRLEN];
    socklen_t addrlen;

    ogs_assert(sock);
    ogs_assert(addr);

    addrlen = ogs_sockaddr_len(addr);
    ogs_assert(addrlen);

    if (bind(sock->fd, &addr->sa, addrlen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "socket bind(%d) [%s]:%d failed",
                addr->ogs_sa_family, OGS_ADDR(addr, buf), OGS_PORT(addr));
        return OGS_ERROR;
    }

    memcpy(&sock->local_addr, addr, sizeof(sock->local_addr));

    ogs_debug("socket bind %s:%d", OGS_ADDR(addr, buf), OGS_PORT(addr));

    return OGS_OK;
}

int ogs_sock_connect(ogs_sock_t *sock, ogs_sockaddr_t *addr)
{
    char buf[OGS_ADDRSTRLEN];
    socklen_t addrlen;

    ogs_assert(sock);
    ogs_assert(addr);

    addrlen = ogs_sockaddr_len(addr);
    ogs_assert(addrlen);

    if (connect(sock->fd, &addr->sa, addrlen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "socket connect[%s]:%d failed",
                OGS_ADDR(addr, buf), OGS_PORT(addr));
        return OGS_ERROR;
    }

    memcpy(&sock->remote_addr, addr, sizeof(sock->remote_addr));

    ogs_debug("socket connect %s:%d\n", OGS_ADDR(addr, buf), OGS_PORT(addr));

    return OGS_OK;
}

int ogs_sock_listen(ogs_sock_t *sock)
{
    int rc;
    ogs_assert(sock);

    rc = listen(sock->fd, 5);
    if (rc < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "listen failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

ogs_sock_t *ogs_sock_accept(ogs_sock_t *sock)
{
    ogs_sock_t *new_sock = NULL;

    int new_fd = -1;
    ogs_sockaddr_t addr;
    socklen_t addrlen;

    ogs_assert(sock);

    memset(&addr, 0, sizeof(addr));
    addrlen = sizeof(addr.ss);

    new_fd = accept(sock->fd, &addr.sa, &addrlen);
    if (new_fd < 0) {
        return NULL;
    }

    new_sock = ogs_sock_create();
    ogs_assert(new_sock);

    new_sock->family = sock->family;
    new_sock->fd = new_fd;

    memcpy(&new_sock->remote_addr, &addr, sizeof(new_sock->remote_addr));

    return new_sock;
}

ssize_t ogs_write(ogs_socket_t fd, const void *buf, size_t len)
{
    ogs_assert(fd != INVALID_SOCKET);

    return write(fd, buf, len);
}

ssize_t ogs_read(ogs_socket_t fd, void *buf, size_t len)
{
    ogs_assert(fd != INVALID_SOCKET);

    return read(fd, buf, len);
}

ssize_t ogs_send(ogs_socket_t fd, const void *buf, size_t len, int flags)
{
    ogs_assert(fd != INVALID_SOCKET);

    return send(fd, buf, len, flags);
}

ssize_t ogs_sendto(ogs_socket_t fd,
        const void *buf, size_t len, int flags, const ogs_sockaddr_t *to)
{
    socklen_t addrlen;

    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(to);

    addrlen = ogs_sockaddr_len(to);
    ogs_assert(addrlen);

    return sendto(fd, buf, len, flags, &to->sa, addrlen);
}

ssize_t ogs_recv(ogs_socket_t fd, void *buf, size_t len, int flags)
{
    ogs_assert(fd != INVALID_SOCKET);
    return recv(fd, buf, len, flags);
}

ssize_t ogs_recvfrom(ogs_socket_t fd,
        void *buf, size_t len, int flags, ogs_sockaddr_t *from)
{
    socklen_t addrlen = sizeof(struct sockaddr_storage);

    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(from);

    memset(from, 0, sizeof *from);
    return recvfrom(fd, buf, len, flags, &from->sa, &addrlen);
}

int ogs_closesocket(ogs_socket_t fd)
{
    int r;
#ifdef _WIN32
    r = closesocket(fd);
#else
    r = close(fd);
#endif
    if (r != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "closesocket failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}
