/*
 * nf_service.h
 *
 *
 */

#ifndef _OpenAPI_nf_service_H_
#define _OpenAPI_nf_service_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "chf_service_info.h"
#include "default_notification_subscription.h"
#include "ip_end_point.h"
#include "nf_service_status.h"
#include "nf_service_version.h"
#include "nf_type.h"
#include "plmn_id.h"
#include "snssai.h"
#include "uri_scheme.h"

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
    OpenAPI_list_t *allowed_nf_types;
    OpenAPI_list_t *allowed_nf_domains;
    OpenAPI_list_t *allowed_nssais;
    int priority;
    int capacity;
    int load;
    char *recovery_time;
    struct OpenAPI_chf_service_info_s *chf_service_info;
    char *supported_features;
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
    OpenAPI_list_t *allowed_nf_types,
    OpenAPI_list_t *allowed_nf_domains,
    OpenAPI_list_t *allowed_nssais,
    int priority,
    int capacity,
    int load,
    char *recovery_time,
    OpenAPI_chf_service_info_t *chf_service_info,
    char *supported_features
    );
void OpenAPI_nf_service_free(OpenAPI_nf_service_t *nf_service);
OpenAPI_nf_service_t *OpenAPI_nf_service_parseFromJSON(cJSON *nf_serviceJSON);
cJSON *OpenAPI_nf_service_convertToJSON(OpenAPI_nf_service_t *nf_service);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_service_H_ */

