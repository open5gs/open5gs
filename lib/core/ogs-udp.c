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

ogs_sock_t *ogs_udp_server(
        ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    ogs_sockopt_t option;

    ogs_assert(sa_list);

    ogs_sockopt_init(&option);
    if (socket_option)
        memcpy(&option, socket_option, sizeof option);

    addr = sa_list;
    while (addr) {
        new = ogs_sock_socket(addr->ogs_sa_family, SOCK_DGRAM, IPPROTO_UDP);
        if (!new) {
            addr = addr->next;
            continue;
        }
        if (ogs_sock_bind(new, addr) != OGS_OK) {
            ogs_sock_destroy(new);
            addr = addr->next;
            continue;
        }
        ogs_debug("udp_server() [%s]:%d", OGS_ADDR(addr, buf), OGS_PORT(addr));
        if (option.so_bindtodevice) {
            if (ogs_bind_to_device(new->fd, option.so_bindtodevice) != OGS_OK) {
                ogs_sock_destroy(new);
                addr = addr->next;
                continue;
            }
            ogs_info("udp_server() [%s]:%d bound to device `%s`",
                    OGS_ADDR(addr, buf), OGS_PORT(addr),
                    option.so_bindtodevice);
        }
        break;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "udp_server() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;
    }

    return new;
}

ogs_sock_t *ogs_udp_client(
        ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;

    ogs_sockopt_t option;

    ogs_assert(sa_list);

    ogs_sockopt_init(&option);
    if (socket_option)
        memcpy(&option, socket_option, sizeof option);

    addr = sa_list;
    while (addr) {
        new = ogs_sock_socket(addr->ogs_sa_family, SOCK_DGRAM, IPPROTO_UDP);
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
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;;
    }

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
