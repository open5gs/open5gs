
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nidd_authorization_info.h"

OpenAPI_nidd_authorization_info_t *OpenAPI_nidd_authorization_info_create(
    OpenAPI_list_t *nidd_authorization_list
)
{
    OpenAPI_nidd_authorization_info_t *nidd_authorization_info_local_var = ogs_malloc(sizeof(OpenAPI_nidd_authorization_info_t));
    ogs_assert(nidd_authorization_info_local_var);

    nidd_authorization_info_local_var->nidd_authorization_list = nidd_authorization_list;

    return nidd_authorization_info_local_var;
}

void OpenAPI_nidd_authorization_info_free(OpenAPI_nidd_authorization_info_t *nidd_authorization_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nidd_authorization_info) {
        return;
    }
    if (nidd_authorization_info->nidd_authorization_list) {
        OpenAPI_list_for_each(nidd_authorization_info->nidd_authorization_list, node) {
            OpenAPI_authorization_info_free(node->data);
        }
        OpenAPI_list_free(nidd_authorization_info->nidd_authorization_list);
        nidd_authorization_info->nidd_authorization_list = NULL;
    }
    ogs_free(nidd_authorization_info);
}

cJSON *OpenAPI_nidd_authorization_info_convertToJSON(OpenAPI_nidd_authorization_info_t *nidd_authorization_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nidd_authorization_info == NULL) {
        ogs_error("OpenAPI_nidd_authorization_info_convertToJSON() failed [NiddAuthorizationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nidd_authorization_info->nidd_authorization_list) {
        ogs_error("OpenAPI_nidd_authorization_info_convertToJSON() failed [nidd_authorization_list]");
        return NULL;
    }
    cJSON *nidd_authorization_listList = cJSON_AddArrayToObject(item, "niddAuthorizationList");
    if (nidd_authorization_listList == NULL) {
        ogs_error("OpenAPI_nidd_authorization_info_convertToJSON() failed [nidd_authorization_list]");
        goto end;
    }
    OpenAPI_list_for_each(nidd_authorization_info->nidd_authorization_list, node) {
        cJSON *itemLocal = OpenAPI_authorization_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nidd_authorization_info_convertToJSON() failed [nidd_authorization_list]");
            goto end;
        }
        cJSON_AddItemToArray(nidd_authorization_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_nidd_authorization_info_t *OpenAPI_nidd_authorization_info_parseFromJSON(cJSON *nidd_authorization_infoJSON)
{
    OpenAPI_nidd_authorization_info_t *nidd_authorization_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nidd_authorization_list = NULL;
    OpenAPI_list_t *nidd_authorization_listList = NULL;
    nidd_authorization_list = cJSON_GetObjectItemCaseSensitive(nidd_authorization_infoJSON, "niddAuthorizationList");
    if (!nidd_authorization_list) {
        ogs_error("OpenAPI_nidd_authorization_info_parseFromJSON() failed [nidd_authorization_list]");
        goto end;
    }
        cJSON *nidd_authorization_list_local = NULL;
        if (!cJSON_IsArray(nidd_authorization_list)) {
            ogs_error("OpenAPI_nidd_authorization_info_parseFromJSON() failed [nidd_authorization_list]");
            goto end;
        }

        nidd_authorization_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nidd_authorization_list_local, nidd_authorization_list) {
            if (!cJSON_IsObject(nidd_authorization_list_local)) {
                ogs_error("OpenAPI_nidd_authorization_info_parseFromJSON() failed [nidd_authorization_list]");
                goto end;
            }
            OpenAPI_authorization_info_t *nidd_authorization_listItem = OpenAPI_authorization_info_parseFromJSON(nidd_authorization_list_local);
            if (!nidd_authorization_listItem) {
                ogs_error("No nidd_authorization_listItem");
                goto end;
            }
            OpenAPI_list_add(nidd_authorization_listList, nidd_authorization_listItem);
        }

    nidd_authorization_info_local_var = OpenAPI_nidd_authorization_info_create (
        nidd_authorization_listList
    );

    return nidd_authorization_info_local_var;
end:
    if (nidd_authorization_listList) {
        OpenAPI_list_for_each(nidd_authorization_listList, node) {
            OpenAPI_authorization_info_free(node->data);
        }
        OpenAPI_list_free(nidd_authorization_listList);
        nidd_authorization_listList = NULL;
    }
    return NULL;
}

OpenAPI_nidd_authorization_info_t *OpenAPI_nidd_authorization_info_copy(OpenAPI_nidd_authorization_info_t *dst, OpenAPI_nidd_authorization_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nidd_authorization_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nidd_authorization_info_convertToJSON() failed");
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

    OpenAPI_nidd_authorization_info_free(dst);
    dst = OpenAPI_nidd_authorization_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

