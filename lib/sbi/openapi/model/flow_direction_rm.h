/*
 * flow_direction_rm.h
 *
 * This data type is defined in the same way as the \&quot;FlowDirection\&quot; data type, with the only difference that it allows null value.
 */

#ifndef _OpenAPI_flow_direction_rm_H_
#define _OpenAPI_flow_direction_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flow_direction.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_flow_direction_rm_s OpenAPI_flow_direction_rm_t;
typedef struct OpenAPI_flow_direction_rm_s {
} OpenAPI_flow_direction_rm_t;

OpenAPI_flow_direction_rm_t *OpenAPI_flow_direction_rm_create(
);
void OpenAPI_flow_direction_rm_free(OpenAPI_flow_direction_rm_t *flow_direction_rm);
OpenAPI_flow_direction_rm_t *OpenAPI_flow_direction_rm_parseFromJSON(cJSON *flow_direction_rmJSON);
cJSON *OpenAPI_flow_direction_rm_convertToJSON(OpenAPI_flow_direction_rm_t *flow_direction_rm);
OpenAPI_flow_direction_rm_t *OpenAPI_flow_direction_rm_copy(OpenAPI_flow_direction_rm_t *dst, OpenAPI_flow_direction_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_direction_rm_H_ */

