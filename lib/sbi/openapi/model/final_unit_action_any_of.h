/*
 * final_unit_action_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_final_unit_action_any_of_H_
#define _OpenAPI_final_unit_action_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_final_unit_action_any_of_NULL = 0, OpenAPI_final_unit_action_any_of_TERMINATE, OpenAPI_final_unit_action_any_of_REDIRECT, OpenAPI_final_unit_action_any_of_RESTRICT_ACCESS } OpenAPI_final_unit_action_any_of_e;

char* OpenAPI_final_unit_action_any_of_ToString(OpenAPI_final_unit_action_any_of_e final_unit_action_any_of);

OpenAPI_final_unit_action_any_of_e OpenAPI_final_unit_action_any_of_FromString(char* final_unit_action_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_final_unit_action_any_of_H_ */

