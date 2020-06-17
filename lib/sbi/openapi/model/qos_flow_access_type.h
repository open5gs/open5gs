/*
 * qos_flow_access_type.h
 *
 * Possible values are   - 3GPP   - NON_3GPP   - 3GPP_AND_NON_3GPP
 */

#ifndef _OpenAPI_qos_flow_access_type_H_
#define _OpenAPI_qos_flow_access_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_access_type_s OpenAPI_qos_flow_access_type_t;
typedef struct OpenAPI_qos_flow_access_type_s {
} OpenAPI_qos_flow_access_type_t;

OpenAPI_qos_flow_access_type_t *OpenAPI_qos_flow_access_type_create(
    );
void OpenAPI_qos_flow_access_type_free(OpenAPI_qos_flow_access_type_t *qos_flow_access_type);
OpenAPI_qos_flow_access_type_t *OpenAPI_qos_flow_access_type_parseFromJSON(cJSON *qos_flow_access_typeJSON);
cJSON *OpenAPI_qos_flow_access_type_convertToJSON(OpenAPI_qos_flow_access_type_t *qos_flow_access_type);
OpenAPI_qos_flow_access_type_t *OpenAPI_qos_flow_access_type_copy(OpenAPI_qos_flow_access_type_t *dst, OpenAPI_qos_flow_access_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_access_type_H_ */

