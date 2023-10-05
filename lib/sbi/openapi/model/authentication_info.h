/*
 * authentication_info.h
 *
 * Contains the UE id (i.e. SUCI or SUPI) and the Serving Network Name.
 */

#ifndef _OpenAPI_authentication_info_H_
#define _OpenAPI_authentication_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "resynchronization_info.h"
#include "trace_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authentication_info_s OpenAPI_authentication_info_t;
typedef struct OpenAPI_authentication_info_s {
    char *supi_or_suci;
    char *serving_network_name;
    struct OpenAPI_resynchronization_info_s *resynchronization_info;
    char *pei;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    char *udm_group_id;
    char *routing_indicator;
    OpenAPI_list_t *cell_cag_info;
    bool is_n5gc_ind;
    int n5gc_ind;
    char *supported_features;
    bool is_nswo_ind;
    int nswo_ind;
    bool is_disaster_roaming_ind;
    int disaster_roaming_ind;
    bool is_onboarding_ind;
    int onboarding_ind;
} OpenAPI_authentication_info_t;

OpenAPI_authentication_info_t *OpenAPI_authentication_info_create(
    char *supi_or_suci,
    char *serving_network_name,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *pei,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *udm_group_id,
    char *routing_indicator,
    OpenAPI_list_t *cell_cag_info,
    bool is_n5gc_ind,
    int n5gc_ind,
    char *supported_features,
    bool is_nswo_ind,
    int nswo_ind,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    bool is_onboarding_ind,
    int onboarding_ind
);
void OpenAPI_authentication_info_free(OpenAPI_authentication_info_t *authentication_info);
OpenAPI_authentication_info_t *OpenAPI_authentication_info_parseFromJSON(cJSON *authentication_infoJSON);
cJSON *OpenAPI_authentication_info_convertToJSON(OpenAPI_authentication_info_t *authentication_info);
OpenAPI_authentication_info_t *OpenAPI_authentication_info_copy(OpenAPI_authentication_info_t *dst, OpenAPI_authentication_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authentication_info_H_ */

