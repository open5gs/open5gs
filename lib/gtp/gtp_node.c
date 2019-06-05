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

gtp_node_t *gtp_node_new(ogs_sockaddr_t *addr)
{
    gtp_node_t *node = NULL;

    ogs_assert(addr);

    ogs_pool_alloc(&pool, &node);
    ogs_assert(node);
    memset(node, 0, sizeof(gtp_node_t));

    node->addr = addr;

    ogs_list_init(&node->local_list);
    ogs_list_init(&node->remote_list);

    return node;
}

void gtp_node_free(gtp_node_t *node)
{
    ogs_assert(node);

    if (node->sock)
        ogs_sock_destroy(node->sock);

    gtp_xact_delete_all(node);

    ogs_freeaddrinfo(node->addr);
    ogs_pool_free(&pool, node);
}

gtp_node_t *gtp_node_add(ogs_list_t *list, gtp_f_teid_t *f_teid,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    int rv;
    gtp_node_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(list);
    ogs_assert(f_teid);
    ogs_assert(port);

    rv = gtp_f_teid_to_sockaddr(f_teid, port, &addr);
    ogs_assert(rv == OGS_OK);

    rv = ogs_filter_ip_version(&addr, no_ipv4, no_ipv6, prefer_ipv4);
    ogs_assert(addr);

    node = gtp_node_new(addr);
    ogs_assert(node);

    ogs_list_add(list, node);

    rv = gtp_f_teid_to_ip(f_teid, &node->ip);
    ogs_assert(rv == OGS_OK);

    rv = ogs_socknode_fill_scope_id_in_local(node->addr);
    ogs_assert(rv == OGS_OK);

    return node;
}

void gtp_node_remove(ogs_list_t *list, gtp_node_t *node)
{
    ogs_assert(node);

    ogs_list_remove(list, node);

    gtp_node_free(node);
}

void gtp_node_remove_all(ogs_list_t *list)
{
    gtp_node_t *node = NULL, *next_node = NULL;
    
    ogs_list_for_each_safe(list, next_node, node)
        gtp_node_remove(list, node);
}

gtp_node_t *gtp_node_find(ogs_list_t *list, gtp_f_teid_t *f_teid)
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
