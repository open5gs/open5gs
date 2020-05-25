/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "test-common.h"

ogs_socknode_t *test_gtpu_server(const char *ipstr, int port)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, port, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);

    sock = ogs_udp_server(node);
    ogs_assert(sock);

    return node;
}

ogs_pkbuf_t *test_gtpu_read(ogs_socknode_t *node)
{
    int rc = 0;
    ogs_pkbuf_t *recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    ogs_assert(node);
    ogs_assert(node->sock);

    while (1) {
        rc = ogs_recv(node->sock->fd, recvbuf->data, recvbuf->len, 0);
        if (rc <= 0) {
            if (errno == EAGAIN) {
                continue;
            }
            break;
        } else {
            break;
        }
    }
    recvbuf->len = rc;

    return recvbuf;
}

void test_gtpu_close(ogs_socknode_t *node)
{
    ogs_socknode_free(node);
}
