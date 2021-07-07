/*
 * secondary_rat_usage_info.h
 *
 * 
 */

#ifndef _OpenAPI_secondary_rat_usage_info_H_
#define _OpenAPI_secondary_rat_usage_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_flow_usage_report.h"
#include "rat_type.h"
#include "volume_timed_report.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_secondary_rat_usage_info_s OpenAPI_secondary_rat_usage_info_t;
typedef struct OpenAPI_secondary_rat_usage_info_s {
    OpenAPI_rat_type_e secondary_rat_type;
    OpenAPI_list_t *qos_flows_usage_data;
    OpenAPI_list_t *pdu_session_usage_data;
} OpenAPI_secondary_rat_usage_info_t;

OpenAPI_secondary_rat_usage_info_t *OpenAPI_secondary_rat_usage_info_create(
    OpenAPI_rat_type_e secondary_rat_type,
    OpenAPI_list_t *qos_flows_usage_data,
    OpenAPI_list_t *pdu_session_usage_data
);
void OpenAPI_secondary_rat_usage_info_free(OpenAPI_secondary_rat_usage_info_t *secondary_rat_usage_info);
OpenAPI_secondary_rat_usage_info_t *OpenAPI_secondary_rat_usage_info_parseFromJSON(cJSON *secondary_rat_usage_infoJSON);
cJSON *OpenAPI_secondary_rat_usage_info_convertToJSON(OpenAPI_secondary_rat_usage_info_t *secondary_rat_usage_info);
OpenAPI_secondary_rat_usage_info_t *OpenAPI_secondary_rat_usage_info_copy(OpenAPI_secondary_rat_usage_info_t *dst, OpenAPI_secondary_rat_usage_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_secondary_rat_usage_info_H_ */

