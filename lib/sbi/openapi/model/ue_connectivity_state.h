/*
 * ue_connectivity_state.h
 *
 * Indicates the connectivity state of a UE.
 */

#ifndef _OpenAPI_ue_connectivity_state_H_
#define _OpenAPI_ue_connectivity_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "cm_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_connectivity_state_s OpenAPI_ue_connectivity_state_t;
typedef struct OpenAPI_ue_connectivity_state_s {
    OpenAPI_access_type_e access_type;
    struct OpenAPI_cm_state_s *connectivitystate;
} OpenAPI_ue_connectivity_state_t;

OpenAPI_ue_connectivity_state_t *OpenAPI_ue_connectivity_state_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_cm_state_t *connectivitystate
);
void OpenAPI_ue_connectivity_state_free(OpenAPI_ue_connectivity_state_t *ue_connectivity_state);
OpenAPI_ue_connectivity_state_t *OpenAPI_ue_connectivity_state_parseFromJSON(cJSON *ue_connectivity_stateJSON);
cJSON *OpenAPI_ue_connectivity_state_convertToJSON(OpenAPI_ue_connectivity_state_t *ue_connectivity_state);
OpenAPI_ue_connectivity_state_t *OpenAPI_ue_connectivity_state_copy(OpenAPI_ue_connectivity_state_t *dst, OpenAPI_ue_connectivity_state_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_connectivity_state_H_ */

