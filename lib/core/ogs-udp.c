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

ogs_sock_t *ogs_udp_socket(int family, ogs_socknode_t *node)
{
    ogs_sock_t *sock = NULL;
    sock = ogs_sock_socket(family, SOCK_DGRAM, IPPROTO_UDP);
    ogs_assert(sock);

    ogs_debug("udp_socket() family:%d", family);

    return sock;
}

ogs_sock_t *ogs_udp_server(ogs_socknode_t *node)
{
    int rv;
    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(node);
    ogs_assert(node->addr);

    addr = node->addr;
    while (addr) {
        new = ogs_udp_socket(addr->ogs_sa_family, node);
        if (new) {
            rv = ogs_listen_reusable(new->fd);
            ogs_assert(rv == OGS_OK);

            if (ogs_sock_bind(new, addr) == OGS_OK) {
                ogs_debug("udp_server() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "udp_server() [%s]:%d failed",
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));
        return NULL;
    }

    node->sock = new;

    return new;
}

ogs_sock_t *ogs_udp_client(ogs_socknode_t *node)
{
    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(node);
    ogs_assert(node->addr);

    addr = node->addr;
    while (addr) {
        new = ogs_udp_socket(addr->ogs_sa_family, node);
        if (new) {
            if (ogs_sock_connect(new, addr) == OGS_OK) {
                ogs_debug("udp_client() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "udp_client() [%s]:%d failed",
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));
        return NULL;;
    }

    node->sock = new;

    return new;
}

int ogs_udp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa_list)
{
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sock);
    ogs_assert(sa_list);

    addr = sa_list;
    while (addr) {
        if (ogs_sock_connect(sock, addr) == OGS_OK) {
            ogs_debug("udp_connect() [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
            break;
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "udp_connect() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return OGS_ERROR;
    }

    return OGS_OK;
}
