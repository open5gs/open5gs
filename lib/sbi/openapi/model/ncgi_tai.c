
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ncgi_tai.h"

OpenAPI_ncgi_tai_t *OpenAPI_ncgi_tai_create(
    OpenAPI_tai_t *tai,
    OpenAPI_list_t *cell_list
)
{
    OpenAPI_ncgi_tai_t *ncgi_tai_local_var = ogs_malloc(sizeof(OpenAPI_ncgi_tai_t));
    ogs_assert(ncgi_tai_local_var);

    ncgi_tai_local_var->tai = tai;
    ncgi_tai_local_var->cell_list = cell_list;

    return ncgi_tai_local_var;
}

void OpenAPI_ncgi_tai_free(OpenAPI_ncgi_tai_t *ncgi_tai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ncgi_tai) {
        return;
    }
    if (ncgi_tai->tai) {
        OpenAPI_tai_free(ncgi_tai->tai);
        ncgi_tai->tai = NULL;
    }
    if (ncgi_tai->cell_list) {
        OpenAPI_list_for_each(ncgi_tai->cell_list, node) {
            OpenAPI_ncgi_free(node->data);
        }
        OpenAPI_list_free(ncgi_tai->cell_list);
        ncgi_tai->cell_list = NULL;
    }
    ogs_free(ncgi_tai);
}

cJSON *OpenAPI_ncgi_tai_convertToJSON(OpenAPI_ncgi_tai_t *ncgi_tai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ncgi_tai == NULL) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [NcgiTai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ncgi_tai->tai) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [tai]");
        return NULL;
    }
    cJSON *tai_local_JSON = OpenAPI_tai_convertToJSON(ncgi_tai->tai);
    if (tai_local_JSON == NULL) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [tai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tai", tai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [tai]");
        goto end;
    }

    if (!ncgi_tai->cell_list) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [cell_list]");
        return NULL;
    }
    cJSON *cell_listList = cJSON_AddArrayToObject(item, "cellList");
    if (cell_listList == NULL) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [cell_list]");
        goto end;
    }
    OpenAPI_list_for_each(ncgi_tai->cell_list, node) {
        cJSON *itemLocal = OpenAPI_ncgi_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed [cell_list]");
            goto end;
        }
        cJSON_AddItemToArray(cell_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_ncgi_tai_t *OpenAPI_ncgi_tai_parseFromJSON(cJSON *ncgi_taiJSON)
{
    OpenAPI_ncgi_tai_t *ncgi_tai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai = NULL;
    OpenAPI_tai_t *tai_local_nonprim = NULL;
    cJSON *cell_list = NULL;
    OpenAPI_list_t *cell_listList = NULL;
    tai = cJSON_GetObjectItemCaseSensitive(ncgi_taiJSON, "tai");
    if (!tai) {
        ogs_error("OpenAPI_ncgi_tai_parseFromJSON() failed [tai]");
        goto end;
    }
    tai_local_nonprim = OpenAPI_tai_parseFromJSON(tai);
    if (!tai_local_nonprim) {
        ogs_error("OpenAPI_tai_parseFromJSON failed [tai]");
        goto end;
    }

    cell_list = cJSON_GetObjectItemCaseSensitive(ncgi_taiJSON, "cellList");
    if (!cell_list) {
        ogs_error("OpenAPI_ncgi_tai_parseFromJSON() failed [cell_list]");
        goto end;
    }
        cJSON *cell_list_local = NULL;
        if (!cJSON_IsArray(cell_list)) {
            ogs_error("OpenAPI_ncgi_tai_parseFromJSON() failed [cell_list]");
            goto end;
        }

        cell_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(cell_list_local, cell_list) {
            if (!cJSON_IsObject(cell_list_local)) {
                ogs_error("OpenAPI_ncgi_tai_parseFromJSON() failed [cell_list]");
                goto end;
            }
            OpenAPI_ncgi_t *cell_listItem = OpenAPI_ncgi_parseFromJSON(cell_list_local);
            if (!cell_listItem) {
                ogs_error("No cell_listItem");
                goto end;
            }
            OpenAPI_list_add(cell_listList, cell_listItem);
        }

    ncgi_tai_local_var = OpenAPI_ncgi_tai_create (
        tai_local_nonprim,
        cell_listList
    );

    return ncgi_tai_local_var;
end:
    if (tai_local_nonprim) {
        OpenAPI_tai_free(tai_local_nonprim);
        tai_local_nonprim = NULL;
    }
    if (cell_listList) {
        OpenAPI_list_for_each(cell_listList, node) {
            OpenAPI_ncgi_free(node->data);
        }
        OpenAPI_list_free(cell_listList);
        cell_listList = NULL;
    }
    return NULL;
}

OpenAPI_ncgi_tai_t *OpenAPI_ncgi_tai_copy(OpenAPI_ncgi_tai_t *dst, OpenAPI_ncgi_tai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ncgi_tai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ncgi_tai_convertToJSON() failed");
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

    OpenAPI_ncgi_tai_free(dst);
    dst = OpenAPI_ncgi_tai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

