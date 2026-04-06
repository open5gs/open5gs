
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_policy_data_patch.h"

OpenAPI_am_policy_data_patch_t *OpenAPI_am_policy_data_patch_create(
    OpenAPI_list_t *restri_status,
    OpenAPI_list_t* spend_lim_info
)
{
    OpenAPI_am_policy_data_patch_t *am_policy_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_am_policy_data_patch_t));
    ogs_assert(am_policy_data_patch_local_var);

    am_policy_data_patch_local_var->restri_status = restri_status;
    am_policy_data_patch_local_var->spend_lim_info = spend_lim_info;

    return am_policy_data_patch_local_var;
}

void OpenAPI_am_policy_data_patch_free(OpenAPI_am_policy_data_patch_t *am_policy_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_policy_data_patch) {
        return;
    }
    if (am_policy_data_patch->restri_status) {
        OpenAPI_list_for_each(am_policy_data_patch->restri_status, node) {
            OpenAPI_restricted_status_free(node->data);
        }
        OpenAPI_list_free(am_policy_data_patch->restri_status);
        am_policy_data_patch->restri_status = NULL;
    }
    if (am_policy_data_patch->spend_lim_info) {
        OpenAPI_list_for_each(am_policy_data_patch->spend_lim_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_policy_counter_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(am_policy_data_patch->spend_lim_info);
        am_policy_data_patch->spend_lim_info = NULL;
    }
    ogs_free(am_policy_data_patch);
}

cJSON *OpenAPI_am_policy_data_patch_convertToJSON(OpenAPI_am_policy_data_patch_t *am_policy_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_policy_data_patch == NULL) {
        ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [AmPolicyDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (am_policy_data_patch->restri_status) {
    cJSON *restri_statusList = cJSON_AddArrayToObject(item, "restriStatus");
    if (restri_statusList == NULL) {
        ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [restri_status]");
        goto end;
    }
    OpenAPI_list_for_each(am_policy_data_patch->restri_status, node) {
        cJSON *itemLocal = OpenAPI_restricted_status_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [restri_status]");
            goto end;
        }
        cJSON_AddItemToArray(restri_statusList, itemLocal);
    }
    }

    if (am_policy_data_patch->spend_lim_info) {
    cJSON *spend_lim_info = cJSON_AddObjectToObject(item, "spendLimInfo");
    if (spend_lim_info == NULL) {
        ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [spend_lim_info]");
        goto end;
    }
    cJSON *localMapObject = spend_lim_info;
    if (am_policy_data_patch->spend_lim_info) {
        OpenAPI_list_for_each(am_policy_data_patch->spend_lim_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [spend_lim_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [spend_lim_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_policy_counter_info_rm_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_am_policy_data_patch_t *OpenAPI_am_policy_data_patch_parseFromJSON(cJSON *am_policy_data_patchJSON)
{
    OpenAPI_am_policy_data_patch_t *am_policy_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *restri_status = NULL;
    OpenAPI_list_t *restri_statusList = NULL;
    cJSON *spend_lim_info = NULL;
    OpenAPI_list_t *spend_lim_infoList = NULL;
    restri_status = cJSON_GetObjectItemCaseSensitive(am_policy_data_patchJSON, "restriStatus");
    if (restri_status) {
        cJSON *restri_status_local = NULL;
        if (!cJSON_IsArray(restri_status)) {
            ogs_error("OpenAPI_am_policy_data_patch_parseFromJSON() failed [restri_status]");
            goto end;
        }

        restri_statusList = OpenAPI_list_create();

        cJSON_ArrayForEach(restri_status_local, restri_status) {
            if (!cJSON_IsObject(restri_status_local)) {
                ogs_error("OpenAPI_am_policy_data_patch_parseFromJSON() failed [restri_status]");
                goto end;
            }
            OpenAPI_restricted_status_t *restri_statusItem = OpenAPI_restricted_status_parseFromJSON(restri_status_local);
            if (!restri_statusItem) {
                ogs_error("No restri_statusItem");
                goto end;
            }
            OpenAPI_list_add(restri_statusList, restri_statusItem);
        }
    }

    spend_lim_info = cJSON_GetObjectItemCaseSensitive(am_policy_data_patchJSON, "spendLimInfo");
    if (spend_lim_info) {
        cJSON *spend_lim_info_local_map = NULL;
        if (!cJSON_IsObject(spend_lim_info) && !cJSON_IsNull(spend_lim_info)) {
            ogs_error("OpenAPI_am_policy_data_patch_parseFromJSON() failed [spend_lim_info]");
            goto end;
        }
        if (cJSON_IsObject(spend_lim_info)) {
            spend_lim_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(spend_lim_info_local_map, spend_lim_info) {
                cJSON *localMapObject = spend_lim_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_policy_counter_info_rm_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_am_policy_data_patch_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(spend_lim_infoList, localMapKeyPair);
            }
        }
    }

    am_policy_data_patch_local_var = OpenAPI_am_policy_data_patch_create (
        restri_status ? restri_statusList : NULL,
        spend_lim_info ? spend_lim_infoList : NULL
    );

    return am_policy_data_patch_local_var;
end:
    if (restri_statusList) {
        OpenAPI_list_for_each(restri_statusList, node) {
            OpenAPI_restricted_status_free(node->data);
        }
        OpenAPI_list_free(restri_statusList);
        restri_statusList = NULL;
    }
    if (spend_lim_infoList) {
        OpenAPI_list_for_each(spend_lim_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_policy_counter_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(spend_lim_infoList);
        spend_lim_infoList = NULL;
    }
    return NULL;
}

OpenAPI_am_policy_data_patch_t *OpenAPI_am_policy_data_patch_copy(OpenAPI_am_policy_data_patch_t *dst, OpenAPI_am_policy_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_policy_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_policy_data_patch_convertToJSON() failed");
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

    OpenAPI_am_policy_data_patch_free(dst);
    dst = OpenAPI_am_policy_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

