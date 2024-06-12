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

#ifndef SCP_CONTEXT_H
#define SCP_CONTEXT_H

#include "ogs-sbi.h"
#include "ogs-app.h"

#include "scp-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __scp_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __scp_log_domain

typedef struct scp_context_s {
    ogs_list_t          assoc_list;
} scp_context_t;

typedef struct scp_assoc_s scp_assoc_t;

typedef struct scp_assoc_s {
    ogs_lnode_t lnode;

    ogs_pool_id_t stream_id;

    ogs_sbi_client_t *client;
    ogs_sbi_client_t *nrf_client;

    ogs_sbi_request_t *request;

    ogs_sbi_service_type_e service_type;

    OpenAPI_nf_type_e target_nf_type;
    OpenAPI_nf_type_e requester_nf_type;
    ogs_sbi_discovery_option_t *discovery_option;

    ogs_sbi_nf_instance_t *nf_service_producer;

    char *target_apiroot;
} scp_assoc_t;

void scp_context_init(void);
void scp_context_final(void);
scp_context_t *scp_self(void);

int scp_context_parse_config(void);

scp_assoc_t *scp_assoc_add(ogs_pool_id_t stream_id);
void scp_assoc_remove(scp_assoc_t *assoc);
void scp_assoc_remove_all(void);

#ifdef __cplusplus
}
#endif

#endif /* SCP_CONTEXT_H */
