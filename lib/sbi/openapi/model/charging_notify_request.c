
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_notify_request.h"

OpenAPI_charging_notify_request_t *OpenAPI_charging_notify_request_create(
    OpenAPI_charging_notification_type_e notification_type,
    OpenAPI_list_t *reauthorization_details
)
{
    OpenAPI_charging_notify_request_t *charging_notify_request_local_var = ogs_malloc(sizeof(OpenAPI_charging_notify_request_t));
    ogs_assert(charging_notify_request_local_var);

    charging_notify_request_local_var->notification_type = notification_type;
    charging_notify_request_local_var->reauthorization_details = reauthorization_details;

    return charging_notify_request_local_var;
}

void OpenAPI_charging_notify_request_free(OpenAPI_charging_notify_request_t *charging_notify_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_notify_request) {
        return;
    }
    if (charging_notify_request->reauthorization_details) {
        OpenAPI_list_for_each(charging_notify_request->reauthorization_details, node) {
            OpenAPI_reauthorization_details_free(node->data);
        }
        OpenAPI_list_free(charging_notify_request->reauthorization_details);
        charging_notify_request->reauthorization_details = NULL;
    }
    ogs_free(charging_notify_request);
}

cJSON *OpenAPI_charging_notify_request_convertToJSON(OpenAPI_charging_notify_request_t *charging_notify_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_notify_request == NULL) {
        ogs_error("OpenAPI_charging_notify_request_convertToJSON() failed [ChargingNotifyRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_notify_request->notification_type == OpenAPI_charging_notification_type_NULL) {
        ogs_error("OpenAPI_charging_notify_request_convertToJSON() failed [notification_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationType", OpenAPI_charging_notification_type_ToString(charging_notify_request->notification_type)) == NULL) {
        ogs_error("OpenAPI_charging_notify_request_convertToJSON() failed [notification_type]");
        goto end;
    }

    if (charging_notify_request->reauthorization_details) {
    cJSON *reauthorization_detailsList = cJSON_AddArrayToObject(item, "reauthorizationDetails");
    if (reauthorization_detailsList == NULL) {
        ogs_error("OpenAPI_charging_notify_request_convertToJSON() failed [reauthorization_details]");
        goto end;
    }
    OpenAPI_list_for_each(charging_notify_request->reauthorization_details, node) {
        cJSON *itemLocal = OpenAPI_reauthorization_details_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_charging_notify_request_convertToJSON() failed [reauthorization_details]");
            goto end;
        }
        cJSON_AddItemToArray(reauthorization_detailsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_charging_notify_request_t *OpenAPI_charging_notify_request_parseFromJSON(cJSON *charging_notify_requestJSON)
{
    OpenAPI_charging_notify_request_t *charging_notify_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_type = NULL;
    OpenAPI_charging_notification_type_e notification_typeVariable = 0;
    cJSON *reauthorization_details = NULL;
    OpenAPI_list_t *reauthorization_detailsList = NULL;
    notification_type = cJSON_GetObjectItemCaseSensitive(charging_notify_requestJSON, "notificationType");
    if (!notification_type) {
        ogs_error("OpenAPI_charging_notify_request_parseFromJSON() failed [notification_type]");
        goto end;
    }
    if (!cJSON_IsString(notification_type)) {
        ogs_error("OpenAPI_charging_notify_request_parseFromJSON() failed [notification_type]");
        goto end;
    }
    notification_typeVariable = OpenAPI_charging_notification_type_FromString(notification_type->valuestring);

    reauthorization_details = cJSON_GetObjectItemCaseSensitive(charging_notify_requestJSON, "reauthorizationDetails");
    if (reauthorization_details) {
        cJSON *reauthorization_details_local = NULL;
        if (!cJSON_IsArray(reauthorization_details)) {
            ogs_error("OpenAPI_charging_notify_request_parseFromJSON() failed [reauthorization_details]");
            goto end;
        }

        reauthorization_detailsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reauthorization_details_local, reauthorization_details) {
            if (!cJSON_IsObject(reauthorization_details_local)) {
                ogs_error("OpenAPI_charging_notify_request_parseFromJSON() failed [reauthorization_details]");
                goto end;
            }
            OpenAPI_reauthorization_details_t *reauthorization_detailsItem = OpenAPI_reauthorization_details_parseFromJSON(reauthorization_details_local);
            if (!reauthorization_detailsItem) {
                ogs_error("No reauthorization_detailsItem");
                goto end;
            }
            OpenAPI_list_add(reauthorization_detailsList, reauthorization_detailsItem);
        }
    }

    charging_notify_request_local_var = OpenAPI_charging_notify_request_create (
        notification_typeVariable,
        reauthorization_details ? reauthorization_detailsList : NULL
    );

    return charging_notify_request_local_var;
end:
    if (reauthorization_detailsList) {
        OpenAPI_list_for_each(reauthorization_detailsList, node) {
            OpenAPI_reauthorization_details_free(node->data);
        }
        OpenAPI_list_free(reauthorization_detailsList);
        reauthorization_detailsList = NULL;
    }
    return NULL;
}

OpenAPI_charging_notify_request_t *OpenAPI_charging_notify_request_copy(OpenAPI_charging_notify_request_t *dst, OpenAPI_charging_notify_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_notify_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_notify_request_convertToJSON() failed");
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

    OpenAPI_charging_notify_request_free(dst);
    dst = OpenAPI_charging_notify_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

