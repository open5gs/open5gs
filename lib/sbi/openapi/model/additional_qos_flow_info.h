/*
 * additional_qos_flow_info.h
 *
 *
 */

#ifndef _OpenAPI_additional_qos_flow_info_H_
#define _OpenAPI_additional_qos_flow_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_additional_qos_flow_info_s OpenAPI_additional_qos_flow_info_t;
typedef struct OpenAPI_additional_qos_flow_info_s {
} OpenAPI_additional_qos_flow_info_t;

OpenAPI_additional_qos_flow_info_t *OpenAPI_additional_qos_flow_info_create(
    );
void OpenAPI_additional_qos_flow_info_free(OpenAPI_additional_qos_flow_info_t *additional_qos_flow_info);
OpenAPI_additional_qos_flow_info_t *OpenAPI_additional_qos_flow_info_parseFromJSON(cJSON *additional_qos_flow_infoJSON);
cJSON *OpenAPI_additional_qos_flow_info_convertToJSON(OpenAPI_additional_qos_flow_info_t *additional_qos_flow_info);
OpenAPI_additional_qos_flow_info_t *OpenAPI_additional_qos_flow_info_copy(OpenAPI_additional_qos_flow_info_t *dst, OpenAPI_additional_qos_flow_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_qos_flow_info_H_ */

