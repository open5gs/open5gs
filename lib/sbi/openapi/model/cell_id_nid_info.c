
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cell_id_nid_info.h"

OpenAPI_cell_id_nid_info_t *OpenAPI_cell_id_nid_info_create(
    OpenAPI_list_t *cell_id_nid_list
)
{
    OpenAPI_cell_id_nid_info_t *cell_id_nid_info_local_var = ogs_malloc(sizeof(OpenAPI_cell_id_nid_info_t));
    ogs_assert(cell_id_nid_info_local_var);

    cell_id_nid_info_local_var->cell_id_nid_list = cell_id_nid_list;

    return cell_id_nid_info_local_var;
}

void OpenAPI_cell_id_nid_info_free(OpenAPI_cell_id_nid_info_t *cell_id_nid_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cell_id_nid_info) {
        return;
    }
    if (cell_id_nid_info->cell_id_nid_list) {
        OpenAPI_list_for_each(cell_id_nid_info->cell_id_nid_list, node) {
            OpenAPI_cell_id_nid_free(node->data);
        }
        OpenAPI_list_free(cell_id_nid_info->cell_id_nid_list);
        cell_id_nid_info->cell_id_nid_list = NULL;
    }
    ogs_free(cell_id_nid_info);
}

cJSON *OpenAPI_cell_id_nid_info_convertToJSON(OpenAPI_cell_id_nid_info_t *cell_id_nid_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cell_id_nid_info == NULL) {
        ogs_error("OpenAPI_cell_id_nid_info_convertToJSON() failed [CellIdNidInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cell_id_nid_info->cell_id_nid_list) {
        ogs_error("OpenAPI_cell_id_nid_info_convertToJSON() failed [cell_id_nid_list]");
        return NULL;
    }
    cJSON *cell_id_nid_listList = cJSON_AddArrayToObject(item, "cellIdNidList");
    if (cell_id_nid_listList == NULL) {
        ogs_error("OpenAPI_cell_id_nid_info_convertToJSON() failed [cell_id_nid_list]");
        goto end;
    }
    OpenAPI_list_for_each(cell_id_nid_info->cell_id_nid_list, node) {
        cJSON *itemLocal = OpenAPI_cell_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_cell_id_nid_info_convertToJSON() failed [cell_id_nid_list]");
            goto end;
        }
        cJSON_AddItemToArray(cell_id_nid_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_cell_id_nid_info_t *OpenAPI_cell_id_nid_info_parseFromJSON(cJSON *cell_id_nid_infoJSON)
{
    OpenAPI_cell_id_nid_info_t *cell_id_nid_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cell_id_nid_list = NULL;
    OpenAPI_list_t *cell_id_nid_listList = NULL;
    cell_id_nid_list = cJSON_GetObjectItemCaseSensitive(cell_id_nid_infoJSON, "cellIdNidList");
    if (!cell_id_nid_list) {
        ogs_error("OpenAPI_cell_id_nid_info_parseFromJSON() failed [cell_id_nid_list]");
        goto end;
    }
        cJSON *cell_id_nid_list_local = NULL;
        if (!cJSON_IsArray(cell_id_nid_list)) {
            ogs_error("OpenAPI_cell_id_nid_info_parseFromJSON() failed [cell_id_nid_list]");
            goto end;
        }

        cell_id_nid_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(cell_id_nid_list_local, cell_id_nid_list) {
            if (!cJSON_IsObject(cell_id_nid_list_local)) {
                ogs_error("OpenAPI_cell_id_nid_info_parseFromJSON() failed [cell_id_nid_list]");
                goto end;
            }
            OpenAPI_cell_id_nid_t *cell_id_nid_listItem = OpenAPI_cell_id_nid_parseFromJSON(cell_id_nid_list_local);
            if (!cell_id_nid_listItem) {
                ogs_error("No cell_id_nid_listItem");
                goto end;
            }
            OpenAPI_list_add(cell_id_nid_listList, cell_id_nid_listItem);
        }

    cell_id_nid_info_local_var = OpenAPI_cell_id_nid_info_create (
        cell_id_nid_listList
    );

    return cell_id_nid_info_local_var;
end:
    if (cell_id_nid_listList) {
        OpenAPI_list_for_each(cell_id_nid_listList, node) {
            OpenAPI_cell_id_nid_free(node->data);
        }
        OpenAPI_list_free(cell_id_nid_listList);
        cell_id_nid_listList = NULL;
    }
    return NULL;
}

OpenAPI_cell_id_nid_info_t *OpenAPI_cell_id_nid_info_copy(OpenAPI_cell_id_nid_info_t *dst, OpenAPI_cell_id_nid_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cell_id_nid_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cell_id_nid_info_convertToJSON() failed");
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

    OpenAPI_cell_id_nid_info_free(dst);
    dst = OpenAPI_cell_id_nid_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

