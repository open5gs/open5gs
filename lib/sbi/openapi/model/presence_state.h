/*
 * presence_state.h
 *
 *
 */

#ifndef _OpenAPI_presence_state_H_
#define _OpenAPI_presence_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_presence_state_s OpenAPI_presence_state_t;
typedef struct OpenAPI_presence_state_s {
} OpenAPI_presence_state_t;

OpenAPI_presence_state_t *OpenAPI_presence_state_create(
    );
void OpenAPI_presence_state_free(OpenAPI_presence_state_t *presence_state);
OpenAPI_presence_state_t *OpenAPI_presence_state_parseFromJSON(cJSON *presence_stateJSON);
cJSON *OpenAPI_presence_state_convertToJSON(OpenAPI_presence_state_t *presence_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_presence_state_H_ */

