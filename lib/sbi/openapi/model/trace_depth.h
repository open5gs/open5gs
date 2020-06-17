/*
 * trace_depth.h
 *
 *
 */

#ifndef _OpenAPI_trace_depth_H_
#define _OpenAPI_trace_depth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trace_depth_s OpenAPI_trace_depth_t;
typedef struct OpenAPI_trace_depth_s {
} OpenAPI_trace_depth_t;

OpenAPI_trace_depth_t *OpenAPI_trace_depth_create(
    );
void OpenAPI_trace_depth_free(OpenAPI_trace_depth_t *trace_depth);
OpenAPI_trace_depth_t *OpenAPI_trace_depth_parseFromJSON(cJSON *trace_depthJSON);
cJSON *OpenAPI_trace_depth_convertToJSON(OpenAPI_trace_depth_t *trace_depth);
OpenAPI_trace_depth_t *OpenAPI_trace_depth_copy(OpenAPI_trace_depth_t *dst, OpenAPI_trace_depth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_depth_H_ */

