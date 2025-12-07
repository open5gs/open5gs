/*
 * positioning_mode_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_positioning_mode_any_of_H_
#define _OpenAPI_positioning_mode_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_positioning_mode_any_of_NULL = 0, OpenAPI_positioning_mode_any_of_UE_BASED, OpenAPI_positioning_mode_any_of_UE_ASSISTED, OpenAPI_positioning_mode_any_of_CONVENTIONAL } OpenAPI_positioning_mode_any_of_e;

char* OpenAPI_positioning_mode_any_of_ToString(OpenAPI_positioning_mode_any_of_e positioning_mode_any_of);

OpenAPI_positioning_mode_any_of_e OpenAPI_positioning_mode_any_of_FromString(char* positioning_mode_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_mode_any_of_H_ */

