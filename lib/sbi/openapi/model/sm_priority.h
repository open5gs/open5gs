/*
 * sm_priority.h
 *
 * 
 */

#ifndef _OpenAPI_sm_priority_H_
#define _OpenAPI_sm_priority_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_priority_NULL = 0, OpenAPI_sm_priority_LOW, OpenAPI_sm_priority_NORMAL, OpenAPI_sm_priority_HIGH } OpenAPI_sm_priority_e;

char* OpenAPI_sm_priority_ToString(OpenAPI_sm_priority_e sm_priority);

OpenAPI_sm_priority_e OpenAPI_sm_priority_FromString(char* sm_priority);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_priority_H_ */

