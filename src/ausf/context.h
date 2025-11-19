/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AUSF_CONTEXT_H
#define AUSF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-crypt.h"
#include "ogs-sbi.h"

#include "ausf-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __ausf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ausf_log_domain

typedef struct ausf_context_s {
    ogs_list_t      ausf_ue_list;
    ogs_hash_t      *suci_hash;
    ogs_hash_t      *supi_hash;

} ausf_context_t;

struct ausf_ue_s {
    ogs_sbi_object_t sbi;
    ogs_pool_id_t id;
    ogs_fsm_t sm;

    char *ctx_id;
    char *suci;
    char *supi;
    char *serving_network_name;

    OpenAPI_auth_type_e auth_type;
#define AUTH_EVENT_CLEAR(__aUSF) \
    do { \
        ogs_assert((__aUSF)); \
        if ((__aUSF)->auth_event.resource_uri) \
            ogs_free((__aUSF)->auth_event.resource_uri); \
        (__aUSF)->auth_event.resource_uri = NULL; \
    } while(0)
#define AUTH_EVENT_STORE(__aUSF, __rESOURCE_URI) \
    do { \
        ogs_assert((__aUSF)); \
        ogs_assert((__rESOURCE_URI)); \
        AUTH_EVENT_CLEAR(__aUSF); \
        (__aUSF)->auth_event.resource_uri = ogs_strdup(__rESOURCE_URI); \
        ogs_assert((__aUSF)->auth_event.resource_uri); \
    } while(0)
    struct {
        char *resource_uri;
        ogs_sbi_client_t *client;
    } auth_event;
    OpenAPI_auth_result_e auth_result;

    uint8_t rand[OGS_RAND_LEN];
    uint8_t xres_star[OGS_MAX_RES_LEN];
    uint8_t hxres_star[OGS_MAX_RES_LEN];
    uint8_t kausf[OGS_SHA256_DIGEST_SIZE];
    uint8_t kseaf[OGS_SHA256_DIGEST_SIZE];
};

void ausf_context_init(void);
void ausf_context_final(void);
ausf_context_t *ausf_self(void);

int ausf_context_parse_config(void);

ausf_ue_t *ausf_ue_add(char *suci);
void ausf_ue_remove(ausf_ue_t *ausf_ue);
void ausf_ue_remove_all(void);
ausf_ue_t *ausf_ue_find_by_suci(char *suci);
ausf_ue_t *ausf_ue_find_by_supi(char *supi);
ausf_ue_t *ausf_ue_find_by_suci_or_supi(char *suci_or_supi);
ausf_ue_t *ausf_ue_find_by_ctx_id(char *ctx_id);
ausf_ue_t *ausf_ue_find_by_id(ogs_pool_id_t id);

int get_ue_load(void);

#ifdef __cplusplus
}
#endif

#endif /* AUSF_CONTEXT_H */
