/*
 * usage_mon_level_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_usage_mon_level_any_of_H_
#define _OpenAPI_usage_mon_level_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_usage_mon_level_any_of_NULL = 0, OpenAPI_usage_mon_level_any_of_SESSION_LEVEL, OpenAPI_usage_mon_level_any_of_SERVICE_LEVEL } OpenAPI_usage_mon_level_any_of_e;

char* OpenAPI_usage_mon_level_any_of_ToString(OpenAPI_usage_mon_level_any_of_e usage_mon_level_any_of);

OpenAPI_usage_mon_level_any_of_e OpenAPI_usage_mon_level_any_of_FromString(char* usage_mon_level_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_mon_level_any_of_H_ */

