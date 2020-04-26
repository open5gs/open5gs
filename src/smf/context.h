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

#ifndef SMF_CONTEXT_H
#define SMF_CONTEXT_H

#include "smf-config.h"

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

extern int __smf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __smf_log_domain

typedef struct smf_context_s {
    const char*         diam_conf_path;   /* SMF Diameter conf path */
    ogs_diam_config_t   *diam_config;     /* SMF Diameter config */

    uint32_t        gtpc_port;      /* Default: SMF GTP-C local port */

    ogs_list_t      gtpc_list;      /* SMF GTPC IPv4 Server List */
    ogs_list_t      gtpc_list6;     /* SMF GTPC IPv6 Server List */
    ogs_sock_t      *gtpc_sock;     /* SMF GTPC IPv4 Socket */
    ogs_sock_t      *gtpc_sock6;    /* SMF GTPC IPv6 Socket */
    ogs_sockaddr_t  *gtpc_addr;     /* SMF GTPC IPv4 Address */
    ogs_sockaddr_t  *gtpc_addr6;    /* SMF GTPC IPv6 Address */

    uint8_t         function_features; /* CP Function Features */

    ogs_queue_t     *queue;         /* Queue for processing SMF control */
    ogs_timer_mgr_t *timer_mgr;     /* Timer Manager */
    ogs_pollset_t   *pollset;       /* Poll Set for I/O Multiplexing */

#define MAX_NUM_OF_DNS              2
    const char      *dns[MAX_NUM_OF_DNS];
    const char      *dns6[MAX_NUM_OF_DNS];

#define MAX_NUM_OF_P_CSCF           16
    const char      *p_cscf[MAX_NUM_OF_P_CSCF];
    int             num_of_p_cscf;
    int             p_cscf_index;
    const char      *p_cscf6[MAX_NUM_OF_P_CSCF];
    int             num_of_p_cscf6;
    int             p_cscf6_index;

    ogs_list_t      sgw_s5c_list;   /* SGW GTPC Node List */
    ogs_list_t      ip_pool_list;

    ogs_hash_t      *sess_hash;     /* hash table (IMSI+APN) */
    ogs_hash_t      *ipv4_hash;     /* hash table (IPv4 Address) */
    ogs_hash_t      *ipv6_hash;     /* hash table (IPv6 Address) */

    uint16_t        mtu;            /* MTU to advertise in PCO */

    ogs_list_t      sess_list;
} smf_context_t;

typedef struct smf_sess_s {
    ogs_lnode_t     lnode;
    uint32_t        index;          /**< An index of this node */

    uint32_t        smf_n4_teid;    /* SMF-N4-TEID is derived from INDEX */
    uint32_t        sgw_s5c_teid;   /* SGW-S5C-TEID is received from SGW */

    uint64_t        smf_n4_seid;    /* SMF SEID is dervied from INDEX */
    uint64_t        upf_n4_seid;    /* UPF SEID is received from Peer */

    char            *gx_sid;        /* Gx Session ID */

    ogs_pfcp_pdr_id_t   pdr_id;     /* ID Generator(1~MAX_NUM_OF_PDR) */
    ogs_pfcp_far_id_t   far_id;     /* ID Generator(1~MAX_NUM_OF_FAR) */
    ogs_pfcp_urr_id_t   urr_id;     /* ID Generator(1~MAX_NUM_OF_URR) */
    ogs_pfcp_qer_id_t   qer_id;     /* ID Generator(1~MAX_NUM_OF_URR) */
    ogs_pfcp_bar_id_t   bar_id;     /* ID Generator(1~MAX_NUM_OF_BAR) */

    /* IMSI */
    uint8_t         imsi[OGS_MAX_IMSI_LEN];
    int             imsi_len;
    char            imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    /* APN Configuration */
    ogs_pdn_t       pdn;
    ogs_pfcp_ue_ip_t *ipv4;
    ogs_pfcp_ue_ip_t *ipv6;

    uint8_t         hash_keybuf[OGS_MAX_IMSI_LEN+OGS_MAX_APN_LEN+1];
    int             hash_keylen;

    ogs_tlv_octet_t ue_pco; /* Saved from S5-C */
    ogs_tlv_octet_t user_location_information; /* Saved from S5-C */
    ogs_tlv_octet_t ue_timezone; /* Saved from S5-C */

    ogs_pcc_rule_t  pcc_rule[OGS_MAX_NUM_OF_PCC_RULE]; /* Saved from Gx */
    int             num_of_pcc_rule;

    ogs_list_t      bearer_list;

    /* Related Context */
    ogs_gtp_node_t  *gnode;
    ogs_pfcp_node_t *pfcp_node;
} smf_sess_t;

#define SMF_BEARER(pfcp_sess) ogs_container_of(pfcp_sess, smf_bearer_t, pfcp)
typedef struct smf_bearer_s {
    ogs_lnode_t     lnode;          /**< A node of list_t */
    uint32_t        index;

    ogs_pfcp_sess_t pfcp;           /* PFCP session context */

    uint8_t         ebi;
    /* 
     * UPF-S5U-TEID     = INDEX         | TEID_RANGE 
     * INDEX            = UPF-S5U-TEID  & ~TEID_RANGE
     */
#define UPF_S5U_TEID_TO_INDEX(__tEID, __iND, __rANGE) \
    (__tEID & ~(__rANGE << (32 - __iND)))
#define UPF_S5U_INDEX_TO_TEID(__iNDEX, __iND, __rANGE) \
    (__iNDEX | (__rANGE << (32 - __iND)))
    uint32_t        upf_n3_teid;    /* UPF_N3 TEID */
    ogs_sockaddr_t  *upf_addr;      /* UPF_N3 IPv4 */
    ogs_sockaddr_t  *upf_addr6;     /* UPF_N3 IPv6 */

    uint32_t        gnb_n3_teid;    /* gNB_N3 TEID */
    ogs_ip_t        gnb_ip;         /* gNB_N3 IP */

    char            *name;          /* PCC Rule Name */
    ogs_qos_t       qos;            /* QoS Infomration */

    /* Packet Filter Identifier Generator(1~15) */
    uint8_t         pf_identifier;
    /* Packet Filter List */
    ogs_list_t      pf_list;

    struct {
        bool created;
        bool tft_updated;
        bool qos_updated;
        bool removed;
    } state;

    smf_sess_t      *sess;
} smf_bearer_t;

typedef struct smf_pf_s {
    ogs_lnode_t     lnode;

ED3(uint8_t spare:2;,
    uint8_t direction:2;,
    uint8_t identifier:4;)
    ogs_ipfw_rule_t rule;

    smf_bearer_t    *bearer;
} smf_pf_t;

void smf_context_init(void);
void smf_context_final(void);
smf_context_t *smf_self(void);

int smf_context_parse_config(void);

smf_sess_t *smf_sess_add_by_message(ogs_gtp_message_t *message);

smf_sess_t *smf_sess_add(
        uint8_t *imsi, int imsi_len, char *apn,
        uint8_t pdn_type, uint8_t ebi, ogs_paa_t *addr);

int smf_sess_remove(smf_sess_t *sess);
void smf_sess_remove_all(void);
smf_sess_t *smf_sess_find(uint32_t index);
smf_sess_t *smf_sess_find_by_teid(uint32_t teid);
smf_sess_t *smf_sess_find_by_seid(uint64_t seid);
smf_sess_t *smf_sess_find_by_imsi_apn(uint8_t *imsi, int imsi_len, char *apn);
smf_sess_t *smf_sess_find_by_ipv4(uint32_t addr);
smf_sess_t *smf_sess_find_by_ipv6(uint32_t *addr6);

smf_bearer_t *smf_bearer_add(smf_sess_t *sess);
int smf_bearer_remove(smf_bearer_t *bearer);
void smf_bearer_remove_all(smf_sess_t *sess);
smf_bearer_t *smf_bearer_find(uint32_t index);
smf_bearer_t *smf_bearer_find_by_smf_s5u_teid(uint32_t smf_s5u_teid);
smf_bearer_t *smf_bearer_find_by_ebi(smf_sess_t *sess, uint8_t ebi);
smf_bearer_t *smf_bearer_find_by_name(smf_sess_t *sess, char *name);
smf_bearer_t *smf_bearer_find_by_qci_arp(smf_sess_t *sess, 
                                uint8_t qci,
                                uint8_t priority_level,
                                uint8_t pre_emption_capability,
                                uint8_t pre_emption_vulnerability);
smf_bearer_t *smf_default_bearer_in_sess(smf_sess_t *sess);
bool smf_bearer_is_default(smf_bearer_t *bearer);
smf_bearer_t *smf_bearer_first(smf_sess_t *sess);
smf_bearer_t *smf_bearer_next(smf_bearer_t *bearer);

smf_pf_t *smf_pf_add(smf_bearer_t *bearer, uint32_t precedence);
int smf_pf_remove(smf_pf_t *pf);
void smf_pf_remove_all(smf_bearer_t *bearer);
smf_pf_t *smf_pf_find_by_id(smf_bearer_t *smf_bearer, uint8_t id);
smf_pf_t *smf_pf_first(smf_bearer_t *bearer);
smf_pf_t *smf_pf_next(smf_pf_t *pf);

void stats_add_session(void);
void stats_remove_session(void);

#ifdef __cplusplus
}
#endif

#endif /* SMF_CONTEXT_H */
