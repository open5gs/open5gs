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
#include "ogs-pfcp.h"
#include "ogs-app.h"

#include "ipfw/ogs-ipfw.h"

#include "timer.h"
#include "upf-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __upf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __upf_log_domain

typedef struct upf_context_s {
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

    uint64_t        upf_n4_seid;        /* UPF SEID is dervied from INDEX */
    uint64_t        smf_n4_seid;        /* SMF SEID is received from Peer */

    /* APN Configuration */
    ogs_pfcp_ue_ip_t *ipv4;
    ogs_pfcp_ue_ip_t *ipv6;

    char            *gx_sid;            /* Gx Session ID */
    ogs_pfcp_node_t *pfcp_node;
} upf_sess_t;

void upf_context_init(void);
void upf_context_final(void);
upf_context_t *upf_self(void);

int upf_context_parse_config(void);

upf_sess_t *upf_sess_add_by_message(ogs_pfcp_message_t *message);

upf_sess_t *upf_sess_add(ogs_pfcp_f_seid_t *f_seid);
int upf_sess_remove(upf_sess_t *sess);
void upf_sess_remove_all(void);
upf_sess_t *upf_sess_find(uint32_t index);
upf_sess_t *upf_sess_find_by_cp_seid(uint64_t seid);
upf_sess_t *upf_sess_find_by_up_seid(uint64_t seid);
upf_sess_t *upf_sess_find_by_ipv4(uint32_t addr);
upf_sess_t *upf_sess_find_by_ipv6(uint32_t *addr6);

uint8_t upf_sess_set_ue_ip(upf_sess_t *sess,
        uint8_t session_type, ogs_pfcp_pdr_t *pdr);

#ifdef __cplusplus
}
#endif

#endif /* UPF_CONTEXT_H */
