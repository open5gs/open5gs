
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_status_notification.h"

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_create(
    OpenAPI_status_info_t *status_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_ddn_failure_status,
    int ddn_failure_status,
    OpenAPI_list_t *notify_correlation_ids_forddn_failure,
    char *new_smf_id,
    char *new_smf_set_id,
    char *old_smf_id,
    char *old_sm_context_ref,
    char *alt_anchor_smf_uri,
    char *alt_anchor_smf_id
)
{
    OpenAPI_sm_context_status_notification_t *sm_context_status_notification_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_status_notification_t));
    ogs_assert(sm_context_status_notification_local_var);

    sm_context_status_notification_local_var->status_info = status_info;
    sm_context_status_notification_local_var->small_data_rate_status = small_data_rate_status;
    sm_context_status_notification_local_var->apn_rate_status = apn_rate_status;
    sm_context_status_notification_local_var->is_ddn_failure_status = is_ddn_failure_status;
    sm_context_status_notification_local_var->ddn_failure_status = ddn_failure_status;
    sm_context_status_notification_local_var->notify_correlation_ids_forddn_failure = notify_correlation_ids_forddn_failure;
    sm_context_status_notification_local_var->new_smf_id = new_smf_id;
    sm_context_status_notification_local_var->new_smf_set_id = new_smf_set_id;
    sm_context_status_notification_local_var->old_smf_id = old_smf_id;
    sm_context_status_notification_local_var->old_sm_context_ref = old_sm_context_ref;
    sm_context_status_notification_local_var->alt_anchor_smf_uri = alt_anchor_smf_uri;
    sm_context_status_notification_local_var->alt_anchor_smf_id = alt_anchor_smf_id;

    return sm_context_status_notification_local_var;
}

void OpenAPI_sm_context_status_notification_free(OpenAPI_sm_context_status_notification_t *sm_context_status_notification)
{
    if (NULL == sm_context_status_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_status_info_free(sm_context_status_notification->status_info);
    OpenAPI_small_data_rate_status_free(sm_context_status_notification->small_data_rate_status);
    OpenAPI_apn_rate_status_free(sm_context_status_notification->apn_rate_status);
    OpenAPI_list_for_each(sm_context_status_notification->notify_correlation_ids_forddn_failure, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_status_notification->notify_correlation_ids_forddn_failure);
    ogs_free(sm_context_status_notification->new_smf_id);
    ogs_free(sm_context_status_notification->new_smf_set_id);
    ogs_free(sm_context_status_notification->old_smf_id);
    ogs_free(sm_context_status_notification->old_sm_context_ref);
    ogs_free(sm_context_status_notification->alt_anchor_smf_uri);
    ogs_free(sm_context_status_notification->alt_anchor_smf_id);
    ogs_free(sm_context_status_notification);
}

cJSON *OpenAPI_sm_context_status_notification_convertToJSON(OpenAPI_sm_context_status_notification_t *sm_context_status_notification)
{
    cJSON *item = NULL;

    if (sm_context_status_notification == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [SmContextStatusNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *status_info_local_JSON = OpenAPI_status_info_convertToJSON(sm_context_status_notification->status_info);
    if (status_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "statusInfo", status_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [status_info]");
        goto end;
    }

    if (sm_context_status_notification->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(sm_context_status_notification->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (sm_context_status_notification->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(sm_context_status_notification->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

    if (sm_context_status_notification->is_ddn_failure_status) {
    if (cJSON_AddBoolToObject(item, "ddnFailureStatus", sm_context_status_notification->ddn_failure_status) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [ddn_failure_status]");
        goto end;
    }
    }

    if (sm_context_status_notification->notify_correlation_ids_forddn_failure) {
    cJSON *notify_correlation_ids_forddn_failure = cJSON_AddArrayToObject(item, "notifyCorrelationIdsForddnFailure");
    if (notify_correlation_ids_forddn_failure == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [notify_correlation_ids_forddn_failure]");
        goto end;
    }

    OpenAPI_lnode_t *notify_correlation_ids_forddn_failure_node;
    OpenAPI_list_for_each(sm_context_status_notification->notify_correlation_ids_forddn_failure, notify_correlation_ids_forddn_failure_node)  {
    if (cJSON_AddStringToObject(notify_correlation_ids_forddn_failure, "", (char*)notify_correlation_ids_forddn_failure_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [notify_correlation_ids_forddn_failure]");
        goto end;
    }
                    }
    }

    if (sm_context_status_notification->new_smf_id) {
    if (cJSON_AddStringToObject(item, "newSmfId", sm_context_status_notification->new_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [new_smf_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->new_smf_set_id) {
    if (cJSON_AddStringToObject(item, "newSmfSetId", sm_context_status_notification->new_smf_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [new_smf_set_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->old_smf_id) {
    if (cJSON_AddStringToObject(item, "oldSmfId", sm_context_status_notification->old_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [old_smf_id]");
        goto end;
    }
    }

    if (sm_context_status_notification->old_sm_context_ref) {
    if (cJSON_AddStringToObject(item, "oldSmContextRef", sm_context_status_notification->old_sm_context_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    if (sm_context_status_notification->alt_anchor_smf_uri) {
    if (cJSON_AddStringToObject(item, "altAnchorSmfUri", sm_context_status_notification->alt_anchor_smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [alt_anchor_smf_uri]");
        goto end;
    }
    }

    if (sm_context_status_notification->alt_anchor_smf_id) {
    if (cJSON_AddStringToObject(item, "altAnchorSmfId", sm_context_status_notification->alt_anchor_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed [alt_anchor_smf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_parseFromJSON(cJSON *sm_context_status_notificationJSON)
{
    OpenAPI_sm_context_status_notification_t *sm_context_status_notification_local_var = NULL;
    cJSON *status_info = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "statusInfo");
    if (!status_info) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [status_info]");
        goto end;
    }

    OpenAPI_status_info_t *status_info_local_nonprim = NULL;
    status_info_local_nonprim = OpenAPI_status_info_parseFromJSON(status_info);

    cJSON *small_data_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "smallDataRateStatus");

    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    }

    cJSON *apn_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "apnRateStatus");

    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    }

    cJSON *ddn_failure_status = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "ddnFailureStatus");

    if (ddn_failure_status) {
    if (!cJSON_IsBool(ddn_failure_status)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [ddn_failure_status]");
        goto end;
    }
    }

    cJSON *notify_correlation_ids_forddn_failure = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "notifyCorrelationIdsForddnFailure");

    OpenAPI_list_t *notify_correlation_ids_forddn_failureList;
    if (notify_correlation_ids_forddn_failure) {
    cJSON *notify_correlation_ids_forddn_failure_local;
    if (!cJSON_IsArray(notify_correlation_ids_forddn_failure)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [notify_correlation_ids_forddn_failure]");
        goto end;
    }
    notify_correlation_ids_forddn_failureList = OpenAPI_list_create();

    cJSON_ArrayForEach(notify_correlation_ids_forddn_failure_local, notify_correlation_ids_forddn_failure) {
    if (!cJSON_IsString(notify_correlation_ids_forddn_failure_local)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [notify_correlation_ids_forddn_failure]");
        goto end;
    }
    OpenAPI_list_add(notify_correlation_ids_forddn_failureList , ogs_strdup(notify_correlation_ids_forddn_failure_local->valuestring));
    }
    }

    cJSON *new_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "newSmfId");

    if (new_smf_id) {
    if (!cJSON_IsString(new_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [new_smf_id]");
        goto end;
    }
    }

    cJSON *new_smf_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "newSmfSetId");

    if (new_smf_set_id) {
    if (!cJSON_IsString(new_smf_set_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [new_smf_set_id]");
        goto end;
    }
    }

    cJSON *old_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "oldSmfId");

    if (old_smf_id) {
    if (!cJSON_IsString(old_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [old_smf_id]");
        goto end;
    }
    }

    cJSON *old_sm_context_ref = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "oldSmContextRef");

    if (old_sm_context_ref) {
    if (!cJSON_IsString(old_sm_context_ref)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    cJSON *alt_anchor_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "altAnchorSmfUri");

    if (alt_anchor_smf_uri) {
    if (!cJSON_IsString(alt_anchor_smf_uri)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [alt_anchor_smf_uri]");
        goto end;
    }
    }

    cJSON *alt_anchor_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_status_notificationJSON, "altAnchorSmfId");

    if (alt_anchor_smf_id) {
    if (!cJSON_IsString(alt_anchor_smf_id)) {
        ogs_error("OpenAPI_sm_context_status_notification_parseFromJSON() failed [alt_anchor_smf_id]");
        goto end;
    }
    }

    sm_context_status_notification_local_var = OpenAPI_sm_context_status_notification_create (
        status_info_local_nonprim,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL,
        ddn_failure_status ? true : false,
        ddn_failure_status ? ddn_failure_status->valueint : 0,
        notify_correlation_ids_forddn_failure ? notify_correlation_ids_forddn_failureList : NULL,
        new_smf_id ? ogs_strdup(new_smf_id->valuestring) : NULL,
        new_smf_set_id ? ogs_strdup(new_smf_set_id->valuestring) : NULL,
        old_smf_id ? ogs_strdup(old_smf_id->valuestring) : NULL,
        old_sm_context_ref ? ogs_strdup(old_sm_context_ref->valuestring) : NULL,
        alt_anchor_smf_uri ? ogs_strdup(alt_anchor_smf_uri->valuestring) : NULL,
        alt_anchor_smf_id ? ogs_strdup(alt_anchor_smf_id->valuestring) : NULL
    );

    return sm_context_status_notification_local_var;
end:
    return NULL;
}

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_copy(OpenAPI_sm_context_status_notification_t *dst, OpenAPI_sm_context_status_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_status_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_status_notification_convertToJSON() failed");
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

    OpenAPI_sm_context_status_notification_free(dst);
    dst = OpenAPI_sm_context_status_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

