
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "target_area.h"

OpenAPI_target_area_t *OpenAPI_target_area_create(
    OpenAPI_list_t *ta_list,
    OpenAPI_list_t *tai_range_list,
    bool is_any_ta,
    int any_ta
)
{
    OpenAPI_target_area_t *target_area_local_var = ogs_malloc(sizeof(OpenAPI_target_area_t));
    ogs_assert(target_area_local_var);

    target_area_local_var->ta_list = ta_list;
    target_area_local_var->tai_range_list = tai_range_list;
    target_area_local_var->is_any_ta = is_any_ta;
    target_area_local_var->any_ta = any_ta;

    return target_area_local_var;
}

void OpenAPI_target_area_free(OpenAPI_target_area_t *target_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == target_area) {
        return;
    }
    if (target_area->ta_list) {
        OpenAPI_list_for_each(target_area->ta_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(target_area->ta_list);
        target_area->ta_list = NULL;
    }
    if (target_area->tai_range_list) {
        OpenAPI_list_for_each(target_area->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(target_area->tai_range_list);
        target_area->tai_range_list = NULL;
    }
    ogs_free(target_area);
}

cJSON *OpenAPI_target_area_convertToJSON(OpenAPI_target_area_t *target_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (target_area == NULL) {
        ogs_error("OpenAPI_target_area_convertToJSON() failed [TargetArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (target_area->ta_list) {
    cJSON *ta_listList = cJSON_AddArrayToObject(item, "taList");
    if (ta_listList == NULL) {
        ogs_error("OpenAPI_target_area_convertToJSON() failed [ta_list]");
        goto end;
    }
    OpenAPI_list_for_each(target_area->ta_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_target_area_convertToJSON() failed [ta_list]");
            goto end;
        }
        cJSON_AddItemToArray(ta_listList, itemLocal);
    }
    }

    if (target_area->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_target_area_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(target_area->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_target_area_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (target_area->is_any_ta) {
    if (cJSON_AddBoolToObject(item, "anyTa", target_area->any_ta) == NULL) {
        ogs_error("OpenAPI_target_area_convertToJSON() failed [any_ta]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_target_area_t *OpenAPI_target_area_parseFromJSON(cJSON *target_areaJSON)
{
    OpenAPI_target_area_t *target_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ta_list = NULL;
    OpenAPI_list_t *ta_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *any_ta = NULL;
    ta_list = cJSON_GetObjectItemCaseSensitive(target_areaJSON, "taList");
    if (ta_list) {
        cJSON *ta_list_local = NULL;
        if (!cJSON_IsArray(ta_list)) {
            ogs_error("OpenAPI_target_area_parseFromJSON() failed [ta_list]");
            goto end;
        }

        ta_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ta_list_local, ta_list) {
            if (!cJSON_IsObject(ta_list_local)) {
                ogs_error("OpenAPI_target_area_parseFromJSON() failed [ta_list]");
                goto end;
            }
            OpenAPI_tai_t *ta_listItem = OpenAPI_tai_parseFromJSON(ta_list_local);
            if (!ta_listItem) {
                ogs_error("No ta_listItem");
                goto end;
            }
            OpenAPI_list_add(ta_listList, ta_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(target_areaJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_target_area_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_target_area_parseFromJSON() failed [tai_range_list]");
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

    any_ta = cJSON_GetObjectItemCaseSensitive(target_areaJSON, "anyTa");
    if (any_ta) {
    if (!cJSON_IsBool(any_ta)) {
        ogs_error("OpenAPI_target_area_parseFromJSON() failed [any_ta]");
        goto end;
    }
    }

    target_area_local_var = OpenAPI_target_area_create (
        ta_list ? ta_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        any_ta ? true : false,
        any_ta ? any_ta->valueint : 0
    );

    return target_area_local_var;
end:
    if (ta_listList) {
        OpenAPI_list_for_each(ta_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(ta_listList);
        ta_listList = NULL;
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

OpenAPI_target_area_t *OpenAPI_target_area_copy(OpenAPI_target_area_t *dst, OpenAPI_target_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_target_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_target_area_convertToJSON() failed");
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

    OpenAPI_target_area_free(dst);
    dst = OpenAPI_target_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

