/*
 * rm_state_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_rm_state_any_of_H_
#define _OpenAPI_rm_state_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rm_state_any_of_NULL = 0, OpenAPI_rm_state_any_of_REGISTERED, OpenAPI_rm_state_any_of_DEREGISTERED } OpenAPI_rm_state_any_of_e;

char* OpenAPI_rm_state_any_of_ToString(OpenAPI_rm_state_any_of_e rm_state_any_of);

OpenAPI_rm_state_any_of_e OpenAPI_rm_state_any_of_FromString(char* rm_state_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rm_state_any_of_H_ */

