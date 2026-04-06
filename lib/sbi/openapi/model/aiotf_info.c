
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aiotf_info.h"

OpenAPI_aiotf_info_t *OpenAPI_aiotf_info_create(
    OpenAPI_list_t *aiot_area_id_list
)
{
    OpenAPI_aiotf_info_t *aiotf_info_local_var = ogs_malloc(sizeof(OpenAPI_aiotf_info_t));
    ogs_assert(aiotf_info_local_var);

    aiotf_info_local_var->aiot_area_id_list = aiot_area_id_list;

    return aiotf_info_local_var;
}

void OpenAPI_aiotf_info_free(OpenAPI_aiotf_info_t *aiotf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aiotf_info) {
        return;
    }
    if (aiotf_info->aiot_area_id_list) {
        OpenAPI_list_for_each(aiotf_info->aiot_area_id_list, node) {
            OpenAPI_aiot_area_id_free(node->data);
        }
        OpenAPI_list_free(aiotf_info->aiot_area_id_list);
        aiotf_info->aiot_area_id_list = NULL;
    }
    ogs_free(aiotf_info);
}

cJSON *OpenAPI_aiotf_info_convertToJSON(OpenAPI_aiotf_info_t *aiotf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aiotf_info == NULL) {
        ogs_error("OpenAPI_aiotf_info_convertToJSON() failed [AiotfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (aiotf_info->aiot_area_id_list) {
    cJSON *aiot_area_id_listList = cJSON_AddArrayToObject(item, "aiotAreaIDList");
    if (aiot_area_id_listList == NULL) {
        ogs_error("OpenAPI_aiotf_info_convertToJSON() failed [aiot_area_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(aiotf_info->aiot_area_id_list, node) {
        cJSON *itemLocal = OpenAPI_aiot_area_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_aiotf_info_convertToJSON() failed [aiot_area_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(aiot_area_id_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_aiotf_info_t *OpenAPI_aiotf_info_parseFromJSON(cJSON *aiotf_infoJSON)
{
    OpenAPI_aiotf_info_t *aiotf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *aiot_area_id_list = NULL;
    OpenAPI_list_t *aiot_area_id_listList = NULL;
    aiot_area_id_list = cJSON_GetObjectItemCaseSensitive(aiotf_infoJSON, "aiotAreaIDList");
    if (aiot_area_id_list) {
        cJSON *aiot_area_id_list_local = NULL;
        if (!cJSON_IsArray(aiot_area_id_list)) {
            ogs_error("OpenAPI_aiotf_info_parseFromJSON() failed [aiot_area_id_list]");
            goto end;
        }

        aiot_area_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(aiot_area_id_list_local, aiot_area_id_list) {
            if (!cJSON_IsObject(aiot_area_id_list_local)) {
                ogs_error("OpenAPI_aiotf_info_parseFromJSON() failed [aiot_area_id_list]");
                goto end;
            }
            OpenAPI_aiot_area_id_t *aiot_area_id_listItem = OpenAPI_aiot_area_id_parseFromJSON(aiot_area_id_list_local);
            if (!aiot_area_id_listItem) {
                ogs_error("No aiot_area_id_listItem");
                goto end;
            }
            OpenAPI_list_add(aiot_area_id_listList, aiot_area_id_listItem);
        }
    }

    aiotf_info_local_var = OpenAPI_aiotf_info_create (
        aiot_area_id_list ? aiot_area_id_listList : NULL
    );

    return aiotf_info_local_var;
end:
    if (aiot_area_id_listList) {
        OpenAPI_list_for_each(aiot_area_id_listList, node) {
            OpenAPI_aiot_area_id_free(node->data);
        }
        OpenAPI_list_free(aiot_area_id_listList);
        aiot_area_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_aiotf_info_t *OpenAPI_aiotf_info_copy(OpenAPI_aiotf_info_t *dst, OpenAPI_aiotf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aiotf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aiotf_info_convertToJSON() failed");
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

    OpenAPI_aiotf_info_free(dst);
    dst = OpenAPI_aiotf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

