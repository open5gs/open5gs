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

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_NODE_H
#define OGS_PFCP_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_SETUP_PFCP_NODE(__cTX, __pNODE) \
    do { \
        ogs_assert((__cTX)); \
        ogs_assert((__pNODE)); \
        (__cTX)->pnode = __pNODE; \
    } while(0)

/**
 * This structure represents the commonalities of PFCP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct ogs_pfcp_node_s {
    ogs_lnode_t     node;           /* A node of list_t */

    ogs_sockaddr_t  *sa_list;       /* Socket Address List */

    ogs_sock_t      *sock;          /* Socket Instance */
    ogs_ip_t        ip;             /* Socket Address */
    ogs_sockaddr_t  remote_addr;    /* Connected Address */

    ogs_list_t      local_list;    
    ogs_list_t      remote_list;   

    ogs_pfcp_node_id_t  node_id;    /* Target Node ID */
    union {
        uint8_t up_functions_features;
        uint8_t cp_functions_features;
    };
    ogs_pfcp_user_plane_ip_resource_information_t user_plane_info;
} ogs_pfcp_node_t;

int ogs_pfcp_node_init(int size);
int ogs_pfcp_node_final(void);

ogs_pfcp_node_t *ogs_pfcp_node_new(ogs_sockaddr_t *sa_list);
void ogs_pfcp_node_free(ogs_pfcp_node_t *node);

ogs_pfcp_node_t *ogs_pfcp_node_add(
        ogs_list_t *list, ogs_pfcp_f_seid_t *f_seid,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4);
ogs_pfcp_node_t *ogs_pfcp_node_add_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr);
void ogs_pfcp_node_remove(ogs_list_t *list, ogs_pfcp_node_t *node);
void ogs_pfcp_node_remove_all(ogs_list_t *list);

ogs_pfcp_node_t *ogs_pfcp_node_find_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr);
ogs_pfcp_node_t *ogs_pfcp_node_find_by_f_seid(
        ogs_list_t *list, ogs_pfcp_f_seid_t *f_seid);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_NODE_H */
