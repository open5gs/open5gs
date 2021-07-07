/*
 * priority_sharing_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_priority_sharing_indicator_H_
#define _OpenAPI_priority_sharing_indicator_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_priority_sharing_indicator_NULL = 0, OpenAPI_priority_sharing_indicator_ENABLED, OpenAPI_priority_sharing_indicator_DISABLED } OpenAPI_priority_sharing_indicator_e;

char* OpenAPI_priority_sharing_indicator_ToString(OpenAPI_priority_sharing_indicator_e priority_sharing_indicator);

OpenAPI_priority_sharing_indicator_e OpenAPI_priority_sharing_indicator_FromString(char* priority_sharing_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_priority_sharing_indicator_H_ */

