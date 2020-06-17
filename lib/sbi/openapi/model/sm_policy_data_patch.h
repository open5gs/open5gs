/*
 * sm_policy_data_patch.h
 *
 * Contains the SM policy data for a given subscriber.
 */

#ifndef _OpenAPI_sm_policy_data_patch_H_
#define _OpenAPI_sm_policy_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_policy_snssai_data_patch.h"
#include "usage_mon_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_data_patch_s OpenAPI_sm_policy_data_patch_t;
typedef struct OpenAPI_sm_policy_data_patch_s {
    OpenAPI_list_t* um_data;
    OpenAPI_list_t* sm_policy_snssai_data;
} OpenAPI_sm_policy_data_patch_t;

OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_create(
    OpenAPI_list_t* um_data,
    OpenAPI_list_t* sm_policy_snssai_data
    );
void OpenAPI_sm_policy_data_patch_free(OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch);
OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_parseFromJSON(cJSON *sm_policy_data_patchJSON);
cJSON *OpenAPI_sm_policy_data_patch_convertToJSON(OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch);
OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_copy(OpenAPI_sm_policy_data_patch_t *dst, OpenAPI_sm_policy_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_data_patch_H_ */

