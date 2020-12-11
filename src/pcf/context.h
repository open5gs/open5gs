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

    OpenAPI_policy_association_request_t *policy_association_request;

    char *association_id;
    char *supi;

    char *notification_uri;

    ogs_guami_t guami;
    OpenAPI_rat_type_e rat_type;

    ogs_list_t sess_list;
};

struct pcf_sess_s {
    ogs_sbi_object_t sbi;
    ogs_fsm_t sm;

    char *sm_policy_id;

    uint8_t psi; /* PDU Session Identity */

    uint8_t pdu_session_type;
    char *dnn;
    char *notification_uri;
    ogs_s_nssai_t s_nssai;

    /* Related Context */
    pcf_ue_t        *pcf_ue;
};

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
pcf_sess_t *pcf_sess_find_by_sm_policy_id(pcf_ue_t *pcf_ue, char *sm_policy_id);
pcf_sess_t *pcf_sess_find_by_psi(pcf_ue_t *pcf_ue, uint8_t psi);
pcf_sess_t *pcf_sess_find_by_dnn(pcf_ue_t *pcf_ue, char *dnn);

pcf_ue_t *pcf_ue_cycle(pcf_ue_t *pcf_ue);
pcf_sess_t *pcf_sess_cycle(pcf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* PCF_CONTEXT_H */
