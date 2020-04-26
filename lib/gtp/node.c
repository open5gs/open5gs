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

static OGS_POOL(pool, ogs_gtp_node_t);

int ogs_gtp_node_init(int size)
{
    ogs_pool_init(&pool, size);

    return OGS_OK;
}
int ogs_gtp_node_final(void)
{
    ogs_pool_final(&pool);

    return OGS_OK;
}

ogs_gtp_node_t *ogs_gtp_node_new(ogs_sockaddr_t *sa_list)
{
    ogs_gtp_node_t *node = NULL;

    ogs_assert(sa_list);

    ogs_pool_alloc(&pool, &node);
    ogs_assert(node);
    memset(node, 0, sizeof(ogs_gtp_node_t));

    node->sa_list = sa_list;

    ogs_list_init(&node->local_list);
    ogs_list_init(&node->remote_list);

    return node;
}

void ogs_gtp_node_free(ogs_gtp_node_t *node)
{
    ogs_assert(node);

    if (node->sock)
        ogs_sock_destroy(node->sock);

    ogs_gtp_xact_delete_all(node);

    ogs_freeaddrinfo(node->sa_list);
    ogs_pool_free(&pool, node);
}

ogs_gtp_node_t *ogs_gtp_node_add_by_f_teid(
        ogs_list_t *list, ogs_gtp_f_teid_t *f_teid,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    ogs_gtp_node_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(list);
    ogs_assert(f_teid);
    ogs_assert(port);

    rv = ogs_gtp_f_teid_to_sockaddr(f_teid, port, &addr);
    ogs_assert(rv == OGS_OK);

    rv = ogs_filter_ip_version(&addr, no_ipv4, no_ipv6, prefer_ipv4);
    ogs_assert(addr);

    rv = ogs_socknode_fill_scope_id_in_local(addr);
    ogs_assert(rv == OGS_OK);

    node = ogs_gtp_node_new(addr);
    ogs_assert(node);

    rv = ogs_gtp_f_teid_to_ip(f_teid, &node->ip);
    ogs_assert(rv == OGS_OK);

    ogs_list_add(list, node);

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_add_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr)
{
    ogs_gtp_node_t *gnode = NULL;
    ogs_sockaddr_t *new = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_copyaddrinfo(&new, addr);
    gnode = ogs_gtp_node_new(new);

    ogs_assert(gnode);
    memcpy(&gnode->addr, new, sizeof gnode->addr);

    ogs_list_add(list, gnode);

    return gnode;
}

void ogs_gtp_node_remove(ogs_list_t *list, ogs_gtp_node_t *node)
{
    ogs_assert(node);

    ogs_list_remove(list, node);

    ogs_gtp_node_free(node);
}

void ogs_gtp_node_remove_all(ogs_list_t *list)
{
    ogs_gtp_node_t *node = NULL, *next_node = NULL;
    
    ogs_list_for_each_safe(list, next_node, node)
        ogs_gtp_node_remove(list, node);
}

ogs_gtp_node_t *ogs_gtp_node_find_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr)
{
    ogs_gtp_node_t *node = NULL;

    ogs_assert(list);
    ogs_assert(addr);

    ogs_list_for_each(list, node) {
        if (ogs_sockaddr_is_equal(&node->addr, addr) == true)
            break;
    }

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_find_by_f_teid(
        ogs_list_t *list, ogs_gtp_f_teid_t *f_teid)
{
    int rv;
    ogs_gtp_node_t *node = NULL;
    ogs_ip_t ip;

    ogs_assert(list);
    ogs_assert(f_teid);

    rv = ogs_gtp_f_teid_to_ip(f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_list_for_each(list, node) {
        if (memcmp(&node->ip, &ip, sizeof(ip)) == 0)
            break;
    }

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_add_by_ip(ogs_list_t *list, ogs_ip_t *ip,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    ogs_gtp_node_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(list);
    ogs_assert(ip);
    ogs_assert(port);

    rv = ogs_ip_to_sockaddr(ip, port, &addr);
    ogs_assert(rv == OGS_OK);

    rv = ogs_filter_ip_version(&addr, no_ipv4, no_ipv6, prefer_ipv4);
    ogs_assert(addr);

    rv = ogs_socknode_fill_scope_id_in_local(addr);
    ogs_assert(rv == OGS_OK);

    node = ogs_gtp_node_new(addr);
    ogs_assert(node);

    memcpy(&node->ip, ip, sizeof(*ip));

    ogs_list_add(list, node);

    return node;
}

ogs_gtp_node_t *ogs_gtp_node_find_by_ip(ogs_list_t *list, ogs_ip_t *ip)
{
    ogs_gtp_node_t *node = NULL;

    ogs_assert(list);
    ogs_assert(ip);

    ogs_list_for_each(list, node) {
        if (node->ip.len == ip->len && memcmp(&node->ip, ip, ip->len) == 0)
            break;
    }

    return node;
}
