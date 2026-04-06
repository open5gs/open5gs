
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aiot_dev_profile_data_list.h"

OpenAPI_aiot_dev_profile_data_list_t *OpenAPI_aiot_dev_profile_data_list_create(
    OpenAPI_list_t *aiot_dev_profile_data_list
)
{
    OpenAPI_aiot_dev_profile_data_list_t *aiot_dev_profile_data_list_local_var = ogs_malloc(sizeof(OpenAPI_aiot_dev_profile_data_list_t));
    ogs_assert(aiot_dev_profile_data_list_local_var);

    aiot_dev_profile_data_list_local_var->aiot_dev_profile_data_list = aiot_dev_profile_data_list;

    return aiot_dev_profile_data_list_local_var;
}

void OpenAPI_aiot_dev_profile_data_list_free(OpenAPI_aiot_dev_profile_data_list_t *aiot_dev_profile_data_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aiot_dev_profile_data_list) {
        return;
    }
    if (aiot_dev_profile_data_list->aiot_dev_profile_data_list) {
        OpenAPI_list_for_each(aiot_dev_profile_data_list->aiot_dev_profile_data_list, node) {
            OpenAPI_aiot_dev_profile_data_free(node->data);
        }
        OpenAPI_list_free(aiot_dev_profile_data_list->aiot_dev_profile_data_list);
        aiot_dev_profile_data_list->aiot_dev_profile_data_list = NULL;
    }
    ogs_free(aiot_dev_profile_data_list);
}

cJSON *OpenAPI_aiot_dev_profile_data_list_convertToJSON(OpenAPI_aiot_dev_profile_data_list_t *aiot_dev_profile_data_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aiot_dev_profile_data_list == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_list_convertToJSON() failed [AiotDevProfileDataList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!aiot_dev_profile_data_list->aiot_dev_profile_data_list) {
        ogs_error("OpenAPI_aiot_dev_profile_data_list_convertToJSON() failed [aiot_dev_profile_data_list]");
        return NULL;
    }
    cJSON *aiot_dev_profile_data_listList = cJSON_AddArrayToObject(item, "aiotDevProfileDataList");
    if (aiot_dev_profile_data_listList == NULL) {
        ogs_error("OpenAPI_aiot_dev_profile_data_list_convertToJSON() failed [aiot_dev_profile_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(aiot_dev_profile_data_list->aiot_dev_profile_data_list, node) {
        cJSON *itemLocal = OpenAPI_aiot_dev_profile_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_aiot_dev_profile_data_list_convertToJSON() failed [aiot_dev_profile_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(aiot_dev_profile_data_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_aiot_dev_profile_data_list_t *OpenAPI_aiot_dev_profile_data_list_parseFromJSON(cJSON *aiot_dev_profile_data_listJSON)
{
    OpenAPI_aiot_dev_profile_data_list_t *aiot_dev_profile_data_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *aiot_dev_profile_data_list = NULL;
    OpenAPI_list_t *aiot_dev_profile_data_listList = NULL;
    aiot_dev_profile_data_list = cJSON_GetObjectItemCaseSensitive(aiot_dev_profile_data_listJSON, "aiotDevProfileDataList");
    if (!aiot_dev_profile_data_list) {
        ogs_error("OpenAPI_aiot_dev_profile_data_list_parseFromJSON() failed [aiot_dev_profile_data_list]");
        goto end;
    }
        cJSON *aiot_dev_profile_data_list_local = NULL;
        if (!cJSON_IsArray(aiot_dev_profile_data_list)) {
            ogs_error("OpenAPI_aiot_dev_profile_data_list_parseFromJSON() failed [aiot_dev_profile_data_list]");
            goto end;
        }

        aiot_dev_profile_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(aiot_dev_profile_data_list_local, aiot_dev_profile_data_list) {
            if (!cJSON_IsObject(aiot_dev_profile_data_list_local)) {
                ogs_error("OpenAPI_aiot_dev_profile_data_list_parseFromJSON() failed [aiot_dev_profile_data_list]");
                goto end;
            }
            OpenAPI_aiot_dev_profile_data_t *aiot_dev_profile_data_listItem = OpenAPI_aiot_dev_profile_data_parseFromJSON(aiot_dev_profile_data_list_local);
            if (!aiot_dev_profile_data_listItem) {
                ogs_error("No aiot_dev_profile_data_listItem");
                goto end;
            }
            OpenAPI_list_add(aiot_dev_profile_data_listList, aiot_dev_profile_data_listItem);
        }

    aiot_dev_profile_data_list_local_var = OpenAPI_aiot_dev_profile_data_list_create (
        aiot_dev_profile_data_listList
    );

    return aiot_dev_profile_data_list_local_var;
end:
    if (aiot_dev_profile_data_listList) {
        OpenAPI_list_for_each(aiot_dev_profile_data_listList, node) {
            OpenAPI_aiot_dev_profile_data_free(node->data);
        }
        OpenAPI_list_free(aiot_dev_profile_data_listList);
        aiot_dev_profile_data_listList = NULL;
    }
    return NULL;
}

OpenAPI_aiot_dev_profile_data_list_t *OpenAPI_aiot_dev_profile_data_list_copy(OpenAPI_aiot_dev_profile_data_list_t *dst, OpenAPI_aiot_dev_profile_data_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aiot_dev_profile_data_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aiot_dev_profile_data_list_convertToJSON() failed");
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

    OpenAPI_aiot_dev_profile_data_list_free(dst);
    dst = OpenAPI_aiot_dev_profile_data_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

