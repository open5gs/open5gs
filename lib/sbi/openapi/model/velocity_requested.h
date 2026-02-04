/*
 * velocity_requested.h
 *
 * Indicates velocity requirement.
 */

#ifndef _OpenAPI_velocity_requested_H_
#define _OpenAPI_velocity_requested_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "velocity_requested_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_velocity_requested_s OpenAPI_velocity_requested_t;
typedef struct OpenAPI_velocity_requested_s {
} OpenAPI_velocity_requested_t;

OpenAPI_velocity_requested_t *OpenAPI_velocity_requested_create(void);
void OpenAPI_velocity_requested_free(OpenAPI_velocity_requested_t *velocity_requested);
OpenAPI_velocity_requested_t *OpenAPI_velocity_requested_parseFromJSON(cJSON *velocity_requestedJSON);
cJSON *OpenAPI_velocity_requested_convertToJSON(OpenAPI_velocity_requested_t *velocity_requested);
OpenAPI_velocity_requested_t *OpenAPI_velocity_requested_copy(OpenAPI_velocity_requested_t *dst, OpenAPI_velocity_requested_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_velocity_requested_H_ */

