/*
 * cm_state.h
 *
 * Describes the connection management state of a UE
 */

#ifndef _OpenAPI_cm_state_H_
#define _OpenAPI_cm_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_cm_state_NULL = 0, OpenAPI_cm_state_IDLE, OpenAPI_cm_state_CONNECTED } OpenAPI_cm_state_e;

char* OpenAPI_cm_state_ToString(OpenAPI_cm_state_e cm_state);

OpenAPI_cm_state_e OpenAPI_cm_state_FromString(char* cm_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cm_state_H_ */

