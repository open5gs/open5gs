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

ogs_socknode_t *testsctp_server(const char *ipstr, int port)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, port, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_server(SOCK_SEQPACKET, node);
    ogs_assert(node->sock);

    return node;
}

ogs_socknode_t *testsctp_client(const char *ipstr, int port)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, port, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_client(SOCK_STREAM, node);
    ogs_assert(node->sock);

    return node;
}

ogs_sockaddr_t last_addr;

ogs_pkbuf_t *testsctp_read(ogs_socknode_t *node, int type)
{
    int size;
    ogs_pkbuf_t *recvbuf = NULL;

    ogs_assert(node);
    ogs_assert(node->sock);

    recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(recvbuf);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    size = ogs_sctp_recvdata(node->sock, recvbuf->data, OGS_MAX_SDU_LEN,
            type == 1 ? &last_addr : NULL, NULL);
    if (size <= 0) {
        ogs_error("sgsap_recv() failed");
        return NULL;
    }

    ogs_pkbuf_trim(recvbuf, size);
    return recvbuf;;
}

int testsctp_send(ogs_socknode_t *node, ogs_pkbuf_t *pkbuf,
        int ppid, uint16_t stream_no, int type)
{
    int sent;

    ogs_assert(node);
    ogs_assert(node->sock);
    ogs_assert(pkbuf);

    sent = ogs_sctp_sendmsg(node->sock, pkbuf->data, pkbuf->len,
            type == 1 ? &last_addr : NULL, ppid, stream_no);
    if (sent < 0 || sent != pkbuf->len) {
        ogs_error("ogs_sctp_sendmsg error (%d:%s)", errno, strerror(errno));
        return OGS_ERROR;
    }
    ogs_pkbuf_free(pkbuf);

    return OGS_OK;
}

ogs_socknode_t *tests1ap_client(int family)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    if (family == AF_INET6)
        ogs_copyaddrinfo(&addr, test_self()->s1ap_addr6);
    else
        ogs_copyaddrinfo(&addr, test_self()->s1ap_addr);

    ogs_assert(addr);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_client(SOCK_STREAM, node);
    ogs_assert(node->sock);

    return node;
}

ogs_socknode_t *testngap_client(int family)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    if (family == AF_INET6)
        ogs_copyaddrinfo(&addr, test_self()->ngap_addr6);
    else
        ogs_copyaddrinfo(&addr, test_self()->ngap_addr);

    ogs_assert(addr);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_client(SOCK_STREAM, node);
    ogs_assert(node->sock);

    return node;
}
