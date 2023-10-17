/*
 * mps_action.h
 *
 * 
 */

#ifndef _OpenAPI_mps_action_H_
#define _OpenAPI_mps_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mps_action_NULL = 0, OpenAPI_mps_action_DISABLE_MPS_FOR_DTS, OpenAPI_mps_action_ENABLE_MPS_FOR_DTS, OpenAPI_mps_action_AUTHORIZE_AND_ENABLE_MPS_FOR_DTS } OpenAPI_mps_action_e;

char* OpenAPI_mps_action_ToString(OpenAPI_mps_action_e mps_action);

OpenAPI_mps_action_e OpenAPI_mps_action_FromString(char* mps_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mps_action_H_ */

