/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef PCF_CONTEXT_H
#define PCF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-crypt.h"
#include "ogs-sbi.h"
#include "ogs-dbi.h"

#include "pcf-sm.h"
#include "metrics.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __pcf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pcf_log_domain

typedef struct pcf_context_s {
    ogs_list_t      pcf_ue_am_list;
#define PCF_UE_SM_IS_LAST_SESSION(__pCF) \
     ((__pCF) && (ogs_list_count(&(__pCF)->sess_list)) == 1)
    ogs_list_t      pcf_ue_sm_list;
    ogs_hash_t      *supi_am_hash;
    ogs_hash_t      *supi_sm_hash;

    ogs_hash_t      *ipv4addr_hash;
    ogs_hash_t      *ipv6prefix_hash;
} pcf_context_t;

struct pcf_ue_am_s {
    ogs_sbi_object_t sbi;
    ogs_pool_id_t id;
    ogs_fsm_t sm;

    char *association_id;
    char *supi;

    char *notification_uri;
    struct {
        ogs_sbi_client_t *client;
    } namf;

    char *gpsi;
    OpenAPI_access_type_e access_type;
    char *pei;

    ogs_guami_t guami;
    OpenAPI_rat_type_e rat_type;

    /* SBI Features */
    uint64_t am_policy_control_features;

    OpenAPI_policy_association_request_t *policy_association_request;
    OpenAPI_ambr_t *subscribed_ue_ambr;
};

struct pcf_ue_sm_s {
    ogs_lnode_t lnode;
    ogs_pool_id_t id;

    char *supi;
    char *gpsi;

    ogs_list_t sess_list;
};

#define PCF_SESS_CLEAR(__sESS) \
    do { \
        pcf_ue_sm_t *pcf_ue_sm = NULL; \
        ogs_assert(__sESS); \
        pcf_ue_sm = pcf_ue_sm_find_by_id((__sESS)->pcf_ue_sm_id); \
        ogs_assert(pcf_ue_sm); \
        if (PCF_UE_SM_IS_LAST_SESSION(pcf_ue_sm)) \
            pcf_ue_sm_remove(pcf_ue_sm); \
        else \
            pcf_sess_remove(__sESS); \
    } while(0)
struct pcf_sess_s {
    ogs_sbi_object_t sbi;
    ogs_pool_id_t id;
    ogs_fsm_t sm;

    char *sm_policy_id;

    /* BSF sends the RESPONSE
     * of [POST] /nbsf-management/v1/PcfBindings */
#define PCF_BINDING_ASSOCIATED(__sESS) \
    ((__sESS) && ((__sESS)->binding.id))
#define PCF_BINDING_CLEAR(__sESS) \
    do { \
        ogs_assert((__sESS)); \
        if ((__sESS)->binding.resource_uri) \
            ogs_free((__sESS)->binding.resource_uri); \
        (__sESS)->binding.resource_uri = NULL; \
        if ((__sESS)->binding.id) \
            ogs_free((__sESS)->binding.id); \
        (__sESS)->binding.id = NULL; \
    } while(0)
#define PCF_BINDING_STORE(__sESS, __rESOURCE_URI, __iD) \
    do { \
        ogs_assert((__sESS)); \
        ogs_assert((__rESOURCE_URI)); \
        ogs_assert((__iD)); \
        PCF_BINDING_CLEAR(__sESS); \
        (__sESS)->binding.resource_uri = ogs_strdup(__rESOURCE_URI); \
        ogs_assert((__sESS)->binding.resource_uri); \
        (__sESS)->binding.id = ogs_strdup(__iD); \
        ogs_assert((__sESS)->binding.id); \
    } while(0)
    struct {
        char *resource_uri;
        char *id;
        ogs_sbi_client_t *client;
    } binding;

    uint8_t psi; /* PDU Session Identity */

    uint8_t pdu_session_type;

    /* DNN */
    char *dnn;
    char *full_dnn;

    /* Serving PLMN ID & Home PLMN ID */
    struct {
        bool presence;
        ogs_plmn_id_t plmn_id;
    } serving, home;

    char *notification_uri;
    struct {
        ogs_sbi_client_t *client;
    } nsmf;

    char *ipv4addr_string;
    char *ipv6prefix_string;

    OpenAPI_list_t *ipv4_frame_route_list;
    OpenAPI_list_t *ipv6_frame_route_list;

    uint32_t ipv4addr;
    struct {
        uint8_t len;
        uint8_t addr6[OGS_IPV6_LEN];
    } ipv6prefix;

    ogs_s_nssai_t s_nssai;

    /* SBI Features */
    uint64_t smpolicycontrol_features;
    uint64_t management_features;
    uint64_t policyauthorization_features;

    OpenAPI_ambr_t *subscribed_sess_ambr;
    OpenAPI_subscribed_default_qos_t *subscribed_default_qos;

    ogs_list_t app_list;

    /* Related Context */
    ogs_pool_id_t pcf_ue_sm_id;
};

typedef struct pcf_app_s {
    ogs_lnode_t lnode;

    char *app_session_id;

    char *notif_uri;
    struct {
        ogs_sbi_client_t *client;
    } naf;

    ogs_pcc_rule_t pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
    int num_of_pcc_rule;

    pcf_sess_t *sess;
} pcf_app_t;

void pcf_context_init(void);
void pcf_context_final(void);
pcf_context_t *pcf_self(void);

int pcf_context_parse_config(void);

pcf_ue_am_t *pcf_ue_am_add(char *supi);
void pcf_ue_am_remove(pcf_ue_am_t *pcf_ue_am);
void pcf_ue_am_remove_all(void);
pcf_ue_am_t *pcf_ue_am_find_by_supi(char *supi);
pcf_ue_am_t *pcf_ue_am_find_by_association_id(char *association_id);

pcf_ue_sm_t *pcf_ue_sm_add(char *supi);
void pcf_ue_sm_remove(pcf_ue_sm_t *pcf_ue_sm);
void pcf_ue_sm_remove_all(void);
pcf_ue_sm_t *pcf_ue_sm_find_by_supi(char *supi);
pcf_ue_sm_t *pcf_ue_sm_find_by_association_id(char *association_id);

pcf_sess_t *pcf_sess_add(pcf_ue_sm_t *pcf_ue_sm, uint8_t psi);
void pcf_sess_remove(pcf_sess_t *sess);
void pcf_sess_remove_all(pcf_ue_sm_t *pcf_ue_sm);

bool pcf_sess_set_ipv4addr(pcf_sess_t *sess, char *ipv4addr);
bool pcf_sess_set_ipv6prefix(pcf_sess_t *sess, char *ipv6prefix);

pcf_sess_t *pcf_sess_find(uint32_t index);
pcf_sess_t *pcf_sess_find_by_sm_policy_id(char *sm_policy_id);
pcf_sess_t *pcf_sess_find_by_psi(pcf_ue_sm_t *pcf_ue_sm, uint8_t psi);
pcf_sess_t *pcf_sess_find_by_dnn(pcf_ue_sm_t *pcf_ue_sm, char *dnn);
pcf_sess_t *pcf_sess_find_by_ipv4addr(char *ipv4addr_string);
pcf_sess_t *pcf_sess_find_by_ipv6addr(char *ipv6addr_string);
pcf_sess_t *pcf_sess_find_by_ipv6prefix(char *ipv6prefix_string);
int pcf_sessions_number_by_snssai_and_dnn(
        pcf_ue_sm_t *pcf_ue_sm, ogs_s_nssai_t *s_nssai, char *dnn);

pcf_ue_am_t *pcf_ue_am_find_by_id(ogs_pool_id_t id);
pcf_ue_sm_t *pcf_ue_sm_find_by_id(ogs_pool_id_t id);
pcf_sess_t *pcf_sess_find_by_id(ogs_pool_id_t id);

pcf_app_t *pcf_app_add(pcf_sess_t *sess);
int pcf_app_remove(pcf_app_t *app);
void pcf_app_remove_all(pcf_sess_t *sess);
pcf_app_t *pcf_app_find(uint32_t index);
pcf_app_t *pcf_app_find_by_app_session_id(char *app_session_id);
int pcf_instance_get_load(void);

int pcf_db_qos_data(char *supi,
        ogs_plmn_id_t *plmn_id, ogs_s_nssai_t *s_nssai, char *dnn,
        ogs_session_data_t *session_data);

#ifdef __cplusplus
}
#endif

#endif /* PCF_CONTEXT_H */
