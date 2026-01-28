/*
 * positioning_mode.h
 *
 * Indicates supported modes used for positioning method.
 */

#ifndef _OpenAPI_positioning_mode_H_
#define _OpenAPI_positioning_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "positioning_mode_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_positioning_mode_s OpenAPI_positioning_mode_t;
typedef struct OpenAPI_positioning_mode_s {
} OpenAPI_positioning_mode_t;

OpenAPI_positioning_mode_t *OpenAPI_positioning_mode_create(void);
void OpenAPI_positioning_mode_free(OpenAPI_positioning_mode_t *positioning_mode);
OpenAPI_positioning_mode_t *OpenAPI_positioning_mode_parseFromJSON(cJSON *positioning_modeJSON);
cJSON *OpenAPI_positioning_mode_convertToJSON(OpenAPI_positioning_mode_t *positioning_mode);
OpenAPI_positioning_mode_t *OpenAPI_positioning_mode_copy(OpenAPI_positioning_mode_t *dst, OpenAPI_positioning_mode_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_mode_H_ */

