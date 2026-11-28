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

#include "ogs-gtp.h"

ogs_sock_t *ogs_gtp_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *gtp;
    ogs_assert(node);

    gtp = ogs_udp_server(node->addr, node->option);
    if (gtp) {
        ogs_info("gtp_server() [%s]:%d",
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

        node->sock = gtp;
    }

    return gtp;
}

int ogs_gtp_connect(ogs_sock_t *ipv4, ogs_sock_t *ipv6, ogs_gtp_node_t *gnode)
{
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(ipv4 || ipv6);
    ogs_assert(gnode);
    ogs_assert(gnode->sa_list);

    addr = gnode->sa_list;
    while (addr) {
        ogs_sock_t *sock = NULL;

        if (addr->ogs_sa_family == AF_INET)
            sock = ipv4;
        else if (addr->ogs_sa_family == AF_INET6)
            sock = ipv6;
        else
            ogs_assert_if_reached();

        if (sock) {
            ogs_info("gtp_connect() [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));

            gnode->sock = sock;
            memcpy(&gnode->addr, addr, sizeof gnode->addr);
            break;
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno,
                "gtp_connect() [%s]:%d failed",
                OGS_ADDR(gnode->sa_list, buf), OGS_PORT(gnode->sa_list));
        return OGS_ERROR;
    }

    return OGS_OK;
}

static ogs_pkbuf_t *gtpu_recv_buffer(ogs_pkbuf_t **slot,
        ogs_pkbuf_pool_t *pool, int headroom)
{
    if (!*slot) {
        *slot = ogs_pkbuf_alloc(pool, OGS_MAX_PKT_LEN);
        if (*slot)
            ogs_pkbuf_reserve(*slot, headroom);
    }
    return *slot;
}

void ogs_gtpu_recv(ogs_sock_t *sock, ogs_pkbuf_pool_t *pool,
        ogs_pkbuf_t **buffers, unsigned int budget, int headroom,
        ogs_gtpu_recv_handler_t handler)
{
    unsigned int i;
#ifdef HAVE_RECVMMSG
    struct mmsghdr msgs[budget];
    struct iovec iov[budget];
    ogs_sockaddr_t from[budget];
    int received;
#endif

    ogs_assert(sock);
    ogs_assert(sock->fd != INVALID_SOCKET);

#ifdef HAVE_RECVMMSG
    memset(msgs, 0, sizeof(msgs));
    memset(from, 0, sizeof(from));
    for (i = 0; i < budget; i++) {
        ogs_pkbuf_t *pkbuf = gtpu_recv_buffer(&buffers[i], pool, headroom);
        if (!pkbuf)
            break;

        iov[i].iov_base = pkbuf->data;
        iov[i].iov_len = ogs_pkbuf_tailroom(pkbuf);
        msgs[i].msg_hdr.msg_iov = &iov[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
        msgs[i].msg_hdr.msg_name = &from[i].sa;
        msgs[i].msg_hdr.msg_namelen = sizeof(from[i].ss);
    }
    if (i == 0)
        return;

    received = recvmmsg(sock->fd, msgs, i, MSG_DONTWAIT, NULL);
    if (received < 0) {
        ogs_err_t err = ogs_socket_errno;
        if (err != OGS_EAGAIN && err != EINTR)
            ogs_log_message(OGS_LOG_ERROR, err, "recvmmsg() failed");
        return;
    }

    /* Leave unused slots ready for the next wakeup. */
    for (i = 0; i < received; i++) {
        ogs_pkbuf_t *pkbuf = buffers[i];

        if (msgs[i].msg_len == 0)
            continue;

        ogs_pkbuf_put(pkbuf, msgs[i].msg_len);
        buffers[i] = NULL;
        handler(sock->fd, sock, pkbuf, &from[i]);
    }
#else
    for (i = 0; i < budget; i++) {
        ssize_t size;
        ogs_sockaddr_t from;
        ogs_pkbuf_t *pkbuf = gtpu_recv_buffer(&buffers[0], pool, headroom);
        if (!pkbuf)
            return;

        size = ogs_recvfrom(sock->fd, pkbuf->data,
                ogs_pkbuf_tailroom(pkbuf), 0, &from);
        if (size < 0) {
            ogs_err_t err = ogs_socket_errno;
            if (err != OGS_EAGAIN)
                ogs_log_message(OGS_LOG_ERROR, err, "ogs_recvfrom() failed");
            return;
        }
        if (size == 0)
            continue;

        ogs_pkbuf_put(pkbuf, size);
        buffers[0] = NULL;
        handler(sock->fd, sock, pkbuf, &from);
    }
#endif
}

void ogs_gtpu_recv_clear(ogs_pkbuf_t **buffers, unsigned int budget)
{
    unsigned int i;

    for (i = 0; i < budget; i++) {
        if (buffers[i]) {
            ogs_pkbuf_free(buffers[i]);
            buffers[i] = NULL;
        }
    }
}

int ogs_gtp_send(ogs_gtp_node_t *gnode, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;

    ogs_assert(gnode);
    ogs_assert(pkbuf);
    sock = gnode->sock;
    ogs_assert(sock);

    sent = ogs_send(sock->fd, pkbuf->data, pkbuf->len, 0);
    if (sent < 0 || sent != pkbuf->len) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_gtp_send() failed");
        }
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp_sendto(ogs_gtp_node_t *gnode, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(gnode);
    ogs_assert(pkbuf);
    sock = gnode->sock;
    ogs_assert(sock);
    addr = &gnode->addr;
    ogs_assert(addr);

    sent = ogs_sendto(sock->fd, pkbuf->data, pkbuf->len, 0, addr);
    if (sent < 0 || sent != pkbuf->len) {
        if (ogs_socket_errno != OGS_EAGAIN) {
            char buf[OGS_ADDRSTRLEN];
            int err = ogs_socket_errno;
            ogs_log_message(OGS_LOG_ERROR, err,
                    "ogs_gtp_sendto(%u, %p, %u, 0, %s:%u) failed",
                    sock->fd, pkbuf->data, pkbuf->len,
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        }
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp_send_with_teid(
        ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, uint32_t teid,
        ogs_sockaddr_t *to)
{
    char buf[OGS_ADDRSTRLEN];
    ssize_t sent;
    ogs_gtp2_header_t *gtp_h = NULL;

    ogs_assert(sock);
    ogs_assert(pkbuf);
    ogs_assert(to);

    gtp_h = (ogs_gtp2_header_t *)pkbuf->data;
    ogs_assert(gtp_h);
    gtp_h->teid = htobe32(teid);

    ogs_trace("SEND GTP-U to Peer[%s] : TEID[0x%x]", OGS_ADDR(to, buf), teid);

    sent = ogs_sendto(sock->fd, pkbuf->data, pkbuf->len, 0, to);
    if (sent < 0 || sent != pkbuf->len) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_sendto() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

void ogs_gtp_send_error_message(
        ogs_gtp_xact_t *xact, uint32_t teid, uint8_t type, uint8_t cause_value)
{
    switch (xact->gtp_version) {
    case 1:
        ogs_gtp1_send_error_message(xact, teid, type, cause_value);
        break;
    case 2:
        ogs_gtp2_send_error_message(xact, teid, type, cause_value);
        break;
    }
}
