
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tac_nid_info.h"

OpenAPI_tac_nid_info_t *OpenAPI_tac_nid_info_create(
    OpenAPI_list_t *tac_nid_list
)
{
    OpenAPI_tac_nid_info_t *tac_nid_info_local_var = ogs_malloc(sizeof(OpenAPI_tac_nid_info_t));
    ogs_assert(tac_nid_info_local_var);

    tac_nid_info_local_var->tac_nid_list = tac_nid_list;

    return tac_nid_info_local_var;
}

void OpenAPI_tac_nid_info_free(OpenAPI_tac_nid_info_t *tac_nid_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tac_nid_info) {
        return;
    }
    if (tac_nid_info->tac_nid_list) {
        OpenAPI_list_for_each(tac_nid_info->tac_nid_list, node) {
            OpenAPI_tac_nid_free(node->data);
        }
        OpenAPI_list_free(tac_nid_info->tac_nid_list);
        tac_nid_info->tac_nid_list = NULL;
    }
    ogs_free(tac_nid_info);
}

cJSON *OpenAPI_tac_nid_info_convertToJSON(OpenAPI_tac_nid_info_t *tac_nid_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tac_nid_info == NULL) {
        ogs_error("OpenAPI_tac_nid_info_convertToJSON() failed [TacNidInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tac_nid_info->tac_nid_list) {
        ogs_error("OpenAPI_tac_nid_info_convertToJSON() failed [tac_nid_list]");
        return NULL;
    }
    cJSON *tac_nid_listList = cJSON_AddArrayToObject(item, "tacNidList");
    if (tac_nid_listList == NULL) {
        ogs_error("OpenAPI_tac_nid_info_convertToJSON() failed [tac_nid_list]");
        goto end;
    }
    OpenAPI_list_for_each(tac_nid_info->tac_nid_list, node) {
        cJSON *itemLocal = OpenAPI_tac_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tac_nid_info_convertToJSON() failed [tac_nid_list]");
            goto end;
        }
        cJSON_AddItemToArray(tac_nid_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_tac_nid_info_t *OpenAPI_tac_nid_info_parseFromJSON(cJSON *tac_nid_infoJSON)
{
    OpenAPI_tac_nid_info_t *tac_nid_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tac_nid_list = NULL;
    OpenAPI_list_t *tac_nid_listList = NULL;
    tac_nid_list = cJSON_GetObjectItemCaseSensitive(tac_nid_infoJSON, "tacNidList");
    if (!tac_nid_list) {
        ogs_error("OpenAPI_tac_nid_info_parseFromJSON() failed [tac_nid_list]");
        goto end;
    }
        cJSON *tac_nid_list_local = NULL;
        if (!cJSON_IsArray(tac_nid_list)) {
            ogs_error("OpenAPI_tac_nid_info_parseFromJSON() failed [tac_nid_list]");
            goto end;
        }

        tac_nid_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tac_nid_list_local, tac_nid_list) {
            if (!cJSON_IsObject(tac_nid_list_local)) {
                ogs_error("OpenAPI_tac_nid_info_parseFromJSON() failed [tac_nid_list]");
                goto end;
            }
            OpenAPI_tac_nid_t *tac_nid_listItem = OpenAPI_tac_nid_parseFromJSON(tac_nid_list_local);
            if (!tac_nid_listItem) {
                ogs_error("No tac_nid_listItem");
                goto end;
            }
            OpenAPI_list_add(tac_nid_listList, tac_nid_listItem);
        }

    tac_nid_info_local_var = OpenAPI_tac_nid_info_create (
        tac_nid_listList
    );

    return tac_nid_info_local_var;
end:
    if (tac_nid_listList) {
        OpenAPI_list_for_each(tac_nid_listList, node) {
            OpenAPI_tac_nid_free(node->data);
        }
        OpenAPI_list_free(tac_nid_listList);
        tac_nid_listList = NULL;
    }
    return NULL;
}

OpenAPI_tac_nid_info_t *OpenAPI_tac_nid_info_copy(OpenAPI_tac_nid_info_t *dst, OpenAPI_tac_nid_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tac_nid_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tac_nid_info_convertToJSON() failed");
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

    OpenAPI_tac_nid_info_free(dst);
    dst = OpenAPI_tac_nid_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

