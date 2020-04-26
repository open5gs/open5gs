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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SOCKADDR_H
#define OGS_SOCKADDR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sockaddr_s ogs_sockaddr_t;
struct ogs_sockaddr_s {
    /* Reserved Area
     *   - Should not add any atrribute in this area.
     *
     *   e.g) 
     *   struct sockaddr addr;
     *   ...
     *   sockaddr_len((ogs_sockaddr_t *)&addr);
     */
#define ogs_sa_family sa.sa_family
#define ogs_sin_port sin.sin_port
    union {
        struct sockaddr_storage ss;
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
        struct sockaddr sa;
    };

    /* User Area
     *   - Could add your attribute.
     */
    char *hostname;

    ogs_sockaddr_t *next;
};

typedef struct ogs_ipsubnet_s {
    int family;

    uint32_t sub[4]; /* big enough for IPv4 and IPv6 addresses */
    uint32_t mask[4];
} ogs_ipsubnet_t;

int ogs_getnameinfo(
    char *hostname, socklen_t hostname_len, ogs_sockaddr_t *addr, int flags);

int ogs_getaddrinfo(ogs_sockaddr_t **sa_list, 
        int family, const char *hostname, uint16_t port, int flags);
int ogs_freeaddrinfo(ogs_sockaddr_t *sa_list);

int ogs_addaddrinfo(ogs_sockaddr_t **sa_list, 
        int family, const char *hostname, uint16_t port, int flags);
int ogs_copyaddrinfo(
        ogs_sockaddr_t **dst, const ogs_sockaddr_t *src);
int ogs_filteraddrinfo(ogs_sockaddr_t **sa_list, int family);
int ogs_sortaddrinfo(ogs_sockaddr_t **sa_list, int family);

ogs_sockaddr_t *ogs_link_local_addr_by_dev(const char *dev);
int ogs_filter_ip_version(ogs_sockaddr_t **addr, 
        int no_ipv4, int no_ipv6, int prefer_ipv4);

#define OGS_ADDRSTRLEN INET6_ADDRSTRLEN
#define OGS_ADDR(__aDDR, __bUF) \
    ogs_inet_ntop(__aDDR, __bUF, OGS_ADDRSTRLEN)
#define OGS_PORT(__aDDR) \
    be16toh((__aDDR)->ogs_sin_port)
const char *ogs_inet_ntop(void *addr, char *buf, int buflen);
int ogs_inet_pton(int family, const char *src, void *addr);

socklen_t ogs_sockaddr_len(const void *addr);
bool ogs_sockaddr_is_equal(void *p, void *q);

int ogs_ipsubnet(ogs_ipsubnet_t *ipsub,
        const char *ipstr, const char *mask_or_numbits);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SOCKADDR_H */
