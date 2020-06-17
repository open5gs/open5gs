/*
 * bdt_policy_data_patch.h
 *
 *
 */

#ifndef _OpenAPI_bdt_policy_data_patch_H_
#define _OpenAPI_bdt_policy_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bdt_policy_data_patch_s OpenAPI_bdt_policy_data_patch_t;
typedef struct OpenAPI_bdt_policy_data_patch_s {
    char *bdt_ref_id;
} OpenAPI_bdt_policy_data_patch_t;

OpenAPI_bdt_policy_data_patch_t *OpenAPI_bdt_policy_data_patch_create(
    char *bdt_ref_id
    );
void OpenAPI_bdt_policy_data_patch_free(OpenAPI_bdt_policy_data_patch_t *bdt_policy_data_patch);
OpenAPI_bdt_policy_data_patch_t *OpenAPI_bdt_policy_data_patch_parseFromJSON(cJSON *bdt_policy_data_patchJSON);
cJSON *OpenAPI_bdt_policy_data_patch_convertToJSON(OpenAPI_bdt_policy_data_patch_t *bdt_policy_data_patch);
OpenAPI_bdt_policy_data_patch_t *OpenAPI_bdt_policy_data_patch_copy(OpenAPI_bdt_policy_data_patch_t *dst, OpenAPI_bdt_policy_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_policy_data_patch_H_ */

