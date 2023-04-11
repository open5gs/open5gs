
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_subscription_info.h"

OpenAPI_hss_subscription_info_t *OpenAPI_hss_subscription_info_create(
    OpenAPI_list_t *hss_subscription_list
)
{
    OpenAPI_hss_subscription_info_t *hss_subscription_info_local_var = ogs_malloc(sizeof(OpenAPI_hss_subscription_info_t));
    ogs_assert(hss_subscription_info_local_var);

    hss_subscription_info_local_var->hss_subscription_list = hss_subscription_list;

    return hss_subscription_info_local_var;
}

void OpenAPI_hss_subscription_info_free(OpenAPI_hss_subscription_info_t *hss_subscription_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == hss_subscription_info) {
        return;
    }
    if (hss_subscription_info->hss_subscription_list) {
        OpenAPI_list_for_each(hss_subscription_info->hss_subscription_list, node) {
            OpenAPI_hss_subscription_item_free(node->data);
        }
        OpenAPI_list_free(hss_subscription_info->hss_subscription_list);
        hss_subscription_info->hss_subscription_list = NULL;
    }
    ogs_free(hss_subscription_info);
}

cJSON *OpenAPI_hss_subscription_info_convertToJSON(OpenAPI_hss_subscription_info_t *hss_subscription_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (hss_subscription_info == NULL) {
        ogs_error("OpenAPI_hss_subscription_info_convertToJSON() failed [HssSubscriptionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!hss_subscription_info->hss_subscription_list) {
        ogs_error("OpenAPI_hss_subscription_info_convertToJSON() failed [hss_subscription_list]");
        return NULL;
    }
    cJSON *hss_subscription_listList = cJSON_AddArrayToObject(item, "hssSubscriptionList");
    if (hss_subscription_listList == NULL) {
        ogs_error("OpenAPI_hss_subscription_info_convertToJSON() failed [hss_subscription_list]");
        goto end;
    }
    OpenAPI_list_for_each(hss_subscription_info->hss_subscription_list, node) {
        cJSON *itemLocal = OpenAPI_hss_subscription_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hss_subscription_info_convertToJSON() failed [hss_subscription_list]");
            goto end;
        }
        cJSON_AddItemToArray(hss_subscription_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_hss_subscription_info_t *OpenAPI_hss_subscription_info_parseFromJSON(cJSON *hss_subscription_infoJSON)
{
    OpenAPI_hss_subscription_info_t *hss_subscription_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *hss_subscription_list = NULL;
    OpenAPI_list_t *hss_subscription_listList = NULL;
    hss_subscription_list = cJSON_GetObjectItemCaseSensitive(hss_subscription_infoJSON, "hssSubscriptionList");
    if (!hss_subscription_list) {
        ogs_error("OpenAPI_hss_subscription_info_parseFromJSON() failed [hss_subscription_list]");
        goto end;
    }
        cJSON *hss_subscription_list_local = NULL;
        if (!cJSON_IsArray(hss_subscription_list)) {
            ogs_error("OpenAPI_hss_subscription_info_parseFromJSON() failed [hss_subscription_list]");
            goto end;
        }

        hss_subscription_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(hss_subscription_list_local, hss_subscription_list) {
            if (!cJSON_IsObject(hss_subscription_list_local)) {
                ogs_error("OpenAPI_hss_subscription_info_parseFromJSON() failed [hss_subscription_list]");
                goto end;
            }
            OpenAPI_hss_subscription_item_t *hss_subscription_listItem = OpenAPI_hss_subscription_item_parseFromJSON(hss_subscription_list_local);
            if (!hss_subscription_listItem) {
                ogs_error("No hss_subscription_listItem");
                goto end;
            }
            OpenAPI_list_add(hss_subscription_listList, hss_subscription_listItem);
        }

    hss_subscription_info_local_var = OpenAPI_hss_subscription_info_create (
        hss_subscription_listList
    );

    return hss_subscription_info_local_var;
end:
    if (hss_subscription_listList) {
        OpenAPI_list_for_each(hss_subscription_listList, node) {
            OpenAPI_hss_subscription_item_free(node->data);
        }
        OpenAPI_list_free(hss_subscription_listList);
        hss_subscription_listList = NULL;
    }
    return NULL;
}

OpenAPI_hss_subscription_info_t *OpenAPI_hss_subscription_info_copy(OpenAPI_hss_subscription_info_t *dst, OpenAPI_hss_subscription_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_subscription_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_subscription_info_convertToJSON() failed");
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

    OpenAPI_hss_subscription_info_free(dst);
    dst = OpenAPI_hss_subscription_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

