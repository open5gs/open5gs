
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_data_patch.h"

OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_create(
    OpenAPI_list_t* um_data,
    OpenAPI_list_t* sm_policy_snssai_data
    )
{
    OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_data_patch_t));
    if (!sm_policy_data_patch_local_var) {
        return NULL;
    }
    sm_policy_data_patch_local_var->um_data = um_data;
    sm_policy_data_patch_local_var->sm_policy_snssai_data = sm_policy_snssai_data;

    return sm_policy_data_patch_local_var;
}

void OpenAPI_sm_policy_data_patch_free(OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch)
{
    if (NULL == sm_policy_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(sm_policy_data_patch->um_data, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_usage_mon_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_data_patch->um_data);
    OpenAPI_list_for_each(sm_policy_data_patch->sm_policy_snssai_data, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_sm_policy_snssai_data_patch_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_data_patch->sm_policy_snssai_data);
    ogs_free(sm_policy_data_patch);
}

cJSON *OpenAPI_sm_policy_data_patch_convertToJSON(OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch)
{
    cJSON *item = NULL;

    if (sm_policy_data_patch == NULL) {
        ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [SmPolicyDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_data_patch->um_data) {
        cJSON *um_data = cJSON_AddObjectToObject(item, "umData");
        if (um_data == NULL) {
            ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [um_data]");
            goto end;
        }
        cJSON *localMapObject = um_data;
        OpenAPI_lnode_t *um_data_node;
        if (sm_policy_data_patch->um_data) {
            OpenAPI_list_for_each(sm_policy_data_patch->um_data, um_data_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)um_data_node->data;
                cJSON *itemLocal = OpenAPI_usage_mon_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [um_data]");
                    goto end;
                }
                cJSON_AddItemToObject(um_data, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_data_patch->sm_policy_snssai_data) {
        cJSON *sm_policy_snssai_data = cJSON_AddObjectToObject(item, "smPolicySnssaiData");
        if (sm_policy_snssai_data == NULL) {
            ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [sm_policy_snssai_data]");
            goto end;
        }
        cJSON *localMapObject = sm_policy_snssai_data;
        OpenAPI_lnode_t *sm_policy_snssai_data_node;
        if (sm_policy_data_patch->sm_policy_snssai_data) {
            OpenAPI_list_for_each(sm_policy_data_patch->sm_policy_snssai_data, sm_policy_snssai_data_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)sm_policy_snssai_data_node->data;
                cJSON *itemLocal = OpenAPI_sm_policy_snssai_data_patch_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [sm_policy_snssai_data]");
                    goto end;
                }
                cJSON_AddItemToObject(sm_policy_snssai_data, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_parseFromJSON(cJSON *sm_policy_data_patchJSON)
{
    OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch_local_var = NULL;
    cJSON *um_data = cJSON_GetObjectItemCaseSensitive(sm_policy_data_patchJSON, "umData");

    OpenAPI_list_t *um_dataList;
    if (um_data) {
        cJSON *um_data_local_map;
        if (!cJSON_IsObject(um_data)) {
            ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [um_data]");
            goto end;
        }
        um_dataList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(um_data_local_map, um_data) {
            cJSON *localMapObject = um_data_local_map;
            if (!cJSON_IsObject(um_data_local_map)) {
                ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [um_data]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_usage_mon_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(um_dataList, localMapKeyPair);
        }
    }

    cJSON *sm_policy_snssai_data = cJSON_GetObjectItemCaseSensitive(sm_policy_data_patchJSON, "smPolicySnssaiData");

    OpenAPI_list_t *sm_policy_snssai_dataList;
    if (sm_policy_snssai_data) {
        cJSON *sm_policy_snssai_data_local_map;
        if (!cJSON_IsObject(sm_policy_snssai_data)) {
            ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [sm_policy_snssai_data]");
            goto end;
        }
        sm_policy_snssai_dataList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(sm_policy_snssai_data_local_map, sm_policy_snssai_data) {
            cJSON *localMapObject = sm_policy_snssai_data_local_map;
            if (!cJSON_IsObject(sm_policy_snssai_data_local_map)) {
                ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [sm_policy_snssai_data]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_sm_policy_snssai_data_patch_parseFromJSON(localMapObject));
            OpenAPI_list_add(sm_policy_snssai_dataList, localMapKeyPair);
        }
    }

    sm_policy_data_patch_local_var = OpenAPI_sm_policy_data_patch_create (
        um_data ? um_dataList : NULL,
        sm_policy_snssai_data ? sm_policy_snssai_dataList : NULL
        );

    return sm_policy_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_copy(OpenAPI_sm_policy_data_patch_t *dst, OpenAPI_sm_policy_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed");
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

    OpenAPI_sm_policy_data_patch_free(dst);
    dst = OpenAPI_sm_policy_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

