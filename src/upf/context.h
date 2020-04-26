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

#ifndef UPF_CONTEXT_H
#define UPF_CONTEXT_H

#include "upf-config.h"

#if HAVE_NET_IF_H
#include <net/if.h>
#endif

#include "ogs-gtp.h"
#include "ogs-diameter-gx.h"
#include "ogs-pfcp.h"
#include "ogs-app.h"

#include "ipfw/ogs-ipfw.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __upf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __upf_log_domain

typedef struct upf_context_s {
    uint32_t        gtpu_port;      /* Default: UPF GTP-U local port */

    ogs_list_t      gtpu_list;      /* UPF GTPU Server List */
    ogs_sock_t      *gtpu_sock;     /* UPF GTPU IPv4 Socket */
    ogs_sock_t      *gtpu_sock6;    /* UPF GTPU IPv6 Socket */
    ogs_list_t      gtpu_resource_list; /* UP IP Resource List */
    uint16_t        function_features; /* UP Function Features */

    ogs_queue_t     *queue;         /* Queue for processing UPF control */
    ogs_timer_mgr_t *timer_mgr;     /* Timer Manager */
    ogs_pollset_t   *pollset;       /* Poll Set for I/O Multiplexing */

    ogs_list_t      gnb_n3_list;    /* gNB N3 Node List */
    ogs_list_t      ip_pool_list;

    ogs_hash_t      *sess_hash;     /* hash table (F-SEID) */
    ogs_hash_t      *ipv4_hash;     /* hash table (IPv4 Address) */
    ogs_hash_t      *ipv6_hash;     /* hash table (IPv6 Address) */

    ogs_list_t      sess_list;
} upf_context_t;

#define UPF_SESS(pfcp_sess) ogs_container_of(pfcp_sess, upf_sess_t, pfcp)
typedef struct upf_sess_s {
    ogs_lnode_t     lnode;
    uint32_t        index;              /**< An index of this node */

    ogs_pfcp_sess_t pfcp;
    ogs_list_t      sdf_filter_list;    /* SDF Filter List */

    uint64_t        upf_n4_seid;        /* UPF SEID is dervied from INDEX */
    uint64_t        smf_n4_seid;        /* SMF SEID is received from Peer */

    /* APN Configuration */
    ogs_pdn_t       pdn;
    ogs_pfcp_ue_ip_t *ipv4;
    ogs_pfcp_ue_ip_t *ipv6;

    char            *gx_sid;            /* Gx Session ID */
    ogs_pfcp_node_t *pfcp_node;
} upf_sess_t;

typedef struct upf_sdf_filter_s {
    ogs_lnode_t     lnode;

    ogs_ipfw_rule_t rule;

    /* Related Context */
    ogs_pfcp_pdr_t  *pdr;
} upf_sdf_filter_t;

void upf_context_init(void);
void upf_context_final(void);
upf_context_t *upf_self(void);

int upf_context_parse_config(void);

upf_sess_t *upf_sess_add_by_message(ogs_pfcp_message_t *message);

upf_sess_t *upf_sess_add(ogs_pfcp_f_seid_t *f_seid,
        const char *apn, uint8_t pdn_type, ogs_pfcp_ue_ip_addr_t *ue_ip,
        ogs_pfcp_pdr_id_t default_pdr_id);
int upf_sess_remove(upf_sess_t *sess);
void upf_sess_remove_all(void);
upf_sess_t *upf_sess_find(uint32_t index);
upf_sess_t *upf_sess_find_by_cp_seid(uint64_t seid);
upf_sess_t *upf_sess_find_by_up_seid(uint64_t seid);
upf_sess_t *upf_sess_find_by_ipv4(uint32_t addr);
upf_sess_t *upf_sess_find_by_ipv6(uint32_t *addr6);

upf_sdf_filter_t *upf_sdf_filter_add(ogs_pfcp_pdr_t *pdr);
void upf_sdf_filter_remove(upf_sdf_filter_t *sdf_filter);
void upf_sdf_filter_remove_all(upf_sess_t *sess);

void stats_add_session(void);
void stats_remove_session(void);

#ifdef __cplusplus
}
#endif

#endif /* UPF_CONTEXT_H */
