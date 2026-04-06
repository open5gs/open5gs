/*
 * final_unit_action.h
 *
 * 
 */

#ifndef _OpenAPI_final_unit_action_H_
#define _OpenAPI_final_unit_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_final_unit_action_NULL = 0, OpenAPI_final_unit_action_TERMINATE, OpenAPI_final_unit_action_REDIRECT, OpenAPI_final_unit_action_RESTRICT_ACCESS } OpenAPI_final_unit_action_e;

char* OpenAPI_final_unit_action_ToString(OpenAPI_final_unit_action_e final_unit_action);

OpenAPI_final_unit_action_e OpenAPI_final_unit_action_FromString(char* final_unit_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_final_unit_action_H_ */

