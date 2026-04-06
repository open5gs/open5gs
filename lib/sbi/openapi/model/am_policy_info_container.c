
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_policy_info_container.h"

OpenAPI_am_policy_info_container_t *OpenAPI_am_policy_info_container_create(
    OpenAPI_list_t* slice_usg_ctrl_info_sets
)
{
    OpenAPI_am_policy_info_container_t *am_policy_info_container_local_var = ogs_malloc(sizeof(OpenAPI_am_policy_info_container_t));
    ogs_assert(am_policy_info_container_local_var);

    am_policy_info_container_local_var->slice_usg_ctrl_info_sets = slice_usg_ctrl_info_sets;

    return am_policy_info_container_local_var;
}

void OpenAPI_am_policy_info_container_free(OpenAPI_am_policy_info_container_t *am_policy_info_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_policy_info_container) {
        return;
    }
    if (am_policy_info_container->slice_usg_ctrl_info_sets) {
        OpenAPI_list_for_each(am_policy_info_container->slice_usg_ctrl_info_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_usage_control_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(am_policy_info_container->slice_usg_ctrl_info_sets);
        am_policy_info_container->slice_usg_ctrl_info_sets = NULL;
    }
    ogs_free(am_policy_info_container);
}

cJSON *OpenAPI_am_policy_info_container_convertToJSON(OpenAPI_am_policy_info_container_t *am_policy_info_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_policy_info_container == NULL) {
        ogs_error("OpenAPI_am_policy_info_container_convertToJSON() failed [AmPolicyInfoContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (am_policy_info_container->slice_usg_ctrl_info_sets) {
    cJSON *slice_usg_ctrl_info_sets = cJSON_AddObjectToObject(item, "sliceUsgCtrlInfoSets");
    if (slice_usg_ctrl_info_sets == NULL) {
        ogs_error("OpenAPI_am_policy_info_container_convertToJSON() failed [slice_usg_ctrl_info_sets]");
        goto end;
    }
    cJSON *localMapObject = slice_usg_ctrl_info_sets;
    if (am_policy_info_container->slice_usg_ctrl_info_sets) {
        OpenAPI_list_for_each(am_policy_info_container->slice_usg_ctrl_info_sets, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_am_policy_info_container_convertToJSON() failed [slice_usg_ctrl_info_sets]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_am_policy_info_container_convertToJSON() failed [slice_usg_ctrl_info_sets]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_slice_usage_control_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_am_policy_info_container_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_am_policy_info_container_t *OpenAPI_am_policy_info_container_parseFromJSON(cJSON *am_policy_info_containerJSON)
{
    OpenAPI_am_policy_info_container_t *am_policy_info_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *slice_usg_ctrl_info_sets = NULL;
    OpenAPI_list_t *slice_usg_ctrl_info_setsList = NULL;
    slice_usg_ctrl_info_sets = cJSON_GetObjectItemCaseSensitive(am_policy_info_containerJSON, "sliceUsgCtrlInfoSets");
    if (slice_usg_ctrl_info_sets) {
        cJSON *slice_usg_ctrl_info_sets_local_map = NULL;
        if (!cJSON_IsObject(slice_usg_ctrl_info_sets) && !cJSON_IsNull(slice_usg_ctrl_info_sets)) {
            ogs_error("OpenAPI_am_policy_info_container_parseFromJSON() failed [slice_usg_ctrl_info_sets]");
            goto end;
        }
        if (cJSON_IsObject(slice_usg_ctrl_info_sets)) {
            slice_usg_ctrl_info_setsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(slice_usg_ctrl_info_sets_local_map, slice_usg_ctrl_info_sets) {
                cJSON *localMapObject = slice_usg_ctrl_info_sets_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_slice_usage_control_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_am_policy_info_container_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(slice_usg_ctrl_info_setsList, localMapKeyPair);
            }
        }
    }

    am_policy_info_container_local_var = OpenAPI_am_policy_info_container_create (
        slice_usg_ctrl_info_sets ? slice_usg_ctrl_info_setsList : NULL
    );

    return am_policy_info_container_local_var;
end:
    if (slice_usg_ctrl_info_setsList) {
        OpenAPI_list_for_each(slice_usg_ctrl_info_setsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_usage_control_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(slice_usg_ctrl_info_setsList);
        slice_usg_ctrl_info_setsList = NULL;
    }
    return NULL;
}

OpenAPI_am_policy_info_container_t *OpenAPI_am_policy_info_container_copy(OpenAPI_am_policy_info_container_t *dst, OpenAPI_am_policy_info_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_policy_info_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_policy_info_container_convertToJSON() failed");
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

    OpenAPI_am_policy_info_container_free(dst);
    dst = OpenAPI_am_policy_info_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

