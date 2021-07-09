/*
 * cm_state_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_cm_state_any_of_H_
#define _OpenAPI_cm_state_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_cm_state_any_of_NULL = 0, OpenAPI_cm_state_any_of_IDLE, OpenAPI_cm_state_any_of_CONNECTED } OpenAPI_cm_state_any_of_e;

char* OpenAPI_cm_state_any_of_ToString(OpenAPI_cm_state_any_of_e cm_state_any_of);

OpenAPI_cm_state_any_of_e OpenAPI_cm_state_any_of_FromString(char* cm_state_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cm_state_any_of_H_ */

