/*
 * cm_state.h
 *
 *
 */

#ifndef _OpenAPI_cm_state_H_
#define _OpenAPI_cm_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cm_state_s OpenAPI_cm_state_t;
typedef struct OpenAPI_cm_state_s {
} OpenAPI_cm_state_t;

OpenAPI_cm_state_t *OpenAPI_cm_state_create(
    );
void OpenAPI_cm_state_free(OpenAPI_cm_state_t *cm_state);
OpenAPI_cm_state_t *OpenAPI_cm_state_parseFromJSON(cJSON *cm_stateJSON);
cJSON *OpenAPI_cm_state_convertToJSON(OpenAPI_cm_state_t *cm_state);
OpenAPI_cm_state_t *OpenAPI_cm_state_copy(OpenAPI_cm_state_t *dst, OpenAPI_cm_state_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cm_state_H_ */

