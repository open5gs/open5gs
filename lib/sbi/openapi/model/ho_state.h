/*
 * ho_state.h
 *
 * Possible values are - NONE - PREPARING - PREPARED - COMPLETED - CANCELLED
 */

#ifndef _OpenAPI_ho_state_H_
#define _OpenAPI_ho_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ho_state_s OpenAPI_ho_state_t;
typedef struct OpenAPI_ho_state_s {
} OpenAPI_ho_state_t;

OpenAPI_ho_state_t *OpenAPI_ho_state_create(
    );
void OpenAPI_ho_state_free(OpenAPI_ho_state_t *ho_state);
OpenAPI_ho_state_t *OpenAPI_ho_state_parseFromJSON(cJSON *ho_stateJSON);
cJSON *OpenAPI_ho_state_convertToJSON(OpenAPI_ho_state_t *ho_state);
OpenAPI_ho_state_t *OpenAPI_ho_state_copy(OpenAPI_ho_state_t *dst, OpenAPI_ho_state_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ho_state_H_ */

