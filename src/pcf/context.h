/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8

extern int __pcf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pcf_log_domain

typedef struct pcf_context_s {
    OpenAPI_nf_type_e   nf_type;

    ogs_list_t      pcf_ue_list;
    ogs_hash_t      *supi_hash;

    ogs_hash_t      *ipv4addr_hash;
    ogs_hash_t      *ipv6prefix_hash;
} pcf_context_t;

#define PCF_NF_INSTANCE_CLEAR(_cAUSE, _nFInstance) \
    do { \
        ogs_assert(_nFInstance); \
        if ((_nFInstance)->reference_count == 1) { \
            ogs_info("[%s] (%s) NF removed", (_nFInstance)->id, (_cAUSE)); \
            pcf_nf_fsm_fini((_nFInstance)); \
        } else { \
            /* There is an assocation with other context */ \
            ogs_info("[%s:%d] (%s) NF suspended", \
                    _nFInstance->id, _nFInstance->reference_count, (_cAUSE)); \
            OGS_FSM_TRAN(&_nFInstance->sm, pcf_nf_state_de_registered); \
            ogs_fsm_dispatch(&_nFInstance->sm, NULL); \
        } \
        ogs_sbi_nf_instance_remove(_nFInstance); \
    } while(0)

struct pcf_ue_s {
    ogs_sbi_object_t sbi;
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

    ogs_list_t sess_list;
};

struct pcf_sess_s {
    ogs_sbi_object_t sbi;
    ogs_fsm_t sm;

    char *sm_policy_id;

    /* BSF sends the RESPONSE
     * of [POST] /nbsf-management/v1/PcfBindings */
    char *binding_id;

    uint8_t psi; /* PDU Session Identity */

    uint8_t pdu_session_type;
    char *dnn;

    char *notification_uri;
    struct {
        ogs_sbi_client_t *client;
    } nsmf;

    char *ipv4addr_string;
    char *ipv6prefix_string;

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
    pcf_ue_t *pcf_ue;
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

pcf_ue_t *pcf_ue_add(char *supi);
void pcf_ue_remove(pcf_ue_t *pcf_ue);
void pcf_ue_remove_all(void);
pcf_ue_t *pcf_ue_find_by_supi(char *supi);
pcf_ue_t *pcf_ue_find_by_association_id(char *association_id);

pcf_sess_t *pcf_sess_add(pcf_ue_t *pcf_ue, uint8_t psi);
void pcf_sess_remove(pcf_sess_t *sess);
void pcf_sess_remove_all(pcf_ue_t *pcf_ue);

bool pcf_sess_set_ipv4addr(pcf_sess_t *sess, char *ipv4addr);
bool pcf_sess_set_ipv6prefix(pcf_sess_t *sess, char *ipv6prefix);

pcf_sess_t *pcf_sess_find(uint32_t index);
pcf_sess_t *pcf_sess_find_by_sm_policy_id(char *sm_policy_id);
pcf_sess_t *pcf_sess_find_by_psi(pcf_ue_t *pcf_ue, uint8_t psi);
pcf_sess_t *pcf_sess_find_by_dnn(pcf_ue_t *pcf_ue, char *dnn);
pcf_sess_t *pcf_sess_find_by_ipv4addr(char *ipv4addr_string);
pcf_sess_t *pcf_sess_find_by_ipv6addr(char *ipv6addr_string);
pcf_sess_t *pcf_sess_find_by_ipv6prefix(char *ipv6prefix_string);
int pcf_sessions_number_by_snssai_and_dnn(
        pcf_ue_t *pcf_ue, ogs_s_nssai_t *s_nssai, char *dnn);

pcf_ue_t *pcf_ue_cycle(pcf_ue_t *pcf_ue);
pcf_sess_t *pcf_sess_cycle(pcf_sess_t *sess);

void pcf_ue_select_nf(pcf_ue_t *pcf_ue, OpenAPI_nf_type_e nf_type);
void pcf_sess_select_nf(pcf_sess_t *sess, OpenAPI_nf_type_e nf_type);

pcf_app_t *pcf_app_add(pcf_sess_t *sess);
int pcf_app_remove(pcf_app_t *app);
void pcf_app_remove_all(pcf_sess_t *sess);
pcf_app_t *pcf_app_find(uint32_t index);
pcf_app_t *pcf_app_find_by_app_session_id(char *app_session_id);

#ifdef __cplusplus
}
#endif

#endif /* PCF_CONTEXT_H */
