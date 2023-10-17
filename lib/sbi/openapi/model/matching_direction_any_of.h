/*
 * matching_direction_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_matching_direction_any_of_H_
#define _OpenAPI_matching_direction_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_matching_direction_any_of_NULL = 0, OpenAPI_matching_direction_any_of_ASCENDING, OpenAPI_matching_direction_any_of_DESCENDING, OpenAPI_matching_direction_any_of_CROSSED } OpenAPI_matching_direction_any_of_e;

char* OpenAPI_matching_direction_any_of_ToString(OpenAPI_matching_direction_any_of_e matching_direction_any_of);

OpenAPI_matching_direction_any_of_e OpenAPI_matching_direction_any_of_FromString(char* matching_direction_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_matching_direction_any_of_H_ */

