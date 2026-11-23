/*
 * redundant_transmission_type.h
 *
 * 
 */

#ifndef _OpenAPI_redundant_transmission_type_H_
#define _OpenAPI_redundant_transmission_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_redundant_transmission_type_NULL = 0, OpenAPI_redundant_transmission_type_NON_TRANSMISSION, OpenAPI_redundant_transmission_type_END_TO_END_USER_PLANE_PATHS, OpenAPI_redundant_transmission_type_N3_N9, OpenAPI_redundant_transmission_type_TRANSPORT_LAYER } OpenAPI_redundant_transmission_type_e;

char* OpenAPI_redundant_transmission_type_ToString(OpenAPI_redundant_transmission_type_e redundant_transmission_type);

OpenAPI_redundant_transmission_type_e OpenAPI_redundant_transmission_type_FromString(char* redundant_transmission_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redundant_transmission_type_H_ */

