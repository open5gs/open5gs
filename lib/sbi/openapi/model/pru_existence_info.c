
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pru_existence_info.h"

OpenAPI_pru_existence_info_t *OpenAPI_pru_existence_info_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
)
{
    OpenAPI_pru_existence_info_t *pru_existence_info_local_var = ogs_malloc(sizeof(OpenAPI_pru_existence_info_t));
    ogs_assert(pru_existence_info_local_var);

    pru_existence_info_local_var->tai_list = tai_list;
    pru_existence_info_local_var->tai_range_list = tai_range_list;

    return pru_existence_info_local_var;
}

void OpenAPI_pru_existence_info_free(OpenAPI_pru_existence_info_t *pru_existence_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pru_existence_info) {
        return;
    }
    if (pru_existence_info->tai_list) {
        OpenAPI_list_for_each(pru_existence_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(pru_existence_info->tai_list);
        pru_existence_info->tai_list = NULL;
    }
    if (pru_existence_info->tai_range_list) {
        OpenAPI_list_for_each(pru_existence_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(pru_existence_info->tai_range_list);
        pru_existence_info->tai_range_list = NULL;
    }
    ogs_free(pru_existence_info);
}

cJSON *OpenAPI_pru_existence_info_convertToJSON(OpenAPI_pru_existence_info_t *pru_existence_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pru_existence_info == NULL) {
        ogs_error("OpenAPI_pru_existence_info_convertToJSON() failed [PruExistenceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pru_existence_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_pru_existence_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(pru_existence_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pru_existence_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (pru_existence_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_pru_existence_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(pru_existence_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pru_existence_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_pru_existence_info_t *OpenAPI_pru_existence_info_parseFromJSON(cJSON *pru_existence_infoJSON)
{
    OpenAPI_pru_existence_info_t *pru_existence_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    tai_list = cJSON_GetObjectItemCaseSensitive(pru_existence_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_pru_existence_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_pru_existence_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(pru_existence_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_pru_existence_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_pru_existence_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    pru_existence_info_local_var = OpenAPI_pru_existence_info_create (
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL
    );

    return pru_existence_info_local_var;
end:
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    return NULL;
}

OpenAPI_pru_existence_info_t *OpenAPI_pru_existence_info_copy(OpenAPI_pru_existence_info_t *dst, OpenAPI_pru_existence_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pru_existence_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pru_existence_info_convertToJSON() failed");
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

    OpenAPI_pru_existence_info_free(dst);
    dst = OpenAPI_pru_existence_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

