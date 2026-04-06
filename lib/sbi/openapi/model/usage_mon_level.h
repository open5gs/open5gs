/*
 * usage_mon_level.h
 *
 * Represents the usage monitoring level.
 */

#ifndef _OpenAPI_usage_mon_level_H_
#define _OpenAPI_usage_mon_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_usage_mon_level_NULL = 0, OpenAPI_usage_mon_level_SESSION_LEVEL, OpenAPI_usage_mon_level_SERVICE_LEVEL } OpenAPI_usage_mon_level_e;

char* OpenAPI_usage_mon_level_ToString(OpenAPI_usage_mon_level_e usage_mon_level);

OpenAPI_usage_mon_level_e OpenAPI_usage_mon_level_FromString(char* usage_mon_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_mon_level_H_ */

