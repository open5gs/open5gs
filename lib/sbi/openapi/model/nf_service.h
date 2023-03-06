/*
 * nf_service.h
 *
 * Information of a given NF Service Instance; it is part of the NFProfile of an NF Instance 
 */

#ifndef _OpenAPI_nf_service_H_
#define _OpenAPI_nf_service_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "default_notification_subscription.h"
#include "ext_snssai.h"
#include "ip_end_point.h"
#include "nf_service_status.h"
#include "nf_service_version.h"
#include "nf_type.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "plmn_oauth2.h"
#include "plmn_snssai.h"
#include "uri_scheme.h"
#include "vendor_specific_feature.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_service_s OpenAPI_nf_service_t;
typedef struct OpenAPI_nf_service_s {
    char *service_instance_id;
    char *service_name;
    OpenAPI_list_t *versions;
    OpenAPI_uri_scheme_e scheme;
    OpenAPI_nf_service_status_e nf_service_status;
    char *fqdn;
    char *inter_plmn_fqdn;
    OpenAPI_list_t *ip_end_points;
    char *api_prefix;
    OpenAPI_list_t *default_notification_subscriptions;
    OpenAPI_list_t *allowed_plmns;
    OpenAPI_list_t *allowed_snpns;
    OpenAPI_list_t *allowed_nf_types;
    OpenAPI_list_t *allowed_nf_domains;
    OpenAPI_list_t *allowed_nssais;
    OpenAPI_list_t* allowed_operations_per_nf_type;
    OpenAPI_list_t* allowed_operations_per_nf_instance;
    bool is_priority;
    int priority;
    bool is_capacity;
    int capacity;
    bool is_load;
    int load;
    char *load_time_stamp;
    char *recovery_time;
    char *supported_features;
    OpenAPI_list_t *nf_service_set_id_list;
    OpenAPI_list_t *s_nssais;
    OpenAPI_list_t *per_plmn_snssai_list;
    char *vendor_id;
    OpenAPI_list_t* supported_vendor_specific_features;
    bool is_oauth2_required;
    int oauth2_required;
    struct OpenAPI_plmn_oauth2_s *per_plmn_oauth2_req_list;
} OpenAPI_nf_service_t;

OpenAPI_nf_service_t *OpenAPI_nf_service_create(
    char *service_instance_id,
    char *service_name,
    OpenAPI_list_t *versions,
    OpenAPI_uri_scheme_e scheme,
    OpenAPI_nf_service_status_e nf_service_status,
    char *fqdn,
    char *inter_plmn_fqdn,
    OpenAPI_list_t *ip_end_points,
    char *api_prefix,
    OpenAPI_list_t *default_notification_subscriptions,
    OpenAPI_list_t *allowed_plmns,
    OpenAPI_list_t *allowed_snpns,
    OpenAPI_list_t *allowed_nf_types,
    OpenAPI_list_t *allowed_nf_domains,
    OpenAPI_list_t *allowed_nssais,
    OpenAPI_list_t* allowed_operations_per_nf_type,
    OpenAPI_list_t* allowed_operations_per_nf_instance,
    bool is_priority,
    int priority,
    bool is_capacity,
    int capacity,
    bool is_load,
    int load,
    char *load_time_stamp,
    char *recovery_time,
    char *supported_features,
    OpenAPI_list_t *nf_service_set_id_list,
    OpenAPI_list_t *s_nssais,
    OpenAPI_list_t *per_plmn_snssai_list,
    char *vendor_id,
    OpenAPI_list_t* supported_vendor_specific_features,
    bool is_oauth2_required,
    int oauth2_required,
    OpenAPI_plmn_oauth2_t *per_plmn_oauth2_req_list
);
void OpenAPI_nf_service_free(OpenAPI_nf_service_t *nf_service);
OpenAPI_nf_service_t *OpenAPI_nf_service_parseFromJSON(cJSON *nf_serviceJSON);
cJSON *OpenAPI_nf_service_convertToJSON(OpenAPI_nf_service_t *nf_service);
OpenAPI_nf_service_t *OpenAPI_nf_service_copy(OpenAPI_nf_service_t *dst, OpenAPI_nf_service_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_service_H_ */

