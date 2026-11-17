/*
 * participant_action_type.h
 *
 * 
 */

#ifndef _OpenAPI_participant_action_type_H_
#define _OpenAPI_participant_action_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_participant_action_type_NULL = 0, OpenAPI_participant_action_type_CREATE, OpenAPI_participant_action_type_JOIN, OpenAPI_participant_action_type_INVITE_INTO, OpenAPI_participant_action_type_QUIT } OpenAPI_participant_action_type_e;

char* OpenAPI_participant_action_type_ToString(OpenAPI_participant_action_type_e participant_action_type);

OpenAPI_participant_action_type_e OpenAPI_participant_action_type_FromString(char* participant_action_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_participant_action_type_H_ */

