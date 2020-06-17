
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iptv_config_data_patch.h"

OpenAPI_iptv_config_data_patch_t *OpenAPI_iptv_config_data_patch_create(
    OpenAPI_list_t* multi_acc_ctrls
    )
{
    OpenAPI_iptv_config_data_patch_t *iptv_config_data_patch_local_var = OpenAPI_malloc(sizeof(OpenAPI_iptv_config_data_patch_t));
    if (!iptv_config_data_patch_local_var) {
        return NULL;
    }
    iptv_config_data_patch_local_var->multi_acc_ctrls = multi_acc_ctrls;

    return iptv_config_data_patch_local_var;
}

void OpenAPI_iptv_config_data_patch_free(OpenAPI_iptv_config_data_patch_t *iptv_config_data_patch)
{
    if (NULL == iptv_config_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(iptv_config_data_patch->multi_acc_ctrls, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_multicast_access_control_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(iptv_config_data_patch->multi_acc_ctrls);
    ogs_free(iptv_config_data_patch);
}

cJSON *OpenAPI_iptv_config_data_patch_convertToJSON(OpenAPI_iptv_config_data_patch_t *iptv_config_data_patch)
{
    cJSON *item = NULL;

    if (iptv_config_data_patch == NULL) {
        ogs_error("OpenAPI_iptv_config_data_patch_convertToJSON() failed [IptvConfigDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (iptv_config_data_patch->multi_acc_ctrls) {
        cJSON *multi_acc_ctrls = cJSON_AddObjectToObject(item, "multiAccCtrls");
        if (multi_acc_ctrls == NULL) {
            ogs_error("OpenAPI_iptv_config_data_patch_convertToJSON() failed [multi_acc_ctrls]");
            goto end;
        }
        cJSON *localMapObject = multi_acc_ctrls;
        OpenAPI_lnode_t *multi_acc_ctrls_node;
        if (iptv_config_data_patch->multi_acc_ctrls) {
            OpenAPI_list_for_each(iptv_config_data_patch->multi_acc_ctrls, multi_acc_ctrls_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)multi_acc_ctrls_node->data;
                cJSON *itemLocal = OpenAPI_multicast_access_control_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_iptv_config_data_patch_convertToJSON() failed [multi_acc_ctrls]");
                    goto end;
                }
                cJSON_AddItemToObject(multi_acc_ctrls, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_iptv_config_data_patch_t *OpenAPI_iptv_config_data_patch_parseFromJSON(cJSON *iptv_config_data_patchJSON)
{
    OpenAPI_iptv_config_data_patch_t *iptv_config_data_patch_local_var = NULL;
    cJSON *multi_acc_ctrls = cJSON_GetObjectItemCaseSensitive(iptv_config_data_patchJSON, "multiAccCtrls");

    OpenAPI_list_t *multi_acc_ctrlsList;
    if (multi_acc_ctrls) {
        cJSON *multi_acc_ctrls_local_map;
        if (!cJSON_IsObject(multi_acc_ctrls)) {
            ogs_error("OpenAPI_iptv_config_data_patch_parseFromJSON() failed [multi_acc_ctrls]");
            goto end;
        }
        multi_acc_ctrlsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(multi_acc_ctrls_local_map, multi_acc_ctrls) {
            cJSON *localMapObject = multi_acc_ctrls_local_map;
            if (!cJSON_IsObject(multi_acc_ctrls_local_map)) {
                ogs_error("OpenAPI_iptv_config_data_patch_parseFromJSON() failed [multi_acc_ctrls]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_multicast_access_control_parseFromJSON(localMapObject));
            OpenAPI_list_add(multi_acc_ctrlsList, localMapKeyPair);
        }
    }

    iptv_config_data_patch_local_var = OpenAPI_iptv_config_data_patch_create (
        multi_acc_ctrls ? multi_acc_ctrlsList : NULL
        );

    return iptv_config_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_iptv_config_data_patch_t *OpenAPI_iptv_config_data_patch_copy(OpenAPI_iptv_config_data_patch_t *dst, OpenAPI_iptv_config_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_iptv_config_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_iptv_config_data_patch_convertToJSON() failed");
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

    OpenAPI_iptv_config_data_patch_free(dst);
    dst = OpenAPI_iptv_config_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

