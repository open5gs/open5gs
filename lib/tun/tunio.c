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

#include "ogs-tun.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

ogs_pkbuf_t *ogs_tun_read(ogs_socket_t fd, ogs_pkbuf_pool_t *packet_pool)
{
    ogs_pkbuf_t *recvbuf = NULL;
    int n;

    ogs_assert(fd != INVALID_SOCKET);

    recvbuf = ogs_pkbuf_alloc(packet_pool, OGS_MAX_PKT_LEN);
    ogs_assert(recvbuf);
    ogs_pkbuf_reserve(recvbuf, OGS_TUN_MAX_HEADROOM);
    ogs_pkbuf_put(recvbuf, OGS_MAX_PKT_LEN-OGS_TUN_MAX_HEADROOM);

    n = ogs_read(fd, recvbuf->data, recvbuf->len);
    if (n <= 0) {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno, "ogs_read() failed");
        ogs_pkbuf_free(recvbuf);
        return NULL;
    }

    ogs_pkbuf_trim(recvbuf, n);

#if defined(__APPLE__)
    /* Remove Null/Loopback Header (4bytes) */
    ogs_pkbuf_pull(recvbuf, 4);
#endif

    return recvbuf;
}

int ogs_tun_write(ogs_socket_t fd, ogs_pkbuf_t *pkbuf)
{
#if defined(__APPLE__)
    uint8_t version;
    uint32_t family;
#endif

    ogs_assert(fd != INVALID_SOCKET);
    ogs_assert(pkbuf);

#if defined(__APPLE__)
    version = (*((unsigned char *)pkbuf->data) >> 4) & 0xf;

    if (version == 4) {
        family = htobe32(AF_INET);
    } else if (version == 6) {
        family = htobe32(AF_INET6);
    } else {
        ogs_error("Invalid packet [IP version:%d, Packet Length:%d]",
                version, pkbuf->len);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        return OGS_ERROR;
    }

    ogs_pkbuf_push(pkbuf, sizeof(family));
    memcpy(pkbuf->data, &family, sizeof(family));
#endif

    if (ogs_write(fd, pkbuf->data, pkbuf->len) <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "ogs_write() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}
