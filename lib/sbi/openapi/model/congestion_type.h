/*
 * congestion_type.h
 *
 * 
 */

#ifndef _OpenAPI_congestion_type_H_
#define _OpenAPI_congestion_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_congestion_type_NULL = 0, OpenAPI_congestion_type_USER_PLANE, OpenAPI_congestion_type_CONTROL_PLANE, OpenAPI_congestion_type_USER_AND_CONTROL_PLANE } OpenAPI_congestion_type_e;

char* OpenAPI_congestion_type_ToString(OpenAPI_congestion_type_e congestion_type);

OpenAPI_congestion_type_e OpenAPI_congestion_type_FromString(char* congestion_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_congestion_type_H_ */

