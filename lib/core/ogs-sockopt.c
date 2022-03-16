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

#if HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

void ogs_sockopt_init(ogs_sockopt_t *option)
{
    ogs_assert(option);

    memset(option, 0, sizeof *option);

    option->sctp.spp_hbinterval = 5000;         /* 5 seconds */
    option->sctp.spp_sackdelay = 200;           /* 200 ms */
    option->sctp.srto_initial = 3000;           /* 3 seconds */
    option->sctp.srto_min = 1000;               /* 1 seconds */
    option->sctp.srto_max = 5000;               /* 5 seconds */
    option->sctp.sinit_num_ostreams = OGS_DEFAULT_SCTP_MAX_NUM_OF_OSTREAMS;
    option->sctp.sinit_max_instreams = 65535;
    option->sctp.sinit_max_attempts = 4;
    option->sctp.sinit_max_init_timeo = 8000;   /* 8 seconds */

    option->sctp_nodelay = true;
    option->tcp_nodelay = true;
}

int ogs_nonblocking(ogs_socket_t fd)
{
#ifdef _WIN32
    int rc;
    ogs_assert(fd != INVALID_SOCKET);

    u_long io_mode = 1;
    rc = ioctlsocket(fd, FIONBIO, &io_mode);
    if (rc != OGS_OK) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "ioctlsocket failed");
        return OGS_ERROR;
    }
#else
    int rc;
    int flags;
    ogs_assert(fd != INVALID_SOCKET);

    flags = fcntl(fd, F_GETFL, NULL);
    if (flags < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "F_GETFL failed");
        return OGS_ERROR;
    }
    if (!(flags & O_NONBLOCK)) {
        rc = fcntl(fd, F_SETFL, (flags | O_NONBLOCK));
        if (rc != OGS_OK) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "F_SETFL failed");
            return OGS_ERROR;
        }
    }
#endif

    return OGS_OK;
}

int ogs_closeonexec(ogs_socket_t fd)
{
#ifndef _WIN32
    int rc;
    int flags;

    ogs_assert(fd != INVALID_SOCKET);
    flags = fcntl(fd, F_GETFD, NULL);
    if (flags < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "F_GETFD failed");
        return OGS_ERROR;
    }
    if (!(flags & FD_CLOEXEC)) {
        rc = fcntl(fd, F_SETFD, (flags | FD_CLOEXEC));
        if (rc != OGS_OK) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "F_SETFD failed");
            return OGS_ERROR;
        }
    }
#endif

    return OGS_OK;
}

int ogs_listen_reusable(ogs_socket_t fd, int on)
{
#if defined(SO_REUSEADDR) && !defined(_WIN32)
    int rc;

    ogs_assert(fd != INVALID_SOCKET);

    ogs_debug("Turn on SO_REUSEADDR");
    rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int));
    if (rc != OGS_OK) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(SOL_SOCKET, SO_REUSEADDR) failed");
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

int ogs_tcp_nodelay(ogs_socket_t fd, int on)
{
#if defined(TCP_NODELAY) && !defined(_WIN32)
    int rc;

    ogs_assert(fd != INVALID_SOCKET);

    ogs_debug("Turn on TCP_NODELAY");
    rc = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(int));
    if (rc != OGS_OK) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(IPPROTO_TCP, TCP_NODELAY) failed");
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

int ogs_so_linger(ogs_socket_t fd, int l_linger)
{
#if defined(SO_LINGER) && !defined(_WIN32)
    struct linger l;
    int rc;

    ogs_assert(fd != INVALID_SOCKET);

    memset(&l, 0, sizeof(l));
    l.l_onoff = 1;
    l.l_linger = l_linger;

    ogs_debug("SO_LINGER:[%d]", l_linger);
    rc = setsockopt(fd, SOL_SOCKET, SO_LINGER,
            (void *)&l, sizeof(struct linger));
    if (rc != OGS_OK) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(SOL_SOCKET, SO_LINGER) failed");
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

int ogs_bind_to_device(ogs_socket_t fd, const char *device)
{
#if defined(SO_BINDTODEVICE) && !defined(_WIN32)
    int rc;

    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(device);

    ogs_debug("SO_BINDTODEVICE:[%s]", device);
    rc = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, device, strlen(device)+1);
    if (rc != OGS_OK) {
        int err = ogs_errno;
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(SOL_SOCKET, SO_BINDTODEVICE, %s) failed", device);
        if (err == OGS_EPERM)
            ogs_error("You need to grant CAP_NET_RAW privileges to use SO_BINDTODEVICE.");
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}
