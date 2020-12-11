/*
 * final_unit_action.h
 *
 *
 */

#ifndef _OpenAPI_final_unit_action_H_
#define _OpenAPI_final_unit_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_final_unit_action_s OpenAPI_final_unit_action_t;
typedef struct OpenAPI_final_unit_action_s {
} OpenAPI_final_unit_action_t;

OpenAPI_final_unit_action_t *OpenAPI_final_unit_action_create(
    );
void OpenAPI_final_unit_action_free(OpenAPI_final_unit_action_t *final_unit_action);
OpenAPI_final_unit_action_t *OpenAPI_final_unit_action_parseFromJSON(cJSON *final_unit_actionJSON);
cJSON *OpenAPI_final_unit_action_convertToJSON(OpenAPI_final_unit_action_t *final_unit_action);
OpenAPI_final_unit_action_t *OpenAPI_final_unit_action_copy(OpenAPI_final_unit_action_t *dst, OpenAPI_final_unit_action_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_final_unit_action_H_ */

