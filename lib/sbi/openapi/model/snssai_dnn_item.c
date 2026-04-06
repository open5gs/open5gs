
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_dnn_item.h"

OpenAPI_snssai_dnn_item_t *OpenAPI_snssai_dnn_item_create(
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *dnn_list
)
{
    OpenAPI_snssai_dnn_item_t *snssai_dnn_item_local_var = ogs_malloc(sizeof(OpenAPI_snssai_dnn_item_t));
    ogs_assert(snssai_dnn_item_local_var);

    snssai_dnn_item_local_var->snssai_list = snssai_list;
    snssai_dnn_item_local_var->dnn_list = dnn_list;

    return snssai_dnn_item_local_var;
}

void OpenAPI_snssai_dnn_item_free(OpenAPI_snssai_dnn_item_t *snssai_dnn_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snssai_dnn_item) {
        return;
    }
    if (snssai_dnn_item->snssai_list) {
        OpenAPI_list_for_each(snssai_dnn_item->snssai_list, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_dnn_item->snssai_list);
        snssai_dnn_item->snssai_list = NULL;
    }
    if (snssai_dnn_item->dnn_list) {
        OpenAPI_list_for_each(snssai_dnn_item->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(snssai_dnn_item->dnn_list);
        snssai_dnn_item->dnn_list = NULL;
    }
    ogs_free(snssai_dnn_item);
}

cJSON *OpenAPI_snssai_dnn_item_convertToJSON(OpenAPI_snssai_dnn_item_t *snssai_dnn_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snssai_dnn_item == NULL) {
        ogs_error("OpenAPI_snssai_dnn_item_convertToJSON() failed [SnssaiDnnItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (snssai_dnn_item->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_snssai_dnn_item_convertToJSON() failed [snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(snssai_dnn_item->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_snssai_dnn_item_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }
    }

    if (snssai_dnn_item->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_snssai_dnn_item_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(snssai_dnn_item->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_snssai_dnn_item_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_snssai_dnn_item_t *OpenAPI_snssai_dnn_item_parseFromJSON(cJSON *snssai_dnn_itemJSON)
{
    OpenAPI_snssai_dnn_item_t *snssai_dnn_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    snssai_list = cJSON_GetObjectItemCaseSensitive(snssai_dnn_itemJSON, "snssaiList");
    if (snssai_list) {
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_snssai_dnn_item_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_snssai_dnn_item_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_ext_snssai_t *snssai_listItem = OpenAPI_ext_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }
    }

    dnn_list = cJSON_GetObjectItemCaseSensitive(snssai_dnn_itemJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_snssai_dnn_item_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_snssai_dnn_item_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    snssai_dnn_item_local_var = OpenAPI_snssai_dnn_item_create (
        snssai_list ? snssai_listList : NULL,
        dnn_list ? dnn_listList : NULL
    );

    return snssai_dnn_item_local_var;
end:
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
    }
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    return NULL;
}

OpenAPI_snssai_dnn_item_t *OpenAPI_snssai_dnn_item_copy(OpenAPI_snssai_dnn_item_t *dst, OpenAPI_snssai_dnn_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_dnn_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_dnn_item_convertToJSON() failed");
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

    OpenAPI_snssai_dnn_item_free(dst);
    dst = OpenAPI_snssai_dnn_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

