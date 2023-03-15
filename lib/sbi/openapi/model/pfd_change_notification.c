
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_change_notification.h"

OpenAPI_pfd_change_notification_t *OpenAPI_pfd_change_notification_create(
    char *application_id,
    bool is_removal_flag,
    int removal_flag,
    bool is_partial_flag,
    int partial_flag,
    OpenAPI_list_t *pfds
)
{
    OpenAPI_pfd_change_notification_t *pfd_change_notification_local_var = ogs_malloc(sizeof(OpenAPI_pfd_change_notification_t));
    ogs_assert(pfd_change_notification_local_var);

    pfd_change_notification_local_var->application_id = application_id;
    pfd_change_notification_local_var->is_removal_flag = is_removal_flag;
    pfd_change_notification_local_var->removal_flag = removal_flag;
    pfd_change_notification_local_var->is_partial_flag = is_partial_flag;
    pfd_change_notification_local_var->partial_flag = partial_flag;
    pfd_change_notification_local_var->pfds = pfds;

    return pfd_change_notification_local_var;
}

void OpenAPI_pfd_change_notification_free(OpenAPI_pfd_change_notification_t *pfd_change_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pfd_change_notification) {
        return;
    }
    if (pfd_change_notification->application_id) {
        ogs_free(pfd_change_notification->application_id);
        pfd_change_notification->application_id = NULL;
    }
    if (pfd_change_notification->pfds) {
        OpenAPI_list_for_each(pfd_change_notification->pfds, node) {
            OpenAPI_pfd_content_free(node->data);
        }
        OpenAPI_list_free(pfd_change_notification->pfds);
        pfd_change_notification->pfds = NULL;
    }
    ogs_free(pfd_change_notification);
}

cJSON *OpenAPI_pfd_change_notification_convertToJSON(OpenAPI_pfd_change_notification_t *pfd_change_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pfd_change_notification == NULL) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [PfdChangeNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pfd_change_notification->application_id) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [application_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "applicationId", pfd_change_notification->application_id) == NULL) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [application_id]");
        goto end;
    }

    if (pfd_change_notification->is_removal_flag) {
    if (cJSON_AddBoolToObject(item, "removalFlag", pfd_change_notification->removal_flag) == NULL) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [removal_flag]");
        goto end;
    }
    }

    if (pfd_change_notification->is_partial_flag) {
    if (cJSON_AddBoolToObject(item, "partialFlag", pfd_change_notification->partial_flag) == NULL) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [partial_flag]");
        goto end;
    }
    }

    if (pfd_change_notification->pfds) {
    cJSON *pfdsList = cJSON_AddArrayToObject(item, "pfds");
    if (pfdsList == NULL) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [pfds]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_change_notification->pfds, node) {
        cJSON *itemLocal = OpenAPI_pfd_content_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed [pfds]");
            goto end;
        }
        cJSON_AddItemToArray(pfdsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_pfd_change_notification_t *OpenAPI_pfd_change_notification_parseFromJSON(cJSON *pfd_change_notificationJSON)
{
    OpenAPI_pfd_change_notification_t *pfd_change_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *application_id = NULL;
    cJSON *removal_flag = NULL;
    cJSON *partial_flag = NULL;
    cJSON *pfds = NULL;
    OpenAPI_list_t *pfdsList = NULL;
    application_id = cJSON_GetObjectItemCaseSensitive(pfd_change_notificationJSON, "applicationId");
    if (!application_id) {
        ogs_error("OpenAPI_pfd_change_notification_parseFromJSON() failed [application_id]");
        goto end;
    }
    if (!cJSON_IsString(application_id)) {
        ogs_error("OpenAPI_pfd_change_notification_parseFromJSON() failed [application_id]");
        goto end;
    }

    removal_flag = cJSON_GetObjectItemCaseSensitive(pfd_change_notificationJSON, "removalFlag");
    if (removal_flag) {
    if (!cJSON_IsBool(removal_flag)) {
        ogs_error("OpenAPI_pfd_change_notification_parseFromJSON() failed [removal_flag]");
        goto end;
    }
    }

    partial_flag = cJSON_GetObjectItemCaseSensitive(pfd_change_notificationJSON, "partialFlag");
    if (partial_flag) {
    if (!cJSON_IsBool(partial_flag)) {
        ogs_error("OpenAPI_pfd_change_notification_parseFromJSON() failed [partial_flag]");
        goto end;
    }
    }

    pfds = cJSON_GetObjectItemCaseSensitive(pfd_change_notificationJSON, "pfds");
    if (pfds) {
        cJSON *pfds_local = NULL;
        if (!cJSON_IsArray(pfds)) {
            ogs_error("OpenAPI_pfd_change_notification_parseFromJSON() failed [pfds]");
            goto end;
        }

        pfdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pfds_local, pfds) {
            if (!cJSON_IsObject(pfds_local)) {
                ogs_error("OpenAPI_pfd_change_notification_parseFromJSON() failed [pfds]");
                goto end;
            }
            OpenAPI_pfd_content_t *pfdsItem = OpenAPI_pfd_content_parseFromJSON(pfds_local);
            if (!pfdsItem) {
                ogs_error("No pfdsItem");
                goto end;
            }
            OpenAPI_list_add(pfdsList, pfdsItem);
        }
    }

    pfd_change_notification_local_var = OpenAPI_pfd_change_notification_create (
        ogs_strdup(application_id->valuestring),
        removal_flag ? true : false,
        removal_flag ? removal_flag->valueint : 0,
        partial_flag ? true : false,
        partial_flag ? partial_flag->valueint : 0,
        pfds ? pfdsList : NULL
    );

    return pfd_change_notification_local_var;
end:
    if (pfdsList) {
        OpenAPI_list_for_each(pfdsList, node) {
            OpenAPI_pfd_content_free(node->data);
        }
        OpenAPI_list_free(pfdsList);
        pfdsList = NULL;
    }
    return NULL;
}

OpenAPI_pfd_change_notification_t *OpenAPI_pfd_change_notification_copy(OpenAPI_pfd_change_notification_t *dst, OpenAPI_pfd_change_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_change_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_change_notification_convertToJSON() failed");
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

    OpenAPI_pfd_change_notification_free(dst);
    dst = OpenAPI_pfd_change_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

