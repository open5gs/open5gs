
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_notification_rsp_data.h"

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_create(
    OpenAPI_list_t *sec_rat_data_usage_list
)
{
    OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data_local_var = ogs_malloc(sizeof(OpenAPI_n2_info_notification_rsp_data_t));
    ogs_assert(n2_info_notification_rsp_data_local_var);

    n2_info_notification_rsp_data_local_var->sec_rat_data_usage_list = sec_rat_data_usage_list;

    return n2_info_notification_rsp_data_local_var;
}

void OpenAPI_n2_info_notification_rsp_data_free(OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_info_notification_rsp_data) {
        return;
    }
    if (n2_info_notification_rsp_data->sec_rat_data_usage_list) {
        OpenAPI_list_for_each(n2_info_notification_rsp_data->sec_rat_data_usage_list, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(n2_info_notification_rsp_data->sec_rat_data_usage_list);
        n2_info_notification_rsp_data->sec_rat_data_usage_list = NULL;
    }
    ogs_free(n2_info_notification_rsp_data);
}

cJSON *OpenAPI_n2_info_notification_rsp_data_convertToJSON(OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_info_notification_rsp_data == NULL) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed [N2InfoNotificationRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_info_notification_rsp_data->sec_rat_data_usage_list) {
    cJSON *sec_rat_data_usage_listList = cJSON_AddArrayToObject(item, "secRatDataUsageList");
    if (sec_rat_data_usage_listList == NULL) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed [sec_rat_data_usage_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_info_notification_rsp_data->sec_rat_data_usage_list, node) {
        cJSON *itemLocal = OpenAPI_n2_sm_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed [sec_rat_data_usage_list]");
            goto end;
        }
        cJSON_AddItemToArray(sec_rat_data_usage_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_parseFromJSON(cJSON *n2_info_notification_rsp_dataJSON)
{
    OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sec_rat_data_usage_list = NULL;
    OpenAPI_list_t *sec_rat_data_usage_listList = NULL;
    sec_rat_data_usage_list = cJSON_GetObjectItemCaseSensitive(n2_info_notification_rsp_dataJSON, "secRatDataUsageList");
    if (sec_rat_data_usage_list) {
        cJSON *sec_rat_data_usage_list_local = NULL;
        if (!cJSON_IsArray(sec_rat_data_usage_list)) {
            ogs_error("OpenAPI_n2_info_notification_rsp_data_parseFromJSON() failed [sec_rat_data_usage_list]");
            goto end;
        }

        sec_rat_data_usage_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sec_rat_data_usage_list_local, sec_rat_data_usage_list) {
            if (!cJSON_IsObject(sec_rat_data_usage_list_local)) {
                ogs_error("OpenAPI_n2_info_notification_rsp_data_parseFromJSON() failed [sec_rat_data_usage_list]");
                goto end;
            }
            OpenAPI_n2_sm_information_t *sec_rat_data_usage_listItem = OpenAPI_n2_sm_information_parseFromJSON(sec_rat_data_usage_list_local);
            if (!sec_rat_data_usage_listItem) {
                ogs_error("No sec_rat_data_usage_listItem");
                goto end;
            }
            OpenAPI_list_add(sec_rat_data_usage_listList, sec_rat_data_usage_listItem);
        }
    }

    n2_info_notification_rsp_data_local_var = OpenAPI_n2_info_notification_rsp_data_create (
        sec_rat_data_usage_list ? sec_rat_data_usage_listList : NULL
    );

    return n2_info_notification_rsp_data_local_var;
end:
    if (sec_rat_data_usage_listList) {
        OpenAPI_list_for_each(sec_rat_data_usage_listList, node) {
            OpenAPI_n2_sm_information_free(node->data);
        }
        OpenAPI_list_free(sec_rat_data_usage_listList);
        sec_rat_data_usage_listList = NULL;
    }
    return NULL;
}

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_copy(OpenAPI_n2_info_notification_rsp_data_t *dst, OpenAPI_n2_info_notification_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_info_notification_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed");
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

    OpenAPI_n2_info_notification_rsp_data_free(dst);
    dst = OpenAPI_n2_info_notification_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

