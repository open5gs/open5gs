/*
 * policy_req_trigger.h
 *
 * 
 */

#ifndef _OpenAPI_policy_req_trigger_H_
#define _OpenAPI_policy_req_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_policy_req_trigger_NULL = 0, OpenAPI_policy_req_trigger_LOCATION_CHANGE, OpenAPI_policy_req_trigger_PRA_CHANGE, OpenAPI_policy_req_trigger_ALLOWED_NSSAI_CHANGE, OpenAPI_policy_req_trigger_NWDAF_DATA_CHANGE, OpenAPI_policy_req_trigger_PLMN_CHANGE, OpenAPI_policy_req_trigger_CON_STATE_CHANGE, OpenAPI_policy_req_trigger_SMF_SELECT_CHANGE, OpenAPI_policy_req_trigger_ACCESS_TYPE_CHANGE } OpenAPI_policy_req_trigger_e;

char* OpenAPI_policy_req_trigger_ToString(OpenAPI_policy_req_trigger_e policy_req_trigger);

OpenAPI_policy_req_trigger_e OpenAPI_policy_req_trigger_FromString(char* policy_req_trigger);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_req_trigger_H_ */

