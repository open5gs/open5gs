
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_dnn_data_patch.h"

OpenAPI_sm_policy_dnn_data_patch_t *OpenAPI_sm_policy_dnn_data_patch_create(
    char *dnn,
    OpenAPI_list_t* bdt_ref_ids
    )
{
    OpenAPI_sm_policy_dnn_data_patch_t *sm_policy_dnn_data_patch_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_dnn_data_patch_t));
    if (!sm_policy_dnn_data_patch_local_var) {
        return NULL;
    }
    sm_policy_dnn_data_patch_local_var->dnn = dnn;
    sm_policy_dnn_data_patch_local_var->bdt_ref_ids = bdt_ref_ids;

    return sm_policy_dnn_data_patch_local_var;
}

void OpenAPI_sm_policy_dnn_data_patch_free(OpenAPI_sm_policy_dnn_data_patch_t *sm_policy_dnn_data_patch)
{
    if (NULL == sm_policy_dnn_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_policy_dnn_data_patch->dnn);
    OpenAPI_list_for_each(sm_policy_dnn_data_patch->bdt_ref_ids, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_dnn_data_patch->bdt_ref_ids);
    ogs_free(sm_policy_dnn_data_patch);
}

cJSON *OpenAPI_sm_policy_dnn_data_patch_convertToJSON(OpenAPI_sm_policy_dnn_data_patch_t *sm_policy_dnn_data_patch)
{
    cJSON *item = NULL;

    if (sm_policy_dnn_data_patch == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_patch_convertToJSON() failed [SmPolicyDnnDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_policy_dnn_data_patch->dnn) {
        ogs_error("OpenAPI_sm_policy_dnn_data_patch_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", sm_policy_dnn_data_patch->dnn) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_patch_convertToJSON() failed [dnn]");
        goto end;
    }

    if (sm_policy_dnn_data_patch->bdt_ref_ids) {
        cJSON *bdt_ref_ids = cJSON_AddObjectToObject(item, "bdtRefIds");
        if (bdt_ref_ids == NULL) {
            ogs_error("OpenAPI_sm_policy_dnn_data_patch_convertToJSON() failed [bdt_ref_ids]");
            goto end;
        }
        cJSON *localMapObject = bdt_ref_ids;
        OpenAPI_lnode_t *bdt_ref_ids_node;
        if (sm_policy_dnn_data_patch->bdt_ref_ids) {
            OpenAPI_list_for_each(sm_policy_dnn_data_patch->bdt_ref_ids, bdt_ref_ids_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)bdt_ref_ids_node->data;
            }
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_dnn_data_patch_t *OpenAPI_sm_policy_dnn_data_patch_parseFromJSON(cJSON *sm_policy_dnn_data_patchJSON)
{
    OpenAPI_sm_policy_dnn_data_patch_t *sm_policy_dnn_data_patch_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_data_patchJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_policy_dnn_data_patch_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_patch_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *bdt_ref_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_data_patchJSON, "bdtRefIds");

    OpenAPI_list_t *bdt_ref_idsList;
    if (bdt_ref_ids) {
        cJSON *bdt_ref_ids_local_map;
        if (!cJSON_IsObject(bdt_ref_ids)) {
            ogs_error("OpenAPI_sm_policy_dnn_data_patch_parseFromJSON() failed [bdt_ref_ids]");
            goto end;
        }
        bdt_ref_idsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(bdt_ref_ids_local_map, bdt_ref_ids) {
            cJSON *localMapObject = bdt_ref_ids_local_map;
            OpenAPI_list_add(bdt_ref_idsList, localMapKeyPair);
        }
    }

    sm_policy_dnn_data_patch_local_var = OpenAPI_sm_policy_dnn_data_patch_create (
        ogs_strdup(dnn->valuestring),
        bdt_ref_ids ? bdt_ref_idsList : NULL
        );

    return sm_policy_dnn_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_dnn_data_patch_t *OpenAPI_sm_policy_dnn_data_patch_copy(OpenAPI_sm_policy_dnn_data_patch_t *dst, OpenAPI_sm_policy_dnn_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_dnn_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_dnn_data_patch_convertToJSON() failed");
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

    OpenAPI_sm_policy_dnn_data_patch_free(dst);
    dst = OpenAPI_sm_policy_dnn_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

