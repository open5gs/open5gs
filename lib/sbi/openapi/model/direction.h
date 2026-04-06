/*
 * direction.h
 *
 * 
 */

#ifndef _OpenAPI_direction_H_
#define _OpenAPI_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_direction_NULL = 0, OpenAPI_direction_NORTH, OpenAPI_direction_SOUTH, OpenAPI_direction_EAST, OpenAPI_direction_WEST, OpenAPI_direction_NORTHWEST, OpenAPI_direction_NORTHEAST, OpenAPI_direction_SOUTHWEST, OpenAPI_direction_SOUTHEAST } OpenAPI_direction_e;

char* OpenAPI_direction_ToString(OpenAPI_direction_e direction);

OpenAPI_direction_e OpenAPI_direction_FromString(char* direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_direction_H_ */

