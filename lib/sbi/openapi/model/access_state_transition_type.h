/*
 * access_state_transition_type.h
 *
 * Access State Transition Type.
 */

#ifndef _OpenAPI_access_state_transition_type_H_
#define _OpenAPI_access_state_transition_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_state_transition_type_NULL = 0, OpenAPI_access_state_transition_type_ACCESS_TYPE_CHANGE_3GPP, OpenAPI_access_state_transition_type_ACCESS_TYPE_CHANGE_N3GPP, OpenAPI_access_state_transition_type_RM_STATE_CHANGE_DEREGISTERED, OpenAPI_access_state_transition_type_RM_STATE_CHANGE_REGISTERED, OpenAPI_access_state_transition_type_CM_STATE_CHANGE_IDLE, OpenAPI_access_state_transition_type_CM_STATE_CHANGE_CONNECTED, OpenAPI_access_state_transition_type_HANDOVER, OpenAPI_access_state_transition_type_MOBILITY_REGISTRATION_UPDATE } OpenAPI_access_state_transition_type_e;

char* OpenAPI_access_state_transition_type_ToString(OpenAPI_access_state_transition_type_e access_state_transition_type);

OpenAPI_access_state_transition_type_e OpenAPI_access_state_transition_type_FromString(char* access_state_transition_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_state_transition_type_H_ */

