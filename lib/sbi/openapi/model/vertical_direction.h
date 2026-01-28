/*
 * vertical_direction.h
 *
 * Indicates direction of vertical speed.
 */

#ifndef _OpenAPI_vertical_direction_H_
#define _OpenAPI_vertical_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_vertical_direction_NULL = 0, OpenAPI_vertical_direction_UPWARD, OpenAPI_vertical_direction_DOWNWARD } OpenAPI_vertical_direction_e;

char* OpenAPI_vertical_direction_ToString(OpenAPI_vertical_direction_e vertical_direction);

OpenAPI_vertical_direction_e OpenAPI_vertical_direction_FromString(char* vertical_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vertical_direction_H_ */

