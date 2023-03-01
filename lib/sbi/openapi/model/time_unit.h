/*
 * time_unit.h
 *
 * Possible values are: - MINUTE: Time unit is per minute. - HOUR: Time unit is per hour. - DAY: Time unit is per day. 
 */

#ifndef _OpenAPI_time_unit_H_
#define _OpenAPI_time_unit_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "time_unit_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_time_unit_s OpenAPI_time_unit_t;
typedef struct OpenAPI_time_unit_s {
} OpenAPI_time_unit_t;

OpenAPI_time_unit_t *OpenAPI_time_unit_create(
);
void OpenAPI_time_unit_free(OpenAPI_time_unit_t *time_unit);
OpenAPI_time_unit_t *OpenAPI_time_unit_parseFromJSON(cJSON *time_unitJSON);
cJSON *OpenAPI_time_unit_convertToJSON(OpenAPI_time_unit_t *time_unit);
OpenAPI_time_unit_t *OpenAPI_time_unit_copy(OpenAPI_time_unit_t *dst, OpenAPI_time_unit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_unit_H_ */

