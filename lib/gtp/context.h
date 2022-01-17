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

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP_CONTEXT_H
#define OGS_GTP_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_gtp_context_s {
    uint32_t        gtpc_port;      /* GTPC local port */
    uint32_t        gtpu_port;      /* GTPU local port */

    ogs_list_t      gtpc_list;      /* GTPC IPv4 Server List */
    ogs_list_t      gtpc_list6;     /* GTPC IPv6 Server List */
    ogs_sock_t      *gtpc_sock;     /* GTPC IPv4 Socket */
    ogs_sock_t      *gtpc_sock6;    /* GTPC IPv6 Socket */
    ogs_sockaddr_t  *gtpc_addr;     /* GTPC IPv4 Address */
    ogs_sockaddr_t  *gtpc_addr6;    /* GTPC IPv6 Address */

    ogs_list_t      gtpu_list;      /* GTPU IPv4/IPv6 Server List */
    ogs_sock_t      *gtpu_sock;     /* GTPU IPv4 Socket */
    ogs_sock_t      *gtpu_sock6;    /* GTPU IPv6 Socket */
    ogs_sockaddr_t  *gtpu_addr;     /* GTPU IPv4 Address */
    ogs_sockaddr_t  *gtpu_addr6;    /* GTPU IPv6 Address */

    ogs_ip_t        gtpu_ip;        /* GTPU IP */;

    ogs_list_t      gtpu_peer_list; /* GTPU Node List */
    ogs_list_t      gtpu_resource_list; /* UP IP Resource List */

    ogs_sockaddr_t *link_local_addr;
} ogs_gtp_context_t;

#define OGS_SETUP_GTP_NODE(__cTX, __gNODE) \
    do { \
        ogs_assert((__cTX)); \
        ogs_assert((__gNODE)); \
        (__cTX)->gnode = __gNODE; \
    } while(0)

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct ogs_gtp_node_s {
    ogs_lnode_t     node;           /* A node of list_t */

    ogs_sockaddr_t  *sa_list;       /* Socket Address List Candidate */

    ogs_sock_t      *sock;          /* Socket Instance */
    ogs_sockaddr_t  addr;           /* Remote Address */

    ogs_ip_t        ip;             /* F-TEID IP Address Duplicate Check */

    ogs_list_t      local_list;
    ogs_list_t      remote_list;
} ogs_gtp_node_t;

typedef struct ogs_gtpu_resource_s {
    ogs_lnode_t lnode;

    ogs_user_plane_ip_resource_info_t info;
} ogs_gtpu_resource_t;

void ogs_gtp_context_init(int num_of_gtpu_resource);
void ogs_gtp_context_final(void);
ogs_gtp_context_t *ogs_gtp_self(void);
int ogs_gtp_context_parse_config(const char *local, const char *remote);

ogs_gtp_node_t *ogs_gtp_node_new(ogs_sockaddr_t *sa_list);
void ogs_gtp_node_free(ogs_gtp_node_t *node);

ogs_gtp_node_t *ogs_gtp_node_add_by_f_teid(
        ogs_list_t *list, ogs_gtp_f_teid_t *f_teid, uint16_t port);
ogs_gtp_node_t *ogs_gtp_node_add_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr);
void ogs_gtp_node_remove(ogs_list_t *list, ogs_gtp_node_t *node);
void ogs_gtp_node_remove_all(ogs_list_t *list);

ogs_gtp_node_t *ogs_gtp_node_find_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr);
ogs_gtp_node_t *ogs_gtp_node_find_by_f_teid(
        ogs_list_t *list, ogs_gtp_f_teid_t *f_teid);

ogs_gtp_node_t *ogs_gtp_node_add_by_ip(
        ogs_list_t *list, ogs_ip_t *ip, uint16_t port);
ogs_gtp_node_t *ogs_gtp_node_find_by_ip(ogs_list_t *list, ogs_ip_t *ip);

ogs_gtpu_resource_t *ogs_gtpu_resource_add(ogs_list_t *list,
        ogs_user_plane_ip_resource_info_t *info);
void ogs_gtpu_resource_remove(ogs_list_t *list,
        ogs_gtpu_resource_t *resource);
void ogs_gtpu_resource_remove_all(ogs_list_t *list);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_CONTEXT_H */
