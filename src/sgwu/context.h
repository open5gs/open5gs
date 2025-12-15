/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SGWU_CONTEXT_H
#define SGWU_CONTEXT_H

#include "ogs-gtp.h"
#include "ogs-pfcp.h"
#include "ogs-app.h"

#include "timer.h"
#include "sgwu-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __sgwu_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __sgwu_log_domain

typedef struct sgwu_context_s {
    ogs_hash_t *sgwu_sxa_seid_hash;    /* hash table (SGWU-SXA-SEID) */
    ogs_hash_t *sgwc_sxa_seid_hash;    /* hash table (SGWC-SXA-SEID) */
    ogs_hash_t *sgwc_sxa_f_seid_hash;  /* hash table (SGWC-SXA-F-SEID) */

    ogs_list_t sess_list;
} sgwu_context_t;

#define SGWU_SESS(pfcp_sess) ogs_container_of(pfcp_sess, sgwu_sess_t, pfcp)
typedef struct sgwu_sess_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;
    ogs_pool_id_t   *sgwu_sxa_seid_node;    /* A node of SGWU-SXA-SEID */

    ogs_pfcp_sess_t pfcp;

    uint64_t        sgwu_sxa_seid;      /* SGW-U SEID is dervied from INDEX */
    struct {
        uint64_t    seid;
        ogs_ip_t    ip;
    } sgwc_sxa_f_seid;                  /* SGW-C SEID is received from Peer */

    ogs_pfcp_node_t *pfcp_node;
} sgwu_sess_t;

void sgwu_context_init(void);
void sgwu_context_final(void);
sgwu_context_t *sgwu_self(void);

int sgwu_context_parse_config(void);

sgwu_sess_t *sgwu_sess_add_by_message(ogs_pfcp_message_t *message);

sgwu_sess_t *sgwu_sess_add(ogs_pfcp_f_seid_t *f_seid);
int sgwu_sess_remove(sgwu_sess_t *sess);
void sgwu_sess_remove_all(void);
sgwu_sess_t *sgwu_sess_find_by_sgwc_sxa_seid(uint64_t seid);
sgwu_sess_t *sgwu_sess_find_by_sgwc_sxa_f_seid(ogs_pfcp_f_seid_t *f_seid);
sgwu_sess_t *sgwu_sess_find_by_sgwu_sxa_seid(uint64_t seid);
sgwu_sess_t *sgwu_sess_find_by_id(ogs_pool_id_t id);

#ifdef __cplusplus
}
#endif

#endif /* SGWU_CONTEXT_H */
