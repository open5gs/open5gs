/*
 * time_unit_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_time_unit_any_of_H_
#define _OpenAPI_time_unit_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_time_unit_any_of_NULL = 0, OpenAPI_time_unit_any_of_MINUTE, OpenAPI_time_unit_any_of_HOUR, OpenAPI_time_unit_any_of_DAY } OpenAPI_time_unit_any_of_e;

char* OpenAPI_time_unit_any_of_ToString(OpenAPI_time_unit_any_of_e time_unit_any_of);

OpenAPI_time_unit_any_of_e OpenAPI_time_unit_any_of_FromString(char* time_unit_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_unit_any_of_H_ */

