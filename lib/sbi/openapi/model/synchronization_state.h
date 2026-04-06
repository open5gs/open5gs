/*
 * synchronization_state.h
 *
 * Indicates the Synchronization State.
 */

#ifndef _OpenAPI_synchronization_state_H_
#define _OpenAPI_synchronization_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_synchronization_state_NULL = 0, OpenAPI_synchronization_state_LOCKED, OpenAPI_synchronization_state_HOLDOVER, OpenAPI_synchronization_state_FREERUN } OpenAPI_synchronization_state_e;

char* OpenAPI_synchronization_state_ToString(OpenAPI_synchronization_state_e synchronization_state);

OpenAPI_synchronization_state_e OpenAPI_synchronization_state_FromString(char* synchronization_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_synchronization_state_H_ */

