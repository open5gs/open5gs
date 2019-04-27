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

#include "gtp_conv.h"
#include "gtp_node.h"
#include "gtp_xact.h"

static OGS_POOL(pool, gtp_node_t);

int gtp_node_init(void)
{
    ogs_pool_init(&pool, base_self()->gtp.node.pool);

    return OGS_OK;
}
int gtp_node_final(void)
{
    ogs_pool_final(&pool);

    return OGS_OK;
}

int gtp_create_node(gtp_node_t **node,
        ogs_sockaddr_t *all_list, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    gtp_node_t *new_node = NULL;
    ogs_sockaddr_t *preferred_list = NULL;

    ogs_assert(all_list);

    rv = ogs_copyaddrinfo(&preferred_list, all_list);
    ogs_assert(rv == OGS_OK);
    if (no_ipv4 == 1)
    {
        rv = ogs_filteraddrinfo(&preferred_list, AF_INET6);
        ogs_assert(rv == OGS_OK);
    }
    if (no_ipv6 == 1)
    {
        rv = ogs_filteraddrinfo(&preferred_list, AF_INET);
        ogs_assert(rv == OGS_OK);
    }
    if (prefer_ipv4 == 1)
    {
        rv = ogs_sortaddrinfo(&preferred_list, AF_INET);
        ogs_assert(rv == OGS_OK);
    }
    else
    {
        rv = ogs_sortaddrinfo(&preferred_list, AF_INET6);
        ogs_assert(rv == OGS_OK);
    }

    if (preferred_list)
    {
        ogs_pool_alloc(&pool, &new_node);
        ogs_assert(new_node);
        memset(new_node, 0, sizeof(gtp_node_t));

        new_node->sa_list = preferred_list;

        ogs_list_init(&new_node->local_list);
        ogs_list_init(&new_node->remote_list);
    }

    *node = new_node;

    return OGS_OK;
}

int gtp_delete_node(gtp_node_t *node)
{
    ogs_assert(node);

    if (node->sock)
        ogs_sock_destroy(node->sock);

    gtp_xact_delete_all(node);

    ogs_freeaddrinfo(node->sa_list);
    ogs_pool_free(&pool, node);

    return OGS_OK;
}

gtp_node_t *gtp_add_node(ogs_list_t *list, gtp_f_teid_t *f_teid,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    gtp_node_t *node = NULL;
    ogs_sockaddr_t *sa_list = NULL;

    ogs_assert(list);
    ogs_assert(f_teid);
    ogs_assert(port);

    rv = gtp_f_teid_to_sockaddr(f_teid, port, &sa_list);
    ogs_assert(rv == OGS_OK);

    rv = gtp_create_node(&node, sa_list, no_ipv4, no_ipv6, prefer_ipv4);
    ogs_assert(rv == OGS_OK);
    if (node == NULL)
    {
        ogs_error("Invalid Parameter : "
                "port[%d], no_ipv4[%d], no_ipv6[%d], prefer_ipv4[%d]",
                port, no_ipv4, no_ipv6, prefer_ipv4);
        return NULL;
    }
    ogs_list_add(list, node);

    rv = gtp_f_teid_to_ip(f_teid, &node->ip);
    ogs_assert(rv == OGS_OK);

    rv = ogs_sock_fill_scope_id_in_local(node->sa_list);
    ogs_assert(rv == OGS_OK);

    ogs_freeaddrinfo(sa_list);

    return node;
}

int gtp_remove_node(ogs_list_t *list, gtp_node_t *node)
{
    int rv;
    ogs_assert(node);

    ogs_list_remove(list, node);

    rv = gtp_delete_node(node);
    ogs_assert(rv == OGS_OK);

    return rv;
}

int gtp_remove_all_nodes(ogs_list_t *list)
{
    gtp_node_t *node = NULL, *next_node = NULL;
    
    ogs_list_for_each_safe(list, next_node, node)
        gtp_remove_node(list, node);

    return OGS_OK;
}

gtp_node_t *gtp_find_node(ogs_list_t *list, gtp_f_teid_t *f_teid)
{
    int rv;
    gtp_node_t *node = NULL;
    ip_t ip;

    ogs_assert(list);
    ogs_assert(f_teid);

    rv = gtp_f_teid_to_ip(f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_list_for_each(list, node)
    {
        if (memcmp(&node->ip, &ip, ip.len) == 0)
            break;
    }

    return node;
}
