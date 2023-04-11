
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_status_change_notification.h"

OpenAPI_amf_status_change_notification_t *OpenAPI_amf_status_change_notification_create(
    OpenAPI_list_t *amf_status_info_list
)
{
    OpenAPI_amf_status_change_notification_t *amf_status_change_notification_local_var = ogs_malloc(sizeof(OpenAPI_amf_status_change_notification_t));
    ogs_assert(amf_status_change_notification_local_var);

    amf_status_change_notification_local_var->amf_status_info_list = amf_status_info_list;

    return amf_status_change_notification_local_var;
}

void OpenAPI_amf_status_change_notification_free(OpenAPI_amf_status_change_notification_t *amf_status_change_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_status_change_notification) {
        return;
    }
    if (amf_status_change_notification->amf_status_info_list) {
        OpenAPI_list_for_each(amf_status_change_notification->amf_status_info_list, node) {
            OpenAPI_amf_status_info_free(node->data);
        }
        OpenAPI_list_free(amf_status_change_notification->amf_status_info_list);
        amf_status_change_notification->amf_status_info_list = NULL;
    }
    ogs_free(amf_status_change_notification);
}

cJSON *OpenAPI_amf_status_change_notification_convertToJSON(OpenAPI_amf_status_change_notification_t *amf_status_change_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_status_change_notification == NULL) {
        ogs_error("OpenAPI_amf_status_change_notification_convertToJSON() failed [AmfStatusChangeNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_status_change_notification->amf_status_info_list) {
        ogs_error("OpenAPI_amf_status_change_notification_convertToJSON() failed [amf_status_info_list]");
        return NULL;
    }
    cJSON *amf_status_info_listList = cJSON_AddArrayToObject(item, "amfStatusInfoList");
    if (amf_status_info_listList == NULL) {
        ogs_error("OpenAPI_amf_status_change_notification_convertToJSON() failed [amf_status_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_status_change_notification->amf_status_info_list, node) {
        cJSON *itemLocal = OpenAPI_amf_status_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_status_change_notification_convertToJSON() failed [amf_status_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(amf_status_info_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_amf_status_change_notification_t *OpenAPI_amf_status_change_notification_parseFromJSON(cJSON *amf_status_change_notificationJSON)
{
    OpenAPI_amf_status_change_notification_t *amf_status_change_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_status_info_list = NULL;
    OpenAPI_list_t *amf_status_info_listList = NULL;
    amf_status_info_list = cJSON_GetObjectItemCaseSensitive(amf_status_change_notificationJSON, "amfStatusInfoList");
    if (!amf_status_info_list) {
        ogs_error("OpenAPI_amf_status_change_notification_parseFromJSON() failed [amf_status_info_list]");
        goto end;
    }
        cJSON *amf_status_info_list_local = NULL;
        if (!cJSON_IsArray(amf_status_info_list)) {
            ogs_error("OpenAPI_amf_status_change_notification_parseFromJSON() failed [amf_status_info_list]");
            goto end;
        }

        amf_status_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(amf_status_info_list_local, amf_status_info_list) {
            if (!cJSON_IsObject(amf_status_info_list_local)) {
                ogs_error("OpenAPI_amf_status_change_notification_parseFromJSON() failed [amf_status_info_list]");
                goto end;
            }
            OpenAPI_amf_status_info_t *amf_status_info_listItem = OpenAPI_amf_status_info_parseFromJSON(amf_status_info_list_local);
            if (!amf_status_info_listItem) {
                ogs_error("No amf_status_info_listItem");
                goto end;
            }
            OpenAPI_list_add(amf_status_info_listList, amf_status_info_listItem);
        }

    amf_status_change_notification_local_var = OpenAPI_amf_status_change_notification_create (
        amf_status_info_listList
    );

    return amf_status_change_notification_local_var;
end:
    if (amf_status_info_listList) {
        OpenAPI_list_for_each(amf_status_info_listList, node) {
            OpenAPI_amf_status_info_free(node->data);
        }
        OpenAPI_list_free(amf_status_info_listList);
        amf_status_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_amf_status_change_notification_t *OpenAPI_amf_status_change_notification_copy(OpenAPI_amf_status_change_notification_t *dst, OpenAPI_amf_status_change_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_status_change_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_status_change_notification_convertToJSON() failed");
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

    OpenAPI_amf_status_change_notification_free(dst);
    dst = OpenAPI_amf_status_change_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

