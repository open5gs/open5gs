/*
 * requested_rule_data_type.h
 *
 * Possible values are - CH_ID: Indicates that the requested rule data is the charging identifier.  - MS_TIME_ZONE: Indicates that the requested access network info type is the UE&#39;s timezone. - USER_LOC_INFO: Indicates that the requested access network info type is the UE&#39;s location. - RES_RELEASE: Indicates that the requested rule data is the result of the release of resource. - SUCC_RES_ALLO: Indicates that the requested rule data is the successful resource allocation. - EPS_FALLBACK: Indicates that the requested rule data is the report of QoS flow rejection due to EPS fallback.
 */

#ifndef _OpenAPI_requested_rule_data_type_H_
#define _OpenAPI_requested_rule_data_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_requested_rule_data_type_s OpenAPI_requested_rule_data_type_t;
typedef struct OpenAPI_requested_rule_data_type_s {
} OpenAPI_requested_rule_data_type_t;

OpenAPI_requested_rule_data_type_t *OpenAPI_requested_rule_data_type_create(
    );
void OpenAPI_requested_rule_data_type_free(OpenAPI_requested_rule_data_type_t *requested_rule_data_type);
OpenAPI_requested_rule_data_type_t *OpenAPI_requested_rule_data_type_parseFromJSON(cJSON *requested_rule_data_typeJSON);
cJSON *OpenAPI_requested_rule_data_type_convertToJSON(OpenAPI_requested_rule_data_type_t *requested_rule_data_type);
OpenAPI_requested_rule_data_type_t *OpenAPI_requested_rule_data_type_copy(OpenAPI_requested_rule_data_type_t *dst, OpenAPI_requested_rule_data_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_rule_data_type_H_ */

