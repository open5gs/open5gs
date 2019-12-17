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

#include "ogs-pfcp.h"

#define OGS_PFCP_F_TEID_HDR_LEN     5
#define OGS_PFCP_F_TEID_IPV4_LEN    (OGS_IPV4_LEN + OGS_PFCP_F_TEID_HDR_LEN)
#define OGS_PFCP_F_TEID_IPV6_LEN    (OGS_IPV6_LEN + OGS_PFCP_F_TEID_HDR_LEN)
#define OGS_PFCP_F_TEID_IPV4V6_LEN  (OGS_IPV4V6_LEN + OGS_PFCP_F_TEID_HDR_LEN)

#define OGS_PFCP_F_SEID_HDR_LEN     9
#define OGS_PFCP_F_SEID_IPV4_LEN    (OGS_IPV4_LEN + OGS_PFCP_F_SEID_HDR_LEN)
#define OGS_PFCP_F_SEID_IPV6_LEN    (OGS_IPV6_LEN + OGS_PFCP_F_SEID_HDR_LEN)
#define OGS_PFCP_F_SEID_IPV4V6_LEN  (OGS_IPV4V6_LEN + OGS_PFCP_F_SEID_HDR_LEN)

int ogs_pfcp_f_seid_to_sockaddr(
    ogs_pfcp_f_seid_t *f_seid, uint16_t port, ogs_sockaddr_t **list)
{
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(f_seid);
    ogs_assert(list);

    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr);
    addr->ogs_sa_family = AF_INET;
    addr->ogs_sin_port = htons(port);

    addr6 = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr6);
    addr6->ogs_sa_family = AF_INET6;
    addr6->ogs_sin_port = htons(port);

    if (f_seid->ipv4 && f_seid->ipv6) {
        addr->next = addr6;

        addr->sin.sin_addr.s_addr = f_seid->both.addr;
        memcpy(addr6->sin6.sin6_addr.s6_addr, f_seid->both.addr6, OGS_IPV6_LEN);

        *list = addr;
    } else if (f_seid->ipv4) {
        addr->sin.sin_addr.s_addr = f_seid->addr;
        ogs_free(addr6);

        *list = addr;
    } else if (f_seid->ipv6) {
        memcpy(addr6->sin6.sin6_addr.s6_addr, f_seid->addr6, OGS_IPV6_LEN);
        ogs_free(addr);

        *list = addr6;
    } else {
        ogs_free(addr);
        ogs_free(addr6);
        ogs_assert_if_reached();
    }

    return OGS_OK;
}

int ogs_pfcp_sockaddr_to_f_seid(
    ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6,
    ogs_pfcp_f_seid_t *f_seid, int *len)
{
    ogs_assert(f_seid);

    if (addr && addr6) {
        f_seid->ipv4 = 1;
        f_seid->both.addr = addr->sin.sin_addr.s_addr;
        f_seid->ipv6 = 1;
        memcpy(f_seid->both.addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_PFCP_F_SEID_IPV4V6_LEN;
    } else if (addr) {
        f_seid->ipv4 = 1;
        f_seid->ipv6 = 0;
        f_seid->addr = addr->sin.sin_addr.s_addr;
        *len = OGS_PFCP_F_SEID_IPV4_LEN;
    } else if (addr6) {
        f_seid->ipv4 = 0;
        f_seid->ipv6 = 1;
        memcpy(f_seid->addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_PFCP_F_SEID_IPV6_LEN;
    } else
        ogs_assert_if_reached();

    return OGS_OK;
}

int ogs_pfcp_f_seid_to_ip(ogs_pfcp_f_seid_t *f_seid, ogs_ip_t *ip)
{
    ogs_assert(ip);
    ogs_assert(f_seid);

    memset(ip, 0, sizeof(ogs_ip_t));

    ip->ipv4 = f_seid->ipv4;
    ip->ipv6 = f_seid->ipv6;

    if (ip->ipv4 && ip->ipv6) {
        ip->both.addr = f_seid->both.addr;
        memcpy(ip->both.addr6, f_seid->both.addr6, OGS_IPV6_LEN);
        ip->len = OGS_IPV4V6_LEN;
    } else if (ip->ipv4) {
        ip->addr = f_seid->addr;
        ip->len = OGS_IPV4_LEN;
    } else if (ip->ipv6) {
        memcpy(ip->addr6, f_seid->addr6, OGS_IPV6_LEN);
        ip->len = OGS_IPV6_LEN;
    } else
        ogs_assert_if_reached();

    return OGS_OK;
}

int ogs_pfcp_sockaddr_to_f_teid(
    ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6,
    ogs_pfcp_f_teid_t *f_teid, int *len)
{
    ogs_assert(f_teid);

    if (addr && addr6) {
        f_teid->ipv4 = 1;
        f_teid->both.addr = addr->sin.sin_addr.s_addr;
        f_teid->ipv6 = 1;
        memcpy(f_teid->both.addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_PFCP_F_TEID_IPV4V6_LEN;
    } else if (addr) {
        f_teid->ipv4 = 1;
        f_teid->ipv6 = 0;
        f_teid->addr = addr->sin.sin_addr.s_addr;
        *len = OGS_PFCP_F_TEID_IPV4_LEN;
    } else if (addr6) {
        f_teid->ipv4 = 0;
        f_teid->ipv6 = 1;
        memcpy(f_teid->addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_PFCP_F_TEID_IPV6_LEN;
    } else
        ogs_assert_if_reached();

    return OGS_OK;
}

int ogs_pfcp_outer_hdr_to_ip(ogs_pfcp_outer_hdr_t *outer_hdr, ogs_ip_t *ip)
{
    ogs_assert(ip);
    ogs_assert(outer_hdr);

    memset(ip, 0, sizeof(ogs_ip_t));

    ip->ipv4 = outer_hdr->gtpu_ipv4;
    ip->ipv6 = outer_hdr->gtpu_ipv6;

    if (ip->ipv4 && ip->ipv6) {
        ip->both.addr = outer_hdr->both.addr;
        memcpy(ip->both.addr6, outer_hdr->both.addr6, OGS_IPV6_LEN);
        ip->len = OGS_IPV4V6_LEN;
    } else if (ip->ipv4) {
        ip->addr = outer_hdr->addr;
        ip->len = OGS_IPV4_LEN;
    } else if (ip->ipv6) {
        memcpy(ip->addr6, outer_hdr->addr6, OGS_IPV6_LEN);
        ip->len = OGS_IPV6_LEN;
    } else
        ogs_assert_if_reached();

    return OGS_OK;
}
