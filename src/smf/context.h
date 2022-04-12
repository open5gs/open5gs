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

#include "ogs-gtp.h"
#include "ogs-diameter-gx.h"
#include "ogs-diameter-gy.h"
#include "ogs-diameter-rx.h"
#include "ogs-diameter-s6b.h"
#include "ogs-pfcp.h"
#include "ogs-sbi.h"
#include "ogs-app.h"
#include "ogs-ngap.h"
#include "ogs-nas-5gs.h"
#include "ipfw/ogs-ipfw.h"

#include "timer.h"
#include "smf-sm.h"

#if HAVE_NET_IF_H
#include <net/if.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int __smf_log_domain;
extern int __gsm_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __smf_log_domain

typedef enum {
    SMF_CTF_ENABLED_AUTO = 0,
    SMF_CTF_ENABLED_YES,
    SMF_CTF_ENABLED_NO,
} smf_ctf_enabled_mode;

typedef struct smf_ctf_config_s {
    smf_ctf_enabled_mode enabled;
} smf_ctf_config_t;

int smf_ctf_config_init(smf_ctf_config_t *ctf_config);

typedef struct smf_context_s {
    smf_ctf_config_t    ctf_config;
    const char*         diam_conf_path;   /* SMF Diameter conf path */
    ogs_diam_config_t   *diam_config;     /* SMF Diameter config */

    OpenAPI_nf_type_e   nf_type;

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

    ogs_hash_t      *supi_hash;     /* hash table (SUPI) */
    ogs_hash_t      *imsi_hash;     /* hash table (IMSI) */
    ogs_hash_t      *ipv4_hash;     /* hash table (IPv4 Address) */
    ogs_hash_t      *ipv6_hash;     /* hash table (IPv6 Address) */
    ogs_hash_t      *n1n2message_hash; /* hash table (N1N2Message Location) */

    uint16_t        mtu;            /* MTU to advertise in PCO */

#define SMF_UE_IS_LAST_SESSION(__sMF) \
     ((__sMF) && (ogs_list_count(&(__sMF)->sess_list)) == 1)
    ogs_list_t      smf_ue_list;
} smf_context_t;

typedef struct smf_ue_s {
    ogs_lnode_t lnode;

    /* SUPI */
    char *supi;

    /* IMSI */
    uint8_t imsi[OGS_MAX_IMSI_LEN];
    int imsi_len;
    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_list_t sess_list;
} smf_ue_t;

#define SMF_NF_INSTANCE_CLEAR(_cAUSE, _nFInstance) \
    do { \
        ogs_assert(_nFInstance); \
        if ((_nFInstance)->reference_count == 1) { \
            ogs_info("[%s] (%s) NF removed", (_nFInstance)->id, (_cAUSE)); \
            smf_nf_fsm_fini((_nFInstance)); \
        } else { \
            /* There is an assocation with other context */ \
            ogs_info("[%s:%d] (%s) NF suspended", \
                    _nFInstance->id, _nFInstance->reference_count, (_cAUSE)); \
            OGS_FSM_TRAN(&_nFInstance->sm, smf_nf_state_de_registered); \
            ogs_fsm_dispatch(&_nFInstance->sm, NULL); \
        } \
        ogs_sbi_nf_instance_remove(_nFInstance); \
    } while(0)
#define SMF_SESS_CLEAR(__sESS) \
    do { \
        smf_ue_t *smf_ue = NULL; \
        ogs_assert(__sESS); \
        smf_ue = __sESS->smf_ue; \
        ogs_assert(smf_ue); \
        if (SMF_UE_IS_LAST_SESSION(smf_ue)) \
            smf_ue_remove(smf_ue); \
        else \
            smf_sess_remove(__sESS); \
    } while(0)

typedef struct smf_bearer_s smf_bearer_t;
typedef struct smf_sess_s smf_sess_t;

typedef struct smf_pf_s {
    ogs_lnode_t     lnode;
    ogs_lnode_t     to_add_node;

ED3(uint8_t spare:2;,
    uint8_t direction:2;,
    uint8_t identifier:4;)

    uint8_t precedence;             /* Only used in EPC */

    uint8_t *identifier_node;       /* Pool-Node for Identifier */
    uint8_t *precedence_node;       /* Pool-Node for Precedence */

    ogs_ipfw_rule_t ipfw_rule;
    char *flow_description;

    smf_bearer_t    *bearer;
} smf_pf_t;

typedef struct smf_bearer_s {
    ogs_lnode_t     lnode;          /**< A node of list_t */

    ogs_lnode_t     to_update_node;
    ogs_lnode_t     to_delete_node;

    uint32_t        index;

    ogs_pfcp_pdr_t  *dl_pdr;
    ogs_pfcp_pdr_t  *ul_pdr;
    ogs_pfcp_far_t  *dl_far;
    ogs_pfcp_far_t  *ul_far;
    ogs_pfcp_urr_t  *urr;
    ogs_pfcp_qer_t  *qer;

    uint8_t         *qfi_node;      /* Pool-Node for 5GC-QFI */
    uint8_t         qfi;            /* 5G Core QFI */
    uint8_t         ebi;            /* EPC EBI */

    uint32_t        pgw_s5u_teid;   /* PGW-S5U TEID */
    ogs_sockaddr_t  *pgw_s5u_addr;  /* PGW-S5U IPv4 */
    ogs_sockaddr_t  *pgw_s5u_addr6; /* PGW-S5U IPv6 */

    uint32_t        sgw_s5u_teid;   /* SGW-S5U TEID */
    ogs_ip_t        sgw_s5u_ip;     /* SGW-S5U IPv4/IPv6 */

    struct {
        char        *name;          /* EPC: PCC Rule Name */
        char        *id;            /* 5GC: PCC Rule Id */
    } pcc_rule;
    ogs_qos_t       qos;            /* QoS Information */

    OGS_POOL(pf_identifier_pool, uint8_t);

    /* Packet Filter List */
    ogs_list_t      pf_list;
    ogs_list_t      pf_to_add_list;

    uint8_t num_of_pf_to_delete;
    uint8_t pf_to_delete[OGS_MAX_NUM_OF_FLOW_IN_NAS];

    smf_sess_t      *sess;
} smf_bearer_t;

#define SMF_SESS(pfcp_sess) ogs_container_of(pfcp_sess, smf_sess_t, pfcp)
typedef struct smf_sess_s {
    ogs_sbi_object_t sbi;
    uint32_t        index;          /**< An index of this node */
    ogs_fsm_t       sm;             /* A state machine */

    ogs_pfcp_sess_t pfcp;           /* PFCP session context */

    uint64_t        smpolicycontrol_features; /* SBI features */

    uint32_t        smf_n4_teid;    /* SMF-N4-TEID is derived from INDEX */

    uint32_t        sgw_s5c_teid;   /* SGW-S5C-TEID is received from SGW */
    ogs_ip_t        sgw_s5c_ip;     /* SGW-S5C IPv4/IPv6 */

    uint64_t        smf_n4_seid;    /* SMF SEID is dervied from INDEX */
    uint64_t        upf_n4_seid;    /* UPF SEID is received from Peer */

    uint32_t        upf_n3_teid;    /* UPF-N3 TEID */
    ogs_sockaddr_t  *upf_n3_addr;   /* UPF-N3 IPv4 */
    ogs_sockaddr_t  *upf_n3_addr6;  /* UPF-N3 IPv6 */

    uint32_t        gnb_n3_teid;    /* gNB-N3 TEID */
    ogs_ip_t        gnb_n3_ip;      /* gNB-N3 IPv4/IPv6 */

    char            *gx_sid;        /* Gx Session ID */
    char            *gy_sid;        /* Gx Session ID */
    char            *s6b_sid;       /* S6b Session ID */

    OGS_POOL(pf_precedence_pool, uint8_t);

#define CLEAR_QOS_FLOW_ID(__sESS) \
    do { \
        ogs_assert((__sESS)); \
        smf_qfi_pool_final(__sESS); \
        smf_qfi_pool_init(__sESS); \
    } while(0)
    OGS_POOL(qfi_pool, uint8_t);

    char            *sm_context_ref; /* smContextRef */
    uint8_t         psi; /* PDU session identity */
    uint8_t         pti; /* 5GS-NAS : Procedure transaction identity */

    char            *sm_context_status_uri; /* SmContextStatusNotification */
    struct {
        ogs_sbi_client_t *client;
    } namf;

    /* PCF sends the RESPONSE
     * of [POST] /npcf-smpolocycontrol/v1/policies */
    char *policy_association_id;

    /* PLMN ID & NID */
    ogs_plmn_id_t   plmn_id;

    /* LTE Location */
    ogs_eps_tai_t   e_tai;
    ogs_e_cgi_t     e_cgi;

    /* NR Location */
    ogs_5gs_tai_t   nr_tai;
    ogs_nr_cgi_t    nr_cgi;
    ogs_time_t      ue_location_timestamp;

    /* PCF ID */
    char            *pcf_id;

    /* Integrity protection maximum data rate */
    struct {
        uint8_t mbr_dl;
        uint8_t mbr_ul;
    } integrity_protection;

    /* S_NSSAI */
    ogs_s_nssai_t s_nssai;
    ogs_s_nssai_t mapped_hplmn;

    /* PDN Configuration */
    ogs_session_t session;
    uint8_t ue_session_type;
    uint8_t ue_ssc_mode;

    ogs_pfcp_ue_ip_t *ipv4;
    ogs_pfcp_ue_ip_t *ipv6;

    /* RAT Type */
    uint8_t gtp_rat_type;
    OpenAPI_rat_type_e sbi_rat_type;

    struct {
        ogs_tlv_octet_t ue_pco;
        ogs_tlv_octet_t user_location_information;
        ogs_tlv_octet_t ue_timezone;
        bool create_session_response_apn_ambr;
        bool create_session_response_bearer_qos;
    } gtp; /* Saved from S5-C */

    struct {
        uint8_t nsapi;
    } gtp1; /* GTPv1C specific fields */

    struct {
        uint64_t ul_octets;
        uint64_t dl_octets;
        ogs_time_t duration;
        /* Snapshot of measurement when last report was sent: */
        struct {
            uint64_t ul_octets;
            uint64_t dl_octets;
            ogs_time_t duration;
        } last_report;
    } gy;

    struct {
        ogs_nas_extended_protocol_configuration_options_t ue_pco;
    } nas; /* Saved from NAS-5GS */

    struct {
        ogs_pcc_rule_t  pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
        int             num_of_pcc_rule;
    } policy; /* Saved from N7 or Gx */

    /* Paging */
    struct {
        bool ue_requested_pdu_session_establishment_done;
        char *n1n2message_location;
    } paging;

    /* State */
#define SMF_NGAP_STATE_NONE                                     0
#define SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED              1
#define SMF_NGAP_STATE_DELETE_TRIGGER_PCF_INITIATED             2
#define SMF_NGAP_STATE_ERROR_INDICATION_RECEIVED_FROM_5G_AN     3
    struct {
        int pdu_session_resource_release;
    } ngap_state;

    /* Handover */
    struct {
        bool prepared;
        bool data_forwarding_not_possible;
        bool indirect_data_forwarding;

        /* NG-U UP Transport Information Saved Temporally */
        uint32_t gnb_n3_teid;
        ogs_ip_t gnb_n3_ip;

        /* Indirect DL Forwarding */
        uint32_t upf_dl_teid;
        ogs_sockaddr_t *upf_dl_addr;
        ogs_sockaddr_t *upf_dl_addr6;
        uint32_t gnb_dl_teid;
        ogs_ip_t gnb_dl_ip;
    } handover;

    /* Charging */
    struct {
        uint32_t id;
    } charging;

    /* Data Forwarding between the CP and UP functions */
    ogs_pfcp_pdr_t  *cp2up_pdr;
    ogs_pfcp_pdr_t  *up2cp_pdr;
    ogs_pfcp_far_t  *cp2up_far;
    ogs_pfcp_far_t  *up2cp_far;

    ogs_list_t      bearer_list;

    ogs_gtp_node_t  *gnode;
    ogs_pfcp_node_t *pfcp_node;

    smf_ue_t *smf_ue;
} smf_sess_t;

void smf_context_init(void);
void smf_context_final(void);
smf_context_t *smf_self(void);

int smf_context_parse_config(void);

int smf_use_gy_iface(void);

smf_ue_t *smf_ue_add_by_supi(char *supi);
smf_ue_t *smf_ue_add_by_imsi(uint8_t *imsi, int imsi_len);
void smf_ue_remove(smf_ue_t *smf_ue);
void smf_ue_remove_all(void);
smf_ue_t *smf_ue_find_by_supi(char *supi);
smf_ue_t *smf_ue_find_by_imsi(uint8_t *imsi, int imsi_len);

smf_sess_t *smf_sess_add_by_gtp1_message(ogs_gtp1_message_t *message);
smf_sess_t *smf_sess_add_by_gtp_message(ogs_gtp2_message_t *message);
smf_sess_t *smf_sess_add_by_apn(smf_ue_t *smf_ue, char *apn, uint8_t rat_type);

smf_sess_t *smf_sess_add_by_sbi_message(ogs_sbi_message_t *message);
smf_sess_t *smf_sess_add_by_psi(smf_ue_t *smf_ue, uint8_t psi);

void smf_sess_select_upf(smf_sess_t *sess);
uint8_t smf_sess_set_ue_ip(smf_sess_t *sess);
void smf_sess_set_paging_n1n2message_location(
        smf_sess_t *sess, char *n1n2message_location);

void smf_sess_remove(smf_sess_t *sess);
void smf_sess_remove_all(smf_ue_t *smf_ue);

smf_sess_t *smf_sess_find(uint32_t index);
smf_sess_t *smf_sess_find_by_teid(uint32_t teid);
smf_sess_t *smf_sess_find_by_seid(uint64_t seid);
smf_sess_t *smf_sess_find_by_apn(smf_ue_t *smf_ue, char *apn, uint8_t rat_type);
smf_sess_t *smf_sess_find_by_psi(smf_ue_t *smf_ue, uint8_t psi);
smf_sess_t *smf_sess_find_by_charging_id(uint32_t charging_id);
smf_sess_t *smf_sess_find_by_sm_context_ref(char *sm_context_ref);
smf_sess_t *smf_sess_find_by_ipv4(uint32_t addr);
smf_sess_t *smf_sess_find_by_ipv6(uint32_t *addr6);
smf_sess_t *smf_sess_find_by_paging_n1n2message_location(
        char *n1n2message_location);
smf_sess_t *smf_sess_find_by_error_indication_report(
        smf_ue_t *smf_ue,
        ogs_pfcp_tlv_error_indication_report_t *error_indication_report);

void smf_sess_create_indirect_data_forwarding(smf_sess_t *sess);
bool smf_sess_have_indirect_data_forwarding(smf_sess_t *sess);
void smf_sess_delete_indirect_data_forwarding(smf_sess_t *sess);

void smf_sess_create_cp_up_data_forwarding(smf_sess_t *sess);
void smf_sess_delete_cp_up_data_forwarding(smf_sess_t *sess);

ogs_pcc_rule_t *smf_pcc_rule_find_by_id(smf_sess_t *sess, char *pcc_rule_id);

smf_bearer_t *smf_qos_flow_add(smf_sess_t *sess);
smf_bearer_t *smf_qos_flow_find_by_qfi(smf_sess_t *sess, uint8_t qfi);
smf_bearer_t *smf_qos_flow_find_by_pcc_rule_id(
        smf_sess_t *sess, char *pcc_rule_id);

smf_bearer_t *smf_bearer_add(smf_sess_t *sess);
int smf_bearer_remove(smf_bearer_t *bearer);
void smf_bearer_remove_all(smf_sess_t *sess);
smf_bearer_t *smf_bearer_find_by_pgw_s5u_teid(
        smf_sess_t *sess, uint32_t pgw_s5u_teid);
smf_bearer_t *smf_bearer_find_by_ebi(smf_sess_t *sess, uint8_t ebi);
smf_bearer_t *smf_bearer_find_by_pcc_rule_name(
        smf_sess_t *sess, char *pcc_rule_name);
smf_bearer_t *smf_bearer_find_by_pdr_id(
        smf_sess_t *sess, ogs_pfcp_pdr_id_t pdr_id);
smf_bearer_t *smf_default_bearer_in_sess(smf_sess_t *sess);

void smf_bearer_tft_update(smf_bearer_t *bearer);
void smf_bearer_qos_update(smf_bearer_t *bearer);

smf_ue_t *smf_ue_cycle(smf_ue_t *smf_ue);
smf_sess_t *smf_sess_cycle(smf_sess_t *sess);
smf_bearer_t *smf_qos_flow_cycle(smf_bearer_t *qos_flow);
smf_bearer_t *smf_bearer_cycle(smf_bearer_t *bearer);

void smf_sess_select_nf(smf_sess_t *sess, OpenAPI_nf_type_e nf_type);

smf_pf_t *smf_pf_add(smf_bearer_t *bearer);
int smf_pf_remove(smf_pf_t *pf);
void smf_pf_remove_all(smf_bearer_t *bearer);
smf_pf_t *smf_pf_find_by_id(smf_bearer_t *smf_bearer, uint8_t id);
smf_pf_t *smf_pf_find_by_flow(
    smf_bearer_t *bearer, uint8_t direction, char *flow_description);
smf_pf_t *smf_pf_first(smf_bearer_t *bearer);
smf_pf_t *smf_pf_next(smf_pf_t *pf);

int smf_pco_build(uint8_t *pco_buf, uint8_t *buffer, int length);

void smf_qfi_pool_init(smf_sess_t *sess);
void smf_qfi_pool_final(smf_sess_t *sess);

void smf_pf_identifier_pool_init(smf_bearer_t *bearer);
void smf_pf_identifier_pool_final(smf_bearer_t *bearer);

void smf_pf_precedence_pool_init(smf_sess_t *sess);
void smf_pf_precedence_pool_final(smf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* SMF_CONTEXT_H */
