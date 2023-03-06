/*
 * congestion_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_congestion_type_any_of_H_
#define _OpenAPI_congestion_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_congestion_type_any_of_NULL = 0, OpenAPI_congestion_type_any_of_USER_PLANE, OpenAPI_congestion_type_any_of_CONTROL_PLANE, OpenAPI_congestion_type_any_of_USER_AND_CONTROL_PLANE } OpenAPI_congestion_type_any_of_e;

char* OpenAPI_congestion_type_any_of_ToString(OpenAPI_congestion_type_any_of_e congestion_type_any_of);

OpenAPI_congestion_type_any_of_e OpenAPI_congestion_type_any_of_FromString(char* congestion_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_congestion_type_any_of_H_ */

