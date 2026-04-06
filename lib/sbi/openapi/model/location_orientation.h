/*
 * location_orientation.h
 *
 * 
 */

#ifndef _OpenAPI_location_orientation_H_
#define _OpenAPI_location_orientation_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_location_orientation_NULL = 0, OpenAPI_location_orientation_HORIZONTAL, OpenAPI_location_orientation_VERTICAL, OpenAPI_location_orientation_HOR_AND_VER } OpenAPI_location_orientation_e;

char* OpenAPI_location_orientation_ToString(OpenAPI_location_orientation_e location_orientation);

OpenAPI_location_orientation_e OpenAPI_location_orientation_FromString(char* location_orientation);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_orientation_H_ */

