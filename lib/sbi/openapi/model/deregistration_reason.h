/*
 * deregistration_reason.h
 *
 * 
 */

#ifndef _OpenAPI_deregistration_reason_H_
#define _OpenAPI_deregistration_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_deregistration_reason_NULL = 0, OpenAPI_deregistration_reason_UE_INITIAL_REGISTRATION, OpenAPI_deregistration_reason_UE_REGISTRATION_AREA_CHANGE, OpenAPI_deregistration_reason_SUBSCRIPTION_WITHDRAWN, OpenAPI_deregistration_reason__5GS_TO_EPS_MOBILITY, OpenAPI_deregistration_reason__5GS_TO_EPS_MOBILITY_UE_INITIAL_REGISTRATION, OpenAPI_deregistration_reason_REREGISTRATION_REQUIRED, OpenAPI_deregistration_reason_SMF_CONTEXT_TRANSFERRED } OpenAPI_deregistration_reason_e;

char* OpenAPI_deregistration_reason_ToString(OpenAPI_deregistration_reason_e deregistration_reason);

OpenAPI_deregistration_reason_e OpenAPI_deregistration_reason_FromString(char* deregistration_reason);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_deregistration_reason_H_ */

