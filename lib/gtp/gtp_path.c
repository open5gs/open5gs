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

#include "gtp_message.h"
#include "gtp_conv.h"
#include "gtp_node.h"

#include "gtp_path.h"

ogs_sock_t *gtp_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *gtp;
    ogs_assert(node);

    gtp = ogs_udp_server(node);
    ogs_assert(gtp);

    ogs_info("gtp_server() [%s]:%d",
            OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

    return gtp;
}

int gtp_connect(ogs_sock_t *ipv4, ogs_sock_t *ipv6, gtp_node_t *gnode)
{
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(ipv4 || ipv6);
    ogs_assert(gnode);
    ogs_assert(gnode->sa_list);

    addr = gnode->sa_list;
    while(addr)
    {
        ogs_sock_t *sock = NULL;

        if (addr->ogs_sa_family == AF_INET) sock = ipv4;
        else if (addr->ogs_sa_family == AF_INET6) sock = ipv6;
        else
            ogs_assert_if_reached();

        if (sock)
        {
            ogs_info("gtp_connect() [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));

            gnode->sock = sock;
            memcpy(&gnode->conn, addr, sizeof gnode->conn);
            break;
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno,
                "gtp_connect() [%s]:%d failed",
                OGS_ADDR(gnode->sa_list, buf), OGS_PORT(gnode->sa_list));
        return OGS_ERROR;
    }

    return OGS_OK;
}

ogs_sock_t *gtp_local_sock_first(ogs_list_t *list)
{
    ogs_socknode_t *snode = NULL;

    ogs_assert(list);
    ogs_list_for_each(list, snode)
    {
        if (snode->sock)
            return snode->sock;
    }

    return NULL;
}

ogs_sockaddr_t *gtp_local_addr_first(ogs_list_t *list)
{
    ogs_socknode_t *snode = NULL;

    ogs_assert(list);
    ogs_list_for_each(list, snode)
    {
        ogs_sock_t *sock = snode->sock;
        ogs_assert(snode->sock);

        return &sock->local_addr;
    }

    return NULL;
}

int gtp_recv(ogs_socket_t fd, ogs_pkbuf_t **pkbuf)
{
    ssize_t size;

    ogs_assert(fd != INVALID_SOCKET);

    *pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
#if DEPRECATED
    if ((*pkbuf) == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        ogs_error("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        ogs_recv(fd, tmp_buf, MAX_SDU_LEN, 0);

        return OGS_ERROR;
    }
#endif
    ogs_pkbuf_put(*pkbuf, MAX_SDU_LEN);

    size = ogs_recv(fd, (*pkbuf)->data, (*pkbuf)->len, 0);
    if (size <= 0)
    {
        ogs_pkbuf_free((*pkbuf));
        ogs_error("ogs_recv failed");

        return OGS_ERROR;
    }
    else
    {
        ogs_pkbuf_trim(*pkbuf, size);

        return OGS_OK;;
    }
}

int gtp_recvfrom(ogs_socket_t fd, ogs_pkbuf_t **pkbuf, ogs_sockaddr_t *from)
{
    ssize_t size;

    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(from);

    *pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
#if DEPRECATED
    if ((*pkbuf) == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        ogs_error("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        ogs_recv(fd, tmp_buf, MAX_SDU_LEN, 0);

        return OGS_ERROR;
    }
#endif
    ogs_pkbuf_put(*pkbuf, MAX_SDU_LEN);

    size = ogs_recvfrom(fd, (*pkbuf)->data, (*pkbuf)->len, 0, from);
    if (size <= 0)
    {
        ogs_pkbuf_free((*pkbuf));
        ogs_error("ogs_recvfrom() failed");

        return OGS_ERROR;
    }
    else
    {
        ogs_pkbuf_trim(*pkbuf, size);

        return OGS_OK;;
    }
}

int gtp_send(gtp_node_t *gnode, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;

    ogs_assert(gnode);
    ogs_assert(pkbuf);
    sock = gnode->sock;
    ogs_assert(sock);

    sent = ogs_send(sock->fd, pkbuf->data, pkbuf->len, 0);
    if (sent < 0 || sent != pkbuf->len)
    {
        ogs_error("ogs_send() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int gtp_sendto(gtp_node_t *gnode, ogs_pkbuf_t *pkbuf)
{
    ssize_t sent;
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *conn = NULL;

    ogs_assert(gnode);
    ogs_assert(pkbuf);
    sock = gnode->sock;
    ogs_assert(sock);
    conn = &gnode->conn;
    ogs_assert(conn);

    sent = ogs_sendto(sock->fd, pkbuf->data, pkbuf->len, 0, conn);
    if (sent < 0 || sent != pkbuf->len)
    {
        ogs_error("ogs_send() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

ogs_pkbuf_t *gtp_handle_echo_req(ogs_pkbuf_t *pkb)
{
    gtp_header_t *gtph = NULL;
    ogs_pkbuf_t *pkb_resp = NULL;
    gtp_header_t *gtph_resp = NULL;
    uint16_t length;
    int idx;

    ogs_assert(pkb);

    gtph = (gtp_header_t *)pkb->data;
    /* Check GTP version. Now only support GTPv1(version = 1) */
    if ((gtph->flags >> 5) != 1)
    {
        return NULL;
    }

    if (gtph->type != GTPU_MSGTYPE_ECHO_REQ)
    {
        return NULL;
    }


    pkb_resp = ogs_pkbuf_alloc(NULL,
            100 /* enough for ECHO_RSP; use smaller buffer */);
    ogs_pkbuf_put(pkb_resp, 100);
    gtph_resp = (gtp_header_t *)pkb_resp->data;

    /* reply back immediately */
    gtph_resp->flags = (1 << 5); /* set version */
    gtph_resp->flags |= (1 << 4); /* set PT */
    gtph_resp->type = GTPU_MSGTYPE_ECHO_RSP;
    length = 0;     /* length of Recovery IE */
    gtph_resp->length = htons(length); /* to be overwriten */
    gtph_resp->teid = 0;
    idx = 8;

    if (gtph->flags & (GTPU_FLAGS_PN | GTPU_FLAGS_S))
    {
        length += 4;
        if (gtph->flags & GTPU_FLAGS_S)
        {
            /* sequence exists */
            gtph_resp->flags |= GTPU_FLAGS_S;
            *((uint8_t *)pkb_resp->data + idx) = *((uint8_t *)pkb->data + idx);
            *((uint8_t *)pkb_resp->data + idx + 1) =
                *((uint8_t *)pkb->data + idx + 1);
        }
        else
        {
            *((uint8_t *)pkb_resp->data + idx) = 0;
            *((uint8_t *)pkb_resp->data + idx + 1) = 0;
        }
        idx += 2;
        if (gtph->flags & GTPU_FLAGS_PN)
        {
            /* sequence exists */
            gtph_resp->flags |= GTPU_FLAGS_PN;
            *((uint8_t *)pkb_resp->data + idx) = *((uint8_t *)pkb->data + idx);
        }
        else
        {
            *((uint8_t *)pkb_resp->data + idx) = 0;
        }
        idx++;
        *((uint8_t *)pkb_resp->data + idx) = 0; /* next-extension header */
        idx++;
    }
    
    /* fill Recovery IE */
    length += 2;
    *((uint8_t *)pkb_resp->data + idx) = 14; idx++; /* type */
    *((uint8_t *)pkb_resp->data + idx) = 0; idx++; /* restart counter */

    gtph_resp->length = htons(length);
    ogs_pkbuf_trim(pkb_resp, idx); /* buffer length */

    return pkb_resp;
}
