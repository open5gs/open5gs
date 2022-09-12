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

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_CONTEXT_H
#define OGS_SBI_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_MAX_NUM_OF_NF_INFO 8

typedef struct ogs_sbi_client_s ogs_sbi_client_t;
typedef struct ogs_sbi_smf_info_s ogs_sbi_smf_info_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;

typedef enum {
    OGS_SBI_DISCOVERY_DELEGATED_AUTO = 0,
    OGS_SBI_DISCOVERY_DELEGATED_YES,
    OGS_SBI_DISCOVERY_DELEGATED_NO,
} ogs_sbi_discovery_delegated_mode;

typedef struct ogs_sbi_discovery_config_s {
    ogs_sbi_discovery_delegated_mode delegated;
    bool no_service_names;
    bool prefer_requester_nf_instance_id;
} ogs_sbi_discovery_config_t;

typedef struct ogs_sbi_context_s {
    ogs_sbi_discovery_config_t discovery_config; /* SCP Discovery Delegated */

    uint32_t sbi_port;                      /* SBI local port */

    ogs_list_t server_list;
    ogs_list_t client_list;

    ogs_uuid_t uuid;

    ogs_list_t nf_instance_list;
    ogs_list_t subscription_data_list;

    ogs_sbi_nf_instance_t *nf_instance;     /* SELF NF Instance */
    ogs_sbi_nf_instance_t *nrf_instance;    /* NRF Instance */
    ogs_sbi_nf_instance_t *scp_instance;    /* SCP Instance */

    const char *content_encoding;

    int num_of_service_name;
    const char *service_name[OGS_SBI_MAX_NUM_OF_SERVICE_TYPE];
} ogs_sbi_context_t;

typedef struct ogs_sbi_nf_instance_s {
    ogs_lnode_t lnode;

    ogs_fsm_t sm;                           /* A state machine */
    ogs_timer_t *t_registration_interval;   /* timer to retry
                                               to register peer node */
    struct {
        int heartbeat_interval;
        int validity_duration;
    } time;

    ogs_timer_t *t_heartbeat_interval;      /* heartbeat interval */
    ogs_timer_t *t_no_heartbeat;            /* check heartbeat */
    ogs_timer_t *t_validity;                /* check validation */

#define NF_INSTANCE_ID_IS_SELF(_iD) \
    (_iD) && ogs_sbi_self()->nf_instance && \
        strcmp((_iD), ogs_sbi_self()->nf_instance->id) == 0
#define NF_INSTANCE_ID_IS_OTHERS(_iD) \
    (_iD) && ogs_sbi_self()->nf_instance && \
        strcmp((_iD), ogs_sbi_self()->nf_instance->id) != 0

#define NF_INSTANCE_TYPE_IS_NRF(__nFInstance) \
    ((__nFInstance->nf_type) == OpenAPI_nf_type_NRF)

    char *id;                               /* NFInstanceId */

    OpenAPI_nf_type_e nf_type;
    OpenAPI_nf_status_e nf_status;

    char *fqdn;

#define OGS_SBI_MAX_NUM_OF_IP_ADDRESS 8
    int num_of_ipv4;
    ogs_sockaddr_t *ipv4[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];
    int num_of_ipv6;
    ogs_sockaddr_t *ipv6[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];

    int num_of_allowed_nf_type;
#define OGS_SBI_MAX_NUM_OF_NF_TYPE 128
    OpenAPI_nf_type_e allowed_nf_type[OGS_SBI_MAX_NUM_OF_NF_TYPE];

#define OGS_SBI_DEFAULT_PRIORITY 0
#define OGS_SBI_DEFAULT_CAPACITY 100
#define OGS_SBI_DEFAULT_LOAD 0
    int priority;
    int capacity;
    int load;

    ogs_list_t nf_service_list;
    ogs_list_t nf_info_list;

    void *client;                       /* only used in CLIENT */
    unsigned int reference_count;       /* reference count for memory free */
} ogs_sbi_nf_instance_t;

typedef enum {
    OGS_SBI_OBJ_BASE = 0,

    OGS_SBI_OBJ_UE_TYPE,
    OGS_SBI_OBJ_SESS_TYPE,

    OGS_SBI_OBJ_TOP,
} ogs_sbi_obj_type_e;

typedef struct ogs_sbi_object_s {
    ogs_lnode_t lnode;

    ogs_sbi_obj_type_e type;

    struct {
        ogs_sbi_nf_instance_t *nf_instance;
    } nf_type_array[OGS_SBI_MAX_NUM_OF_NF_TYPE],
      service_type_array[OGS_SBI_MAX_NUM_OF_SERVICE_TYPE];

    ogs_list_t xact_list;

} ogs_sbi_object_t;

typedef ogs_sbi_request_t *(*ogs_sbi_build_f)(
        void *context, void *data);

typedef struct ogs_sbi_xact_s {
    ogs_lnode_t lnode;

    ogs_sbi_service_type_e service_type;
    ogs_sbi_discovery_option_t *discovery_option;

    ogs_sbi_request_t *request;
    ogs_timer_t *t_response;

    ogs_sbi_stream_t *assoc_stream;
    int state;

    ogs_sbi_object_t *sbi_object;
} ogs_sbi_xact_t;

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
    } version[OGS_SBI_MAX_NUM_OF_SERVICE_VERSION];

    char *fqdn;
    int num_of_addr;
    struct {
        ogs_sockaddr_t *ipv4;
        ogs_sockaddr_t *ipv6;
        int port;
    } addr[OGS_SBI_MAX_NUM_OF_IP_ADDRESS];

    int num_of_allowed_nf_type;
    OpenAPI_nf_type_e allowed_nf_type[OGS_SBI_MAX_NUM_OF_NF_TYPE];

    int priority;
    int capacity;
    int load;

    /* Related Context */
    ogs_sbi_nf_instance_t *nf_instance;
    void *client;
} ogs_sbi_nf_service_t;

typedef struct ogs_sbi_subscription_data_s {
    ogs_lnode_t lnode;

    struct {
        int validity_duration;
    } time;

    ogs_timer_t *t_validity;                /* check validation */

    char *id;                               /* SubscriptionId */
    char *req_nf_instance_id;               /* reqNfInstanceId */
    OpenAPI_nf_type_e req_nf_type;          /* reqNfType */
    OpenAPI_nf_status_e nf_status;
    char *notification_uri;

    struct {
        OpenAPI_nf_type_e nf_type;          /* nfType */
        char *service_name;           /* ServiceName */
    } subscr_cond;

    uint64_t requester_features;
    uint64_t nrf_supported_features;

    void *client;                           /* only used in SERVER */
} ogs_sbi_subscription_data_t;

typedef struct ogs_sbi_smf_info_s {
    int num_of_slice;
    struct {
        ogs_s_nssai_t s_nssai;

        int num_of_dnn;
        char *dnn[OGS_MAX_NUM_OF_DNN];
    } slice[OGS_MAX_NUM_OF_SLICE];

    int num_of_nr_tai;
    ogs_5gs_tai_t nr_tai[OGS_MAX_NUM_OF_TAI];

    int num_of_nr_tai_range;
    struct {
        ogs_plmn_id_t plmn_id;
        /*
         * TS29.510 6.1.6.2.28 Type: TacRange
         *
         * Either the start and end attributes, or
         * the pattern attribute, shall be present.
         */
        int num_of_tac_range;
        ogs_uint24_t start[OGS_MAX_NUM_OF_TAI], end[OGS_MAX_NUM_OF_TAI];
    } nr_tai_range[OGS_MAX_NUM_OF_TAI];
} ogs_sbi_smf_info_t;

typedef struct ogs_sbi_nf_info_s {
    ogs_lnode_t lnode;

    OpenAPI_nf_type_e nf_type;
    union {
        ogs_sbi_smf_info_t smf;
    };
} ogs_sbi_nf_info_t;

void ogs_sbi_context_init(void);
void ogs_sbi_context_final(void);
ogs_sbi_context_t *ogs_sbi_self(void);
int ogs_sbi_context_parse_config(
        const char *local, const char *nrf, const char *scp);

bool ogs_sbi_nf_service_is_available(const char *name);

ogs_sbi_nf_instance_t *ogs_sbi_scp_instance(void);

ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_add(void);
void ogs_sbi_nf_instance_set_id(ogs_sbi_nf_instance_t *nf_instance, char *id);
void ogs_sbi_nf_instance_set_type(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e nf_type);
void ogs_sbi_nf_instance_set_status(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_status_e nf_status);
void ogs_sbi_nf_instance_add_allowed_nf_type(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e allowed_nf_type);
bool ogs_sbi_nf_instance_is_allowed_nf_type(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e allowed_nf_type);
void ogs_sbi_nf_instance_clear(ogs_sbi_nf_instance_t *nf_instance);
void ogs_sbi_nf_instance_remove(ogs_sbi_nf_instance_t *nf_instance);
void ogs_sbi_nf_instance_remove_all(void);
ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find(char *id);
ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find_by_discovery_param(
        OpenAPI_nf_type_e nf_type,
        ogs_sbi_discovery_option_t *discovery_option);
ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find_by_service_type(
        ogs_sbi_service_type_e service_type);
bool ogs_sbi_nf_instance_maximum_number_is_reached(void);

ogs_sbi_nf_service_t *ogs_sbi_nf_service_add(
        ogs_sbi_nf_instance_t *nf_instance,
        char *id, const char *name, OpenAPI_uri_scheme_e scheme);
void ogs_sbi_nf_service_add_version(
        ogs_sbi_nf_service_t *nf_service,
        const char *in_uri, const char *full, const char *expiry);
void ogs_sbi_nf_service_add_allowed_nf_type(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_type_e allowed_nf_type);
bool ogs_sbi_nf_service_is_allowed_nf_type(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_type_e allowed_nf_type);
void ogs_sbi_nf_service_clear(ogs_sbi_nf_service_t *nf_service);
void ogs_sbi_nf_service_remove(ogs_sbi_nf_service_t *nf_service);
void ogs_sbi_nf_service_remove_all(ogs_sbi_nf_instance_t *nf_instance);
ogs_sbi_nf_service_t *ogs_sbi_nf_service_find_by_id(
        ogs_sbi_nf_instance_t *nf_instance, char *id);
ogs_sbi_nf_service_t *ogs_sbi_nf_service_find_by_name(
        ogs_sbi_nf_instance_t *nf_instance, char *name);

ogs_sbi_nf_info_t *ogs_sbi_nf_info_add(
        ogs_list_t *list, OpenAPI_nf_type_e nf_type);
void ogs_sbi_nf_info_remove(ogs_list_t *list, ogs_sbi_nf_info_t *nf_info);
void ogs_sbi_nf_info_remove_all(ogs_list_t *list);
ogs_sbi_nf_info_t *ogs_sbi_nf_info_find(
        ogs_list_t *list, OpenAPI_nf_type_e nf_type);

void ogs_sbi_nf_instance_build_default(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_type_e nf_type);
ogs_sbi_nf_service_t *ogs_sbi_nf_service_build_default(
        ogs_sbi_nf_instance_t *nf_instance, const char *name);

ogs_sbi_client_t *ogs_sbi_client_find_by_service_name(
        ogs_sbi_nf_instance_t *nf_instance, char *name, char *version);

void ogs_sbi_client_associate(ogs_sbi_nf_instance_t *nf_instance);

OpenAPI_uri_scheme_e ogs_sbi_default_uri_scheme(void);

#define OGS_SBI_SETUP_NF_INSTANCE(__cTX, __nFInstance) \
    do { \
        ogs_assert(__nFInstance); \
        \
        if ((__cTX).nf_instance) { \
            ogs_warn("NF Instance updated [%s]", (__nFInstance)->id); \
            ogs_sbi_nf_instance_remove((__cTX).nf_instance); \
        } \
        \
        OGS_OBJECT_REF(__nFInstance); \
        ((__cTX).nf_instance) = (__nFInstance); \
    } while(0)

bool ogs_sbi_discovery_param_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type,
        ogs_sbi_discovery_option_t *discovery_option);

bool ogs_sbi_discovery_option_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option);

void ogs_sbi_object_free(ogs_sbi_object_t *sbi_object);

ogs_sbi_xact_t *ogs_sbi_xact_add(
        ogs_sbi_object_t *sbi_object,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_build_f build, void *context, void *data);
void ogs_sbi_xact_remove(ogs_sbi_xact_t *xact);
void ogs_sbi_xact_remove_all(ogs_sbi_object_t *sbi_object);
ogs_sbi_xact_t *ogs_sbi_xact_cycle(ogs_sbi_xact_t *xact);

ogs_sbi_subscription_data_t *ogs_sbi_subscription_data_add(void);
void ogs_sbi_subscription_data_set_id(
        ogs_sbi_subscription_data_t *subscription_data, char *id);
void ogs_sbi_subscription_data_remove(
        ogs_sbi_subscription_data_t *subscription_data);
void ogs_sbi_subscription_data_remove_all_by_nf_instance_id(
        char *nf_instance_id);
void ogs_sbi_subscription_data_remove_all(void);
ogs_sbi_subscription_data_t *ogs_sbi_subscription_data_find(char *id);
void ogs_sbi_subscription_data_build_default(
        OpenAPI_nf_type_e nf_type, const char *service_name);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_CONTEXT_H */
