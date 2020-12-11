/*
 * requested_qos_monitoring_parameter.h
 *
 *
 */

#ifndef _OpenAPI_requested_qos_monitoring_parameter_H_
#define _OpenAPI_requested_qos_monitoring_parameter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_requested_qos_monitoring_parameter_s OpenAPI_requested_qos_monitoring_parameter_t;
typedef struct OpenAPI_requested_qos_monitoring_parameter_s {
} OpenAPI_requested_qos_monitoring_parameter_t;

OpenAPI_requested_qos_monitoring_parameter_t *OpenAPI_requested_qos_monitoring_parameter_create(
    );
void OpenAPI_requested_qos_monitoring_parameter_free(OpenAPI_requested_qos_monitoring_parameter_t *requested_qos_monitoring_parameter);
OpenAPI_requested_qos_monitoring_parameter_t *OpenAPI_requested_qos_monitoring_parameter_parseFromJSON(cJSON *requested_qos_monitoring_parameterJSON);
cJSON *OpenAPI_requested_qos_monitoring_parameter_convertToJSON(OpenAPI_requested_qos_monitoring_parameter_t *requested_qos_monitoring_parameter);
OpenAPI_requested_qos_monitoring_parameter_t *OpenAPI_requested_qos_monitoring_parameter_copy(OpenAPI_requested_qos_monitoring_parameter_t *dst, OpenAPI_requested_qos_monitoring_parameter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_qos_monitoring_parameter_H_ */

