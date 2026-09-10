/*
 * equipment_status.h
 *
 * Represents equipment status of the PEI. This data type is a string.
 */

#ifndef _OpenAPI_equipment_status_H_
#define _OpenAPI_equipment_status_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_equipment_status_NULL = 0, OpenAPI_equipment_status_WHITELISTED, OpenAPI_equipment_status_BLACKLISTED, OpenAPI_equipment_status_GREYLISTED } OpenAPI_equipment_status_e;

char* OpenAPI_equipment_status_ToString(OpenAPI_equipment_status_e equipment_status);

OpenAPI_equipment_status_e OpenAPI_equipment_status_FromString(char* equipment_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_equipment_status_H_ */

