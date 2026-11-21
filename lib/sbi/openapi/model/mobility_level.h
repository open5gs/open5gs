/*
 * mobility_level.h
 *
 * 
 */

#ifndef _OpenAPI_mobility_level_H_
#define _OpenAPI_mobility_level_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mobility_level_NULL = 0, OpenAPI_mobility_level_STATIONARY, OpenAPI_mobility_level_NOMADIC, OpenAPI_mobility_level_RESTRICTED_MOBILITY, OpenAPI_mobility_level_FULL_MOBILITY } OpenAPI_mobility_level_e;

char* OpenAPI_mobility_level_ToString(OpenAPI_mobility_level_e mobility_level);

OpenAPI_mobility_level_e OpenAPI_mobility_level_FromString(char* mobility_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mobility_level_H_ */

