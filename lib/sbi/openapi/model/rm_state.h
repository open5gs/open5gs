/*
 * rm_state.h
 *
 * Describes the registration management state of a UE
 */

#ifndef _OpenAPI_rm_state_H_
#define _OpenAPI_rm_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rm_state_NULL = 0, OpenAPI_rm_state_REGISTERED, OpenAPI_rm_state_DEREGISTERED } OpenAPI_rm_state_e;

char* OpenAPI_rm_state_ToString(OpenAPI_rm_state_e rm_state);

OpenAPI_rm_state_e OpenAPI_rm_state_FromString(char* rm_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rm_state_H_ */

