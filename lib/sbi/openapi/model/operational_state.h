/*
 * operational_state.h
 *
 * 
 */

#ifndef _OpenAPI_operational_state_H_
#define _OpenAPI_operational_state_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_operational_state_NULL = 0, OpenAPI_operational_state_ENABLED, OpenAPI_operational_state_DISABLED } OpenAPI_operational_state_e;

char* OpenAPI_operational_state_ToString(OpenAPI_operational_state_e operational_state);

OpenAPI_operational_state_e OpenAPI_operational_state_FromString(char* operational_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_operational_state_H_ */

