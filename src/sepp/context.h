/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SEPP_CONTEXT_H
#define SEPP_CONTEXT_H

#include "ogs-sbi.h"
#include "ogs-app.h"

#include "timer.h"
#include "sepp-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __sepp_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __sepp_log_domain

typedef struct sepp_context_s {
    char *sender;

    struct {
        bool tls;
        bool prins;
    } security_capability;

    bool target_apiroot_supported;

    ogs_list_t peer_list;

    ogs_list_t assoc_list;
} sepp_context_t;

typedef struct sepp_node_s sepp_node_t;

typedef struct sepp_node_s {
    ogs_lnode_t lnode;

    char *receiver;
    OpenAPI_security_capability_e negotiated_security_scheme;
    bool target_apiroot_supported;

    ogs_plmn_id_t plmn_id[OGS_MAX_NUM_OF_PLMN];
    int num_of_plmn_id;

    bool target_plmn_id_presence;
    ogs_plmn_id_t target_plmn_id;

    uint64_t supported_features;

    ogs_fsm_t sm;                           /* A state machine */
    ogs_timer_t *t_establish_interval;      /* timer to retry
                                               to establish peer node */

    void *client;                           /* only used in CLIENT */
    struct {
        void *client;                       /* For n32 forwarding interface */
    } n32f;
} sepp_node_t;

typedef struct sepp_assoc_s sepp_assoc_t;

typedef struct sepp_assoc_s {
    ogs_lnode_t lnode;

    ogs_pool_id_t stream_id;

    ogs_sbi_client_t *client;
    ogs_sbi_client_t *nrf_client;

    ogs_sbi_request_t *request;
    ogs_sbi_service_type_e service_type;
    OpenAPI_nf_type_e requester_nf_type;

    ogs_sbi_nf_instance_t *nf_service_producer;
} sepp_assoc_t;

void sepp_context_init(void);
void sepp_context_final(void);
sepp_context_t *sepp_self(void);

int sepp_context_parse_config(void);

sepp_node_t *sepp_node_add(char *receiver);
void sepp_node_remove(sepp_node_t *sepp_node);
void sepp_node_remove_all(void);
sepp_node_t *sepp_node_find_by_receiver(char *receiver);
sepp_node_t *sepp_node_find_by_plmn_id(uint16_t mcc, uint16_t mnc);

sepp_assoc_t *sepp_assoc_add(ogs_pool_id_t stream_id);
void sepp_assoc_remove(sepp_assoc_t *assoc);
void sepp_assoc_remove_all(void);

#ifdef __cplusplus
}
#endif

#endif /* SEPP_CONTEXT_H */
