
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_data_patch.h"

OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_create(
    bool is_um_data_null,
    OpenAPI_list_t* um_data,
    OpenAPI_list_t* sm_policy_snssai_data
)
{
    OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_data_patch_t));
    ogs_assert(sm_policy_data_patch_local_var);

    sm_policy_data_patch_local_var->is_um_data_null = is_um_data_null;
    sm_policy_data_patch_local_var->um_data = um_data;
    sm_policy_data_patch_local_var->sm_policy_snssai_data = sm_policy_snssai_data;

    return sm_policy_data_patch_local_var;
}

void OpenAPI_sm_policy_data_patch_free(OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_policy_data_patch) {
        return;
    }
    if (sm_policy_data_patch->um_data) {
        OpenAPI_list_for_each(sm_policy_data_patch->um_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_data_patch->um_data);
        sm_policy_data_patch->um_data = NULL;
    }
    if (sm_policy_data_patch->sm_policy_snssai_data) {
        OpenAPI_list_for_each(sm_policy_data_patch->sm_policy_snssai_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_sm_policy_snssai_data_patch_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_data_patch->sm_policy_snssai_data);
        sm_policy_data_patch->sm_policy_snssai_data = NULL;
    }
    ogs_free(sm_policy_data_patch);
}

cJSON *OpenAPI_sm_policy_data_patch_convertToJSON(OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    if (sm_policy_data_patch->um_data) {
        OpenAPI_list_for_each(sm_policy_data_patch->um_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [um_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [um_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_usage_mon_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_data_patch->is_um_data_null) {
        if (cJSON_AddNullToObject(item, "umData") == NULL) {
            ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [um_data]");
            goto end;
        }
    }

    if (sm_policy_data_patch->sm_policy_snssai_data) {
    cJSON *sm_policy_snssai_data = cJSON_AddObjectToObject(item, "smPolicySnssaiData");
    if (sm_policy_snssai_data == NULL) {
        ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [sm_policy_snssai_data]");
        goto end;
    }
    cJSON *localMapObject = sm_policy_snssai_data;
    if (sm_policy_data_patch->sm_policy_snssai_data) {
        OpenAPI_list_for_each(sm_policy_data_patch->sm_policy_snssai_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [sm_policy_snssai_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [sm_policy_snssai_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_sm_policy_snssai_data_patch_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_data_patch_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_data_patch_t *OpenAPI_sm_policy_data_patch_parseFromJSON(cJSON *sm_policy_data_patchJSON)
{
    OpenAPI_sm_policy_data_patch_t *sm_policy_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *um_data = NULL;
    OpenAPI_list_t *um_dataList = NULL;
    cJSON *sm_policy_snssai_data = NULL;
    OpenAPI_list_t *sm_policy_snssai_dataList = NULL;
    um_data = cJSON_GetObjectItemCaseSensitive(sm_policy_data_patchJSON, "umData");
    if (um_data) {
    if (!cJSON_IsNull(um_data)) {
        cJSON *um_data_local_map = NULL;
        if (!cJSON_IsObject(um_data) && !cJSON_IsNull(um_data)) {
            ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [um_data]");
            goto end;
        }
        if (cJSON_IsObject(um_data)) {
            um_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(um_data_local_map, um_data) {
                cJSON *localMapObject = um_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_usage_mon_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(um_dataList, localMapKeyPair);
            }
        }
    }
    }

    sm_policy_snssai_data = cJSON_GetObjectItemCaseSensitive(sm_policy_data_patchJSON, "smPolicySnssaiData");
    if (sm_policy_snssai_data) {
        cJSON *sm_policy_snssai_data_local_map = NULL;
        if (!cJSON_IsObject(sm_policy_snssai_data) && !cJSON_IsNull(sm_policy_snssai_data)) {
            ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [sm_policy_snssai_data]");
            goto end;
        }
        if (cJSON_IsObject(sm_policy_snssai_data)) {
            sm_policy_snssai_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(sm_policy_snssai_data_local_map, sm_policy_snssai_data) {
                cJSON *localMapObject = sm_policy_snssai_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_sm_policy_snssai_data_patch_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_data_patch_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(sm_policy_snssai_dataList, localMapKeyPair);
            }
        }
    }

    sm_policy_data_patch_local_var = OpenAPI_sm_policy_data_patch_create (
        um_data && cJSON_IsNull(um_data) ? true : false,
        um_data ? um_dataList : NULL,
        sm_policy_snssai_data ? sm_policy_snssai_dataList : NULL
    );

    return sm_policy_data_patch_local_var;
end:
    if (um_dataList) {
        OpenAPI_list_for_each(um_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(um_dataList);
        um_dataList = NULL;
    }
    if (sm_policy_snssai_dataList) {
        OpenAPI_list_for_each(sm_policy_snssai_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_sm_policy_snssai_data_patch_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_snssai_dataList);
        sm_policy_snssai_dataList = NULL;
    }
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

