/*
 * matching_direction.h
 *
 * 
 */

#ifndef _OpenAPI_matching_direction_H_
#define _OpenAPI_matching_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_matching_direction_NULL = 0, OpenAPI_matching_direction_ASCENDING, OpenAPI_matching_direction_DESCENDING, OpenAPI_matching_direction_CROSSED } OpenAPI_matching_direction_e;

char* OpenAPI_matching_direction_ToString(OpenAPI_matching_direction_e matching_direction);

OpenAPI_matching_direction_e OpenAPI_matching_direction_FromString(char* matching_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_matching_direction_H_ */

