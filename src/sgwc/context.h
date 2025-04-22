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

#ifndef SGWC_CONTEXT_H
#define SGWC_CONTEXT_H

#include "ogs-app.h"
#include "ogs-gtp.h"
#include "ogs-pfcp.h"

#include "timer.h"
#include "sgwc-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __sgwc_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __sgwc_log_domain

typedef struct sgwc_tunnel_s sgwc_tunnel_t;

typedef struct sgwc_context_s {
    ogs_list_t mme_s11_list;    /* MME GTPC Node List */
    ogs_list_t pgw_s5c_list;    /* PGW GTPC Node List */

    ogs_hash_t *imsi_ue_hash;   /* hash table (IMSI : SGW_UE) */
    ogs_hash_t *sgw_s11_teid_hash;  /* hash table (SGW-S11-TEID : SGW_UE) */
    ogs_hash_t *sgwc_sxa_seid_hash; /* hash table (SGWC-SXA-SEID : Session) */

    ogs_list_t sgw_ue_list;    /* SGW_UE List */
} sgwc_context_t;

typedef struct sgwc_ue_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;
    ogs_pool_id_t   *sgw_s11_teid_node; /* A node of SGW-S11-TEID */

    uint32_t        sgw_s11_teid;   /* SGW-S11-TEID is derived from NODE */
    uint32_t        mme_s11_teid;   /* MME-S11-TEID is received from MME */

    /* UE identity */
    uint8_t         imsi[OGS_MAX_IMSI_LEN];
    int             imsi_len;
    char            imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    /* User-Location-Info */
    bool            uli_presence;
    ogs_eps_tai_t   e_tai;
    ogs_e_cgi_t     e_cgi;

    ogs_list_t      sess_list;

    ogs_gtp_node_t  *gnode;
} sgwc_ue_t;

#define SGWC_SESS(pfcp_sess) ogs_container_of(pfcp_sess, sgwc_sess_t, pfcp)
typedef struct sgwc_sess_s {
    ogs_lnode_t     lnode;                  /* A node of list_t */
    ogs_pool_id_t   id;
    ogs_pool_id_t   *sgwc_sxa_seid_node;    /* A node of SGWC-SXA-SEID */

    ogs_pfcp_sess_t pfcp;           /* PFCP session context */

    uint32_t        sgw_s5c_teid;   /* SGW-S5C-TEID is derived from NODE */
    uint32_t        pgw_s5c_teid;   /* PGW-S5C-TEID is received from PGW */

    uint64_t        sgwc_sxa_seid;  /* SGW-C SEID is derived from NODE */
    uint64_t        sgwu_sxa_seid;  /* SGW-U SEID is received from Peer */

    /* APN Configuration */
    ogs_session_t   session;

    /* PDN Address Allocation (PAA) */
    ogs_paa_t       paa;

    ogs_list_t      bearer_list;

    /* Related Context */
    ogs_gtp_node_t  *gnode;
    ogs_pfcp_node_t *pfcp_node;

    ogs_pool_id_t   sgwc_ue_id;
} sgwc_sess_t;

typedef struct sgwc_bearer_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;
    ogs_lnode_t     to_modify_node;

    uint8_t         ebi;

    ogs_list_t      tunnel_list;
    ogs_pool_id_t   sess_id;
    ogs_pool_id_t   sgwc_ue_id;
} sgwc_bearer_t;

typedef struct sgwc_tunnel_s {
    ogs_lnode_t     lnode;
    ogs_pool_id_t   id;

    uint8_t         interface_type;

    ogs_pfcp_pdr_t  *pdr;
    ogs_pfcp_far_t  *far;

    uint32_t        local_teid;
    ogs_sockaddr_t  *local_addr;
    ogs_sockaddr_t  *local_addr6;

    uint32_t        remote_teid;
    ogs_ip_t        remote_ip;

    /* Related Context */
    ogs_pool_id_t   bearer_id;
    ogs_gtp_node_t  *gnode;
} sgwc_tunnel_t;

void sgwc_context_init(void);
void sgwc_context_final(void);
sgwc_context_t *sgwc_self(void);

int sgwc_context_parse_config(void);

sgwc_ue_t *sgwc_ue_add_by_message(ogs_gtp2_message_t *message);
sgwc_ue_t *sgwc_ue_find_by_imsi(uint8_t *imsi, int imsi_len);
sgwc_ue_t *sgwc_ue_find_by_imsi_bcd(char *imsi_bcd);
sgwc_ue_t *sgwc_ue_find_by_teid(uint32_t teid);

sgwc_ue_t *sgwc_ue_add(uint8_t *imsi, int imsi_len);
int sgwc_ue_remove(sgwc_ue_t *sgwc_ue);
void sgwc_ue_remove_all(void);
sgwc_ue_t *sgwc_ue_find_by_id(ogs_pool_id_t id);

sgwc_sess_t *sgwc_sess_add(sgwc_ue_t *sgwc_ue, char *apn);

void sgwc_sess_select_sgwu(sgwc_sess_t *sess);

int sgwc_sess_remove(sgwc_sess_t *sess);
void sgwc_sess_remove_all(sgwc_ue_t *sgwc_ue);

sgwc_sess_t *sgwc_sess_find_by_teid(uint32_t teid);
sgwc_sess_t *sgwc_sess_find_by_seid(uint64_t seid);

sgwc_sess_t *sgwc_sess_find_by_apn(sgwc_ue_t *sgwc_ue, char *apn);
sgwc_sess_t *sgwc_sess_find_by_ebi(sgwc_ue_t *sgwc_ue, uint8_t ebi);
sgwc_sess_t *sgwc_sess_find_by_id(ogs_pool_id_t id);

#define SGWC_SESSION_SYNC_DONE(__sGWC, __tYPE, __fLAGS) \
    (sgwc_sess_pfcp_xact_count(__sGWC, __tYPE, __fLAGS) == 0)
int sgwc_sess_pfcp_xact_count(
        sgwc_ue_t *sgwc_ue, uint8_t pfcp_type, uint64_t modify_flags);

sgwc_bearer_t *sgwc_bearer_add(sgwc_sess_t *sess);
int sgwc_bearer_remove(sgwc_bearer_t *bearer);
void sgwc_bearer_remove_all(sgwc_sess_t *sess);
sgwc_bearer_t *sgwc_bearer_find_by_sess_ebi(
                                sgwc_sess_t *sess, uint8_t ebi);
sgwc_bearer_t *sgwc_bearer_find_by_ue_ebi(
                                sgwc_ue_t *sgwc_ue, uint8_t ebi);
sgwc_bearer_t *sgwc_default_bearer_in_sess(sgwc_sess_t *sess);
sgwc_bearer_t *sgwc_bearer_find_by_id(ogs_pool_id_t id);

sgwc_tunnel_t *sgwc_tunnel_add(
        sgwc_bearer_t *bearer, uint8_t interface_type);
int sgwc_tunnel_remove(sgwc_tunnel_t *tunnel);
void sgwc_tunnel_remove_all(sgwc_bearer_t *bearer);
sgwc_tunnel_t *sgwc_tunnel_find_by_teid(sgwc_ue_t *sgwc_ue, uint32_t teid);
sgwc_tunnel_t *sgwc_tunnel_find_by_interface_type(
        sgwc_bearer_t *bearer, uint8_t interface_type);
sgwc_tunnel_t *sgwc_tunnel_find_by_pdr_id(
        sgwc_sess_t *sess, ogs_pfcp_pdr_id_t pdr_id);
sgwc_tunnel_t *sgwc_tunnel_find_by_far_id(
        sgwc_sess_t *sess, ogs_pfcp_far_id_t far_id);
sgwc_tunnel_t *sgwc_dl_tunnel_in_bearer(sgwc_bearer_t *bearer);
sgwc_tunnel_t *sgwc_ul_tunnel_in_bearer(sgwc_bearer_t *bearer);
sgwc_tunnel_t *sgwc_tunnel_find_by_id(ogs_pool_id_t id);

#ifdef __cplusplus
}
#endif

#endif /* SGWC_CONTEXT_H */
