
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "status_notification.h"

OpenAPI_status_notification_t *OpenAPI_status_notification_create(
    OpenAPI_status_info_t *status_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status
)
{
    OpenAPI_status_notification_t *status_notification_local_var = ogs_malloc(sizeof(OpenAPI_status_notification_t));
    ogs_assert(status_notification_local_var);

    status_notification_local_var->status_info = status_info;
    status_notification_local_var->small_data_rate_status = small_data_rate_status;
    status_notification_local_var->apn_rate_status = apn_rate_status;

    return status_notification_local_var;
}

void OpenAPI_status_notification_free(OpenAPI_status_notification_t *status_notification)
{
    if (NULL == status_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_status_info_free(status_notification->status_info);
    OpenAPI_small_data_rate_status_free(status_notification->small_data_rate_status);
    OpenAPI_apn_rate_status_free(status_notification->apn_rate_status);
    ogs_free(status_notification);
}

cJSON *OpenAPI_status_notification_convertToJSON(OpenAPI_status_notification_t *status_notification)
{
    cJSON *item = NULL;

    if (status_notification == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [StatusNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *status_info_local_JSON = OpenAPI_status_info_convertToJSON(status_notification->status_info);
    if (status_info_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "statusInfo", status_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }

    if (status_notification->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(status_notification->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (status_notification->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(status_notification->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_status_notification_t *OpenAPI_status_notification_parseFromJSON(cJSON *status_notificationJSON)
{
    OpenAPI_status_notification_t *status_notification_local_var = NULL;
    cJSON *status_info = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "statusInfo");
    if (!status_info) {
        ogs_error("OpenAPI_status_notification_parseFromJSON() failed [status_info]");
        goto end;
    }

    OpenAPI_status_info_t *status_info_local_nonprim = NULL;
    status_info_local_nonprim = OpenAPI_status_info_parseFromJSON(status_info);

    cJSON *small_data_rate_status = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "smallDataRateStatus");

    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    }

    cJSON *apn_rate_status = cJSON_GetObjectItemCaseSensitive(status_notificationJSON, "apnRateStatus");

    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    }

    status_notification_local_var = OpenAPI_status_notification_create (
        status_info_local_nonprim,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL
    );

    return status_notification_local_var;
end:
    return NULL;
}

OpenAPI_status_notification_t *OpenAPI_status_notification_copy(OpenAPI_status_notification_t *dst, OpenAPI_status_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_status_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_status_notification_convertToJSON() failed");
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

    OpenAPI_status_notification_free(dst);
    dst = OpenAPI_status_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

