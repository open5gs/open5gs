
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "area_of_validity.h"

OpenAPI_area_of_validity_t *OpenAPI_area_of_validity_create(
    OpenAPI_list_t *tai_list
    )
{
    OpenAPI_area_of_validity_t *area_of_validity_local_var = OpenAPI_malloc(sizeof(OpenAPI_area_of_validity_t));
    if (!area_of_validity_local_var) {
        return NULL;
    }
    area_of_validity_local_var->tai_list = tai_list;

    return area_of_validity_local_var;
}

void OpenAPI_area_of_validity_free(OpenAPI_area_of_validity_t *area_of_validity)
{
    if (NULL == area_of_validity) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(area_of_validity->tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(area_of_validity->tai_list);
    ogs_free(area_of_validity);
}

cJSON *OpenAPI_area_of_validity_convertToJSON(OpenAPI_area_of_validity_t *area_of_validity)
{
    cJSON *item = NULL;

    if (area_of_validity == NULL) {
        ogs_error("OpenAPI_area_of_validity_convertToJSON() failed [AreaOfValidity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!area_of_validity->tai_list) {
        ogs_error("OpenAPI_area_of_validity_convertToJSON() failed [tai_list]");
        goto end;
    }
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_area_of_validity_convertToJSON() failed [tai_list]");
        goto end;
    }

    OpenAPI_lnode_t *tai_list_node;
    if (area_of_validity->tai_list) {
        OpenAPI_list_for_each(area_of_validity->tai_list, tai_list_node) {
            cJSON *itemLocal = OpenAPI_tai_convertToJSON(tai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_area_of_validity_convertToJSON() failed [tai_list]");
                goto end;
            }
            cJSON_AddItemToArray(tai_listList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_area_of_validity_t *OpenAPI_area_of_validity_parseFromJSON(cJSON *area_of_validityJSON)
{
    OpenAPI_area_of_validity_t *area_of_validity_local_var = NULL;
    cJSON *tai_list = cJSON_GetObjectItemCaseSensitive(area_of_validityJSON, "taiList");
    if (!tai_list) {
        ogs_error("OpenAPI_area_of_validity_parseFromJSON() failed [tai_list]");
        goto end;
    }

    OpenAPI_list_t *tai_listList;

    cJSON *tai_list_local_nonprimitive;
    if (!cJSON_IsArray(tai_list)) {
        ogs_error("OpenAPI_area_of_validity_parseFromJSON() failed [tai_list]");
        goto end;
    }

    tai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(tai_list_local_nonprimitive, tai_list ) {
        if (!cJSON_IsObject(tai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_area_of_validity_parseFromJSON() failed [tai_list]");
            goto end;
        }
        OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local_nonprimitive);

        OpenAPI_list_add(tai_listList, tai_listItem);
    }

    area_of_validity_local_var = OpenAPI_area_of_validity_create (
        tai_listList
        );

    return area_of_validity_local_var;
end:
    return NULL;
}

OpenAPI_area_of_validity_t *OpenAPI_area_of_validity_copy(OpenAPI_area_of_validity_t *dst, OpenAPI_area_of_validity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_area_of_validity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_area_of_validity_convertToJSON() failed");
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

    OpenAPI_area_of_validity_free(dst);
    dst = OpenAPI_area_of_validity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

