/*
 * ue_policy_set_patch.h
 *
 * Contains the UE policy set for a given subscriber.
 */

#ifndef _OpenAPI_ue_policy_set_patch_H_
#define _OpenAPI_ue_policy_set_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_policy_section.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_policy_set_patch_s OpenAPI_ue_policy_set_patch_t;
typedef struct OpenAPI_ue_policy_set_patch_s {
    OpenAPI_list_t* ue_policy_sections;
    OpenAPI_list_t *upsis;
    bool is_andsp_ind;
    int andsp_ind;
    char *pei;
    OpenAPI_list_t *os_ids;
} OpenAPI_ue_policy_set_patch_t;

OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_create(
    OpenAPI_list_t* ue_policy_sections,
    OpenAPI_list_t *upsis,
    bool is_andsp_ind,
    int andsp_ind,
    char *pei,
    OpenAPI_list_t *os_ids
);
void OpenAPI_ue_policy_set_patch_free(OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch);
OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_parseFromJSON(cJSON *ue_policy_set_patchJSON);
cJSON *OpenAPI_ue_policy_set_patch_convertToJSON(OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch);
OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_copy(OpenAPI_ue_policy_set_patch_t *dst, OpenAPI_ue_policy_set_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_policy_set_patch_H_ */

