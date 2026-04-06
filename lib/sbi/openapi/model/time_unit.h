/*
 * time_unit.h
 *
 * 
 */

#ifndef _OpenAPI_time_unit_H_
#define _OpenAPI_time_unit_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_time_unit_NULL = 0, OpenAPI_time_unit_MINUTE, OpenAPI_time_unit_HOUR, OpenAPI_time_unit_DAY } OpenAPI_time_unit_e;

char* OpenAPI_time_unit_ToString(OpenAPI_time_unit_e time_unit);

OpenAPI_time_unit_e OpenAPI_time_unit_FromString(char* time_unit);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_unit_H_ */

