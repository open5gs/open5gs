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

ogs_sock_t *ogs_tcp_server(
        ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    ogs_sockopt_t option;

    ogs_assert(sa_list);

    ogs_sockopt_init(&option);
    if (socket_option)
        memcpy(&option, socket_option, sizeof option);

    addr = sa_list;
    while(addr) {
        new = ogs_sock_socket(addr->ogs_sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (new) {
            if (option.tcp_nodelay == true) {
                rv = ogs_tcp_nodelay(new->fd, true);
                ogs_assert(rv == OGS_OK);
            } else
                ogs_warn("TCP NO_DELAY Disabled");

            if (option.so_linger.l_onoff == true) {
                rv = ogs_so_linger(new->fd, option.so_linger.l_linger);
                ogs_assert(rv == OGS_OK);
            }

            rv = ogs_listen_reusable(new->fd, true);
            ogs_assert(rv == OGS_OK);

            if (ogs_sock_bind(new, addr) == OGS_OK) {
                ogs_debug("tcp_server() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "tcp_server() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;
    }

    rv = ogs_sock_listen(new);
    ogs_assert(rv == OGS_OK);

    return new;
}

ogs_sock_t *ogs_tcp_client(
        ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option)
{
    int rv;
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
        new = ogs_sock_socket(addr->ogs_sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (new) {
            if (option.sctp_nodelay == true) {
                rv = ogs_tcp_nodelay(new->fd, true);
                ogs_assert(rv == OGS_OK);
            } else
                ogs_warn("TCP NO_DELAY Disabled");

            if (option.so_linger.l_onoff == true) {
                rv = ogs_so_linger(new->fd, option.so_linger.l_linger);
                ogs_assert(rv == OGS_OK);
            }

            if (ogs_sock_connect(new, addr) == OGS_OK) {
                ogs_debug("tcp_client() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "tcp_client() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;
    }

    return new;
}
