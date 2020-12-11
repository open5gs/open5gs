/*
 * qos_flow_usage.h
 *
 * Possible values are - GENERAL: Indicate no specific QoS flow usage information is available.  - IMS_SIG: Indicate that the QoS flow is used for IMS signalling only.
 */

#ifndef _OpenAPI_qos_flow_usage_H_
#define _OpenAPI_qos_flow_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_usage_s OpenAPI_qos_flow_usage_t;
typedef struct OpenAPI_qos_flow_usage_s {
} OpenAPI_qos_flow_usage_t;

OpenAPI_qos_flow_usage_t *OpenAPI_qos_flow_usage_create(
    );
void OpenAPI_qos_flow_usage_free(OpenAPI_qos_flow_usage_t *qos_flow_usage);
OpenAPI_qos_flow_usage_t *OpenAPI_qos_flow_usage_parseFromJSON(cJSON *qos_flow_usageJSON);
cJSON *OpenAPI_qos_flow_usage_convertToJSON(OpenAPI_qos_flow_usage_t *qos_flow_usage);
OpenAPI_qos_flow_usage_t *OpenAPI_qos_flow_usage_copy(OpenAPI_qos_flow_usage_t *dst, OpenAPI_qos_flow_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_usage_H_ */

