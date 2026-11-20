/*
 * administrative_state.h
 *
 * 
 */

#ifndef _OpenAPI_administrative_state_H_
#define _OpenAPI_administrative_state_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_administrative_state_NULL = 0, OpenAPI_administrative_state_LOCKED, OpenAPI_administrative_state_UNLOCKED } OpenAPI_administrative_state_e;

char* OpenAPI_administrative_state_ToString(OpenAPI_administrative_state_e administrative_state);

OpenAPI_administrative_state_e OpenAPI_administrative_state_FromString(char* administrative_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_administrative_state_H_ */

