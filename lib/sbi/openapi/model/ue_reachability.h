/*
 * ue_reachability.h
 *
 * Describes the reachability of the UE
 */

#ifndef _OpenAPI_ue_reachability_H_
#define _OpenAPI_ue_reachability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_reachability_NULL = 0, OpenAPI_ue_reachability_UNREACHABLE, OpenAPI_ue_reachability_REACHABLE, OpenAPI_ue_reachability_REGULATORY_ONLY } OpenAPI_ue_reachability_e;

char* OpenAPI_ue_reachability_ToString(OpenAPI_ue_reachability_e ue_reachability);

OpenAPI_ue_reachability_e OpenAPI_ue_reachability_FromString(char* ue_reachability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_reachability_H_ */

