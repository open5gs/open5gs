/*
 * up_cnx_state.h
 *
 * Possible values are - ACTIVATED - DEACTIVATED - ACTIVATING - SUSPENDED
 */

#ifndef _OpenAPI_up_cnx_state_H_
#define _OpenAPI_up_cnx_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_up_cnx_state_s OpenAPI_up_cnx_state_t;
typedef struct OpenAPI_up_cnx_state_s {
} OpenAPI_up_cnx_state_t;

OpenAPI_up_cnx_state_t *OpenAPI_up_cnx_state_create(
    );
void OpenAPI_up_cnx_state_free(OpenAPI_up_cnx_state_t *up_cnx_state);
OpenAPI_up_cnx_state_t *OpenAPI_up_cnx_state_parseFromJSON(cJSON *up_cnx_stateJSON);
cJSON *OpenAPI_up_cnx_state_convertToJSON(OpenAPI_up_cnx_state_t *up_cnx_state);
OpenAPI_up_cnx_state_t *OpenAPI_up_cnx_state_copy(OpenAPI_up_cnx_state_t *dst, OpenAPI_up_cnx_state_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_cnx_state_H_ */

