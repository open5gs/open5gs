
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_determination_info.h"

OpenAPI_pfd_determination_info_t *OpenAPI_pfd_determination_info_create(
    char *app_id,
    OpenAPI_list_t *sugg_pfd_info_list
)
{
    OpenAPI_pfd_determination_info_t *pfd_determination_info_local_var = ogs_malloc(sizeof(OpenAPI_pfd_determination_info_t));
    ogs_assert(pfd_determination_info_local_var);

    pfd_determination_info_local_var->app_id = app_id;
    pfd_determination_info_local_var->sugg_pfd_info_list = sugg_pfd_info_list;

    return pfd_determination_info_local_var;
}

void OpenAPI_pfd_determination_info_free(OpenAPI_pfd_determination_info_t *pfd_determination_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pfd_determination_info) {
        return;
    }
    if (pfd_determination_info->app_id) {
        ogs_free(pfd_determination_info->app_id);
        pfd_determination_info->app_id = NULL;
    }
    if (pfd_determination_info->sugg_pfd_info_list) {
        OpenAPI_list_for_each(pfd_determination_info->sugg_pfd_info_list, node) {
            OpenAPI_suggested_pfd_info_free(node->data);
        }
        OpenAPI_list_free(pfd_determination_info->sugg_pfd_info_list);
        pfd_determination_info->sugg_pfd_info_list = NULL;
    }
    ogs_free(pfd_determination_info);
}

cJSON *OpenAPI_pfd_determination_info_convertToJSON(OpenAPI_pfd_determination_info_t *pfd_determination_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pfd_determination_info == NULL) {
        ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed [PfdDeterminationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pfd_determination_info->app_id) {
        ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed [app_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "appId", pfd_determination_info->app_id) == NULL) {
        ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed [app_id]");
        goto end;
    }

    if (!pfd_determination_info->sugg_pfd_info_list) {
        ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed [sugg_pfd_info_list]");
        return NULL;
    }
    cJSON *sugg_pfd_info_listList = cJSON_AddArrayToObject(item, "suggPfdInfoList");
    if (sugg_pfd_info_listList == NULL) {
        ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed [sugg_pfd_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_determination_info->sugg_pfd_info_list, node) {
        cJSON *itemLocal = OpenAPI_suggested_pfd_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed [sugg_pfd_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(sugg_pfd_info_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_pfd_determination_info_t *OpenAPI_pfd_determination_info_parseFromJSON(cJSON *pfd_determination_infoJSON)
{
    OpenAPI_pfd_determination_info_t *pfd_determination_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *sugg_pfd_info_list = NULL;
    OpenAPI_list_t *sugg_pfd_info_listList = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(pfd_determination_infoJSON, "appId");
    if (!app_id) {
        ogs_error("OpenAPI_pfd_determination_info_parseFromJSON() failed [app_id]");
        goto end;
    }
    if (!cJSON_IsString(app_id)) {
        ogs_error("OpenAPI_pfd_determination_info_parseFromJSON() failed [app_id]");
        goto end;
    }

    sugg_pfd_info_list = cJSON_GetObjectItemCaseSensitive(pfd_determination_infoJSON, "suggPfdInfoList");
    if (!sugg_pfd_info_list) {
        ogs_error("OpenAPI_pfd_determination_info_parseFromJSON() failed [sugg_pfd_info_list]");
        goto end;
    }
        cJSON *sugg_pfd_info_list_local = NULL;
        if (!cJSON_IsArray(sugg_pfd_info_list)) {
            ogs_error("OpenAPI_pfd_determination_info_parseFromJSON() failed [sugg_pfd_info_list]");
            goto end;
        }

        sugg_pfd_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sugg_pfd_info_list_local, sugg_pfd_info_list) {
            if (!cJSON_IsObject(sugg_pfd_info_list_local)) {
                ogs_error("OpenAPI_pfd_determination_info_parseFromJSON() failed [sugg_pfd_info_list]");
                goto end;
            }
            OpenAPI_suggested_pfd_info_t *sugg_pfd_info_listItem = OpenAPI_suggested_pfd_info_parseFromJSON(sugg_pfd_info_list_local);
            if (!sugg_pfd_info_listItem) {
                ogs_error("No sugg_pfd_info_listItem");
                goto end;
            }
            OpenAPI_list_add(sugg_pfd_info_listList, sugg_pfd_info_listItem);
        }

    pfd_determination_info_local_var = OpenAPI_pfd_determination_info_create (
        ogs_strdup(app_id->valuestring),
        sugg_pfd_info_listList
    );

    return pfd_determination_info_local_var;
end:
    if (sugg_pfd_info_listList) {
        OpenAPI_list_for_each(sugg_pfd_info_listList, node) {
            OpenAPI_suggested_pfd_info_free(node->data);
        }
        OpenAPI_list_free(sugg_pfd_info_listList);
        sugg_pfd_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_pfd_determination_info_t *OpenAPI_pfd_determination_info_copy(OpenAPI_pfd_determination_info_t *dst, OpenAPI_pfd_determination_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_determination_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_determination_info_convertToJSON() failed");
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

    OpenAPI_pfd_determination_info_free(dst);
    dst = OpenAPI_pfd_determination_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

