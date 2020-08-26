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

#ifndef OGS_GTP_NODE_H
#define OGS_GTP_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

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

int ogs_gtp_node_init(void);
void ogs_gtp_node_final(void);

ogs_gtp_node_t *ogs_gtp_node_new(ogs_sockaddr_t *sa_list);
void ogs_gtp_node_free(ogs_gtp_node_t *node);

ogs_gtp_node_t *ogs_gtp_node_add_by_f_teid(
        ogs_list_t *list, ogs_gtp_f_teid_t *f_teid,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4);
ogs_gtp_node_t *ogs_gtp_node_add_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr);
void ogs_gtp_node_remove(ogs_list_t *list, ogs_gtp_node_t *node);
void ogs_gtp_node_remove_all(ogs_list_t *list);

ogs_gtp_node_t *ogs_gtp_node_find_by_addr(
        ogs_list_t *list, ogs_sockaddr_t *addr);
ogs_gtp_node_t *ogs_gtp_node_find_by_f_teid(
        ogs_list_t *list, ogs_gtp_f_teid_t *f_teid);

ogs_gtp_node_t *ogs_gtp_node_add_by_ip(ogs_list_t *list, ogs_ip_t *ip,
        uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4);
ogs_gtp_node_t *ogs_gtp_node_find_by_ip(ogs_list_t *list, ogs_ip_t *ip);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_NODE_H */
