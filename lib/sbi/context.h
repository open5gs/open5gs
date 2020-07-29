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

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_CONTEXT_H
#define OGS_SBI_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_SBI_MAX_NF_TYPE 64

typedef struct ogs_sbi_client_s ogs_sbi_client_t;
typedef struct ogs_sbi_context_s {
    ogs_pollset_t       *pollset;       /* Poll Set for I/O Multiplexing */
    ogs_timer_mgr_t     *timer_mgr;     /* Timer Manager */

    uint32_t            http_port;      /* SBI HTTP local port */
    uint32_t            https_port;     /* SBI HTTPS local port */

    ogs_list_t          server_list;
    ogs_list_t          client_list;

    ogs_uuid_t          uuid;
    char                nf_instance_id[OGS_UUID_FORMATTED_LENGTH + 1];

    ogs_list_t          nf_instance_list;
    ogs_list_t          subscription_list;

    const char          *content_encoding;
} ogs_sbi_context_t;

#define OGS_SETUP_SBI_NF_INSTANCE(__cTX, __pNF_INSTANCE) \
    do { \
        ogs_assert((__cTX)); \
        ogs_assert((__pNF_INSTANCE)); \
        if ((__cTX)->nf_instance != __pNF_INSTANCE) \
            __pNF_INSTANCE->reference_count++; \
        (__cTX)->nf_instance = __pNF_INSTANCE; \
        ogs_trace("nf_instance->reference_count = %d", \
                __pNF_INSTANCE->reference_count); \
    } while(0)
typedef struct ogs_sbi_nf_instance_s {
    ogs_lnode_t     lnode;

    ogs_fsm_t       sm;                         /* A state machine */
    ogs_timer_t     *t_registration_interval;   /* timer to retry
                                                   to register peer node */
    struct {
        int heartbeat_interval;
        int validity_duration;
    } time;

    ogs_timer_t     *t_heartbeat_interval;  /* heartbeat interval */
    ogs_timer_t     *t_no_heartbeat;        /* check heartbeat */
    ogs_timer_t     *t_validity;            /* check validation */

#define NF_INSTANCE_IS_SELF(_iD) \
    strcmp((_iD), ogs_sbi_self()->nf_instance_id) == 0
#define NF_INSTANCE_IS_OTHERS(_iD) \
    strcmp((_iD), ogs_sbi_self()->nf_instance_id) != 0

    char *id;                           /* NFInstanceId */

    OpenAPI_nf_type_e nf_type;
    OpenAPI_nf_status_e nf_status;

    char fqdn[OGS_MAX_FQDN_LEN];

#define OGS_SBI_MAX_NUM_OF_IP_ADDRESS 8
    int num_of_ipv4;
    ogs_sockaddr_t *ipv4[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];
    int num_of_ipv6;
    ogs_sockaddr_t *ipv6[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];

    ogs_list_t nf_service_list;

    void *client;                   /* only used in CLIENT */
    unsigned int reference_count;   /* reference count for memory free */
} ogs_sbi_nf_instance_t;

#define OGS_SBI_NF_INSTANCE_GET(__aRRAY, __nFType) \
    ((__aRRAY)[__nFType].nf_instance) 
typedef struct ogs_sbi_nf_types_s {
    ogs_sbi_nf_instance_t *nf_instance;
} ogs_sbi_nf_types_t[OGS_SBI_MAX_NF_TYPE];

typedef struct ogs_sbi_object_s {
    ogs_lnode_t lnode;

    struct {
        ogs_timer_t *timer;
        ogs_time_t duration;
    } client_wait;

    bool running;

    OpenAPI_nf_type_e nf_type;
    ogs_sbi_request_t *request;

    ogs_sbi_nf_types_t nf_types;

    ogs_sbi_session_t *session;
    void *nf_state_registered;

    ogs_sbi_client_cb_f client_cb;
} ogs_sbi_object_t;

typedef struct ogs_sbi_nf_service_s {
    ogs_lnode_t lnode;

    char *id;
    char *name;
    OpenAPI_uri_scheme_e scheme;

    OpenAPI_nf_service_status_e status;

#define OGS_SBI_MAX_NUM_OF_SERVICE_VERSION 8
    int num_of_version;
    struct {
        char *in_uri;
        char *full;
        char *expiry;
    } versions[OGS_SBI_MAX_NUM_OF_SERVICE_VERSION];

    char fqdn[OGS_MAX_FQDN_LEN];
    int num_of_addr;
    struct {
        ogs_sockaddr_t *ipv4;
        ogs_sockaddr_t *ipv6;
        int port;
    } addr[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];

    /* Related Context */
    ogs_sbi_nf_instance_t *nf_instance;
    void *client;
} ogs_sbi_nf_service_t;

typedef struct ogs_sbi_subscription_s {
    ogs_lnode_t lnode;

    struct {
        int validity_duration;
    } time;

    ogs_timer_t *t_validity;            /* check validation */

    char *id;                           /* SubscriptionId */
    char *nf_instance_id;               /* NFInstanceId */
    OpenAPI_nf_type_e nf_type;
    OpenAPI_nf_status_e nf_status;
    char *notification_uri;

    void *client;                       /* only used in SERVER */
} ogs_sbi_subscription_t;

typedef struct ogs_sbi_discover_s {
    OpenAPI_nf_type_e nf_type;

    char *uri;
    struct {
        char *name;
    } service;
    struct {
        char *version;
    } api;

    ogs_sbi_request_t *request;
} ogs_sbi_discover_t;

void ogs_sbi_context_init(ogs_pollset_t *pollset, ogs_timer_mgr_t *timer_mgr);
void ogs_sbi_context_final(void);
ogs_sbi_context_t *ogs_sbi_self(void);
int ogs_sbi_context_parse_config(const char *local, const char *remote);

ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_add(char *id);
void ogs_sbi_nf_instance_clear(ogs_sbi_nf_instance_t *nf_instance);
void ogs_sbi_nf_instance_remove(ogs_sbi_nf_instance_t *nf_instance);
void ogs_sbi_nf_instance_remove_all(void);
ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find(char *id);
ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find_by_nf_type(
        OpenAPI_nf_type_e nf_type);

ogs_sbi_nf_service_t *ogs_sbi_nf_service_add(ogs_sbi_nf_instance_t *nf_instance,
        char *id, char *name, OpenAPI_uri_scheme_e scheme);
void ogs_sbi_nf_service_add_version(ogs_sbi_nf_service_t *nf_service,
        char *in_uri, char *full, char *expiry);
void ogs_sbi_nf_service_clear(ogs_sbi_nf_service_t *nf_service);
void ogs_sbi_nf_service_remove(ogs_sbi_nf_service_t *nf_service);
void ogs_sbi_nf_service_remove_all(ogs_sbi_nf_instance_t *nf_instance);
ogs_sbi_nf_service_t *ogs_sbi_nf_service_find(
        ogs_sbi_nf_instance_t *nf_instance, char *name);

void ogs_sbi_nf_instance_build_default(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e nf_type);
ogs_sbi_nf_service_t *ogs_sbi_nf_service_build_default(
        ogs_sbi_nf_instance_t *nf_instance, char *name);

ogs_sbi_client_t *ogs_sbi_client_find_by_service_name(
        ogs_sbi_nf_instance_t *nf_instance, char *name, char *version);

bool ogs_sbi_client_associate(ogs_sbi_nf_instance_t *nf_instance);
bool ogs_sbi_nf_types_associate(
        ogs_sbi_nf_types_t nf_types, OpenAPI_nf_type_e nf_type, void *state);

void ogs_sbi_object_free(ogs_sbi_object_t *sbi_object);

ogs_sbi_subscription_t *ogs_sbi_subscription_add(void);
void ogs_sbi_subscription_set_id(
        ogs_sbi_subscription_t *subscription, char *id);
void ogs_sbi_subscription_remove(ogs_sbi_subscription_t *subscription);
void ogs_sbi_subscription_remove_all_by_nf_instance_id(char *nf_instance_id);
void ogs_sbi_subscription_remove_all(void);
ogs_sbi_subscription_t *ogs_sbi_subscription_find(char *id);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_CONTEXT_H */
