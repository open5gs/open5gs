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

#if HAVE_NET_IF_H
#include <net/if.h>
#endif

#if HAVE_IFADDRS_H
#include <ifaddrs.h>
#endif

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

ogs_socknode_t *ogs_socknode_new(ogs_sockaddr_t *addr)
{
    ogs_socknode_t *node = NULL;

    ogs_assert(addr);

    node = ogs_calloc(1, sizeof(ogs_socknode_t));
    ogs_expect_or_return_val(node, NULL);

    node->addr = addr;

    return node;
}

void ogs_socknode_free(ogs_socknode_t *node)
{
    ogs_assert(node);

    ogs_freeaddrinfo(node->addr);
    if (node->dev)
        ogs_free(node->dev);
    if (node->poll)
        ogs_pollset_remove(node->poll);
    if (node->sock) {
        if (node->cleanup)
            node->cleanup(node->sock);
        else
            ogs_sock_destroy(node->sock);
    }
    if (node->option)
        ogs_free(node->option);
    ogs_free(node);
}

ogs_socknode_t *ogs_socknode_add(ogs_list_t *list,
        int family, ogs_sockaddr_t *addr, ogs_sockopt_t *option)
{
    ogs_socknode_t *node = NULL;
    ogs_sockaddr_t *dup = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_assert(OGS_OK == ogs_copyaddrinfo(&dup, addr));
    if (family != AF_UNSPEC)
        ogs_filteraddrinfo(&dup, family);

    if (dup) {
        node = ogs_socknode_new(dup);
        ogs_assert(node);
        ogs_list_add(list, node);

        if (option)
            node->option = ogs_memdup(option, sizeof *option);
    }

    return node;
}

void ogs_socknode_remove(ogs_list_t *list, ogs_socknode_t *node)
{
    ogs_assert(node);

    ogs_list_remove(list, node);
    ogs_socknode_free(node);
}

void ogs_socknode_remove_all(ogs_list_t *list)
{
    ogs_socknode_t *node = NULL, *saved_node = NULL;

    ogs_list_for_each_safe(list, saved_node, node)
        ogs_socknode_remove(list, node);
}

int ogs_socknode_probe(ogs_list_t *list, ogs_list_t *list6,
        const char *dev, uint16_t port, ogs_sockopt_t *option)
{
#if defined(HAVE_GETIFADDRS)
    ogs_socknode_t *node = NULL;
	struct ifaddrs *iflist, *cur;
    int rc;

	rc = getifaddrs(&iflist);
    if (rc != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "getifaddrs failed");
        return OGS_ERROR;
    }

	for (cur = iflist; cur != NULL; cur = cur->ifa_next) {
        ogs_sockaddr_t *addr = NULL;

        if (cur->ifa_flags & IFF_LOOPBACK)
            continue;

        if (cur->ifa_flags & IFF_POINTOPOINT)
            continue;

		if (cur->ifa_addr == NULL) /* may happen with ppp interfaces */
			continue;

        if (dev && strcmp(dev, cur->ifa_name) != 0)
            continue;

        addr = (ogs_sockaddr_t *)cur->ifa_addr;
        if (cur->ifa_addr->sa_family == AF_INET) {
            if (!list) continue;

#ifndef IN_IS_ADDR_LOOPBACK
#define IN_IS_ADDR_LOOPBACK(a) \
  ((((long int) (a)->s_addr) & be32toh(0xff000000)) == be32toh(0x7f000000))
#endif /* IN_IS_ADDR_LOOPBACK */

/* An IP equivalent to IN6_IS_ADDR_UNSPECIFIED */
#ifndef IN_IS_ADDR_UNSPECIFIED
#define IN_IS_ADDR_UNSPECIFIED(a) \
  (((long int) (a)->s_addr) == 0x00000000)
#endif /* IN_IS_ADDR_UNSPECIFIED */
            if (IN_IS_ADDR_UNSPECIFIED(&addr->sin.sin_addr) ||
                IN_IS_ADDR_LOOPBACK(&addr->sin.sin_addr))
                continue;
        } else if (cur->ifa_addr->sa_family == AF_INET6) {
            if (!list6) continue;

            if (IN6_IS_ADDR_UNSPECIFIED(&addr->sin6.sin6_addr) ||
                IN6_IS_ADDR_LOOPBACK(&addr->sin6.sin6_addr) ||
                IN6_IS_ADDR_MULTICAST(&addr->sin6.sin6_addr) ||
                IN6_IS_ADDR_LINKLOCAL(&addr->sin6.sin6_addr) ||
                IN6_IS_ADDR_SITELOCAL(&addr->sin6.sin6_addr))
                continue;
        } else
            continue;

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        memcpy(&addr->sa, cur->ifa_addr, ogs_sockaddr_len(cur->ifa_addr));
        addr->ogs_sin_port = htobe16(port);

        node = ogs_calloc(1, sizeof(ogs_socknode_t));
        node->addr = addr;
        if (dev)
            node->dev = ogs_strdup(dev);

        if (addr->ogs_sa_family == AF_INET) {
            ogs_assert(list);
            ogs_list_add(list, node);
        } else if (addr->ogs_sa_family == AF_INET6) {
            ogs_assert(list6);
            ogs_list_add(list6, node);
        } else
            ogs_assert_if_reached();

        if (option)
            node->option = ogs_memdup(option, sizeof *option);
	}

	freeifaddrs(iflist);
    return OGS_OK;
#elif defined(_WIN32)
    return OGS_OK;
#else
    ogs_assert_if_reached();
    return OGS_ERROR;
#endif

}

#if 0 /* deprecated */
int ogs_socknode_fill_scope_id_in_local(ogs_sockaddr_t *sa_list)
{
#if defined(HAVE_GETIFADDRS)
	struct ifaddrs *iflist = NULL, *cur;
    int rc;
    ogs_sockaddr_t *addr, *ifaddr;

    for (addr = sa_list; addr != NULL; addr = addr->next) {
        if (addr->ogs_sa_family != AF_INET6)
            continue;

        if (!IN6_IS_ADDR_LINKLOCAL(&addr->sin6.sin6_addr))
            continue;

        if (addr->sin6.sin6_scope_id != 0)
            continue;

        if (iflist == NULL) {
            rc = getifaddrs(&iflist);
            if (rc != 0) {
                ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                        "getifaddrs failed");
                return OGS_ERROR;
            }
        }

        for (cur = iflist; cur != NULL; cur = cur->ifa_next) {
            ifaddr = (ogs_sockaddr_t *)cur->ifa_addr;

            if (cur->ifa_addr == NULL) /* may happen with ppp interfaces */
                continue;

            if (cur->ifa_addr->sa_family != AF_INET6)
                continue;

            if (!IN6_IS_ADDR_LINKLOCAL(&ifaddr->sin6.sin6_addr))
                continue;

            if (memcmp(&addr->sin6.sin6_addr,
                    &ifaddr->sin6.sin6_addr, sizeof(struct in6_addr)) == 0) {
                /* Fill Scope ID in localhost */
                addr->sin6.sin6_scope_id = ifaddr->sin6.sin6_scope_id;
            }
        }
    }

    if (iflist)
        freeifaddrs(iflist);

    return OGS_OK;
#elif defined(_WIN32)
    return OGS_OK;
#else
    ogs_assert_if_reached();
    return OGS_ERROR;
#endif
}
#endif

void ogs_socknode_set_cleanup(
        ogs_socknode_t *node, void (*cleanup)(ogs_sock_t *))
{
    ogs_assert(node);
    ogs_assert(cleanup);

    node->cleanup = cleanup;
}

ogs_sock_t *ogs_socknode_sock_first(ogs_list_t *list)
{
    ogs_socknode_t *snode = NULL;

    ogs_assert(list);
    ogs_list_for_each(list, snode) {
        if (snode->sock)
            return snode->sock;
    }

    return NULL;
}
