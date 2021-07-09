/*
 * reachability_filter_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_reachability_filter_any_of_H_
#define _OpenAPI_reachability_filter_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reachability_filter_any_of_NULL = 0, OpenAPI_reachability_filter_any_of_UE_REACHABILITY_STATUS_CHANGE, OpenAPI_reachability_filter_any_of_UE_REACHABLE_DL_TRAFFIC } OpenAPI_reachability_filter_any_of_e;

char* OpenAPI_reachability_filter_any_of_ToString(OpenAPI_reachability_filter_any_of_e reachability_filter_any_of);

OpenAPI_reachability_filter_any_of_e OpenAPI_reachability_filter_any_of_FromString(char* reachability_filter_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reachability_filter_any_of_H_ */

