/*
 * app_session_context_update_data_patch.h
 *
 * Identifies the modifications to an Individual Application Session Context and/or the modifications to the sub-resource Events Subscription.
 */

#ifndef _OpenAPI_app_session_context_update_data_patch_H_
#define _OpenAPI_app_session_context_update_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_session_context_update_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_session_context_update_data_patch_s OpenAPI_app_session_context_update_data_patch_t;
typedef struct OpenAPI_app_session_context_update_data_patch_s {
    struct OpenAPI_app_session_context_update_data_s *asc_req_data;
} OpenAPI_app_session_context_update_data_patch_t;

OpenAPI_app_session_context_update_data_patch_t *OpenAPI_app_session_context_update_data_patch_create(
    OpenAPI_app_session_context_update_data_t *asc_req_data
);
void OpenAPI_app_session_context_update_data_patch_free(OpenAPI_app_session_context_update_data_patch_t *app_session_context_update_data_patch);
OpenAPI_app_session_context_update_data_patch_t *OpenAPI_app_session_context_update_data_patch_parseFromJSON(cJSON *app_session_context_update_data_patchJSON);
cJSON *OpenAPI_app_session_context_update_data_patch_convertToJSON(OpenAPI_app_session_context_update_data_patch_t *app_session_context_update_data_patch);
OpenAPI_app_session_context_update_data_patch_t *OpenAPI_app_session_context_update_data_patch_copy(OpenAPI_app_session_context_update_data_patch_t *dst, OpenAPI_app_session_context_update_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_session_context_update_data_patch_H_ */

