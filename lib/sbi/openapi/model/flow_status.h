/*
 * flow_status.h
 *
 *
 */

#ifndef _OpenAPI_flow_status_H_
#define _OpenAPI_flow_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_flow_status_s OpenAPI_flow_status_t;
typedef struct OpenAPI_flow_status_s {
} OpenAPI_flow_status_t;

OpenAPI_flow_status_t *OpenAPI_flow_status_create(
    );
void OpenAPI_flow_status_free(OpenAPI_flow_status_t *flow_status);
OpenAPI_flow_status_t *OpenAPI_flow_status_parseFromJSON(cJSON *flow_statusJSON);
cJSON *OpenAPI_flow_status_convertToJSON(OpenAPI_flow_status_t *flow_status);
OpenAPI_flow_status_t *OpenAPI_flow_status_copy(OpenAPI_flow_status_t *dst, OpenAPI_flow_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_status_H_ */

