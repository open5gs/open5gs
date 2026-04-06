/*
 * locality_type.h
 *
 * Type of locality description. An operator may define custom locality type values other  than those listed in this enumeration.  
 */

#ifndef _OpenAPI_locality_type_H_
#define _OpenAPI_locality_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_locality_type_NULL = 0, OpenAPI_locality_type_DATA_CENTER, OpenAPI_locality_type_CITY, OpenAPI_locality_type_COUNTY, OpenAPI_locality_type_DISTRICT, OpenAPI_locality_type_STATE, OpenAPI_locality_type_CANTON, OpenAPI_locality_type_REGION, OpenAPI_locality_type_PROVINCE, OpenAPI_locality_type_PREFECTURE, OpenAPI_locality_type_COUNTRY } OpenAPI_locality_type_e;

char* OpenAPI_locality_type_ToString(OpenAPI_locality_type_e locality_type);

OpenAPI_locality_type_e OpenAPI_locality_type_FromString(char* locality_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_locality_type_H_ */

