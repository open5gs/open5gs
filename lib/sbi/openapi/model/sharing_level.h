/*
 * sharing_level.h
 *
 * 
 */

#ifndef _OpenAPI_sharing_level_H_
#define _OpenAPI_sharing_level_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sharing_level_NULL = 0, OpenAPI_sharing_level_SHARED, OpenAPI_sharing_level_NON_SHARED } OpenAPI_sharing_level_e;

char* OpenAPI_sharing_level_ToString(OpenAPI_sharing_level_e sharing_level);

OpenAPI_sharing_level_e OpenAPI_sharing_level_FromString(char* sharing_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sharing_level_H_ */

