/*
 * rm_state.h
 *
 * Describes the registration management state of a UE
 */

#ifndef _OpenAPI_rm_state_H_
#define _OpenAPI_rm_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "rm_state_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_rm_state_s OpenAPI_rm_state_t;
typedef struct OpenAPI_rm_state_s {
} OpenAPI_rm_state_t;

OpenAPI_rm_state_t *OpenAPI_rm_state_create(
);
void OpenAPI_rm_state_free(OpenAPI_rm_state_t *rm_state);
OpenAPI_rm_state_t *OpenAPI_rm_state_parseFromJSON(cJSON *rm_stateJSON);
cJSON *OpenAPI_rm_state_convertToJSON(OpenAPI_rm_state_t *rm_state);
OpenAPI_rm_state_t *OpenAPI_rm_state_copy(OpenAPI_rm_state_t *dst, OpenAPI_rm_state_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rm_state_H_ */

