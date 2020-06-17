
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_policy_data_patch.h"

OpenAPI_bdt_policy_data_patch_t *OpenAPI_bdt_policy_data_patch_create(
    char *bdt_ref_id
    )
{
    OpenAPI_bdt_policy_data_patch_t *bdt_policy_data_patch_local_var = OpenAPI_malloc(sizeof(OpenAPI_bdt_policy_data_patch_t));
    if (!bdt_policy_data_patch_local_var) {
        return NULL;
    }
    bdt_policy_data_patch_local_var->bdt_ref_id = bdt_ref_id;

    return bdt_policy_data_patch_local_var;
}

void OpenAPI_bdt_policy_data_patch_free(OpenAPI_bdt_policy_data_patch_t *bdt_policy_data_patch)
{
    if (NULL == bdt_policy_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(bdt_policy_data_patch->bdt_ref_id);
    ogs_free(bdt_policy_data_patch);
}

cJSON *OpenAPI_bdt_policy_data_patch_convertToJSON(OpenAPI_bdt_policy_data_patch_t *bdt_policy_data_patch)
{
    cJSON *item = NULL;

    if (bdt_policy_data_patch == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_patch_convertToJSON() failed [BdtPolicyDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!bdt_policy_data_patch->bdt_ref_id) {
        ogs_error("OpenAPI_bdt_policy_data_patch_convertToJSON() failed [bdt_ref_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "bdtRefId", bdt_policy_data_patch->bdt_ref_id) == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_patch_convertToJSON() failed [bdt_ref_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_bdt_policy_data_patch_t *OpenAPI_bdt_policy_data_patch_parseFromJSON(cJSON *bdt_policy_data_patchJSON)
{
    OpenAPI_bdt_policy_data_patch_t *bdt_policy_data_patch_local_var = NULL;
    cJSON *bdt_ref_id = cJSON_GetObjectItemCaseSensitive(bdt_policy_data_patchJSON, "bdtRefId");
    if (!bdt_ref_id) {
        ogs_error("OpenAPI_bdt_policy_data_patch_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }


    if (!cJSON_IsString(bdt_ref_id)) {
        ogs_error("OpenAPI_bdt_policy_data_patch_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }

    bdt_policy_data_patch_local_var = OpenAPI_bdt_policy_data_patch_create (
        ogs_strdup(bdt_ref_id->valuestring)
        );

    return bdt_policy_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_bdt_policy_data_patch_t *OpenAPI_bdt_policy_data_patch_copy(OpenAPI_bdt_policy_data_patch_t *dst, OpenAPI_bdt_policy_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bdt_policy_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bdt_policy_data_patch_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_bdt_policy_data_patch_free(dst);
    dst = OpenAPI_bdt_policy_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

