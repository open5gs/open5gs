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

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

int ogs_socketpair(int family, int type, int protocol, ogs_socket_t fd[2])
{
#ifndef WIN32
    return socketpair(family, type, protocol, fd);
#else
    int rc;
    ogs_socket_t server = INVALID_SOCKET;
    ogs_socket_t acceptor = INVALID_SOCKET;
    ogs_socket_t client = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t size;

    ogs_assert(family == AF_INET);
    ogs_assert(type == SOCK_STREAM);
    ogs_assert(protocol == 0);

    server = socket(family, type, protocol);
    ogs_assert(server != INVALID_SOCKET);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htobe32(INADDR_LOOPBACK);
    server_addr.sin_port = 0;

    rc = bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ogs_assert(rc == 0);
    rc = listen(server, 1);
    ogs_assert(rc == 0);

    client = socket(AF_INET, SOCK_STREAM, 0);
    ogs_assert(client != INVALID_SOCKET);

    memset(&client_addr, 0, sizeof(client_addr));
    size = sizeof(client_addr);
    rc = getsockname(server, (struct sockaddr *)&client_addr, &size);
    ogs_assert(rc == 0);
    ogs_assert(size == sizeof(client_addr));

    rc = connect(client, (struct sockaddr *)&client_addr, sizeof(client_addr));
    ogs_assert(rc == 0);

    size = sizeof(server_addr);
    acceptor = accept(server, (struct sockaddr *)&server_addr, &size);
    ogs_assert(acceptor != INVALID_SOCKET);
    ogs_assert(size == sizeof(server_addr));

    rc = getsockname(client, (struct sockaddr *)&client_addr, &size);
    ogs_assert(rc == 0);
    ogs_assert(size == sizeof(client_addr));
    ogs_assert(server_addr.sin_family == client_addr.sin_family);
    ogs_assert(server_addr.sin_addr.s_addr == client_addr.sin_addr.s_addr);
    ogs_assert(server_addr.sin_port == client_addr.sin_port);

    ogs_closesocket(server);
    fd[0] = client;
    fd[1] = acceptor;

    return OGS_OK;
#endif
}

