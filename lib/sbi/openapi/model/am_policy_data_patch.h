/*
 * am_policy_data_patch.h
 *
 * Contains the modifiable AM policy data for a given subscriber.
 */

#ifndef _OpenAPI_am_policy_data_patch_H_
#define _OpenAPI_am_policy_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_am_policy_data_patch_s OpenAPI_am_policy_data_patch_t;
#include "policy_counter_info_rm.h"
#include "restricted_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_am_policy_data_patch_s {
    OpenAPI_list_t *restri_status;
    OpenAPI_list_t* spend_lim_info;
};

OpenAPI_am_policy_data_patch_t *OpenAPI_am_policy_data_patch_create(
    OpenAPI_list_t *restri_status,
    OpenAPI_list_t* spend_lim_info
);
void OpenAPI_am_policy_data_patch_free(OpenAPI_am_policy_data_patch_t *am_policy_data_patch);
OpenAPI_am_policy_data_patch_t *OpenAPI_am_policy_data_patch_parseFromJSON(cJSON *am_policy_data_patchJSON);
cJSON *OpenAPI_am_policy_data_patch_convertToJSON(OpenAPI_am_policy_data_patch_t *am_policy_data_patch);
OpenAPI_am_policy_data_patch_t *OpenAPI_am_policy_data_patch_copy(OpenAPI_am_policy_data_patch_t *dst, OpenAPI_am_policy_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_policy_data_patch_H_ */

