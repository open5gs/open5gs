
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tai_weight_info.h"

OpenAPI_tai_weight_info_t *OpenAPI_tai_weight_info_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    int weight
)
{
    OpenAPI_tai_weight_info_t *tai_weight_info_local_var = ogs_malloc(sizeof(OpenAPI_tai_weight_info_t));
    ogs_assert(tai_weight_info_local_var);

    tai_weight_info_local_var->tai_list = tai_list;
    tai_weight_info_local_var->tai_range_list = tai_range_list;
    tai_weight_info_local_var->weight = weight;

    return tai_weight_info_local_var;
}

void OpenAPI_tai_weight_info_free(OpenAPI_tai_weight_info_t *tai_weight_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tai_weight_info) {
        return;
    }
    if (tai_weight_info->tai_list) {
        OpenAPI_list_for_each(tai_weight_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_weight_info->tai_list);
        tai_weight_info->tai_list = NULL;
    }
    if (tai_weight_info->tai_range_list) {
        OpenAPI_list_for_each(tai_weight_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_weight_info->tai_range_list);
        tai_weight_info->tai_range_list = NULL;
    }
    ogs_free(tai_weight_info);
}

cJSON *OpenAPI_tai_weight_info_convertToJSON(OpenAPI_tai_weight_info_t *tai_weight_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tai_weight_info == NULL) {
        ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed [TaiWeightInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tai_weight_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(tai_weight_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (tai_weight_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(tai_weight_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (cJSON_AddNumberToObject(item, "weight", tai_weight_info->weight) == NULL) {
        ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed [weight]");
        goto end;
    }

end:
    return item;
}

OpenAPI_tai_weight_info_t *OpenAPI_tai_weight_info_parseFromJSON(cJSON *tai_weight_infoJSON)
{
    OpenAPI_tai_weight_info_t *tai_weight_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *weight = NULL;
    tai_list = cJSON_GetObjectItemCaseSensitive(tai_weight_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_tai_weight_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_tai_weight_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(tai_weight_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_tai_weight_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_tai_weight_info_parseFromJSON() failed [tai_range_list]");
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

    weight = cJSON_GetObjectItemCaseSensitive(tai_weight_infoJSON, "weight");
    if (!weight) {
        ogs_error("OpenAPI_tai_weight_info_parseFromJSON() failed [weight]");
        goto end;
    }
    if (!cJSON_IsNumber(weight)) {
        ogs_error("OpenAPI_tai_weight_info_parseFromJSON() failed [weight]");
        goto end;
    }

    tai_weight_info_local_var = OpenAPI_tai_weight_info_create (
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        
        weight->valuedouble
    );

    return tai_weight_info_local_var;
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

OpenAPI_tai_weight_info_t *OpenAPI_tai_weight_info_copy(OpenAPI_tai_weight_info_t *dst, OpenAPI_tai_weight_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tai_weight_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tai_weight_info_convertToJSON() failed");
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

    OpenAPI_tai_weight_info_free(dst);
    dst = OpenAPI_tai_weight_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

