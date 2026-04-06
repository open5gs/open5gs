/*
 * traffic_direction.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_direction_H_
#define _OpenAPI_traffic_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_traffic_direction_NULL = 0, OpenAPI_traffic_direction_UL_AND_DL, OpenAPI_traffic_direction_UL, OpenAPI_traffic_direction_DL } OpenAPI_traffic_direction_e;

char* OpenAPI_traffic_direction_ToString(OpenAPI_traffic_direction_e traffic_direction);

OpenAPI_traffic_direction_e OpenAPI_traffic_direction_FromString(char* traffic_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_direction_H_ */

