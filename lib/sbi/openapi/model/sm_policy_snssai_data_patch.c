
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_snssai_data_patch.h"

OpenAPI_sm_policy_snssai_data_patch_t *OpenAPI_sm_policy_snssai_data_patch_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t* sm_policy_dnn_data
)
{
    OpenAPI_sm_policy_snssai_data_patch_t *sm_policy_snssai_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_snssai_data_patch_t));
    ogs_assert(sm_policy_snssai_data_patch_local_var);

    sm_policy_snssai_data_patch_local_var->snssai = snssai;
    sm_policy_snssai_data_patch_local_var->sm_policy_dnn_data = sm_policy_dnn_data;

    return sm_policy_snssai_data_patch_local_var;
}

void OpenAPI_sm_policy_snssai_data_patch_free(OpenAPI_sm_policy_snssai_data_patch_t *sm_policy_snssai_data_patch)
{
    if (NULL == sm_policy_snssai_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(sm_policy_snssai_data_patch->snssai);
    OpenAPI_list_for_each(sm_policy_snssai_data_patch->sm_policy_dnn_data, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_sm_policy_dnn_data_patch_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_snssai_data_patch->sm_policy_dnn_data);
    ogs_free(sm_policy_snssai_data_patch);
}

cJSON *OpenAPI_sm_policy_snssai_data_patch_convertToJSON(OpenAPI_sm_policy_snssai_data_patch_t *sm_policy_snssai_data_patch)
{
    cJSON *item = NULL;

    if (sm_policy_snssai_data_patch == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_convertToJSON() failed [SmPolicySnssaiDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_policy_snssai_data_patch->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_convertToJSON() failed [snssai]");
        goto end;
    }

    if (sm_policy_snssai_data_patch->sm_policy_dnn_data) {
    cJSON *sm_policy_dnn_data = cJSON_AddObjectToObject(item, "smPolicyDnnData");
    if (sm_policy_dnn_data == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_convertToJSON() failed [sm_policy_dnn_data]");
        goto end;
    }
    cJSON *localMapObject = sm_policy_dnn_data;
    OpenAPI_lnode_t *sm_policy_dnn_data_node;
    if (sm_policy_snssai_data_patch->sm_policy_dnn_data) {
        OpenAPI_list_for_each(sm_policy_snssai_data_patch->sm_policy_dnn_data, sm_policy_dnn_data_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)sm_policy_dnn_data_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_sm_policy_dnn_data_patch_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_snssai_data_patch_convertToJSON() failed [sm_policy_dnn_data]");
            goto end;
        }
        cJSON_AddItemToObject(sm_policy_dnn_data, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_snssai_data_patch_t *OpenAPI_sm_policy_snssai_data_patch_parseFromJSON(cJSON *sm_policy_snssai_data_patchJSON)
{
    OpenAPI_sm_policy_snssai_data_patch_t *sm_policy_snssai_data_patch_local_var = NULL;
    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(sm_policy_snssai_data_patchJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_parseFromJSON() failed [snssai]");
        goto end;
    }

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);

    cJSON *sm_policy_dnn_data = cJSON_GetObjectItemCaseSensitive(sm_policy_snssai_data_patchJSON, "smPolicyDnnData");

    OpenAPI_list_t *sm_policy_dnn_dataList;
    if (sm_policy_dnn_data) {
    cJSON *sm_policy_dnn_data_local_map;
    if (!cJSON_IsObject(sm_policy_dnn_data)) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_parseFromJSON() failed [sm_policy_dnn_data]");
        goto end;
    }
    sm_policy_dnn_dataList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(sm_policy_dnn_data_local_map, sm_policy_dnn_data) {
        cJSON *localMapObject = sm_policy_dnn_data_local_map;
        if (cJSON_IsObject(sm_policy_dnn_data_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_sm_policy_dnn_data_patch_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(sm_policy_dnn_data_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_sm_policy_snssai_data_patch_parseFromJSON() failed [sm_policy_dnn_data]");
            goto end;
        }
        OpenAPI_list_add(sm_policy_dnn_dataList , localMapKeyPair);
    }
    }

    sm_policy_snssai_data_patch_local_var = OpenAPI_sm_policy_snssai_data_patch_create (
        snssai_local_nonprim,
        sm_policy_dnn_data ? sm_policy_dnn_dataList : NULL
    );

    return sm_policy_snssai_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_snssai_data_patch_t *OpenAPI_sm_policy_snssai_data_patch_copy(OpenAPI_sm_policy_snssai_data_patch_t *dst, OpenAPI_sm_policy_snssai_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_snssai_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_snssai_data_patch_convertToJSON() failed");
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

    OpenAPI_sm_policy_snssai_data_patch_free(dst);
    dst = OpenAPI_sm_policy_snssai_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

