/*
 * rm_state.h
 *
 *
 */

#ifndef _OpenAPI_rm_state_H_
#define _OpenAPI_rm_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

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

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rm_state_H_ */

