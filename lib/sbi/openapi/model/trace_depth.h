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

typedef enum { OpenAPI_trace_depth_NULL = 0, OpenAPI_trace_depth_MINIMUM, OpenAPI_trace_depth_MEDIUM, OpenAPI_trace_depth_MAXIMUM, OpenAPI_trace_depth_MINIMUM_WO_VENDOR_EXTENSION, OpenAPI_trace_depth_MEDIUM_WO_VENDOR_EXTENSION, OpenAPI_trace_depth_MAXIMUM_WO_VENDOR_EXTENSION } OpenAPI_trace_depth_e;

char* OpenAPI_trace_depth_ToString(OpenAPI_trace_depth_e trace_depth);

OpenAPI_trace_depth_e OpenAPI_trace_depth_FromString(char* trace_depth);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_depth_H_ */

