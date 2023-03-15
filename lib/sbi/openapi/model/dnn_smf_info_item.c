
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_smf_info_item.h"

OpenAPI_dnn_smf_info_item_t *OpenAPI_dnn_smf_info_item_create(
    char *dnn,
    OpenAPI_list_t *dnai_list
)
{
    OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item_local_var = ogs_malloc(sizeof(OpenAPI_dnn_smf_info_item_t));
    ogs_assert(dnn_smf_info_item_local_var);

    dnn_smf_info_item_local_var->dnn = dnn;
    dnn_smf_info_item_local_var->dnai_list = dnai_list;

    return dnn_smf_info_item_local_var;
}

void OpenAPI_dnn_smf_info_item_free(OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_smf_info_item) {
        return;
    }
    if (dnn_smf_info_item->dnn) {
        ogs_free(dnn_smf_info_item->dnn);
        dnn_smf_info_item->dnn = NULL;
    }
    if (dnn_smf_info_item->dnai_list) {
        OpenAPI_list_for_each(dnn_smf_info_item->dnai_list, node) {
            OpenAPI_dnn_smf_info_item_dnai_list_inner_free(node->data);
        }
        OpenAPI_list_free(dnn_smf_info_item->dnai_list);
        dnn_smf_info_item->dnai_list = NULL;
    }
    ogs_free(dnn_smf_info_item);
}

cJSON *OpenAPI_dnn_smf_info_item_convertToJSON(OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_smf_info_item == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [DnnSmfInfoItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_smf_info_item->dnn) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_smf_info_item->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_smf_info_item->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_smf_info_item->dnai_list, node) {
        cJSON *itemLocal = OpenAPI_dnn_smf_info_item_dnai_list_inner_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed [dnai_list]");
            goto end;
        }
        cJSON_AddItemToArray(dnai_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_dnn_smf_info_item_t *OpenAPI_dnn_smf_info_item_parseFromJSON(cJSON *dnn_smf_info_itemJSON)
{
    OpenAPI_dnn_smf_info_item_t *dnn_smf_info_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }

    dnai_list = cJSON_GetObjectItemCaseSensitive(dnn_smf_info_itemJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            if (!cJSON_IsObject(dnai_list_local)) {
                ogs_error("OpenAPI_dnn_smf_info_item_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnai_listItem = OpenAPI_dnn_smf_info_item_dnai_list_inner_parseFromJSON(dnai_list_local);
            if (!dnai_listItem) {
                ogs_error("No dnai_listItem");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, dnai_listItem);
        }
    }

    dnn_smf_info_item_local_var = OpenAPI_dnn_smf_info_item_create (
        ogs_strdup(dnn->valuestring),
        dnai_list ? dnai_listList : NULL
    );

    return dnn_smf_info_item_local_var;
end:
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            OpenAPI_dnn_smf_info_item_dnai_list_inner_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    return NULL;
}

OpenAPI_dnn_smf_info_item_t *OpenAPI_dnn_smf_info_item_copy(OpenAPI_dnn_smf_info_item_t *dst, OpenAPI_dnn_smf_info_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_smf_info_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_smf_info_item_convertToJSON() failed");
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

    OpenAPI_dnn_smf_info_item_free(dst);
    dst = OpenAPI_dnn_smf_info_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

