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

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_CONTEXT_H
#define OGS_SBI_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_MAX_NUM_OF_NF_INFO 8
#define OGS_MAX_NUM_OF_SCP_DOMAIN 8

typedef struct ogs_sbi_client_s ogs_sbi_client_t;
typedef struct ogs_sbi_smf_info_s ogs_sbi_smf_info_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;

typedef enum {
    OGS_SBI_CLIENT_DELEGATED_AUTO = 0,
    OGS_SBI_CLIENT_DELEGATED_YES,
    OGS_SBI_CLIENT_DELEGATED_NO,
} ogs_sbi_client_delegated_mode_e;

/* To hold all delegated config under sbi.client.delegated */
typedef struct ogs_sbi_client_delegated_config_s {
    struct {
        ogs_sbi_client_delegated_mode_e nfm;  /* e.g. Registration, Heartbeat */
        ogs_sbi_client_delegated_mode_e disc; /* NF discovery */
    } nrf;
    struct {
        ogs_sbi_client_delegated_mode_e next; /* Next-hop SCP delegation */
    } scp;
} ogs_sbi_client_delegated_config_t;

typedef struct ogs_sbi_context_s {
    /* For sbi.client.delegated */
    ogs_sbi_client_delegated_config_t client_delegated_config;

#define OGS_HOME_NETWORK_PKI_VALUE_MIN 1
#define OGS_HOME_NETWORK_PKI_VALUE_MAX 254

    struct {
        uint8_t avail;
        uint8_t scheme;
        uint8_t key[OGS_ECCKEY_LEN]; /* 32 bytes Private Key */
    } hnet[OGS_HOME_NETWORK_PKI_VALUE_MAX+1]; /* PKI Value : 1 ~ 254 */

    struct {
        struct {
            OpenAPI_uri_scheme_e scheme;

            const char *private_key;
            const char *cert;
            const char *sslkeylog;

            bool verify_client;
            const char *verify_client_cacert;
        } server;
        struct {
            OpenAPI_uri_scheme_e scheme;

            bool insecure_skip_verify;
            const char *cacert;

            const char *private_key;
            const char *cert;
            const char *sslkeylog;
        } client;
    } tls;

    ogs_list_t server_list;
    ogs_list_t client_list;

    ogs_uuid_t uuid;

    ogs_list_t nf_instance_list;
    ogs_list_t subscription_spec_list;
    ogs_list_t subscription_data_list;

    ogs_sbi_nf_instance_t *nf_instance;     /* SELF NF Instance */
    ogs_sbi_nf_instance_t *nrf_instance;    /* NRF Instance */
    ogs_sbi_nf_instance_t *scp_instance;    /* SCP Instance */
    ogs_sbi_nf_instance_t *sepp_instance;   /* SEPP Instance */

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

    /*
     * Issues #2034
     *
     * Other NF instances are obtained through NRF
     * or created directly through configuration files.
     *
     * Other NFs created by the config file should not be passed
     * through NRF discovery or anything like that.
     *
     * Since self-created NF Instances do not have an ID,
     * they are implemented to exclude them from NRF Discovery.
     */
#define NF_INSTANCE_EXCLUDED_FROM_DISCOVERY(__nFInstance) \
    (!(__nFInstance) || !((__nFInstance)->id))

#define NF_INSTANCE_ID(__nFInstance) \
    ((__nFInstance) ? ((__nFInstance)->id) : NULL)
#define NF_INSTANCE_ID_IS_SELF(_iD) \
    (_iD) && ogs_sbi_self()->nf_instance && \
        strcmp((_iD), ogs_sbi_self()->nf_instance->id) == 0
#define NF_INSTANCE_ID_IS_OTHERS(_iD) \
    (_iD) && ogs_sbi_self()->nf_instance && \
        strcmp((_iD), ogs_sbi_self()->nf_instance->id) != 0
    char *id;

#define NF_INSTANCE_TYPE(__nFInstance) \
    ((__nFInstance) ? ((__nFInstance)->nf_type) : OpenAPI_nf_type_NULL)
#define NF_INSTANCE_TYPE_IS_NRF(__nFInstance) \
    (NF_INSTANCE_TYPE(__nFInstance) == OpenAPI_nf_type_NRF)
    OpenAPI_nf_type_e nf_type;
    OpenAPI_nf_status_e nf_status;

    ogs_plmn_id_t plmn_id[OGS_MAX_NUM_OF_PLMN];
    int num_of_plmn_id;

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

#define NF_INSTANCE_CLIENT(__nFInstance) \
    ((__nFInstance) ? ((__nFInstance)->client) : NULL)
    void *client;                       /* only used in CLIENT */
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

        /*
         * Search.Result stored in nf_instance->time.validity_duration;
         *
         * validity_timeout = nf_instance->validity->timeout =
         *     ogs_get_monotonic_time() + nf_instance->time.validity_duration;
         *
         * if no validityPeriod in SearchResult, validity_timeout is 0.
         */
        ogs_time_t validity_timeout;
    } nf_type_array[OGS_SBI_MAX_NUM_OF_NF_TYPE],
      service_type_array[OGS_SBI_MAX_NUM_OF_SERVICE_TYPE];

    ogs_list_t xact_list;

} ogs_sbi_object_t;

typedef ogs_sbi_request_t *(*ogs_sbi_build_f)(
        void *context, void *data);

typedef struct ogs_sbi_xact_s {
    ogs_lnode_t lnode;

    ogs_pool_id_t id;

    ogs_sbi_service_type_e service_type;
    OpenAPI_nf_type_e requester_nf_type;
    ogs_sbi_discovery_option_t *discovery_option;

    ogs_sbi_request_t *request;
    ogs_timer_t *t_response;

    ogs_pool_id_t assoc_stream_id;

    int state;
    char *target_apiroot;

    ogs_sbi_object_t *sbi_object;
    ogs_pool_id_t sbi_object_id;
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
        bool is_port;
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

typedef struct ogs_sbi_subscription_spec_s {
    ogs_lnode_t lnode;

    struct {
        OpenAPI_nf_type_e nf_type;          /* nfType */
        char *service_name;                 /* ServiceName */
    } subscr_cond;

} ogs_sbi_subscription_spec_t;

typedef struct ogs_sbi_subscription_data_s {
    ogs_lnode_t lnode;

    ogs_time_t validity_duration;           /* valditiyTime(unit: usec) */
    ogs_timer_t *t_validity;                /* check validation */
    ogs_timer_t *t_patch;                   /* for sending PATCH */

    char *id;                               /* SubscriptionId */
    char *req_nf_instance_id;               /* reqNfInstanceId */
    OpenAPI_nf_type_e req_nf_type;          /* reqNfType */
    OpenAPI_nf_status_e nf_status;
    char *notification_uri;
    char *resource_uri;

    struct {
        OpenAPI_nf_type_e nf_type;          /* nfType */
        char *service_name;                 /* ServiceName */
        char *nf_instance_id;               /* NF Instance Id */
    } subscr_cond;

    uint64_t requester_features;
    uint64_t nrf_supported_features;

    void *client;
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

typedef struct ogs_sbi_scp_info_s {
    ogs_port_t http, https;

    int num_of_domain;
    struct {
        char *name;
        char *fqdn;
        ogs_port_t http, https;
    } domain[OGS_MAX_NUM_OF_SCP_DOMAIN];

} ogs_sbi_scp_info_t;

typedef struct ogs_sbi_sepp_info_s {
    ogs_port_t http, https;
} ogs_sbi_sepp_info_t;

typedef struct ogs_sbi_amf_info_s {
    uint8_t amf_set_id;
    uint16_t amf_region_id;

    int num_of_guami;
    ogs_guami_t guami[OGS_MAX_NUM_OF_SERVED_GUAMI];

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
} ogs_sbi_amf_info_t;

typedef struct ogs_sbi_nf_info_s {
    ogs_lnode_t lnode;

    OpenAPI_nf_type_e nf_type;
    union {
        ogs_sbi_smf_info_t smf;
        ogs_sbi_amf_info_t amf;
        ogs_sbi_scp_info_t scp;
        ogs_sbi_sepp_info_t sepp;
    };
} ogs_sbi_nf_info_t;

void ogs_sbi_context_init(OpenAPI_nf_type_e nf_type);
void ogs_sbi_context_final(void);
ogs_sbi_context_t *ogs_sbi_self(void);
int ogs_sbi_context_parse_config(
        const char *local, const char *nrf, const char *scp);
int ogs_sbi_context_parse_hnet_config(ogs_yaml_iter_t *root_iter);
int ogs_sbi_context_parse_server_config(
        ogs_yaml_iter_t *parent, const char *interface);
ogs_sbi_client_t *ogs_sbi_context_parse_client_config(
        ogs_yaml_iter_t *iter);

bool ogs_sbi_nf_service_is_available(const char *name);

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
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option);
ogs_sbi_nf_instance_t *ogs_sbi_nf_instance_find_by_service_type(
        ogs_sbi_service_type_e service_type,
        OpenAPI_nf_type_e requester_nf_type);
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

bool ogs_sbi_check_amf_info_guami(
        ogs_sbi_amf_info_t *amf_info, ogs_guami_t *guami);
bool ogs_sbi_check_smf_info_slice(
        ogs_sbi_smf_info_t *smf_info, ogs_s_nssai_t *s_nssai, char *dnn);
bool ogs_sbi_check_smf_info_tai(
        ogs_sbi_smf_info_t *smf_info, ogs_5gs_tai_t *tai);

void ogs_sbi_nf_instance_build_default(ogs_sbi_nf_instance_t *nf_instance);
ogs_sbi_nf_service_t *ogs_sbi_nf_service_build_default(
        ogs_sbi_nf_instance_t *nf_instance, const char *name);

ogs_sbi_client_t *ogs_sbi_client_find_by_service_name(
        ogs_sbi_nf_instance_t *nf_instance, char *name, char *version);
ogs_sbi_client_t *ogs_sbi_client_find_by_service_type(
        ogs_sbi_nf_instance_t *nf_instance,
        ogs_sbi_service_type_e service_type);

void ogs_sbi_client_associate(ogs_sbi_nf_instance_t *nf_instance);

int ogs_sbi_default_client_port(OpenAPI_uri_scheme_e scheme);

#define OGS_SBI_SETUP_NF_INSTANCE(__cTX, __nFInstance) \
    do { \
        ogs_assert(__nFInstance); \
        ogs_assert((__nFInstance)->id); \
        ogs_assert((__nFInstance)->t_validity); \
        \
        if ((__cTX).nf_instance) { \
            ogs_warn("[%s] NF Instance updated [type:%s validity:%ds]", \
                    ((__cTX).nf_instance)->id, \
                    OpenAPI_nf_type_ToString(((__cTX).nf_instance)->nf_type), \
                    ((__cTX).nf_instance)->time.validity_duration); \
        } \
        \
        ((__cTX).nf_instance) = __nFInstance; \
        if ((__nFInstance)->time.validity_duration) { \
            ((__cTX).validity_timeout) = (__nFInstance)->t_validity->timeout; \
        } else { \
            ((__cTX).validity_timeout) = 0; \
        } \
        ogs_info("[%s] NF Instance setup [type:%s validity:%ds]", \
                (__nFInstance)->id, \
                OpenAPI_nf_type_ToString((__nFInstance)->nf_type), \
                (__nFInstance)->time.validity_duration); \
    } while(0)

/*
 * Search.Result stored in nf_instance->time.validity_duration;
 *
 * validity_timeout = nf_instance->validity->timeout =
 *     ogs_get_monotonic_time() + nf_instance->time.validity_duration;
 *
 * if no validityPeriod in SearchResult, validity_timeout is 0.
 */
#define OGS_SBI_GET_NF_INSTANCE(__cTX) \
    ((__cTX).validity_timeout == 0 || \
     (__cTX).validity_timeout > ogs_get_monotonic_time() ? \
        ((__cTX).nf_instance) : NULL)

#define OGS_SBI_NF_INSTANCE_VALID(__nFInstance) \
    (((__nFInstance) && ((__nFInstance)->t_validity) && \
     ((__nFInstance)->time.validity_duration == 0 || \
      (__nFInstance)->t_validity->timeout > ogs_get_monotonic_time())) ? \
         true : false)

bool ogs_sbi_discovery_param_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option);

bool ogs_sbi_discovery_param_serving_plmn_list_is_matched(
        ogs_sbi_nf_instance_t *nf_instance);

bool ogs_sbi_discovery_option_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option);
bool ogs_sbi_discovery_option_service_names_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option);
bool ogs_sbi_discovery_option_requester_plmn_list_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        ogs_sbi_discovery_option_t *discovery_option);
bool ogs_sbi_discovery_option_target_plmn_list_is_matched(
        ogs_sbi_nf_instance_t *nf_instance,
        ogs_sbi_discovery_option_t *discovery_option);

void ogs_sbi_object_free(ogs_sbi_object_t *sbi_object);

ogs_sbi_xact_t *ogs_sbi_xact_add(
        ogs_pool_id_t sbi_object_id,
        ogs_sbi_object_t *sbi_object,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_build_f build, void *context, void *data);
void ogs_sbi_xact_remove(ogs_sbi_xact_t *xact);
void ogs_sbi_xact_remove_all(ogs_sbi_object_t *sbi_object);
ogs_sbi_xact_t *ogs_sbi_xact_find_by_id(ogs_pool_id_t id);

ogs_sbi_subscription_spec_t *ogs_sbi_subscription_spec_add(
        OpenAPI_nf_type_e nf_type, const char *service_name);
void ogs_sbi_subscription_spec_remove(
        ogs_sbi_subscription_spec_t *subscription_spec);
void ogs_sbi_subscription_spec_remove_all(void);

ogs_sbi_subscription_data_t *ogs_sbi_subscription_data_add(void);
void ogs_sbi_subscription_data_set_resource_uri(
        ogs_sbi_subscription_data_t *subscription_data, char *resource_uri);
void ogs_sbi_subscription_data_set_id(
        ogs_sbi_subscription_data_t *subscription_data, char *id);
void ogs_sbi_subscription_data_remove(
        ogs_sbi_subscription_data_t *subscription_data);
void ogs_sbi_subscription_data_remove_all_by_nf_instance_id(
        char *nf_instance_id);
void ogs_sbi_subscription_data_remove_all(void);
ogs_sbi_subscription_data_t *ogs_sbi_subscription_data_find(char *id);

bool ogs_sbi_supi_in_vplmn(char *supi);
bool ogs_sbi_plmn_id_in_vplmn(ogs_plmn_id_t *plmn_id);
bool ogs_sbi_fqdn_in_vplmn(char *fqdn);

void ogs_sbi_keylog_callback(const SSL *ssl, const char *line);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_CONTEXT_H */
