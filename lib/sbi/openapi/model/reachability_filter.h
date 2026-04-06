/*
 * reachability_filter.h
 *
 * Event filter for REACHABILITY_REPORT event type
 */

#ifndef _OpenAPI_reachability_filter_H_
#define _OpenAPI_reachability_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reachability_filter_NULL = 0, OpenAPI_reachability_filter_UE_REACHABILITY_STATUS_CHANGE, OpenAPI_reachability_filter_UE_REACHABLE_DL_TRAFFIC } OpenAPI_reachability_filter_e;

char* OpenAPI_reachability_filter_ToString(OpenAPI_reachability_filter_e reachability_filter);

OpenAPI_reachability_filter_e OpenAPI_reachability_filter_FromString(char* reachability_filter);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reachability_filter_H_ */

