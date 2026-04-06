
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "search_result_info.h"

OpenAPI_search_result_info_t *OpenAPI_search_result_info_create(
    OpenAPI_list_t *unsatisfied_tai_list
)
{
    OpenAPI_search_result_info_t *search_result_info_local_var = ogs_malloc(sizeof(OpenAPI_search_result_info_t));
    ogs_assert(search_result_info_local_var);

    search_result_info_local_var->unsatisfied_tai_list = unsatisfied_tai_list;

    return search_result_info_local_var;
}

void OpenAPI_search_result_info_free(OpenAPI_search_result_info_t *search_result_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == search_result_info) {
        return;
    }
    if (search_result_info->unsatisfied_tai_list) {
        OpenAPI_list_for_each(search_result_info->unsatisfied_tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(search_result_info->unsatisfied_tai_list);
        search_result_info->unsatisfied_tai_list = NULL;
    }
    ogs_free(search_result_info);
}

cJSON *OpenAPI_search_result_info_convertToJSON(OpenAPI_search_result_info_t *search_result_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (search_result_info == NULL) {
        ogs_error("OpenAPI_search_result_info_convertToJSON() failed [SearchResultInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (search_result_info->unsatisfied_tai_list) {
    cJSON *unsatisfied_tai_listList = cJSON_AddArrayToObject(item, "unsatisfiedTaiList");
    if (unsatisfied_tai_listList == NULL) {
        ogs_error("OpenAPI_search_result_info_convertToJSON() failed [unsatisfied_tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(search_result_info->unsatisfied_tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_search_result_info_convertToJSON() failed [unsatisfied_tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(unsatisfied_tai_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_search_result_info_t *OpenAPI_search_result_info_parseFromJSON(cJSON *search_result_infoJSON)
{
    OpenAPI_search_result_info_t *search_result_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *unsatisfied_tai_list = NULL;
    OpenAPI_list_t *unsatisfied_tai_listList = NULL;
    unsatisfied_tai_list = cJSON_GetObjectItemCaseSensitive(search_result_infoJSON, "unsatisfiedTaiList");
    if (unsatisfied_tai_list) {
        cJSON *unsatisfied_tai_list_local = NULL;
        if (!cJSON_IsArray(unsatisfied_tai_list)) {
            ogs_error("OpenAPI_search_result_info_parseFromJSON() failed [unsatisfied_tai_list]");
            goto end;
        }

        unsatisfied_tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(unsatisfied_tai_list_local, unsatisfied_tai_list) {
            if (!cJSON_IsObject(unsatisfied_tai_list_local)) {
                ogs_error("OpenAPI_search_result_info_parseFromJSON() failed [unsatisfied_tai_list]");
                goto end;
            }
            OpenAPI_tai_t *unsatisfied_tai_listItem = OpenAPI_tai_parseFromJSON(unsatisfied_tai_list_local);
            if (!unsatisfied_tai_listItem) {
                ogs_error("No unsatisfied_tai_listItem");
                goto end;
            }
            OpenAPI_list_add(unsatisfied_tai_listList, unsatisfied_tai_listItem);
        }
    }

    search_result_info_local_var = OpenAPI_search_result_info_create (
        unsatisfied_tai_list ? unsatisfied_tai_listList : NULL
    );

    return search_result_info_local_var;
end:
    if (unsatisfied_tai_listList) {
        OpenAPI_list_for_each(unsatisfied_tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(unsatisfied_tai_listList);
        unsatisfied_tai_listList = NULL;
    }
    return NULL;
}

OpenAPI_search_result_info_t *OpenAPI_search_result_info_copy(OpenAPI_search_result_info_t *dst, OpenAPI_search_result_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_search_result_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_search_result_info_convertToJSON() failed");
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

    OpenAPI_search_result_info_free(dst);
    dst = OpenAPI_search_result_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

