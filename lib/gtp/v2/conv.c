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

int ogs_gtp2_f_teid_to_sockaddr(
    ogs_gtp2_f_teid_t *f_teid, uint16_t port, ogs_sockaddr_t **list)
{
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(f_teid);
    ogs_assert(list);

    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    if (!addr) {
        ogs_error("ogs_calloc() failed");
        return OGS_ERROR;
    }
    addr->ogs_sa_family = AF_INET;
    addr->ogs_sin_port = htobe16(port);

    addr6 = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    if (!addr6) {
        ogs_error("ogs_calloc() failed");
        ogs_free(addr);
        return OGS_ERROR;
    }
    addr6->ogs_sa_family = AF_INET6;
    addr6->ogs_sin_port = htobe16(port);

    if (f_teid->ipv4 && f_teid->ipv6) {
        addr->next = addr6;

        addr->sin.sin_addr.s_addr = f_teid->both.addr;
        memcpy(addr6->sin6.sin6_addr.s6_addr, f_teid->both.addr6, OGS_IPV6_LEN);

        *list = addr;
    } else if (f_teid->ipv4) {
        addr->sin.sin_addr.s_addr = f_teid->addr;
        ogs_free(addr6);

        *list = addr;
    } else if (f_teid->ipv6) {
        memcpy(addr6->sin6.sin6_addr.s6_addr, f_teid->addr6, OGS_IPV6_LEN);
        ogs_free(addr);

        *list = addr6;
    } else {
        ogs_error("No IPv4 or IPv6");
        ogs_free(addr);
        ogs_free(addr6);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp2_sockaddr_to_f_teid(ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6,
        ogs_gtp2_f_teid_t *f_teid, int *len)
{
    ogs_assert(f_teid);

    if (addr && addr6) {
        f_teid->ipv4 = 1;
        f_teid->both.addr = addr->sin.sin_addr.s_addr;
        f_teid->ipv6 = 1;
        memcpy(f_teid->both.addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_GTP2_F_TEID_IPV4V6_LEN;
    } else if (addr) {
        f_teid->ipv4 = 1;
        f_teid->ipv6 = 0;
        f_teid->addr = addr->sin.sin_addr.s_addr;
        *len = OGS_GTP2_F_TEID_IPV4_LEN;
    } else if (addr6) {
        f_teid->ipv4 = 0;
        f_teid->ipv6 = 1;
        memcpy(f_teid->addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_GTP2_F_TEID_IPV6_LEN;
    } else {
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp2_f_teid_to_ip(ogs_gtp2_f_teid_t *f_teid, ogs_ip_t *ip)
{
    ogs_assert(ip);
    ogs_assert(f_teid);

    memset(ip, 0, sizeof(ogs_ip_t));

    ip->ipv4 = f_teid->ipv4;
    ip->ipv6 = f_teid->ipv6;

    if (ip->ipv4 && ip->ipv6) {
        ip->addr = f_teid->both.addr;
        memcpy(ip->addr6, f_teid->both.addr6, OGS_IPV6_LEN);
        ip->len = OGS_IPV4V6_LEN;
    } else if (ip->ipv4) {
        ip->addr = f_teid->addr;
        ip->len = OGS_IPV4_LEN;
    } else if (ip->ipv6) {
        memcpy(ip->addr6, f_teid->addr6, OGS_IPV6_LEN);
        ip->len = OGS_IPV6_LEN;
    } else {
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp2_ip_to_f_teid(ogs_ip_t *ip, ogs_gtp2_f_teid_t *f_teid, int *len)
{
    ogs_assert(ip);
    ogs_assert(f_teid);

    f_teid->ipv4 = ip->ipv4;
    f_teid->ipv6 = ip->ipv6;

    if (f_teid->ipv4 && f_teid->ipv6) {
        f_teid->both.addr = ip->addr;
        memcpy(f_teid->both.addr6, ip->addr6, OGS_IPV6_LEN);
        *len = OGS_GTP2_F_TEID_IPV4V6_LEN;
    } else if (f_teid->ipv4) {
        f_teid->addr = ip->addr;
        *len = OGS_GTP2_F_TEID_IPV4_LEN;
    } else if (f_teid->ipv6) {
        memcpy(f_teid->addr6, ip->addr6, OGS_IPV6_LEN);
        *len = OGS_GTP2_F_TEID_IPV6_LEN;
    } else {
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}
